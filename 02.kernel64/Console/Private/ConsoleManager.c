#include "Types.h"

BOOL mintPrintString(int iX, int iY, const char *pcString, BYTE Attribute)
{
	CHARACTER *pstScreen = (CHARACTER *)0xB8000;
	int i;

	if (iX >= 80 || iY >= 25) {
		return FALSE;
	}

	pstScreen += (iY * 80) + iX;
	for (i = 0; pcString[i] != 0; i++) {
		pstScreen[i].bCharactor = pcString[i];
		pstScreen[i].bAttribute = Attribute;
	}
	return TRUE;
}

