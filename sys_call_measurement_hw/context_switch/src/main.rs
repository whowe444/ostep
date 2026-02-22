use nix::unistd::{pipe, fork, ForkResult, write, read};
use std::process::exit;
use std::io::{self};
use nix::sys::wait::wait;
use nix::libc;
use std::time::Instant;

const ITERATIONS: u64 = 10000;

fn set_affinity(cpu: usize) {
    unsafe {
        let mut mask: libc::cpu_set_t = std::mem::zeroed();
        libc::CPU_SET(cpu, &mut mask);

        let result = libc::sched_setaffinity(
            0,
            std::mem::size_of::<libc::cpu_set_t>(),
            &mask
        );

        if result != 0 {
            eprintln!("sched_setaffinity failed");
        }
    }
}

unsafe fn context_switch() -> io::Result<()> {
    // Create two pipes
    let (reader1, writer1) = pipe()?;
    let (reader2, writer2) = pipe()?;

    // Fork the current process
    match fork() {
        Ok(ForkResult::Parent { .. }) => {
            // If we are in the parent we need to fork again
            match fork() {
                Ok(ForkResult::Parent { .. }) => {
                    // If we are in the parent we don't do anything so we can 
                    // close our copy of the pipes.
                    nix::unistd::close(reader1)?;
                    nix::unistd::close(reader2)?;
                    nix::unistd::close(writer1)?;
                    nix::unistd::close(writer2)?;

                    // Wait for Process 2 to finish.
                    wait().unwrap();
                }
                Ok(ForkResult::Child) => {
                    set_affinity(0);

                    // If we are in the child we are Process 2.
                    nix::unistd::close(reader2)?;
                    nix::unistd::close(writer1)?;

                    let mut counter = 0;
                    while counter < ITERATIONS {
                        // Write to pipe2
                        let message = "Hello, Process 1!";
                        write(writer2, message.as_bytes())?;

                        // Read from pipe1
                        let mut buffer = [0u8; 128];
                        let bytes_read = read(reader1, &mut buffer)?;
                        let recv_message = String::from_utf8_lossy(&buffer[..bytes_read]);
                        println!("{}", recv_message);

                        counter += 1;
                    }

                    unsafe {libc::_exit(0) };
                }
                Err(e) => {
                    eprintln!("Fork failed: {}", e);
                    exit(1);
                }
            }

            // Wait for Process 1 to finish.
            wait().unwrap();
        }
        Ok(ForkResult::Child) => {
            // If we are in the child we are Process 1.
            nix::unistd::close(reader1)?;
            nix::unistd::close(writer2)?;

            set_affinity(0);

            let mut counter = 0;
            while counter < ITERATIONS {
                // Write to pipe1
                let message = "Hello, Process 2!";
                write(writer1, message.as_bytes())?;

                // Read from pipe2
                let mut buffer = [0u8; 128];
                let bytes_read = read(reader2, &mut buffer)?;
                let recv_message = String::from_utf8_lossy(&buffer[..bytes_read]);
                println!("{}", recv_message);

                counter += 1;
            }

            unsafe {libc::_exit(0) };
        }
        Err(e) => {
            eprintln!("Fork failed: {}", e);
            exit(1);
        }
    }
    Ok(())
}

fn main() {
    let start = Instant::now();
    unsafe {
        match context_switch() {
            Ok(_) => {},
            Err(e) => {eprintln!("Context switch function failed {}.", e)}
        };
    }
    let elapsed = start.elapsed();
    println!("Context switching took {} us per iteration!", elapsed.as_micros() as u64 / ITERATIONS)
}