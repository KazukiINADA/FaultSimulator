# simulator_template
This repository is a template for creating a fault simulator.

# Usage(for template)
1. Create a repository for your fault simulator implementation from this template repository.
1. Update README.md. Change the title to your own repository name and delete `Usage(for template)`.  
Also, please update the repository description below the title.

Then feel free to develop!  
For example, you could split the C code, add a Makefile, or implement it in C++ or Rust.  

# Usage
```bash
$ gcc main.c
$ ./a.out
Hello, World!
```

# Environment
- OS: Ubuntu20.04 LTS (GNU/Linux)
- Language: C
- GCC: gcc (Ubuntu 9.3.0-10ubuntu2) 9.3.0
- Memory: 32GB
- CPU: Intel(R) Core(TM) i7-4790 CPU @ 3.60GHz

# File & Dir Structure
```bash
$ tree simulator_template
simulator_template/
├── README.md
├── iscas85
│   ├── Faultset
│   ├── Logic
│   ├── Pattern
│   ├── Table
│   └── Testcase
├── iscas89_cs
│   ├── Faultset
│   ├── Logic
│   ├── Pattern
│   ├── Table
│   └── Testcase
├── main.c
├── simulation_result.pdf
└── time.csv
```

## File
- main.c  
template C code
- simulation_result.pdf  
Verification data for fault simulator implementation
- time.csv  
Execution time of the implemented fault simulator

## Benchmark files
- iscas85, iscas89_cs
    - Faultset  
    Fault information
    - Logic  
    Logic simulator results
    - Pattern  
    Pattern to be executed by simulator
    - Table  
    Circuit information
    - Testcase  
    Test case for fault simulator
