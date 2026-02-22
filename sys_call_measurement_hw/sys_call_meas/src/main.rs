use std::io::Read;
use std::fs::File;
use std::time::Instant;

const ITERATIONS: u64 = 10000;

fn zero_byte_read() {
    let mut file = File::open("test.txt").unwrap();
    let mut buf = [0u8; 0];

    let start = Instant::now();

    let mut counter = 0;
    while counter < ITERATIONS {
        let n = file.read(&mut buf).unwrap();
        println!("Read {} bytes: {:?}", n, &buf[..n]);
        counter += 1;
    }

    let elapsed = start.elapsed();
    println!("Sys call took {} us per iteration!", elapsed.as_micros() as u64 / ITERATIONS);
}

fn main() {
    zero_byte_read();
}
