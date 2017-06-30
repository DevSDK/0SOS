#include "Page.h"



void InitializePageTable()
{

	PML4ENTRY*	pml4entry  = (PML4ENTRY*) 0x100000;
	PDPTENTRY*	pdptentry  = (PDPTENTRY*) 0x101000;
	PDENTRY*	pdentry    = (PDENTRY*	) 0x102000;

	SetPageEntryData(&pml4entry[0], 0x00, 0x101000, PAGE_FLAG_DEFAULT, 0);

	for(int i = 1; i< PAGE_MAX_ENTRY_COUNT; i++)
	{
		SetPageEntryData(&pml4entry[i], 0,0,0,0);
	}

	for(int i = 0; i < 64; i++)
	{
		SetPageEntryData(&pdptentry[i], 0, 0x10200 + i * PAGE_TABLE_SIZE,
						 PAGE_FLAG_DEFAULT, 0);
	}

	for(int i=64; i < PAGE_MAX_ENTRY_COUNT; i++)
	{
		SetPageEntryData(&pdptentry[i], 0, 0, 0, 0);
	}

	DWORD LowMapping = 0; 
	
	for(int i=0; i<PAGE_MAX_ENTRY_COUNT * 64; i++)
	{
		DWORD high = (i * (PAGE_FLAG_DEFAULT >> 20) ) >> 12;
		SetPageEntryData(&pdentry[i], high, LowMapping, 
						PAGE_FLAG_DEFAULT | PAGE_FLAG_PS, 0);				

		LowMapping += PAGE_DEFAULT_SIZE;
	}
	 

}


void SetPageEntryData(PTENTRY* pEntry, DWORD dwHighBaseAdress, DWORD dwLowBaseAdress,
					  DWORD dwLowFlag, DWORD dwHighFlag)
{
	pEntry->dwLowAdress  = dwLowBaseAdress  | dwLowFlag;
	pEntry->dwHighAdress = (dwHighBaseAdress & 0xFF )| dwHighFlag;
}
