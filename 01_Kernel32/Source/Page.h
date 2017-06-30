#ifndef __PAGE_H__
#define __PAGE_H__

#include "Types.h"

#define PAGE_FLAG_P			0x00000001
#define PAGE_FLAG_RW		0x00000002
#define PAGE_FLAG_US		0x00000004
#define PAGE_FLAG_PWT		0x00000008
#define PAGE_FLAG_PCD		0x00000010
#define PAGE_FLAG_A			0x00000020
#define PAGE_FLAG_D			0x00000040
#define PAGE_FLAG_PS		0x00000080
#define PAGE_FLAG_G			0x00000100
#define PAGE_FLAG_PAT		0x00001000	
#define PAGE_FLAG_EXB		0x80000000	

#define PAGE_FLAG_DEFAULT	(PAGE_FLAG_P | PAGE_FLAG_RW)
#define PAGE_TABLE_SIZE		0x1000 //4KB
#define PAGE_DEFAULT_SIZE	0X200000

#define PAGE_MAX_ENTRY_COUNT		512

#pragma(push, 1)



/*

	For IA-32e Paging
	IA-32e Adress Structure
	63			48 47	39 38			   30 29	   21 20			0
	|SIGNEXTENSION| PML4 | DIRECTORY POINTER | DIRCTORY  |    OFFSET     |

	PML4 Refernce PML4 ENTRY
	and, that Refernce DIRECTORY POINTER ENTRY using DIRECTORY POINTER
	and, that Refernce DIRCTORY ENTRY using DIRECTORY 
	and, DIRECTORY + OFFSET is Memory Adress. 
	
	so, We Need Space for this Structure
	
	PML4 Table Need 512 * 8 Byte = 4KB
	PAGE DIRECTORY POINTER Table Need 512 * 8 Byte = 4KB
	PAGE DIRECTORY Need 512 * 8 Byte * 64 = 256KB (for 64GB Memory)
	
	So we using 4KB + 4KB+ 256KB = 264KB Memory Space.

	Following Code Implementation
	
	We Use 4 Level Paging, So NOT USE PTENTRY.
*/

typedef struct __Struct_PageEntry
{	
	
	/*0----------------31 bit */
	DWORD dwLowAdress;
	/*32---------------64 bit */
	DWORD dwHighAdress;

} PML4ENTRY, PDPTENTRY, PDENTRY, PTENTRY;	

#pragma(pop)

void InitializePageTable();
void SetPageEntryData(PTENTRY* pEntry, DWORD dwHighBaseAdress, 
					  DWORD dwLowBaseAdress, DWORD dwLowFlag, DWORD dwHighFlag);


	
#endif /*__PAGE_H__ */
