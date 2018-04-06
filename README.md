# Summary

The Operating System on x86_64 Architecture.
This source tree will be used to learn about the foundation and internal of Operating Systems.

## Completion of development
 
  BootLoader
  
  x86 Kernel 
  ```
    For start up x64_86 Kernel
  ```
   
  x86_64(IA32e) Kernel
  ``` 
    1. Memory Paging
    2. Interrupt Handling 
    3. Device Driver (Keyboard, Timer)
    4. Basical Librarys (Such as, printf, atoi, etc..)
    5. Context Switch 
    6. Scheduler (Round Robin, Timesliced)
```
  
  Shell
  

## Build

0. Prepare require environment
  
  QEMU, GCC, NASM, and more.
  
1. Build
 ```
 make -f makefile
 ```
 
 2. And Launch
 
 ```
    ./launch_qemu
 ```
 
[https://devsdk.github.io/tags/#0SOS](https://devsdk.github.io/tags/#0SOS)
 
 
