#ifndef __STRING_H__
#define __STRING_H__
#include <Types.h>
int __StringLength(const char* _str);
BOOL _itoa(long _value, char* _result, int _base);
BOOL _atoi(const char* _number, long* _value, int _base);
BOOL _u_itoa(QWORD _value, char* _result, unsigned int _base);
#endif /*__STRING_H__*/