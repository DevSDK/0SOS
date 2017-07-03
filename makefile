all: BootLoader Kernel32 Kernel64 disk.img


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

Kernel64:
	@echo
	@echo ===================   Build Start   ====================	
	@echo
	make -C 02_Kernel64
	@echo
	@echo ===================  Build complete  ===================
	@echo


disk.img:	00_BootLoader/BootLoader.bin 01_Kernel32/Kernel32.bin 02_Kernel64/Kernel64.bin
	@echo
	@echo ================ Disk Image Build ==============
	@echo
	
	04_Utility/ImageMake/ImageMake $^
	@echo
	@echo ================ AllBuild Complate ==============
	@echo

clean:
	make -C 00_BootLoader clean
	make -C 01_Kernel32 clean
	make -C 02_Kernel64 clean
	rm -f disk.img
