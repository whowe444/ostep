use nix::unistd::{fork, ForkResult};
use std::process::exit;

unsafe fn fork_function() {
    // Start the value of x out at 10.
    let mut x = 10;
    println!("Value of x is: {} in process: {}", x, std::process::id());

    // Fork the current process
    match fork() {
        Ok(ForkResult::Parent { .. }) => {
            println!("Value of x is: {} in the parent.", x);
            x = 20;
            println!("Value of x is {} in the parent.", x);
        }
        Ok(ForkResult::Child) => {
            println!("Value of x is {} in the child.", x);

            x = 30;
            println!("Value of x is {} in the child.", x);

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
