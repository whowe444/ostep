extern crate nix;

use nix::unistd::{fork, pipe, close, dup2, ForkResult};
use nix::sys::wait::{waitpid, WaitStatus};
use std::process;

use std::io;

use std::process::exit;

fn main() {
    // Create a pipe to connect stdout of the first child to stdin of the second
    let (reader, writer) = pipe().expect("Failed to create pipe");

    match unsafe { fork() } {
        Ok(ForkResult::Parent { child, .. }) => {
            // Wait for the first child to finish
            let status = waitpid(child, None).expect("Failed to wait for child");
            match status {
                WaitStatus::Exited(pid, exit_code) => {
                    println!("Child process (PID: {}) exited with status: {}", pid, exit_code);
                }
                _ => {
                    println!("Child process did not exit normally.");
                }
            }

            match unsafe { fork() } {
                Ok(ForkResult::Parent { child, .. }) => {
                    // In the parent, wait for the second child
                    let status = waitpid(child, None).expect("Failed to wait for second child");
                    match status {
                        WaitStatus::Exited(pid, exit_code) => {
                            println!("Second child process (PID: {}) exited with status: {}", pid, exit_code);
                        }
                        _ => {
                            println!("Second child process did not exit normally.");
                        }
                    }
                }
                Ok(ForkResult::Child) => {
                    close(writer).expect("Failed to close pipe writer");
                    // Second child: redirect stdin to the reader end of the pipe
                    dup2(reader, libc::STDIN_FILENO).expect("Failed to redirect stdin to pipe");

                    // Close the original reader since it is duplicated
                    close(reader).expect("Failed to close the pipe reader");

                    let mut input = String::new();
                
                    // Read input from stdin
                    io::stdin()
                        .read_line(&mut input)
                        .expect("Failed to read line");
                
                    // Trim the newline character and print the input
                    println!("{}", input);                
                }
                Err(err) => {
                    eprintln!("Failed to fork second child: {}", err);
                    process::exit(1);
                }
            }
        }
        Ok(ForkResult::Child) => {
            close(reader).expect("Failed to close pipe reader");
            // First child: redirect stdout to the writer end of the pipe
            dup2(writer, libc::STDOUT_FILENO).expect("Failed to redirect stdout to pipe");

            // Close the original writer since it is duplicated
            close(writer).expect("Failed to close pipe writer");

            let msg = "Hello from the first child";
            println!("{}\n", msg);
            exit(0);        }
        Err(err) => {
            eprintln!("Failed to fork: {}", err);
            process::exit(1);
        }
    }
}
