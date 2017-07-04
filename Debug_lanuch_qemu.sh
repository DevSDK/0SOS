qemu-system-x86_64 -L . -m 64 -S -gdb tcp::22225 -fda disk.img  -localtime -M pc -cpu Nehalem &
gdb -s 02_Kernel64/Obj/Kernel64.elf  --command=GDBInit.gb

