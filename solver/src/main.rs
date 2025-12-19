#![allow(warnings)]

mod dispatcher;
mod stackvm;
mod regvm;
use dispatcher::dispatcher;
use std::env;
use std::fs;


fn main() {
    // == FILE INTAKE == 
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        println!("Usage: {} <filename>",args[0]);
        std::process::exit(1);
    }

    let filename = &args[1];

    // read in
    let masm_data: Vec<u8> = fs::read(filename).expect("Failed to read file");
    println!("Read in {} bytes from {}", masm_data.len(), filename);

    // get mapped version of data
    let mapped_masm_sections: MappedMasmSections = map_segments(&masm_data);

    // start disassembling
    dispatcher(&mapped_masm_sections);
}

struct SegmentEntry {
    segment_type: u8,
    raw_addr: u16,
    raw_size: u16
}

struct MappedMasmSections {
    text: Vec<u8>,
    data: Vec<u8>,
    arch: Vec<u8>
}

// there are 3 segments defined in the segment header.
// they are 5 bytes in length and are at byte positions 4, 9, and 0xe(14)
fn map_segments(masm_data: &Vec<u8>) -> MappedMasmSections {
    let mut mapped_masm_sections = MappedMasmSections {
        text: vec![0;0x1000],
        data: vec![0;0x1000],
        arch: vec![0;0x1000]
    };
    let mut fpos:usize = 4; // starts at 4 after MASM header
    let mut segment_vec:Vec<SegmentEntry> = Vec::new();

    for i in 0..3 { // VM only supports 3 sections
        let segment = SegmentEntry {
            segment_type: masm_data[fpos],
            raw_addr: u16::from_le_bytes([masm_data[fpos+1],masm_data[fpos+2]]),
            raw_size: u16::from_le_bytes([masm_data[fpos+3],masm_data[fpos+4]]),
        };

        println!("section type:{}, section addr:{}, section size: {}", segment.segment_type, segment.raw_addr, segment.raw_size);
        segment_vec.push(segment);
        fpos += 5;
    }

    for segment in segment_vec {
        if segment.segment_type == 1{
            for i in 0..segment.raw_size as usize {
                mapped_masm_sections.text[i] = masm_data[segment.raw_addr as usize + i];
            }
        } else if segment.segment_type == 2{
            for i in 0..segment.raw_size as usize {
                mapped_masm_sections.data[i] = masm_data[segment.raw_addr as usize + i];
            }
        } else if segment.segment_type == 3{
            for i in 0..segment.raw_size as usize{
                mapped_masm_sections.arch[i] = masm_data[segment.raw_addr as usize + i];
            }
        } else {
            panic!("invalid segment");
        }
    }

    return mapped_masm_sections
}
