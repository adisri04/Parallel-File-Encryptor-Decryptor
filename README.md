# Parallel File Encryptor/Decryptor in C++ (Using Multi Processing)
<img width="1133" alt="Screenshot 2024-08-08 at 2 27 40 PM" src="https://github.com/user-attachments/assets/5e7ad9eb-292c-44d8-98ca-882482f20c11">

## Overview

This C++ project is a command-line utility that allows for the parallel encryption and decryption of files using a variety of encryption algorithms. It leverages the Semaphore synchronization primitive, multiprocessing, and fork system calls to distribute the workload across multiple CPU cores for maximum efficiency.

### Features

- Parallel processing to maximize CPU utilization
- Progress reporting during long-running operations
- Configurable block size and number of worker processes
- Supports both encryption and decryption operations

## Requirements

- C++17 or newer

