#ifndef __IO_VGA_H__
#define __IO_VGA_H__

#define VGA_PORT_INDEX      0x3D4
#define VGA_PORT_DATA       0x3D5
#define VGA_INDEX_HIGH_CURSOR   0x0E
#define VGA_INDEX_LOW_CURSOR    0x0F

void _SetCursor(int _x, int _y);

void _GetCursor(int* _x_out, int* _y_out);

#endif /*__IO_VGA_H__*/