use std::ffi::CString;

use nix::unistd::{fork, ForkResult};
use std::ptr;

use nix::sys::wait::waitpid;

use libc;

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

fn fork_function_with_execl() -> Result<(), Box<dyn std::error::Error>> {
    // Fork the current process
    match unsafe { fork() }? {
        ForkResult::Parent { child, .. } => {
            // In the parent process, wait for the child process to finish
            println!("Parent process, waiting for child...");
            waitpid(child, None)?;
            println!("Child process finished");
        }
        ForkResult::Child => {
            // In the child process, replace the current process with /bin/ls

            // Prepare arguments for execl
            let program = CString::new("/bin/ls")?;
            let arg1 = CString::new("/bin/ls")?;  // The first argument is usually the name of the program itself
            let arg2 = CString::new("-l")?;       // The second argument is the `-l` flag

            // Call execl to replace the current process with /bin/ls
            unsafe {
                libc::execl(
                    program.as_ptr(),
                    arg1.as_ptr(),
                    arg2.as_ptr(),
                    ptr::null::<CString>(),
                );
            }

            // If execl returns, something went wrong
            eprintln!("execl failed");
            std::process::exit(1);

        }
    }

    Ok(())
}

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

fn fork_function_with_execlp() -> Result<(), Box<dyn std::error::Error>> {
    // Fork the current process
    match unsafe { fork() }? {
        ForkResult::Parent { child, .. } => {
            // In the parent process, wait for the child process to finish
            println!("Parent process, waiting for child...");
            waitpid(child, None)?;
            println!("Child process finished");
        }
        ForkResult::Child => {
            // In the child process, replace the current process with /bin/ls

            // Prepare arguments for execl
            let program = CString::new("/bin/ls")?;
            let arg1 = CString::new("/bin/ls")?;  // The first argument is usually the name of the program itself
            let arg2 = CString::new("-l")?;       // The second argument is the `-l` flag

            // Call execl to replace the current process with /bin/ls
            unsafe {
                libc::execlp(
                    program.as_ptr(),
                    arg1.as_ptr(),
                    arg2.as_ptr(),
                    ptr::null::<CString>(),
                );
            }

            // If execlp returns, something went wrong
            eprintln!("execlp failed");
            std::process::exit(1);

        }
    }

    Ok(())
}

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

fn fork_function_with_execle() -> Result<(), Box<dyn std::error::Error>> {
    // Fork the current process
    match unsafe { fork() }? {
        ForkResult::Parent { child, .. } => {
            // In the parent process, wait for the child process to finish
            println!("Parent process, waiting for child...");
            waitpid(child, None)?;
            println!("Child process finished");
        }
        ForkResult::Child => {
            // Command to execute: "/bin/ls"
            let path = CString::new("/bin/ls").expect("CString::new failed");

            // Arguments to pass to the executable
            let args = [
                CString::new("ls").expect("CString::new failed"),
                CString::new("-l").expect("CString::new failed"),
            ];

            // Environment (we'll just pass a minimal environment)
            let env = [
                CString::new("PATH=/bin").expect("CString::new failed"),
            ];

            // Call execle with the path to the executable, arguments, and environment
            // TODO:  There is an issue here with the environment...
            unsafe {
                libc::execle(
                    path.as_ptr(),
                    args[0].as_ptr(), args[1].as_ptr(), ptr::null::<CString>(),
                    env[0].as_ptr(),
                );
            }

            // If execle returns, something went wrong
            eprintln!("execle failed: {}", std::io::Error::last_os_error());
            std::process::exit(1);

        }
    }

    Ok(())
}

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

fn fork_function_with_execv() -> Result<(), Box<dyn std::error::Error>> {
    // Fork the current process
    match unsafe { fork() }? {
        ForkResult::Parent { child, .. } => {
            // In the parent process, wait for the child process to finish
            println!("Parent process, waiting for child...");
            waitpid(child, None)?;
            println!("Child process finished");
        }
        ForkResult::Child => {
            // Path to the executable you want to run
            let path = "/bin/ls";

            // Arguments to pass to /bin/ls, typically empty args or something like `["ls", "-l"]`
            let args = ["ls", "-l"];

            // Convert the arguments to CString (null-terminated)
            let c_path = CString::new(path).expect("CString::new failed");
            let c_args: Vec<CString> = args.iter().map(|&arg| CString::new(arg).expect("CString::new failed")).collect();

            // Prepare the arguments as a null-terminated array of pointers
            let mut c_args_ptr: Vec<*const i8> = c_args.iter().map(|arg| arg.as_ptr()).collect();
            c_args_ptr.push(ptr::null()); // Null-terminate the array

            // Call execv to replace the current process with /bin/ls
            unsafe {
                let result = libc::execv(c_path.as_ptr(), c_args_ptr.as_mut_ptr());

                // If execv fails, we will return here and handle the error
                if result == -1 {
                    eprintln!("execv failed: {}", std::io::Error::last_os_error());
                }
            }

        }
    }

    Ok(())
}

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

fn fork_function_with_execvp() -> Result<(), Box<dyn std::error::Error>> {
    // Fork the current process
    match unsafe { fork() }? {
        ForkResult::Parent { child, .. } => {
            // In the parent process, wait for the child process to finish
            println!("Parent process, waiting for child...");
            waitpid(child, None)?;
            println!("Child process finished");
        }
        ForkResult::Child => {
            // Path to the executable you want to run
            let path = "/bin/ls";

            // Arguments to pass to /bin/ls, typically empty args or something like `["ls", "-l"]`
            let args = ["ls", "-l"];

            // Convert the arguments to CString (null-terminated)
            let c_path = CString::new(path).expect("CString::new failed");
            let c_args: Vec<CString> = args.iter().map(|&arg| CString::new(arg).expect("CString::new failed")).collect();

            // Prepare the arguments as a null-terminated array of pointers
            let mut c_args_ptr: Vec<*const i8> = c_args.iter().map(|arg| arg.as_ptr()).collect();
            c_args_ptr.push(ptr::null()); // Null-terminate the array

            // Call execvp to replace the current process with /bin/ls
            unsafe {
                let result = libc::execvp(c_path.as_ptr(), c_args_ptr.as_mut_ptr());

                // If execvp fails, we will return here and handle the error
                if result == -1 {
                    eprintln!("execvp failed: {}", std::io::Error::last_os_error());
                }
            }

        }
    }

    Ok(())
}

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

fn fork_function_with_execvpe() -> Result<(), Box<dyn std::error::Error>> {
    // Fork the current process
    match unsafe { fork() }? {
        ForkResult::Parent { child, .. } => {
            // In the parent process, wait for the child process to finish
            println!("Parent process, waiting for child...");
            waitpid(child, None)?;
            println!("Child process finished");
        }
        ForkResult::Child => {
            // Path to the executable you want to run
            let path = "/bin/ls";

            // Arguments to pass to /bin/ls, typically empty args or something like `["ls", "-l"]`
            let args = ["ls", "-l"];
            let envs = ["PATH=/bin"];

            // Convert the arguments to CString (null-terminated)
            let c_path = CString::new(path).expect("CString::new failed");
            let c_args: Vec<CString> = args.iter().map(|&arg| CString::new(arg).expect("CString::new failed")).collect();
            let c_envs: Vec<CString> = envs.iter().map(|&env| CString::new(env).expect("CString::new failed")).collect();

            // Prepare the arguments as a null-terminated array of pointers
            let mut c_args_ptr: Vec<*const i8> = c_args.iter().map(|arg| arg.as_ptr()).collect();
            c_args_ptr.push(ptr::null()); // Null-terminate the array

            let mut c_envs_ptr: Vec<*const i8> = c_envs.iter().map(|env| env.as_ptr()).collect();
            c_envs_ptr.push(ptr::null()); // Null-terminate the array

            // Call execvp to replace the current process with /bin/ls
            unsafe {
                // execvpe not portable on my system
                //let result = libc::execvpe(c_path.as_ptr(), c_args_ptr.as_mut_ptr(), c_envs_ptr.as_mut_ptr());

                // If execvpe fails, we will return here and handle the error
                //if result == -1 {
                //    eprintln!("execvpe failed: {}", std::io::Error::last_os_error());
                //}
            }

        }
    }

    Ok(())
}

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------

fn main() {
    match fork_function_with_execl() {
        Ok(_) => {},
        Err(e) => {eprintln!("Fork function failed {}.", e)}
    };
    match fork_function_with_execlp() {
        Ok(_) => {},
        Err(e) => {eprintln!("Fork function failed {}.", e)}
    };
    match fork_function_with_execle() {
        Ok(_) => {},
        Err(e) => {eprintln!("Fork function failed {}.", e)}
    };
    match fork_function_with_execv() {
        Ok(_) => {},
        Err(e) => {eprintln!("Fork function failed {}.", e)}
    };
    match fork_function_with_execvp() {
        Ok(_) => {},
        Err(e) => {eprintln!("Fork function failed {}.", e)}
    };
    match fork_function_with_execvpe() {
        Ok(_) => {},
        Err(e) => {eprintln!("Fork function failed {}.", e)}
    };
}
