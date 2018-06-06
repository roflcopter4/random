use std::*;

fn main()
{
        println!("Don't be a gigantic faggot.");
        let mut guess = String::new();
        io::stdin().read_line(&mut guess).expect("Fucked up.");

        println!("{}", guess);
}

// vim: sw=8 sts=8
