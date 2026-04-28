# POSIX Multithreading & Watchdog Timer

A system-level application that demonstrates advanced multithreading management, signal handling, and Thread Local Storage (TLS) in a Linux environment.

## Features
- **Watchdog Pattern:** The main thread acts as a monitor, managing the lifecycle of multiple worker threads and terminating them based on randomized lifetimes.
- **Signal Handling:** Implements asynchronous thread termination using `SIGUSR1` and `pthread_kill`.
- **Thread Local Storage (TLS):** Uses `pthread_key_create` and `pthread_getspecific` to maintain independent high-precision timers for each thread.
- **Dynamic Thread Management:** Configurable number of threads and execution time via command-line arguments.

## System Architecture
1. **Main Thread (Watchdog):** Spawns $N$ worker threads, assigns them a random lifetime, and monitors them in a loop. When a thread's time expires, the watchdog sends a signal to terminate it.
2. **Worker Threads:** Perform computational tasks while an internal timer tracks their specific CPU/real-time usage.
3. **Timer Library:** A custom implementation using `clock_gettime` to provide millisecond-precision performance metrics per thread.

## How to Build and Run

### Prerequisites
- GCC Compiler
- POSIX-compliant environment (Linux/Unix)

### Compilation
Compile the project using the following command:
```bash
gcc -Wall -Wextra -pthread main.c timer.c -o thread_manager
