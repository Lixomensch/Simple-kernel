# Simple-kernel

A modular 32-bit x86 operating system kernel written in C and Assembly.

## 🚀 Features

- **Architecture (x86)**: Custom IDT, ISR, and PIC management.
- **Memory Management**: 
  - Physical Memory Manager (PMM) with bitmap-based allocation.
  - Paging support with identity mapping for the kernel and dynamic mapping capabilities.
  - Kernel Heap (kmalloc/kfree) using a best-fit allocator.
- **File System**:
  - Virtual File System (VFS) abstraction layer.
  - Dynamic RAMFS integrated with the VFS, supporting file creation and read/write operations.
- **Drivers**:
  - VGA Text Mode driver with scrolling and color support.
  - Keyboard driver with scancode mapping and Shift/CapsLock support.
  - PIT (Programmable Interval Timer) management.
  - ATA driver (Read-only support for IDE disks).
- **User Interface**:
  - Internal Shell with support for commands (`ls`, `touch`, `write`, `cat`, `meminfo`, `uptime`, etc.).
- **Stability**:
  - Detailed Page Fault handler (ISR 14) with register and address info.
  - `KASSERT` macro for robust internal validation.

## 🛠️ Requirements

To build and run this project, you need:

- `gcc` (with multilib support for 32-bit)
- `binutils` (as, ld)
- `make`
- `qemu-system-i386` (for emulation)

## ⚙️ Build and Run

### 1. Build the Kernel
The build process compiles each module independently into the `build/` directory.

```bash
make
```

### 2. Run in QEMU
You can build and run the kernel in one command:

```bash
make run
```

### 3. Cleanup
To remove all build artifacts:

```bash
make clean
```

## 📜 License

This project is open-source and licensed under the MIT License.

---
*Based on the original work by TerminalRoot (https://github.com/terroo/terminalroot-os)*
