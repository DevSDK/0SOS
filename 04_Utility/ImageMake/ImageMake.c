#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define SECTORSIZE 512


int AdjustmentInSectorSize(int Fd, int SourceSize);
void WriteKernelInfo(int Target, int KernelSelectorCount);
int CopyFile(int Source, int Target);
	

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		fprintf(stderr, "[ERROR] ImageMake BootLoader.bin Kernel32.bin\n");
		exit(-1);
	}
	
	int TargetFd = open("disk.img", O_RDWR | O_CREAT | O_TRUNC );

	if( TargetFd == -1)
	{
		fprintf(stderr,"[ERROR] disk.img open fail.\n");
		exit(-1);
	}
	
	//----------------------------------------------
	// Copy BootLoader Data to image 
	//----------------------------------------------

	printf("[INFO]  Copy BooLoader to image file\n");
	int SourceFd = open(argv[1], O_RDONLY);

	if( SourceFd == -1)
	{
		fprintf(stderr, "[ERROR] %s Open Fail. \n", argv[1]);
		exit(-1);
	}
	
	int SourceSize = CopyFile(SourceFd, TargetFd);
	close(SourceFd);
	// Align File Sector "512Byte" left memory fill 0x00.
	int BootLoaderSize = AdjustmentInSectorSize(TargetFd, SourceSize);


	//-----------------------------------------------
	//	copy Protected Mode Kernel to image file.
	//-----------------------------------------------
	printf("[INFO] Copy Protected mode kernel to image file\n");
	SourceFd = open( argv[2], O_RDONLY );

	if(SourceFd == -1)
	{
		fprintf(stderr, "[ERROR] %s Open Fail.\n", argv[2]);
		exit(-1);
	}

	SourceSize = CopyFile(SourceFd, TargetFd);
	close(SourceFd);
	
	//Left Memory Fill 0x00. For align 512 Byte
	int KernelSelectorCount = AdjustmentInSectorSize(TargetFd, SourceSize);
	printf("[INFO] %s Size = [%d] and Sector Count = [%d]\n", argv[2], SourceSize, KernelSelectorCount);
	
	printf("[INFO] Start to Write Kernel Information\n" );
	WriteKernelInfo(TargetFd, KernelSelectorCount);

	printf("[INFO] Image File Generation Complate\n");

	close(TargetFd);
	return 0;

}




int AdjustmentInSectorSize(int Fd, int Size)
{
	int AdjustSelector = Size % SECTORSIZE;
	char ch = 0x00;
	
	if( AdjustSelector != 0)
	{
		AdjustSelector = 512 - AdjustSelector;
		printf("[INFO] File Size [%lu] and fill [%u] byte\n",Size, AdjustSelector);
		
		for(int i =0; i < AdjustSelector; i++)
		{
			write(Fd, ch, 1);	
		}
		
	}
	else
	{
		printf("[INFO] File Size is Aligned 512 Byte\n");
	}
	
	return (Size + AdjustSelector)/ SECTORSIZE;



}

void WriteKernelInfo(int Fd, int Count)
{

	int position = lseek(Fd, 5, SEEK_SET);
	if( position == -1)
	{
		fprintf(stderr, "[ERROR] lseek fail. return value = %d, errno = %d, %d \n", position, errno, SEEK_SET);
		exit(-1);
	}
	int data = (unsigned short ) Count;
	write(Fd, &data, 2);
	printf("[INFO] Total Sector Count = %d - except boot loader.\n", Count);


}

int CopyFile(int SourceFd, int TargetFd)
{
	int SourceFileSize = 0;
	char Buffer[SECTORSIZE];
	while(1)
	{
		int Read = read(SourceFd, Buffer, sizeof(Buffer));
		int Write = write(TargetFd, Buffer, Read);

		if(Read != Write)
		{
			fprintf(stderr, "[ERROR] Read != Write. \n");
			exit(-1);
		}
		SourceFileSize += Read;
		if(Read != sizeof(Buffer))
		{
			break;
		}
	}
	return SourceFileSize;
}
