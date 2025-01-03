use nix::unistd::{fork, ForkResult};

use std::fs::File;
use std::io::{self, Write};

unsafe fn fork_function() -> io::Result<()> {
    // Open up a file descriptor.
    let t_path = "file.txt";
    let mut t_file = File::create(t_path)?;

    // Data to write to the file
    let mut t_text = "Hello, this is a test message from, ".to_string() + &std::process::id().to_string() + "!\n";

    // Write data to a file
    t_file.write_all(t_text.as_bytes())?;

    // Fork the current process
    match fork() {
        Ok(ForkResult::Parent { .. }) => {
            // Data to write to the file
            t_text = "Hello, this is a test message from, ".to_string() + &std::process::id().to_string() + "!\n";
            t_file.write_all(t_text.as_bytes())
        }
        Ok(ForkResult::Child) => {
            // Data to write to the file
            t_text = "Hello, this is a test message from, ".to_string() + &std::process::id().to_string() + "!\n";
            t_file.write_all(t_text.as_bytes())
        }
        Err(e) => {
            eprintln!("Fork failed: {}", e);
            Ok(())
        }
    }
}

fn main() {
    unsafe {
        match fork_function() {
            Ok(_) => {},
            Err(e) => {eprintln!("Fork function failed {}.", e)}
        };
    }
}
