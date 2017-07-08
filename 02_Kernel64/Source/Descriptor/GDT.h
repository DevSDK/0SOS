#ifndef __GDT_H__
#define __GDT_H__
#include <Types.h>

/*
GDT Descriptor
Load GDTR on 0x142000
*/


#define GDT_TYPE_CODE       0x0A    //0b00001010
#define GDT_TYPE_DATA       0x02    //0b00000010
#define GDT_TYPE_TSS        0x09    //0b00001001
//Flag Low Area
#define GDT_ENTRY_LOW_S     0x10    //0b00010000
#define GDT_ENTRY_LOW_DPL0  0x00    //0b00000000
#define GDT_ENTRY_LOW_DPL1  0x20    //0b00100000
#define GDT_ENTRY_LOW_DPL2  0x40    //0b01000000
#define GDT_ENTRY_LOW_DPL3  0x60    //0b01100000
#define GDT_ENTRY_LOW_P     0x80    //0b10000000
//Flag High Area
#define GDT_ENTRY_HIGH_L    0x20    //0b00100000
#define GDT_ENTRY_HIGH_DB   0x40    //0b01000000
#define GDT_ENTRY_HIGH_G    0x80    //0b10000000

//Kernel Code Low Area Flags
#define GDT_ENTRY_LOW_KERNEL_CODE   (GDT_TYPE_CODE |GDT_ENTRY_LOW_S| \
                                GDT_ENTRY_LOW_DPL0 | GDT_ENTRY_LOW_P)
//Kernel Data Low Area Flags
#define GDT_ENTRY_LOW_KERNEL_DATA   (GDT_TYPE_DATA |GDT_ENTRY_LOW_S| \
                                GDT_ENTRY_LOW_DPL0 | GDT_ENTRY_LOW_P)
//User Code Low Area Flags
#define GDT_ENTRY_LOW_USER_CODE   (GDT_TYPE_CODE |GDT_ENTRY_LOW_S| \
                                GDT_ENTRY_LOW_DPL3 | GDT_ENTRY_LOW_P)
//User Data Low Area Flags
#define GDT_ENTRY_LOW_USER_DATA   (GDT_TYPE_DATA |GDT_ENTRY_LOW_S| \
                                GDT_ENTRY_LOW_DPL3 | GDT_ENTRY_LOW_P)


#define GDT_ENTRY_HIGH_CODE (GDT_ENTRY_HIGH_G | GDT_ENTRY_HIGH_L)
#define GDT_ENTRY_HIGH_DATA (GDT_ENTRY_HIGH_G | GDT_ENTRY_HIGH_L)

#define GDT_ENTRY_LOW_TSS   (GDT_ENTRY_LOW_DPL0 | GDT_ENTRY_LOW_P)
#define GDT_ENTRY_HIGH_TSS  (GDT_ENTRY_HIGH_G)

#define GDT_KERNEL_CODE_SEGMENT 0x08
#define GDT_KERNEL_DATA_SEGMENT 0x10
#define GDT_TSS_SEGMENT         0x18

#define GDTR_POINTER            0x142000    //1MB + 256KB

#define GDT_MAX_ENTRY8_COUNT    3
#define GDT_MAX_ENTRY16COUNT    1  

#define GDT_TABLE_SIZE          ((sizeof(GDT_ENTRY8) * GDT_MAX_ENTRY8_COUNT)\
                                  + (sizeof(GDT_ENTRY16) * GDT_MAX_ENTRY16COUNT)) 

#define TSS_SEGMENT_SIZE        (sizeof(TSS_SEGMENT))

#pragma pack(push, 1)

typedef struct _Struct_DTR_Struct
{
    WORD Size;              //2Byte
    QWORD BaseAddress;      //8Byte
    WORD _WORD_PADDING;     //2Byte
    DWORD _DWORD_PADDING;   //4Byte
                            //=16Byte .
    
}GDTR, IDTR;

typedef struct _Struct_GDT_Entry8
{
    WORD Low_Size;          //2Byte
    WORD Low_BaseAddress;   //2Byte

    BYTE Low_BaseAddress1;  //1Byte
    BYTE Low_Flags;         //1Byte
    BYTE High_FlagsAndSize; //1Byte
    BYTE High_BaseAddress;  //1Byte
}GDT_ENTRY8;

typedef struct _Struct_GDT_Entry16
{
    WORD Low_Size;
    WORD Low_BaseAddress;

    BYTE Mid_BaseAddress;
    BYTE Low_Flags;
    BYTE High_FlagsAndSize;
    BYTE High_BaseAddress;

    DWORD High_BaseAddress2;
    DWORD Reserved;

}GDT_ENTRY16;

typedef struct _Struct_TSS_Segment
{
    DWORD Reserved;
    QWORD RSP[3];
    QWORD Reserved2;
    QWORD IST[7];
    QWORD Reserved3;
    WORD  Reserved4;
    WORD  IOMapBaseAddress; 
}TSS_SEGMENT;


#pragma pack(pop)





void InitializeGDTWithTSS();
void InitializeTSSSegment(TSS_SEGMENT* _tss); 

void SetGDT_Entry8(GDT_ENTRY8* _entry, DWORD _BaseAddress,
                     DWORD _Size, BYTE _HighFlags, BYTE _LowFlags, BYTE _Type);

void SetGDT_Entry16(GDT_ENTRY16* _entry, QWORD _BaseAddress,
                     DWORD _Size, BYTE _HighFlags, BYTE _LowFlags, BYTE _Type);


#endif /* __GDT_H__ */