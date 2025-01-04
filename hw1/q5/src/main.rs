use nix::unistd::{fork, ForkResult, getpid};
use nix::sys::wait::{wait, WaitStatus};

use std::process::exit;

use std::thread;
use std::time::Duration;

unsafe fn fork_function() {
    // Fork the current process
    match fork() {
        Ok(ForkResult::Parent { .. }) => {
            // In the parent process, wait for any child to finish
            println!("Parent process (PID: {}), waiting for any child process.", getpid());

            // Wait for any child process to terminate
            let status = wait().expect("Failed to wait for child process");

            match status {
                WaitStatus::Exited(pid, exit_code) => {
                    println!("Child process (PID: {}) exited with status: {}", pid, exit_code);
                }
                WaitStatus::Signaled(pid, signal, _) => {
                    println!("Child process (PID: {}) was killed by signal: {}", pid, signal);
                }
                _ => {
                    println!("Child process did not exit normally.");
                }
            }
            println!("goodbye");
        }
        Ok(ForkResult::Child) => {
            // sleep to simulate work
            // Sleep for 2 seconds
            thread::sleep(Duration::new(2, 0));
            println!("hello");

            exit(0);  // Child exits here

        }
        Err(e) => {
            eprintln!("Fork failed: {}", e);
            exit(1);
        }
    }
}

fn main() {
    unsafe {
        fork_function();
    }
}
