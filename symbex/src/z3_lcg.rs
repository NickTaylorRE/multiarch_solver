// Standalone Z3-based glibc LCG implementation
// This models glibc's random() with TYPE_3 (31-state trinomial PRNG)

use z3::ast::{Ast, BV};
use z3::{Context, Solver, SatResult, Config, Model};
use std::collections::HashMap;

const DEG: usize = 31;
const SEP: usize = 3;
const WARMUP: usize = DEG * 10; // 310 iterations

/// Reduced warmup LCG for testing if symbolic solving works
pub struct Z3LcgReduced<'ctx> {
    ctx: &'ctx Context,
    state: Vec<BV<'ctx>>,
    fptr: usize,
    rptr: usize,
}

impl<'ctx> Z3LcgReduced<'ctx> {
    /// Initialize with reduced warmup (for testing symbolic solving feasibility)
    pub fn srand_reduced(ctx: &'ctx Context, seed: LcgValue<'ctx>, warmup_iters: usize) -> Self {
        let mut state: Vec<BV<'ctx>> = Vec::with_capacity(DEG);

        let seed_bv = seed.to_bv(ctx);
        let zero = BV::from_u64(ctx, 0, 32);
        let one = BV::from_u64(ctx, 1, 32);
        let actual_seed = seed_bv._eq(&zero).ite(&one, &seed_bv);

        state.push(actual_seed.clone());

        let mut word = actual_seed;
        for _ in 1..DEG {
            word = Z3Lcg::lcg_step(ctx, &word);
            state.push(word.clone());
        }

        let mut lcg = Z3LcgReduced {
            ctx,
            state,
            fptr: SEP,
            rptr: 0,
        };

        // Reduced warmup
        for _ in 0..warmup_iters {
            let _ = lcg.rand_internal();
        }

        lcg
    }

    fn rand_internal(&mut self) -> BV<'ctx> {
        let val = self.state[self.fptr].bvadd(&self.state[self.rptr]);
        self.state[self.fptr] = val.clone();
        let result = val.bvlshr(&BV::from_u64(self.ctx, 1, 32));

        self.fptr += 1;
        if self.fptr >= DEG {
            self.fptr = 0;
            self.rptr += 1;
        } else {
            self.rptr += 1;
            if self.rptr >= DEG {
                self.rptr = 0;
            }
        }
        result
    }

    pub fn rand(&mut self) -> BV<'ctx> {
        self.rand_internal()
    }
}

// ============== Mathematical LCG Inversion ==============

/// Modular inverse of 16807 mod (2^31 - 1)
const LCG_INV: u64 = 1407677000;
const LCG_MOD: u64 = 2147483647;
const LCG_MUL: u64 = 16807;

/// Forward LCG step (pure math, no Z3)
pub fn lcg_forward(x: u32) -> u32 {
    ((x as u64 * LCG_MUL) % LCG_MOD) as u32
}

/// Reverse LCG step using modular inverse
pub fn lcg_reverse(x: u32) -> u32 {
    ((x as u64 * LCG_INV) % LCG_MOD) as u32
}

/// Concrete glibc-compatible srand/rand implementation
pub struct ConcreteLcg {
    state: Vec<u32>,
    fptr: usize,
    rptr: usize,
}

impl ConcreteLcg {
    pub fn srand(mut seed: u32) -> Self {
        if seed == 0 { seed = 1; }

        let mut state = vec![0u32; DEG];
        state[0] = seed;

        let mut word = seed;
        for i in 1..DEG {
            word = lcg_forward(word);
            state[i] = word;
        }

        let mut lcg = ConcreteLcg {
            state,
            fptr: SEP,
            rptr: 0,
        };

        // 310 warmup iterations
        for _ in 0..WARMUP {
            let _ = lcg.rand_internal();
        }

        lcg
    }

    fn rand_internal(&mut self) -> u32 {
        let val = self.state[self.fptr].wrapping_add(self.state[self.rptr]);
        self.state[self.fptr] = val;
        let result = val >> 1;

        self.fptr += 1;
        if self.fptr >= DEG {
            self.fptr = 0;
            self.rptr += 1;
        } else {
            self.rptr += 1;
            if self.rptr >= DEG {
                self.rptr = 0;
            }
        }
        result
    }

    pub fn rand(&mut self) -> u32 {
        self.rand_internal()
    }
}

/// Brute force search for seed that produces target rand() value
pub fn brute_force_seed(target: u32, max_seed: u32) -> Option<u32> {
    for seed in 1..=max_seed {
        let mut lcg = ConcreteLcg::srand(seed);
        if lcg.rand() == target {
            return Some(seed);
        }
    }
    None
}

/// Parallel brute force (uses rayon if available, otherwise sequential)
pub fn brute_force_seed_parallel(target: u32) -> Option<u32> {
    use std::sync::atomic::{AtomicBool, AtomicU32, Ordering};
    use std::thread;

    let found = AtomicBool::new(false);
    let result = AtomicU32::new(0);

    let num_threads = thread::available_parallelism()
        .map(|n| n.get())
        .unwrap_or(4);

    let chunk_size = 0x80000000u32 / num_threads as u32;

    thread::scope(|s| {
        for t in 0..num_threads {
            let start = (t as u32 * chunk_size).max(1);
            let end = if t == num_threads - 1 {
                0x80000000u32
            } else {
                (t + 1) as u32 * chunk_size
            };

            let found = &found;
            let result = &result;

            s.spawn(move || {
                for seed in start..end {
                    if found.load(Ordering::Relaxed) {
                        return;
                    }
                    let mut lcg = ConcreteLcg::srand(seed);
                    if lcg.rand() == target {
                        found.store(true, Ordering::Relaxed);
                        result.store(seed, Ordering::Relaxed);
                        return;
                    }
                }
            });
        }
    });

    if found.load(Ordering::Relaxed) {
        Some(result.load(Ordering::Relaxed))
    } else {
        None
    }
}

/// Test solving with reduced warmup to verify approach
pub fn solve_seed_reduced_warmup(target: u32, warmup: usize, timeout_ms: u32) -> Option<u32> {
    let cfg = Config::new();
    let ctx = Context::new(&cfg);
    let tactic = z3::Tactic::new(&ctx, "qfbv");
    let solver = tactic.solver();

    let mut params = z3::Params::new(&ctx);
    params.set_u32("timeout", timeout_ms);
    solver.set_params(&params);

    let seed = LcgValue::symbolic(&ctx, "seed");
    let seed_bv = match &seed {
        LcgValue::Symbolic(bv) => bv.clone(),
        _ => unreachable!(),
    };

    let mut lcg = Z3LcgReduced::srand_reduced(&ctx, seed, warmup);
    let rand_output = lcg.rand();

    let target_bv = BV::from_u64(&ctx, target as u64, 32);
    solver.assert(&rand_output._eq(&target_bv));
    solver.assert(&seed_bv.bvult(&BV::from_u64(&ctx, 0x80000000, 32)));

    println!("Solving with warmup={}, timeout={}ms...", warmup, timeout_ms);

    match solver.check() {
        SatResult::Sat => {
            println!("SAT");
            let model = solver.get_model()?;
            let seed_val = model.eval(&seed_bv, true)?;
            seed_val.as_u64().map(|v| v as u32)
        }
        SatResult::Unsat => {
            println!("UNSAT");
            None
        }
        SatResult::Unknown => {
            println!("UNKNOWN (timeout)");
            None
        }
    }
}

/// Represents either a concrete u32 or a symbolic Z3 bitvector
pub enum LcgValue<'ctx> {
    Concrete(u32),
    Symbolic(BV<'ctx>),
}

impl<'ctx> LcgValue<'ctx> {
    pub fn concrete(val: u32) -> Self {
        LcgValue::Concrete(val)
    }

    pub fn symbolic(ctx: &'ctx Context, name: &str) -> Self {
        LcgValue::Symbolic(BV::new_const(ctx, name, 32))
    }

    pub fn to_bv(&self, ctx: &'ctx Context) -> BV<'ctx> {
        match self {
            LcgValue::Concrete(v) => BV::from_u64(ctx, *v as u64, 32),
            LcgValue::Symbolic(bv) => bv.clone(),
        }
    }
}

/// Z3-based LCG state that mirrors glibc's random_data structure
pub struct Z3Lcg<'ctx> {
    ctx: &'ctx Context,
    state: Vec<BV<'ctx>>,
    fptr: usize,
    rptr: usize,
}

impl<'ctx> Z3Lcg<'ctx> {
    /// Single LCG step: (16807 * x) mod (2^31 - 1) using Schrage's method
    fn lcg_step(ctx: &'ctx Context, word: &BV<'ctx>) -> BV<'ctx> {
        let c_127773 = BV::from_u64(ctx, 127773, 32);
        let c_16807 = BV::from_u64(ctx, 16807, 32);
        let c_2836 = BV::from_u64(ctx, 2836, 32);
        let c_mod = BV::from_u64(ctx, 2147483647, 32);
        let c_zero = BV::from_u64(ctx, 0, 32);

        let hi = word.bvudiv(&c_127773);
        let lo = word.bvurem(&c_127773);

        let term1 = lo.bvmul(&c_16807);
        let term2 = hi.bvmul(&c_2836);

        // Use wider bitvector for subtraction to handle negative case
        let term1_64 = term1.zero_ext(32);
        let term2_64 = term2.zero_ext(32);
        let c_mod_64 = c_mod.zero_ext(32);

        let diff_64 = term1_64.bvsub(&term2_64);

        // If term1 < term2, result is negative, add modulus
        let is_negative = term1.bvult(&term2);
        let adjusted_64 = diff_64.bvadd(&c_mod_64);

        let result_64 = is_negative.ite(&adjusted_64, &diff_64);
        result_64.extract(31, 0)
    }

    /// Initialize LCG with a seed (concrete or symbolic)
    pub fn srand(ctx: &'ctx Context, seed: LcgValue<'ctx>) -> Self {
        let mut state: Vec<BV<'ctx>> = Vec::with_capacity(DEG);

        // Ensure seed is not 0 (glibc uses 1 if seed is 0)
        let seed_bv = seed.to_bv(ctx);
        let zero = BV::from_u64(ctx, 0, 32);
        let one = BV::from_u64(ctx, 1, 32);
        let actual_seed = seed_bv._eq(&zero).ite(&one, &seed_bv);

        // state[0] = seed
        state.push(actual_seed.clone());

        // Initialize state[1..31] using LCG
        let mut word = actual_seed;
        for _ in 1..DEG {
            word = Self::lcg_step(ctx, &word);
            state.push(word.clone());
        }

        let mut lcg = Z3Lcg {
            ctx,
            state,
            fptr: SEP, // rand_sep = 3
            rptr: 0,
        };

        // Warmup: 310 iterations
        for _ in 0..WARMUP {
            let _ = lcg.rand_internal();
        }

        lcg
    }

    /// Single rand primitive call (one __random_r call)
    fn rand_internal(&mut self) -> BV<'ctx> {
        // val = state[fptr] + state[rptr], with 32-bit wrapping
        let val = self.state[self.fptr].bvadd(&self.state[self.rptr]);

        // state[fptr] = val
        self.state[self.fptr] = val.clone();

        // result = val >> 1 (discard least random bit)
        let result = val.bvlshr(&BV::from_u64(self.ctx, 1, 32));

        // Advance pointers
        self.fptr += 1;
        if self.fptr >= DEG {
            self.fptr = 0;
            self.rptr += 1;
        } else {
            self.rptr += 1;
            if self.rptr >= DEG {
                self.rptr = 0;
            }
        }

        result
    }

    /// Get next random value (equivalent to rand())
    pub fn rand(&mut self) -> BV<'ctx> {
        self.rand_internal()
    }
}

/// Solve for seed given a target rand() output
pub fn solve_seed_for_rand(target: u32) -> Option<u32> {
    solve_seed_for_rand_with_timeout(target, 60000)
}

/// Solve for seed with configurable timeout (in milliseconds)
pub fn solve_seed_for_rand_with_timeout(target: u32, timeout_ms: u32) -> Option<u32> {
    let cfg = Config::new();
    let ctx = Context::new(&cfg);

    // Use a tactic-based solver optimized for bitvectors
    let tactic = z3::Tactic::new(&ctx, "qfbv");
    let solver = tactic.solver();

    // Set timeout
    let mut params = z3::Params::new(&ctx);
    params.set_u32("timeout", timeout_ms);
    solver.set_params(&params);

    // Create symbolic seed
    let seed = LcgValue::symbolic(&ctx, "seed");
    let seed_bv = match &seed {
        LcgValue::Symbolic(bv) => bv.clone(),
        _ => unreachable!(),
    };

    // Run LCG with symbolic seed
    let mut lcg = Z3Lcg::srand(&ctx, seed);
    let rand_output = lcg.rand();

    // Constrain output to target
    let target_bv = BV::from_u64(&ctx, target as u64, 32);
    solver.assert(&rand_output._eq(&target_bv));

    // Constrain seed to reasonable range (positive 32-bit)
    solver.assert(&seed_bv.bvult(&BV::from_u64(&ctx, 0x80000000, 32)));

    println!("Solving with {} ms timeout...", timeout_ms);

    match solver.check() {
        SatResult::Sat => {
            println!("SAT - solution found");
            let model = solver.get_model()?;
            let seed_val = model.eval(&seed_bv, true)?;
            seed_val.as_u64().map(|v| v as u32)
        }
        SatResult::Unsat => {
            println!("UNSAT - no solution exists");
            None
        }
        SatResult::Unknown => {
            println!("UNKNOWN - likely timeout");
            None
        }
    }
}

// ============== Tests ==============

#[cfg(test)]
mod tests {
    use super::*;

    /// Test that concrete Z3 LCG matches libc
    #[test]
    fn test_concrete_matches_libc() {
        let seeds = [1u32, 12345, 0x2B6043C, 0x7FFFFFFF, 42];

        for seed in seeds {
            // Get libc result
            unsafe { libc::srand(seed) };
            let libc_rand = unsafe { libc::rand() } as u32;

            // Get Z3 result with concrete seed
            let cfg = Config::new();
            let ctx = Context::new(&cfg);
            let mut lcg = Z3Lcg::srand(&ctx, LcgValue::concrete(seed));
            let z3_rand = lcg.rand();

            // Evaluate the concrete result
            let solver = Solver::new(&ctx);
            solver.check();
            let model = solver.get_model().unwrap();
            let z3_result = model.eval(&z3_rand, true)
                .and_then(|v| v.as_u64())
                .map(|v| v as u32)
                .unwrap();

            println!("Seed: {:#X}, libc: {:#X}, Z3: {:#X}", seed, libc_rand, z3_result);
            assert_eq!(libc_rand, z3_result,
                "Mismatch for seed {:#X}: libc={:#X}, Z3={:#X}",
                seed, libc_rand, z3_result);
        }
    }

    /// Test multiple consecutive rand() calls
    #[test]
    fn test_multiple_rands_match_libc() {
        let seed = 0x2B6043Cu32;

        // Get libc results
        unsafe { libc::srand(seed) };
        let mut libc_results = Vec::new();
        for _ in 0..5 {
            libc_results.push(unsafe { libc::rand() } as u32);
        }

        // Get Z3 results
        let cfg = Config::new();
        let ctx = Context::new(&cfg);
        let mut lcg = Z3Lcg::srand(&ctx, LcgValue::concrete(seed));

        let solver = Solver::new(&ctx);
        solver.check();
        let model = solver.get_model().unwrap();

        for (i, &expected) in libc_results.iter().enumerate() {
            let z3_rand = lcg.rand();
            let z3_result = model.eval(&z3_rand, true)
                .and_then(|v| v.as_u64())
                .map(|v| v as u32)
                .unwrap();

            println!("rand[{}]: libc={:#X}, Z3={:#X}", i, expected, z3_result);
            assert_eq!(expected, z3_result, "Mismatch at rand[{}]", i);
        }
    }

    /// Test solving for seed given rand output
    #[test]
    fn test_solve_for_seed() {
        // First, get a real rand value from a known seed
        let original_seed = 0x2B6043Cu32;
        unsafe { libc::srand(original_seed) };
        let target_rand = unsafe { libc::rand() } as u32;

        println!("Original seed: {:#X}", original_seed);
        println!("Target rand value: {:#X}", target_rand);

        // Now try to solve for the seed
        let solved_seed = solve_seed_for_rand(target_rand);

        match solved_seed {
            Some(seed) => {
                println!("Solved seed: {:#X}", seed);

                // Verify the solved seed produces the same rand value
                unsafe { libc::srand(seed) };
                let verify_rand = unsafe { libc::rand() } as u32;
                println!("Verification rand: {:#X}", verify_rand);

                assert_eq!(target_rand, verify_rand,
                    "Solved seed {:#X} doesn't produce target rand {:#X}",
                    seed, target_rand);
            }
            None => {
                panic!("Failed to solve for seed");
            }
        }
    }

    /// Test solving with varying warmup iterations to find threshold
    #[test]
    #[ignore]
    fn test_solve_reduced_warmup() {
        // Test with 0 warmup first (should be fast)
        for warmup in [0, 10, 30, 50, 100, 150, 200, 310] {
            println!("\n=== Testing warmup={} ===", warmup);

            // Use a simple target value
            let result = solve_seed_reduced_warmup(0x12345678, warmup, 30000);

            match result {
                Some(seed) => {
                    println!("Found seed: {:#X} for warmup={}", seed, warmup);
                }
                None => {
                    println!("Failed for warmup={}", warmup);
                    break; // Stop at first failure
                }
            }
        }
    }

    /// Test LCG forward/reverse mathematical operations
    #[test]
    fn test_lcg_math() {
        // Test forward
        let x = 12345u32;
        let next = lcg_forward(x);
        println!("Forward: {} -> {}", x, next);

        // Test reverse
        let prev = lcg_reverse(next);
        println!("Reverse: {} -> {}", next, prev);
        assert_eq!(prev, x, "LCG reverse should recover original");

        // Test multiple steps
        let mut val = 1u32;
        for i in 0..10 {
            let next_val = lcg_forward(val);
            let back = lcg_reverse(next_val);
            assert_eq!(back, val, "Round trip failed at step {}", i);
            val = next_val;
        }
        println!("LCG forward/reverse: all round trips passed");
    }

    /// Test concrete LCG matches libc
    #[test]
    fn test_concrete_lcg_matches_libc() {
        let seeds = [1u32, 12345, 0x2B6043C, 42];

        for seed in seeds {
            unsafe { libc::srand(seed) };
            let libc_rand = unsafe { libc::rand() } as u32;

            let mut lcg = ConcreteLcg::srand(seed);
            let our_rand = lcg.rand();

            println!("Seed {:#X}: libc={:#X}, ours={:#X}", seed, libc_rand, our_rand);
            assert_eq!(libc_rand, our_rand, "Mismatch for seed {:#X}", seed);
        }
    }

    /// Test brute force can find a seed
    #[test]
    fn test_brute_force_small() {
        // Get a rand value from a small seed
        let original_seed = 1000u32;
        let mut lcg = ConcreteLcg::srand(original_seed);
        let target = lcg.rand();

        println!("Target: {:#X} from seed {}", target, original_seed);

        // Brute force should find it
        let found = brute_force_seed(target, 10000);
        assert!(found.is_some(), "Should find seed");
        println!("Found seed: {}", found.unwrap());

        // Verify
        let mut verify = ConcreteLcg::srand(found.unwrap());
        assert_eq!(verify.rand(), target, "Found seed should produce target");
    }

    /// Test parallel brute force (this may take a while for large seeds)
    #[test]
    fn test_brute_force_parallel() {
        let original_seed = 0x2B6043Cu32;
        let mut lcg = ConcreteLcg::srand(original_seed);
        let target = lcg.rand();

        println!("Target: {:#X} from seed {:#X}", target, original_seed);
        println!("Starting parallel brute force...");

        let start = std::time::Instant::now();
        let found = brute_force_seed_parallel(target);
        let elapsed = start.elapsed();

        println!("Elapsed: {:?}", elapsed);

        match found {
            Some(seed) => {
                println!("Found seed: {:#X}", seed);
                let mut verify = ConcreteLcg::srand(seed);
                assert_eq!(verify.rand(), target, "Found seed should produce target");
            }
            None => {
                panic!("Failed to find seed");
            }
        }
    }

    /// Test if Z3 can reverse a single LCG step
    #[test]
    #[ignore] // Z3 can't handle division/modulo efficiently
    fn test_reverse_single_lcg() {
        let cfg = Config::new();
        let ctx = Context::new(&cfg);
        let solver = Solver::new(&ctx);

        // Compute a concrete LCG step: next = (16807 * 12345) % 2147483647
        let seed: u64 = 12345;
        let next = (16807u64 * seed) % 2147483647u64;
        println!("Concrete: {} -> {}", seed, next);

        // Now try to reverse: given next, find seed
        let sym_seed = BV::new_const(&ctx, "seed", 32);

        // Build LCG symbolically
        let c_127773 = BV::from_u64(&ctx, 127773, 32);
        let c_16807 = BV::from_u64(&ctx, 16807, 32);
        let c_2836 = BV::from_u64(&ctx, 2836, 32);
        let c_mod = BV::from_u64(&ctx, 2147483647, 32);

        let hi = sym_seed.bvudiv(&c_127773);
        let lo = sym_seed.bvurem(&c_127773);
        let term1 = lo.bvmul(&c_16807);
        let term2 = hi.bvmul(&c_2836);

        let term1_64 = term1.zero_ext(32);
        let term2_64 = term2.zero_ext(32);
        let c_mod_64 = c_mod.zero_ext(32);
        let diff_64 = term1_64.bvsub(&term2_64);
        let is_negative = term1.bvult(&term2);
        let adjusted_64 = diff_64.bvadd(&c_mod_64);
        let result_64 = is_negative.ite(&adjusted_64, &diff_64);
        let result = result_64.extract(31, 0);

        // Constrain result to known value
        solver.assert(&result._eq(&BV::from_u64(&ctx, next, 32)));

        let mut params = z3::Params::new(&ctx);
        params.set_u32("timeout", 10000);
        solver.set_params(&params);

        println!("Solving single LCG reversal...");
        match solver.check() {
            SatResult::Sat => {
                let model = solver.get_model().unwrap();
                let solved = model.eval(&sym_seed, true).unwrap().as_u64().unwrap();
                println!("Solved seed: {}", solved);
                assert_eq!(solved, seed, "Should find original seed");
            }
            SatResult::Unsat => {
                panic!("UNSAT - unexpected");
            }
            SatResult::Unknown => {
                panic!("Timeout on single LCG step!");
            }
        }
    }

    /// Test solving with longer timeout
    #[test]
    #[ignore] // Takes too long
    fn test_solve_for_seed_long() {
        // First, get a real rand value from a known seed
        let original_seed = 1u32; // Simple seed
        unsafe { libc::srand(original_seed) };
        let target_rand = unsafe { libc::rand() } as u32;

        println!("Original seed: {:#X}", original_seed);
        println!("Target rand value: {:#X}", target_rand);

        // Try with 5 minute timeout
        let solved_seed = solve_seed_for_rand_with_timeout(target_rand, 300000);

        match solved_seed {
            Some(seed) => {
                println!("Solved seed: {:#X}", seed);

                // Verify the solved seed produces the same rand value
                unsafe { libc::srand(seed) };
                let verify_rand = unsafe { libc::rand() } as u32;
                println!("Verification rand: {:#X}", verify_rand);

                assert_eq!(target_rand, verify_rand,
                    "Solved seed {:#X} doesn't produce target rand {:#X}",
                    seed, target_rand);
            }
            None => {
                panic!("Failed to solve for seed");
            }
        }
    }

    /// Test that seed=0 is treated as seed=1
    #[test]
    fn test_zero_seed() {
        // libc treats seed 0 as seed 1
        unsafe { libc::srand(0) };
        let libc_rand_0 = unsafe { libc::rand() } as u32;

        unsafe { libc::srand(1) };
        let libc_rand_1 = unsafe { libc::rand() } as u32;

        assert_eq!(libc_rand_0, libc_rand_1, "libc: seed 0 should equal seed 1");

        // Z3 should do the same
        let cfg = Config::new();
        let ctx = Context::new(&cfg);

        let mut lcg_0 = Z3Lcg::srand(&ctx, LcgValue::concrete(0));
        let mut lcg_1 = Z3Lcg::srand(&ctx, LcgValue::concrete(1));

        let solver = Solver::new(&ctx);
        solver.check();
        let model = solver.get_model().unwrap();

        let z3_rand_0 = model.eval(&lcg_0.rand(), true).unwrap().as_u64().unwrap() as u32;
        let z3_rand_1 = model.eval(&lcg_1.rand(), true).unwrap().as_u64().unwrap() as u32;

        assert_eq!(z3_rand_0, z3_rand_1, "Z3: seed 0 should equal seed 1");
        assert_eq!(libc_rand_0, z3_rand_0, "Z3 should match libc for seed 0/1");
    }
}
