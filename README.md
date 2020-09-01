# simulator_template
This repository is a template for creating a fault simulator.

#作成ファイル一覧
・'dedu_fault_simulator_e.c'、'dedu_fault_simulator_cs.c':演繹法を取り入れた故障シミュレーション(高速化には至らず)
・'easy_fault_e.c'、'easy_fault_cs.c':高速化なしの故障シミュレーション
・'easy_fault_list_e.c'、'easy_fault_list_cs.c':高速化なしの故障シミュレーション(高速化との時間比較に利用、故障リストを利用)
・'logic_simulator_e.c'、'logic_simulator_cs.c':論理シミュレーション
・'para_fault_simulator_e.c'、`para_fault_simulator_cs.c`:テストパターンを並列化させることで高速化させた故障シミュレーション（longのついたものはunsigned long long int 型で並列化）
・'para2_fault_simulator_e.c'、'para2_fault_simulator_cs.c':故障パターンを並列化させることで高速化させた故障シミュレーション（longのついたものはunsigned long long int 型で並列化）

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
