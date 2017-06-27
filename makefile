all: BootLoader Kernel32 disk.img


BootLoader:
	@echo
	@echo ===================   Build Start   ====================	
	@echo
	make -C 00_BootLoader
	@echo
	@echo ===================  Build complete  ===================
	@echo

Kernel32:
	@echo
	@echo ===================   Build Start   ====================	
	@echo
	make -C 01_Kernel32
	@echo
	@echo ===================  Build complete  ===================
	@echo



disk.img: BootLoader Kernel32
	@echo
	@echo ================ Disk Image Build ==============
	@echo
	cat 00_BootLoader/BootLoader.bin 01_Kernel32/VirtualOS.bin > disk.img
	@echo
	@echo ================ AllBuild Complate ==============
	@echo

clean:
	make -C 00_BootLoader clean
	make -C 01_Kernel32 clean
	rm -f disk.img
