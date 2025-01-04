use nix::unistd::{fork, ForkResult, getpid};
use nix::sys::wait::waitpid;

use std::process::exit;

use std::thread;
use std::time::Duration;

unsafe fn fork_function() {
    // Fork the current process
    match fork() {
        Ok(ForkResult::Parent { child, .. }) => {
            // In the parent process, wait for the child to finish
            println!("Parent process (PID: {}), waiting for child PID: {}", getpid(), child);

            let status = waitpid(child, None).expect("Failed to wait for child process");

            match status {
                nix::sys::wait::WaitStatus::Exited(pid, exit_code) => {
                    println!("Child process (PID: {}) exited with status: {}", pid, exit_code);
                }
                nix::sys::wait::WaitStatus::Signaled(pid, signal, _) => {
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
