use std::env;
use std::thread::sleep;
use std::time::Duration;

fn use_memory(mib: f64) {
    let bytes = (mib * 1024.0 * 1024.0) as usize;
    let count = bytes / std::mem::size_of::<i32>();

    println!("Allocating {:.4} MiB ({} bytes, {} ints)...", mib, bytes, count);

    let mut array: Vec<i32> = vec![0; count];

    let mut iteration = 0;
    loop {
        println!("Entering iteration {}", iteration);
        for (index, val) in array.iter_mut().enumerate() {
            *val = index as i32
        }
        iteration += 1;
        sleep(Duration::from_secs(1));
    }
}

fn main() {
    // Collect the cmd line arguments
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        eprintln!("Usage: {} <size_in_MiB>", args[0]);
        std::process::exit(1);
    }

    let memory_in_mebibytes: f64 = match args[1].parse() {
        Ok(v) => v,
        Err(e) => {
            eprintln!("Error: Invalid number '{}: {}", args[1], e);
            std::process::exit(1);
        }
    };

    if memory_in_mebibytes <= 0.0 {
        eprintln!("Error: Size must be greater than zero!");
        std::process::exit(1);
    }

    println!("Received: {} MiB", memory_in_mebibytes);

    use_memory(memory_in_mebibytes);
}