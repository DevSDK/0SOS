NASM32 = nasm
GCC32 = gcc -C -m32 -ffreestanding
LD32 = ld -elf_i386 -nostdlib -T ../../LinkerScript.x
OBJCOPY32 = objcopy -j .text -j .data -j .rodata -j bss -S -O binary 


OBJECTCOPYDIR = Temp
SOURCECOPYDIR = Source


all: prepare Kernel32.bin

prepare:
	mkdir -p $(OBJECTCOPYDIR)

$(OBJECTCOPYDIR)/EntryPoint.bin: $(SOURCECOPYDIR)/EntryPointKernel32.s
	$(NASM32) -o $@ $<

dep:
	@echo =========== Make Dependancy File ==============
	make -C $(OBJECTCOPYDIR) -f ../makefile InternalDependency
	@echo ======== Dependancy Search Complate ===========

ExecuteInternalBuild: dep
	make -C $(OBJECTCOPYDIR) -f ../makefile Kernel32.elf

$(OBJECTCOPYDIR)/Kernel32.elf.bin: ExecuteInternalBuild
	$(OBJCOPY32) $(OBJECTCOPYDIR)/Kernel32.elf $@

Kernel32.bin: $(OBJECTCOPYDIR)/EntryPoint.bin $(OBJECTCOPYDIR)/Kernel32.elf.bin
	cat $^ > $@

clean:
	rm -rf *.bin
	rm -rf $(OBJECTCOPYDIR)/*.*


CENTRYPOINTOBJECTFILE = Main.o
CSOURCEFILES = $(wildcard ../$(SOURCECOPYDIR)/*.c)
ASSEMBLYSOURCEFILES = $(wildcard ../$(SOURCECOPYDIR)/*.asm)
COBJECTFILES = $(subst Main.o, , $(notdir $(patsubst %.c,%.o,$(CSOURCEFILES))))
ASSEMBLYOBJECTFILES =  $(notdir $(patsubst %.asm, %.o, $(ASSEMBLYSOURCEFILES)))

%.o: ../$(SOURCECOPYDIR)/%.c
		$(GCC32) -c $<

%.o: ../$(SOURCECOPYDIR)/%.asm
		$(NASM32) -f elf32 -o $@ $<

InternalDependency:
	$(GCC32) -MM $(CSOURCEFILES) > Dependancy.dep
Kernel32.elf: $(CENTRYPOINTOBJECTFILE) $(COBJECTFILES) $(ASSEMBLYOBJECTFILES)
	$(LD32) -o $@ $^ 

ifeq (Dependancy.dep, $(wildcard Dependancy.dep))
include Dependancy.dep
endif
