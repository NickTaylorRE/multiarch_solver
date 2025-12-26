#![allow(warnings)]
// this is a symbolic exectuion engine for the VM

//use std::rc::Rc;
use std::ops::{Add, Sub, Mul, BitXor, BitAnd, BitOr, Shl, Shr, Not, Deref, DerefMut};
use std::fmt;

use z3::ast::{Ast, BV};
use z3::{Context, Solver, SatResult, Config};
use std::collections::HashMap;

use rayon::prelude::*;


// we build an AST for the symvar because its easy to convert to z3
#[derive(Clone, Debug)]
pub enum SymVar {
    Concrete(u64),

    Var(String),

    Add(Box<SymVar>, Box<SymVar>),
    Sub(Box<SymVar>, Box<SymVar>),
    Mul(Box<SymVar>, Box<SymVar>),
    Div(Box<SymVar>, Box<SymVar>),
    Mod(Box<SymVar>, Box<SymVar>),
    And(Box<SymVar>, Box<SymVar>),
    Or(Box<SymVar>, Box<SymVar>),
    Xor(Box<SymVar>, Box<SymVar>),
    Not(Box<SymVar>),

    Shr(Box<SymVar>, Box<SymVar>),
    Shl(Box<SymVar>, Box<SymVar>),

    Eq(Box<SymVar>, Box<SymVar>),
    Ne(Box<SymVar>, Box<SymVar>),
    Lt(Box<SymVar>, Box<SymVar>),
    Le(Box<SymVar>, Box<SymVar>),
    Gt(Box<SymVar>, Box<SymVar>),
    Ge(Box<SymVar>, Box<SymVar>),

    Rand(Box<SymVar>, u32),
}

impl SymVar {
    pub fn concrete(val: u64) -> SymVar {
        return SymVar::Concrete(val)
    }
    pub fn symbolic(name: String) -> SymVar {
        return SymVar::Var(name)
    }
    // try_concrete tries to grab a concrete value if the SymVar is concrete
    pub fn try_concrete(&self) -> Option<u64> {
        match self {
            SymVar::Concrete(v) => Some(*v),
            _ => None,
        }
    }
    // try_solve calls z3 to find a solution to the SymVars expression.
    // it is expected that the top most expression is some form of Eq. otherwise there is no
    // constraint to solve for.
    pub fn try_solve(&self) -> Option<Vec<(String, u64)>> {
        let cfg = Config::new();
        let ctx = Context::new(&cfg);
        let solver = Solver::new(&ctx);
        let mut vars: HashMap<String, BV> = HashMap::new();
    
        let mut params = z3::Params::new(&ctx);                                                                                  
        params.set_u32("timeout", 120000);  // 2 minute timeout                                                                
        solver.set_params(&params);  

        let z3_expr = self.to_z3(&ctx, &mut vars);

        for (name, z3_var) in &vars {
            if name.contains("_b") {  // byte variables
                solver.assert(&z3_var.bvule(&BV::from_u64(&ctx, 0xff, 64)));
            }
        }
    
        //println!("Z3 expr: {}", z3_expr);    
        solver.assert(&z3_expr._eq(&BV::from_u64(&ctx, 1, 64)));

        match solver.check() {
            SatResult::Sat => {
                //println!("Z3 found sat");
                let model = solver.get_model().expect("failed to get model when z3 solved");
                let mut final_solution = Vec::new();
                for (name, z3_var) in &vars {
                    if let Some(val) = model.eval(z3_var,true) {
                        let solution = val.as_u64().expect("failed to convert to u64");
                        final_solution.push((name.clone(),solution));
                    }
                }
                return Some(final_solution)
                // some solutions are going to be trivial and not have vars
                // this happens by accident, but we might as well deal with it.
                //return self.try_concrete()
            }
            SatResult::Unsat => {
                //println!("Z3 found unsat");
                return None
            }
            SatResult::Unknown => {
                //println!("Z3 returned unknown");
                return None
            }
            _ => {
                //println!("Error solving");
                return None
            }
        }
        return None
    }
}

// rand handling
impl SymVar {
    pub fn find_rand_inners(&self) -> Vec<SymVar> {
        match self {
            SymVar::Rand(inner, index) => vec![inner.as_ref().clone(), SymVar::concrete(index.clone() as u64)],

            SymVar::Add(a, b) | SymVar::Sub(a, b) | SymVar::Mul(a, b) |
              SymVar::Div(a, b) | SymVar::Mod(a, b) | SymVar::And(a, b) |
              SymVar::Or(a, b) | SymVar::Xor(a, b) | SymVar::Shl(a, b) |
              SymVar::Shr(a, b) | SymVar::Eq(a, b) | SymVar::Ne(a, b) |
              SymVar::Lt(a, b) | SymVar::Le(a, b) | SymVar::Gt(a, b) |
              SymVar::Ge(a, b) => {
                  let mut result = a.find_rand_inners();
                  result.extend(b.find_rand_inners());
                  result
              },

            SymVar::Not(a) => a.find_rand_inners(),

            SymVar::Concrete(_) | SymVar::Var(_) => vec![],
        }
    }
    pub fn max_rand_index(&self) -> Option<u32> {
        match self {
            SymVar::Rand(_, index) => Some(*index),

            SymVar::Add(a, b) | SymVar::Sub(a, b) | SymVar::Mul(a, b) |
            SymVar::Div(a, b) | SymVar::Mod(a, b) | SymVar::And(a, b) |
            SymVar::Or(a, b) | SymVar::Xor(a, b) | SymVar::Shl(a, b) |
            SymVar::Shr(a, b) | SymVar::Eq(a, b) | SymVar::Ne(a, b) |
            SymVar::Lt(a, b) | SymVar::Le(a, b) | SymVar::Gt(a, b) |
            SymVar::Ge(a, b) => {
                match (a.max_rand_index(), b.max_rand_index()) {
                    (Some(x), Some(y)) => Some(x.max(y)),
                    (Some(x), None) => Some(x),
                    (None, Some(y)) => Some(y),
                    (None, None) => None,
                }
            }

            SymVar::Not(a) => a.max_rand_index(),

            SymVar::Concrete(_) | SymVar::Var(_) => None,
        }
    }
    // https://elixir.bootlin.com/glibc/glibc-2.42.9000/source/stdlib/random_r.c#L370
    pub fn compute_rand_values(&self, seed: u32, count: usize) -> Vec<u64> {
        const DEG: usize = 31;
        const SEP: usize = 3;

        let seed = if seed == 0 { 1 } else { seed };

        let mut state = [0i32; DEG];
        state[0] = seed as i32;

        let mut word = seed as i32;
        for i in 1..DEG {
            let hi = word / 127773;
            let lo = word % 127773;
            word = (16807 * lo) - (2836 * hi);
            if word < 0 {
                word += 2147483647;
            }
            state[i] = word as i32;
        }

        let mut fptr: usize = SEP;
        let mut rptr: usize = 0;

        let mut next_rand = || -> u64 {
            let val = (state[fptr] as u32).wrapping_add(state[rptr] as u32);
            state[fptr] = val as i32;
            let result = ((state[fptr] as u32) >> 1) as u64;

            fptr += 1;
            if fptr >= DEG {
                fptr = 0;
                rptr += 1;
            } else {
                rptr += 1;
                if rptr >= DEG {
                    rptr = 0;
                }
            }
            result
        }; 

        for _ in 0..(10 * DEG) {
            next_rand();
        }

        return (0..count).map(|_| next_rand()).collect()
    }
    // this function does not finish in reasonable time with the current loop.
    // i need to optimize it
    pub fn solve_with_rand(&self) -> Option<Vec<(String, u64)>> {
        let rand_inners = self.find_rand_inners();
        // no rand inners means no rand.
        if rand_inners.is_empty() {
            return self.try_solve();
        }

        let inner_expr = &rand_inners[0].clone();
        // i want this to fail because i call solve_with_rand explicitely knowing there is rand calls in the expression
        let max_rand_idx = self.max_rand_index().expect("failed to find max_rand");

        let num_threads = rayon::current_num_threads();
        let total_seeds = 0xffffffffu64;
        let chunk_size = (total_seeds / num_threads as u64) as u32;

        //println!("max_rand_idx: {}", max_rand_idx);

        (0..num_threads).into_par_iter().find_map_any(
            |thread_idx| {
                let start = (thread_idx as u32).saturating_mul(chunk_size);
                let end = if thread_idx == num_threads - 1 {
                    0xffffffff
                } else {
                    start.saturating_add(chunk_size)
                };
            
                let cfg = Config::new();
                let ctx = Context::new(&cfg);
                let solver = Solver::new(&ctx);

                let mut params = z3::Params::new(&ctx);
                params.set_u32("timeout", 5000);
                solver.set_params(&params);

                let mut vars: HashMap<String, BV> = HashMap::new();
                let mut rand_vars: Vec<BV> = Vec::new();
                for i in 0..=max_rand_idx {
                    let name = format!("rand{}", i);
                    let bv = BV::new_const(&ctx, name.as_str(), 64);
                    rand_vars.push(bv.clone());
                    vars.insert(name, bv);
                }

                let z3_expr = self.to_z3(&ctx, &mut vars);

                for (name, z3_var) in &vars {
                    if name.contains("_b") {
                        solver.assert(&z3_var.bvule(&BV::from_u64(&ctx, 0xff, 64)))
                    }
                }
                solver.assert(&z3_expr._eq(&BV::from_u64(&ctx, 1, 64)));

                for srand_guess in start..=end {
                    let mut rand_values: Vec<u64> = Vec::new();

                    let rand_values = self.compute_rand_values(srand_guess, max_rand_idx as usize + 1);

                    /*if (srand_guess % 0x100000) == 0 {
                        println!("guess z3: {:#X}", srand_guess);
                    }*/
                    // save solver state
                    solver.push();

                    // replace rand values with calculated values
                    for (i, rand_val) in rand_values.iter().enumerate() {
                        solver.assert(&rand_vars[i]._eq(&BV::from_u64(&ctx, *rand_val, 64)));
                    }

                    if solver.check() == SatResult::Sat {
                        let inner_constraint = SymVar::Eq(
                            Box::new(inner_expr.clone()),
                            Box::new(SymVar::Concrete(srand_guess as u64))
                        );
                        if let Some(solution) = inner_constraint.try_solve() {
                            return Some(solution);
                        }
                    }
                    solver.pop(1);
                }
                return None
            })
    }
}

impl SymVar {
    pub fn add(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::Concrete(x.wrapping_add(*y));
        }
        return SymVar::Add(Box::new(self),Box::new(other))
    }
    pub fn sub(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(x.wrapping_sub(*y));
        }
        return SymVar::Sub(Box::new(self),Box::new(other))
    }
    pub fn mul(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(x.wrapping_mul(*y));
        }
        return SymVar::Mul(Box::new(self),Box::new(other))
    }
    pub fn udiv(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            if *y == 0 { panic!("Division by zero"); }
            return SymVar::concrete(x / y);
        }
        return SymVar::Div(Box::new(self), Box::new(other))
    }
    pub fn umod(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            if *y == 0 { panic!("Modulo by zero"); }
            return SymVar::concrete(x % y);
        }
        return SymVar::Mod(Box::new(self), Box::new(other))
    }
    pub fn bitxor(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(x ^ y);
        }
        return SymVar::Xor(Box::new(self),Box::new(other))
    }
    pub fn bitand(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(x & y);
        }
        return SymVar::And(Box::new(self),Box::new(other))
    }
    pub fn bitor(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(x | y);
        }
        return SymVar::Or(Box::new(self),Box::new(other))
    }
    pub fn not(self) -> SymVar {
        if let SymVar::Concrete(x) = &self {
            return SymVar::concrete(!x);
        }
        return SymVar::Not(Box::new(self))
    }
    pub fn shl(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(x << y);
        }
        return SymVar::Shl(Box::new(self),Box::new(other))
    }
    pub fn shr(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(x >> y);
        }
        return SymVar::Shr(Box::new(self),Box::new(other))
    }
    pub fn eq(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(if x == y { 1 } else { 0 });
        }
        return SymVar::Eq(Box::new(self),Box::new(other))
    }
    pub fn ne(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(if x != y { 1 } else { 0 });
        }
        return SymVar::Ne(Box::new(self),Box::new(other))
    }
    pub fn lt(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(if x < y { 1 } else { 0 });
        }
        return SymVar::Lt(Box::new(self),Box::new(other))
    }
    pub fn le(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(if x <= y { 1 } else { 0 });
        }
        return SymVar::Le(Box::new(self),Box::new(other))
    }
    pub fn gt(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(if x > y { 1 } else { 0 });
        }
        return SymVar::Gt(Box::new(self),Box::new(other))
    }
    pub fn ge(self, other: SymVar) -> SymVar {
        if let (SymVar::Concrete(x), SymVar::Concrete(y)) = (&self, &other) {
            return SymVar::concrete(if x >= y { 1 } else { 0 });
        }
        return SymVar::Ge(Box::new(self),Box::new(other))
    }
    pub fn rand(self,index: u32) -> SymVar {
        // we do not handle concrete rand situations
        // concrete cases are handled right away in symbex_vm
        // so we only get symvar cases here.
        return SymVar::Rand(Box::new(self), index)
    }
}

impl SymVar {
    pub fn to_z3<'ctx>(&self, ctx: &'ctx Context, vars: &mut HashMap<String, BV<'ctx>>) -> BV<'ctx> {
        match self {
            SymVar::Concrete(val) => BV::from_u64(ctx, *val as u64, 64),

            SymVar::Var(name) => {
                vars.entry(name.clone())
                    .or_insert_with(|| BV::new_const(ctx, name.as_str(), 64))
                    .clone()
            }
            
            SymVar::Add(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvadd(&b_z3)
            }

            SymVar::Sub(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvsub(&b_z3)
            }

            SymVar::Mul(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvmul(&b_z3)
            }
            SymVar::Div(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvudiv(&b_z3)
            }

            SymVar::Mod(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvurem(&b_z3)
            }

            SymVar::And(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvand(&b_z3)
            }

            SymVar::Or(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvor(&b_z3)
            }

            SymVar::Xor(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvxor(&b_z3)
            }

            SymVar::Not(a) => {
                let a_z3 = a.to_z3(ctx, vars);
                a_z3.bvnot()
            }

            SymVar::Shl(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvshl(&b_z3)
            }
            
            SymVar::Shr(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvlshr(&b_z3)
            }
            
            SymVar::Eq(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3._eq(&b_z3).ite(
                    &BV::from_u64(ctx, 1, 64),
                    &BV::from_u64(ctx, 0, 64),
                )
            }

            SymVar::Ne(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3._eq(&b_z3).ite(
                    &BV::from_u64(ctx, 0, 64),
                    &BV::from_u64(ctx, 1, 64),
                )
            }

            SymVar::Lt(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvult(&b_z3).ite(
                    &BV::from_u64(ctx, 1, 64),
                    &BV::from_u64(ctx, 0, 64),
                )
            }

            SymVar::Le(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvule(&b_z3).ite(
                    &BV::from_u64(ctx, 1, 64),
                    &BV::from_u64(ctx, 0, 64),
                )
            }

            SymVar::Gt(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvugt(&b_z3).ite(
                    &BV::from_u64(ctx, 1, 64),
                    &BV::from_u64(ctx, 0, 64),
                )
            }

            SymVar::Ge(a, b) => {
                let a_z3 = a.to_z3(ctx, vars);
                let b_z3 = b.to_z3(ctx, vars);
                a_z3.bvuge(&b_z3).ite(
                    &BV::from_u64(ctx, 1, 64),
                    &BV::from_u64(ctx, 0, 64),
                )
            }
            
            // there is no rand to z3
            SymVar::Rand(_, index) => {
                let rand_name = format!("rand{}", index);
                vars.get(&rand_name)
                    .expect(&format!("rand variable {} not pre-populated in vars", rand_name))
                    .clone()
            }
        }
    }
}

impl fmt::Display for SymVar {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            SymVar::Concrete(val) => write!(f, "{}", val),
            SymVar::Var(name) => write!(f, "{}", name),
            SymVar::Add(a, b) => write!(f, "{} + {}", a, b),
            SymVar::Sub(a, b) => write!(f, "{} - {}", a, b),
            SymVar::Mul(a, b) => write!(f, "{} * {}", a, b),
            SymVar::Div(a, b) => write!(f, "{} / {}", a, b),
            SymVar::Mod(a, b) => write!(f, "{} % {}", a, b),
            SymVar::Xor(a, b) => write!(f, "{} ^ {}", a, b),
            SymVar::And(a, b) => write!(f, "{} & {}", a, b),
            SymVar::Or(a, b) => write!(f, "{} | {}", a, b),
            SymVar::Shl(a, b) => write!(f, "{} << {}", a, b),
            SymVar::Shr(a, b) => write!(f, "{} >> {}", a, b),
            SymVar::Not(a) => write!(f, "!{}", a),
            SymVar::Eq(a, b) => write!(f, "{} == {}", a, b),
            SymVar::Ne(a, b) => write!(f, "{} != {}", a, b),
            SymVar::Lt(a, b) => write!(f, "{} < {}", a, b),
            SymVar::Le(a, b) => write!(f, "{} <= {}", a, b),
            SymVar::Gt(a, b) => write!(f, "{} > {}", a, b),
            SymVar::Ge(a, b) => write!(f, "{} >= {}", a, b),
            SymVar::Rand(a, i) => write!(f, "rand({}_{})", a, i),
        }
    }
}


// SymVarVec is a wrapper on SymVar which offers convenience functions on Vec<SymVar> objects.
// this is all necessary because we have to handle u8, u16, and u32 data which means we need to
// split up u16 and u32 data.
// our stack implementation is fucked for pointer arithmetic. so we track whether we have a pointer
// or not and reverse the arithmetic accordingly.
#[derive(Clone)]
pub struct SymVarVec(Vec<SymVar>);

impl SymVarVec {
    pub fn new(size: usize) -> Self {
        SymVarVec(
            vec![SymVar::Concrete(0);size]
            )
    }
    pub fn concrete_n(n: usize) -> SymVarVec {
        let tmp = SymVarVec((0..n)
                    .map(|i| SymVar::concrete(0)).collect()
                  );
        return tmp
    }
    pub fn concrete_u32(val: u32) -> SymVarVec {
        let svv = SymVarVec(vec![
            SymVar::Concrete(((val >> 0x18) & 0xff).into()),
            SymVar::Concrete(((val >> 0x10) & 0xff).into()),
            SymVar::Concrete(((val >> 0x8) & 0xff).into()),
            SymVar::Concrete((val & 0xff).into()),
        ]);
        return svv
    }
    pub fn concrete_u16(val: u16) -> SymVarVec {
        let svv = SymVarVec(vec![
            SymVar::Concrete(((val >> 0x8) & 0xff).into()),
            SymVar::Concrete((val & 0xff).into()),
        ]);
        return svv
    }
    pub fn concrete_u8(val: u8) -> SymVarVec {
        let svv = SymVarVec(vec![SymVar::Concrete((val & 0xff).into())]);
        return svv
    }
    pub fn symbolic_n(name:String, n: usize) -> SymVarVec {
        let tmp = SymVarVec((0..n)
                    .map(|i| SymVar::symbolic(format!("{}_b{}", name, i))).collect()
                  );
        return tmp
    }
    pub fn symbolic_u32(name: String) -> SymVarVec {
        return SymVarVec(vec![
            SymVar::symbolic(format!("{}_b0",name)),
            SymVar::symbolic(format!("{}_b1",name)),
            SymVar::symbolic(format!("{}_b2",name)),
            SymVar::symbolic(format!("{}_b3",name)),
        ])
    }
    pub fn symbolic_u16(name: String) -> SymVarVec {
        return SymVarVec(vec![
            SymVar::symbolic(format!("{}_b0",name)),
            SymVar::symbolic(format!("{}_b1",name)),
        ])
    }
    pub fn symbolic_u8(name: String) -> SymVarVec {
        return SymVarVec(vec![
            SymVar::symbolic(format!("{}_b0",name))
        ])
    }
    pub fn try_concrete_u32(&self) -> Option<u32> {
        if self.len() < 4 {
            return None
        }
        let b0 = self[0].try_concrete()? as u32;
        let b1 = self[1].try_concrete()? as u32;
        let b2 = self[2].try_concrete()? as u32;
        let b3 = self[3].try_concrete()? as u32;
        return Some((b0 << 0x18) | (b1 << 0x10) | (b2 << 0x8) | b3)
    }
    pub fn try_concrete_u16(&self) -> Option<u16> {
        if self.len() < 2 {
            return None
        }
        let b0 = self[0].try_concrete()? as u16;
        let b1 = self[1].try_concrete()? as u16;
        return Some((b0 << 0x8) | b1)
    }
    pub fn try_concrete_u8(&self) -> Option<u8> {
        if self.len() < 1 {
            return None
        }
        let b0 = self[0].try_concrete()? as u8;
        return Some(b0)
    }
    // unnecessary because we only solve flags because we only solve on conditional jumps.
    /*pub fn try_solve_u32(&self) -> Option<u32> {
        if self.len() < 4 {
            return None
        }
        let a = self.to_symvar_u32();
        let result = a.try_solve(32).map(|v| v as u32)?;
        return Some(result);
    }
    pub fn try_solve_u16(&self) -> Option<u16> {
        if self.len() < 2 {
            return None
        }
        let a = self.to_symvar_u16();
        let result = a.try_solve(16).map(|v| v as u16)?;
        return Some(result);
    } 
    pub fn try_solve_u8(&self) -> Option<u8> {
        if self.len() < 1 {
            return None
        }
        let a = self.to_symvar_u8();
        let result = a.try_solve(8).map(|v| v as u8)?;
        return Some(result);
    }*/
    pub fn to_symvar_u32(&self) -> SymVar {
        if self.len() < 4 {
            panic!("SymVarVec too short for u32 conversion");
        }
        // Big-endian: (b0 << 8) | b1)
        return self[0].clone().shl(SymVar::concrete(24))
            .bitor(self[1].clone().shl(SymVar::concrete(16)))
            .bitor(self[2].clone().shl(SymVar::concrete(8)))
            .bitor(self[3].clone())
    }
    pub fn to_symvar_u16(&self) -> SymVar {
        if self.len() < 2 {
            panic!("SymVarVec too short for u32 conversion");
        }
        // Big-endian: (b0 << 24) | (b1 << 16) | (b2 << 8) | b3
        return self[0].clone().shl(SymVar::concrete(8))
            .bitor(self[1].clone())
    }
    pub fn to_symvar_u8(&self) -> SymVar {
        if self.len() < 1 {
            panic!("SymVarVec too short for u32 conversion");
        }
        return self[0].clone()
    }
    pub fn from_symvar_u32(sv: SymVar) -> SymVarVec {
        let b0 = sv.clone().shr(SymVar::concrete(24)).bitand(SymVar::concrete(0xff));
        let b1 = sv.clone().shr(SymVar::concrete(16)).bitand(SymVar::concrete(0xff));
        let b2 = sv.clone().shr(SymVar::concrete(8)).bitand(SymVar::concrete(0xff));
        let b3 = sv.bitand(SymVar::concrete(0xff));
        return SymVarVec(vec![b0, b1, b2, b3])
    }
    pub fn from_symvar_u16(sv: SymVar) -> SymVarVec {
        let b0 = sv.clone().shr(SymVar::concrete(8)).bitand(SymVar::concrete(0xff));
        let b1 = sv.bitand(SymVar::concrete(0xff));
        return SymVarVec(vec![b0, b1])
    }
    pub fn from_symvar_u8(sv: SymVar) -> SymVarVec {
        let b0 = sv.clone();
        return SymVarVec(vec![b0])
    }
}

// these are the public interfaces for SymVarVec
// NOTE: push and pop should not be used outside SymVarVec. only the sized pushs and pops
impl SymVarVec {
    /*pub fn push(&mut self, other: SymVarVec) {
        self.0.extend(other.0) // forward stack
    }
    pub fn pop(&mut self, len: usize) -> Option<SymVarVec> {
        if self.0.len() < len {
            return None;
        }
        let popped = self.0.split_off(self.0.len() - len);
        return Some(SymVarVec(popped))
    }*/
    pub fn assign(&mut self, offset: usize, value: &SymVarVec) {
        let mut new_value = value.0.clone();
        for (i, sv) in value.0.iter().enumerate() {
            if offset - i - 1 > 0 {
                self.0[offset - i - 1] = sv.clone();
            }
        }
    }
    pub fn get(&self, offset: usize, len: usize) -> SymVarVec {
        //let mut slice = &self.0[offset-len..offset];
        let mut slice = &self.0[offset..offset+len];
        let mut slice_vec = slice.to_vec();
        slice_vec.reverse();
        return SymVarVec(slice_vec)
    }
}

// for all of our math operations, we convert to an appropriately sized data type
// then do the math operations.
// then convert back to the vec.
impl SymVarVec {
    pub fn addp(self, other: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for addp")
        }

        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.add(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn subp(self, other: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for subp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.sub(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn mulp(self, other: SymVarVec) -> (SymVarVec,SymVarVec) {
        if self.len() < 4 {
            panic!("SymVarVec is too short for mulp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.mul(b);

        let low = result.clone().bitand(SymVar::concrete(0xFFFFFFFF));
        let high = result.shr(SymVar::concrete(32));

        return (SymVarVec::from_symvar_u32(high), SymVarVec::from_symvar_u32(low))
    }
    pub fn mulp_low(self, other: SymVarVec) -> SymVarVec {
        let (_, low) = self.mulp(other);
        return low
    }
    pub fn divp(self, other: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for divp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.udiv(b);
        return SymVarVec::from_symvar_u32(result)
    }

    pub fn modp(self, other: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for modp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.umod(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn bitxorp(self, other: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for bitxorp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.bitxor(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn bitandp(self, other: SymVarVec) -> SymVarVec {
        if self.0.len() < 4 {
            panic!("SymVarVec is too short for bitandp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.bitand(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn bitorp(self, other: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for bitorp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.bitor(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn shrp(self, shift: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for shrp")
        }
        let a = self.to_symvar_u32();
        let b = shift.to_symvar_u32();
        let result = a.shr(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn shlp(self, shift: SymVarVec) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for shrp")
        }
        let a = self.to_symvar_u32();
        let b = shift.to_symvar_u32();
        let result = a.shl(b);
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn notp(self) -> SymVarVec {
        if self.len() < 4 {
            panic!("SymVarVec is too short for notp")
        }
        let a = self.to_symvar_u32();
        let result = a.not();
        return SymVarVec::from_symvar_u32(result)
    }
    pub fn eqp(self, other: SymVarVec) -> SymVar {
        if self.len() < 4 {
            panic!("SymVarVec is too short for eqp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.eq(b);
        return result
    }
    pub fn nep(self, other: SymVarVec) -> SymVar {
        if self.len() < 4 {
            panic!("SymVarVec is too short for nep")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.ne(b);
        return result
    }
    pub fn ltp(self, other: SymVarVec) -> SymVar {
        if self.len() < 4 {
            panic!("SymVarVec is too short for ltp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.lt(b);
        return result
    }
    pub fn lep(self, other: SymVarVec) -> SymVar {
        if self.len() < 4 {
            panic!("SymVarVec is too short for lep")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.le(b);
        return result
    }
    pub fn gtp(self, other: SymVarVec) -> SymVar {
        if self.len() < 4 {
            panic!("SymVarVec is too short for gtp")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.gt(b);
        return result
    }
    pub fn gep(self, other: SymVarVec) -> SymVar {
        if self.len() < 4 {
            panic!("SymVarVec is too short for gep")
        }
        let a = self.to_symvar_u32();
        let b = other.to_symvar_u32();
        let result = a.ge(b);
        return result
    }
    pub fn rand(self, index: u32) -> SymVarVec {
        let a = self.to_symvar_u32();
        let result = a.rand(index);
        return SymVarVec::from_symvar_u32(result)
    }
}

impl Deref for SymVarVec {
    type Target = Vec<SymVar>;
    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl DerefMut for SymVarVec {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.0
    }
}

impl fmt::Display for SymVarVec {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            SymVarVec(val) => {
                let mut printvec: Vec<Vec<SymVar>> = Vec::new();
                for entry in val.chunks(4) {
                    printvec.push(entry.iter().cloned().collect());
                }
                write!(f, "{:?}", printvec)
            }
        }
    }
}



#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_rand_matches_libc() {
        for seed in [0u32, 1, 42, 12345, 0xDEADBEEF] {
            unsafe {
                libc::srand(seed);
            }
            let expected: Vec<u64> = (0..10).map(|_| unsafe { libc::rand() } as u64).collect();
            let sv = SymVar::concrete(0x0);
            let actual = sv.compute_rand_values(seed, 10);
            assert_eq!(expected, actual, "Mismatch for seed {}", seed);
        }
    }
}
