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