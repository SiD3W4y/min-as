# Min-as
Min language assembler
## Documentation
## What is it ?
This project is an assembler for the min assembly language used by min-vm. It is a complete rewrite of a previous iteration (called min-cc), but this time in c++ with the goal of having a better stability and user experience. It can produce a radare2 symbol file to make debugging work easier.

## Installing
```
$ cmake .
$ make
```
## Usage

```
./min-as {OPTIONS}

    min-as : The min assembler

  OPTIONS:

      --help, --h                       Displays this menu
      --symbols, --s                    Dumps a radare2 file with min object
                                        symbols
      --input=[input file], --i=[input
      file]                             Specifies the input .min asm file
      --output=[output file],
      --o=[output file]                 Specifies the output .mx file (default a.mx)
```

Importing symbols into radare2 :
```
$ r2 -i symbols.r2 <file.mx>
```

## Roadmap
- [ ] Error logging
- [ ] Code cleaning
- [X] Command line arguments
- [X] Non file based stream

- [x] Lexer
- [x] Parser
- [x] Codegen

