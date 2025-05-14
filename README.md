Intra-Procedural Constant Propagation

## Overview

This assignment implements the **constant propagation analysis phase** over LLVM IR as part of the course *Introduction to Program Analysis and Compiler Optimization*. The goal is to statically evaluate constant values in a program and propagate them through its control flow graph to enable compile-time optimizations.

Constant propagation is a classical dataflow analysis technique where variables assigned constant values are replaced by those constants in subsequent instructions, wherever safe.

## Objective

- Implement an intra-procedural constant propagation analysis pass using the LLVM framework.
- Track and propagate constant values across variables in a program.
- Output the constant value information for each LLVM instruction.

## Features

- **Dataflow Lattice**: Utilizes a domain of `{TOP, Z (a constant value), BOTTOM}` to represent the abstract state of each variable.
- **CFG Traversal**: Propagates values through control flow to find stable (fixed-point) constant assignments.
- **LLVM Pass**: Implemented as a custom LLVM analysis pass, analyzing `.ll` files and generating constant information.
- **Accurate Tracking**: Replaces variables in expressions with known constants wherever applicable.

## Directory Structure

assignment-2/ ├── assign/ # Contains input LLVM IR (.ll) files ├── output/ # Contains generated output with constant value analysis ├── cp_lib.cpp # Your constant propagation pass implementation ├── checker.sh # Script to verify structure and naming conventions

shell
Copy
Edit

## Example

### Input LLVM IR (`main.ll`)
```llvm
%a = alloca i32
store i32 2, i32* %a
%0 = load i32, i32* %a
%add = add i32 %0, 4
Output (main.txt)
perl
Copy
Edit
%a = 2
%0 = 2
%add = 6
How to Run
Build your LLVM pass inside the llvm-project folder.

Run the analysis pass on .ll files located in the assign/ directory.

Ensure that output .txt files are created in the output/ directory with the same base filename.


