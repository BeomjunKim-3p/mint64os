#include "Utility.h"

void mintMemSet(void *pvDestination, BYTE bData, int iSize)
{
        int i;

        for (i = 0; i < iSize; i++) {
                ((BYTE *)pvDestination)[i] = bData;
        }
}

int mintMemCpy(void *pvDestination, const void *pvSource, int iSize)
{
        int i;

        for (i = 0; i < iSize; i++) {
                ((BYTE *)pvDestination)[i] = ((BYTE *)pvSource)[i];
        }
        return iSize;
}

int mintMemCmp(const void *pvDestination, const void *pvSource, int iSize)
{
        int i;
        char cTemp;

        for (i = 0; i < iSize; i++) {
                cTemp = ((BYTE *)pvDestination)[i] - ((BYTE *)pvSource)[i];
                if (cTemp != 0) {
                        return (int)cTemp;
                }
        }
        return 0;
}


void itoa(int n, char str[])
{
	int i = 0;
	BOOL isNegative = FALSE;

	if (n == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return;
	}
	if (n < 0) {
		isNegative = TRUE;
		n = -n;
	}
	while (n != 0) {
		int rem = n % 10;
		str[i++] = rem + '0';
		n = n / 10;
	}
	if (isNegative) {
		str[i++] = '-';
	}
	str[i] = '\0';

	// 문자열을 뒤집음
	int start = isNegative ? 1 : 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}
