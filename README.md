# Command Line Tokenizer in C

A lexical analyzer that reads a C source file and classifies every token — keywords, identifiers, operators, literals, and more.

## Project Structure

```
Tokenizer/
├── tokenizer.c   # lexical analyzer source
└── test.c        # input file to tokenize
```

## Requirements

- GCC compiler
- Linux / WSL / Mac / Windows

## Run

```bash
# Navigate to the folder
cd Tokenizer

# Compile
gcc tokenizer.c -o tokenizer

# Run
./tokenizer
```

The tokenizer reads from `test.c` by default. To use a different input file, change this line in `main()`:

```c
ptr = fopen("test.c", "r");
```
