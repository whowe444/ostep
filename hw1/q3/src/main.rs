use nix::unistd::{pipe, fork, ForkResult, write, read};
use std::process::exit;
use std::io::{self};

unsafe fn fork_function() -> io::Result<()> {
    // Create a pipe (returns a tuple of two file descriptors: read and write ends)
    let (reader, writer) = pipe()?;

    // Fork the current process
    match fork() {
        Ok(ForkResult::Parent { .. }) => {
            // Close the writer end in the parent, since we are only reading.
            nix::unistd::close(writer)?;

            // Create a buffer to read from the pipe.
            let mut buffer = [0u8; 128];
            let bytes_read = read(reader, &mut buffer)?;
            let _message = String::from_utf8_lossy(&buffer[..bytes_read]);

            println!("goodbye");

            // Close the pipe reader.
            nix::unistd::close(reader)?;
        }
        Ok(ForkResult::Child) => {
            println!("hello");

            nix::unistd::close(reader)?;

            let message = "Hello from child process!";
            // Write to the pipe (from child to parent)
            write(writer, message.as_bytes())?;
            
            // Close the pipe writer.
            nix::unistd::close(writer)?;
        }
        Err(e) => {
            eprintln!("Fork failed: {}", e);
            exit(1);
        }
    }
    Ok(())
}

fn main() {
    unsafe {
        match fork_function() {
            Ok(_) => {},
            Err(e) => {eprintln!("Fork function failed {}.", e)}
        };
    }
}
