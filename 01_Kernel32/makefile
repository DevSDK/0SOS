all: Kernel32.bin

Kernel32.bin: Source/EntryPointKernel32.asm

	nasm -o Kernel32.bin $<

clean:
	rm -f Kernel32.bin
