GCC64 = gcc -g -std=c99 -C -m64 -ffreestanding
LD64 = ld -elf_x86-64  -nostdlib -T ../LinkerScript64.x
OBJCOPY64 = objcopy -j .text -j .data -j .rodata -j bss -S -O binary 
NASM64 = nasm -f elf64

OBJECTCOPYDIR = Obj
SOURCEDIR = Source


all: prepare Kernel64.bin

prepare:
	mkdir -p $(OBJECTCOPYDIR)

dep:
	@echo =========== Make Dependancy File ==============
	make -C $(OBJECTCOPYDIR) -f ../makefile InternalDependency
	@echo ======== Dependancy Search Complate ===========

ExecuteInternalBuild: dep
	make -C $(OBJECTCOPYDIR) -f ../makefile Kernel64.elf

Kernel64.bin: ExecuteInternalBuild
		$(OBJCOPY64) $(OBJECTCOPYDIR)/Kernel64.elf $@

clean:
	rm -rf *.bin
	rm -rf $(OBJECTCOPYDIR)/*.*

ENTRYPOINTFILE	= ../$(SOURCEDIR)/EntryPoint.s
ENTRYPOINTOBJ	=  EntryPoint.o
CSOURCEFILES = $(wildcard ../$(SOURCEDIR)/*.c)
ASSEMBLYSOURCEFILES = $(wildcard ../$(SOURCEDIR)/*.asm)
COBJECTFILES =  $(notdir $(patsubst %.c,%.o,$(CSOURCEFILES)))
ASSEMBLYOBJECTFILES =  $(notdir $(patsubst %.asm, %.o, $(ASSEMBLYSOURCEFILES)))

$(ENTRYPOINTOBJ): $(ENTRYPOINTFILE)
		$(NASM64) -o $@ $<

%.o: ../$(SOURCEDIR)/%.c
		$(GCC64) -c $<
%.o: ../$(SOURCEDIR)%.asm
		$(NASM64) -c $@ $<

InternalDependency:
	$(GCC64) -MM $(CSOURCEFILES) > Dependancy.dep
Kernel64.elf: $(ENTRYPOINTOBJ) $(COBJECTFILES) $(ASSEMBLYOBJECTFILES)
	$(LD64) -o $@ $^ 

ifeq (Dependancy.dep, $(wildcard Dependancy.dep))
include Dependancy.dep
endif
