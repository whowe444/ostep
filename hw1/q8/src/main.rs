extern crate nix;

use nix::unistd::{fork, pipe, execv, close, dup2, ForkResult};
use nix::sys::wait::{waitpid, WaitStatus};
use std::ffi::CString;
use std::process;

fn main() {
    // Create a pipe to connect stdout of the first child to stdin of the second
    let (reader, writer) = pipe().expect("Failed to create pipe");

    match unsafe { fork() } {
        Ok(ForkResult::Parent { child, .. }) => {
            // In the parent process, close the reader end of the pipe (we don't need it)
            close(reader).expect("Failed to close pipe reader");

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
                    // Second child: redirect stdin to the reader end of the pipe
                    close(writer).expect("Failed to close pipe writer");
                    let stdin_fd = reader;
                    dup2(stdin_fd, 0).expect("Failed to redirect stdin to pipe");

                    // Execute a command that reads from stdin (e.g., `cat` to print the input)
                    let cmd = CString::new("/bin/cat").expect("CString::new failed");
                    let args = [CString::new("cat").expect("CString::new failed")];
                    let args: Vec<*const i8> = args.iter().map(|arg| arg.as_ptr()).collect();
                    let mut args_with_null = args.clone();
                    args_with_null.push(std::ptr::null()); // Null-terminate the array

                    execv(&cmd, &args_with_null).expect("Failed to execute second child");
                }
                Err(err) => {
                    eprintln!("Failed to fork second child: {}", err);
                    process::exit(1);
                }
            }
        }
        Ok(ForkResult::Child) => {
            // First child: redirect stdout to the writer end of the pipe
            close(reader).expect("Failed to close pipe reader");
            let stdout_fd = writer;
            dup2(stdout_fd, 1).expect("Failed to redirect stdout to pipe");

            // Execute a command that writes to stdout (e.g., `echo`)
            let cmd = CString::new("/bin/echo").expect("CString::new failed");
            let args = [
                CString::new("Hello from first child!").expect("CString::new failed"),
            ];
            let args: Vec<*const i8> = args.iter().map(|arg| arg.as_ptr()).collect();
            let mut args_with_null = args.clone();
            args_with_null.push(std::ptr::null()); // Null-terminate the array

            execv(&cmd, &args_with_null).expect("Failed to execute first child");
        }
        Err(err) => {
            eprintln!("Failed to fork: {}", err);
            process::exit(1);
        }
    }
}
