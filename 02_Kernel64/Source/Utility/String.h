#ifndef __STRING_H__
#define __STRING_H__
#include <Types.h>
int __StringLength(const char* _str);
BOOL _itoa(int _value, char* _result, int _base);
BOOL _atoi(const char* _number, int* _result, int _base);
#endif /*__STRING_H__*/