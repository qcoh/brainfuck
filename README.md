# brainfuck

A small brainfuck compiler targeting x86\_64 Linux.

The main purpose of this is to learn some assembly. Comments and criticism very welcome!

## Usage

    ./brainfuck program.bf
    nasm -felf64 out.s && ld out.o
    ./a.out
