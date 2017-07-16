#include "String.h"

BOOL _itoa(int _value, char* _result, int _base)
{
	if (_base < 2 || _base > 36) { *_result = '\0'; return FALSE; }

	char* ptr = _result, *ptr1 = _result, tmp_char;
	int tmp_value;


	//음수 양수 상관없이 처리가 가능함.
	do {
		tmp_value = _value;
		_value /= _base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - _value * _base)];
	} while (_value);

	//Sign 처리
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	//Reverse
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return TRUE;
}


int _ctoi(char _c)
{
	switch (_c)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a':
	case 'A':
		return 10;
	case 'b':
	case 'B':
		return 11;
	case 'c':
	case 'C':
		return 12;
	case 'd':
	case 'D':
		return 13;
	case 'e':
	case 'E':
		return 14;
	case 'f':
	case 'F':
		return 15;
	case 'g':
	case 'G':
		return 16;
	case 'h':
	case 'H':
		return 17;
	case 'i':
	case 'I':
		return 18;
	case 'j':
	case 'J':
		return 19;
	case 'k':
	case 'K':
		return 20;
	case 'l':
	case 'L':
		return 21;
	case 'm':
	case 'M':
		return 22;
	case 'n':
	case 'N':
		return 23;
	case 'o':
	case 'O':
		return 24;
	case 'p':
	case 'P':
		return 25;
	case 'q':
	case 'Q':
		return 26;
	case 'r':
	case 'R':
		return 27;
	case 's':
	case 'S':
		return 28;
	case 't':
	case 'T':
		return 29;
	case 'u':
	case 'U':
		return 30;
	case 'v':
	case 'V':
		return 31;
	case 'w':
	case 'W':
		return 32;
	case 'x':
	case 'X':
		return 33;
	case 'y':
	case 'Y':
		return 34;
	case 'z':
	case 'Z':
		return 35;
	}
}


int __StringLength(const char* _str)
{
	int result = 0;
	for (result = 0; _str[result] != '\0'; result++);
	return result;
}