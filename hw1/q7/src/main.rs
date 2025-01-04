use nix::unistd::{fork, ForkResult};
use std::process::exit;

use std::io::{self, Write};

use libc;

unsafe fn fork_function() {
    // Fork the current process
    match fork() {
        Ok(ForkResult::Parent { .. }) => {
            println!("goodbye");
        }
        Ok(ForkResult::Child) => {
            unsafe {
                // Close the file descriptor for stdout (FD 1)
                libc::close(libc::STDOUT_FILENO);
            }
        
            // Try writing to stdout after closing it (this will fail)
            if let Err(e) = writeln!(io::stdout(), "hello") {
                eprintln!("Error writing to stdout: {}", e);
            }
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
