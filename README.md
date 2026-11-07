# SQA-Steam-Clone

Course project for CSCI 3060U (Software Quality Assurance) - a command-line game buy/sell system.

## Project layout

```
SQA-Steam-Clone/
├── Phase2/                 Front end source code (C++), CMakeLists.txt
│   └── src/
├── Phase3/                 Front end requirements testing
│   ├── phase1_tests/       One folder per test: input.txt, output.txt (expected), expected.atf (optional)
│   ├── test_data/          currentaccounts.txt, availablegames.txt
│   ├── build.sh            Compiles the front end
│   ├── run_tests.sh        Runs every test, saves actual results
│   ├── check_tests.sh      Compares actual vs expected results
│   ├── run_all.sh          Build + run + check in one command
│   └── README.md           Full instructions for the TA
├── docs/                   Design document, UML diagram, test case descriptions
└── .vscode/                Editor settings (build task compiles ALL source files)
```

## Quick start

Build and run (any OS with g++; on Windows use Git Bash):

```
cd Phase3
./run_all.sh "Run 1"
```

See `Phase3/README.md` for details and for how to run the program by hand.
