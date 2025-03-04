# Simple Kernel

A minimal operating system kernel 32-bit written in C and Assembly, designed to run on x86 architecture.

## Features

- Basic bootstrapping with a custom bootloader
- Kernel written in C with a low-level Assembly entry point
- Multiboot-compliant

## Requirements

To build and run this project, you need:

- `gcc` (with multilib support for 32-bit compilation)
- `binutils` (including `as` and `ld`)
- `make`
- `qemu-system-i386` (for testing)

## Build and Run

### Compile the Kernel

```sh
make
```

### Run with QEMU

```sh
qemu-system-i386 -kernel build/kernel.bin
```

## License

This project is open-source and licensed under the MIT License.

## Credits

TerminalRoot
https://github.com/terroo/terminalroot-os
