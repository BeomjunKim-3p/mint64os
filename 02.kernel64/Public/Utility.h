#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Types.h"

// 함수
void mintMemSet(void *pvDestination, BYTE bData, int iSize);
int mintMemCpy(void *pvDestination, const void *pvSource, int iSize);
int mintMemCmp(const void *pvDestination, const void *pvSource, int iSize);
void itoa(int n, char str[]);

#endif /* __UTILITY_H__ */
