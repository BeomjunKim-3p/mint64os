#if 1 /** original */
#include "AssemblyUtility.h"
#include "Descriptor.h"
#include "Keyboard.h"
#include "Types.h"
#include "Utility.h"
#include "Console.h"

#define SUCCESSCOLOR 0x0A
#define ERRORCOLOR 0x0C
#define PASSCOLOR 0x0A
#define FAILCOLOR 0x0C
#define TEXTCOLOR 0x0F


const char *SuccessMsg = "[Success]";
const char *ErrorMsg = "[Error]";
const char *PassMsg = "[Pass]";
const char *FailMsg = "[Fail]";

BYTE screenline;

void Main(void)
{
	//screenline = *((BYTE *)0x90000);
    screenline = 14;

	char vcTemp[2] = {
		0,
	};
	BYTE bFlags;
	BYTE bTemp;
	int i = 0;

	mintPrintString(70, screenline, SuccessMsg, SUCCESSCOLOR);
	mintPrintString(1, 1, "DEBUG", 0x0A);
	mintPrintString(1, ++screenline, "IA-32e C Language Kernel Started",
			TEXTCOLOR);

	mintPrintString(1, ++screenline,
			"GDT Initialize And Switch For IA-32e Mode", TEXTCOLOR);
	mintInitializeGDTTableAndTSS();
	mintLoadGDTR(GDTR_STARTADDRESS);
	mintPrintString(73, screenline, PassMsg, PASSCOLOR);

	mintPrintString(1, ++screenline, "TSS Segment Load", TEXTCOLOR);
	mintLoadTR(GDT_TSSSEGMENT);
	mintPrintString(73, screenline, PassMsg, PASSCOLOR);

	mintPrintString(1, ++screenline, "IDT Initialize", TEXTCOLOR);
	mintInitializeIDTTable();
	mintLoadIDTR(IDTR_STARTADDRESS);
	mintPrintString(73, screenline, PassMsg, PASSCOLOR);

	mintPrintString(1, ++screenline, "Keyboard Activate", TEXTCOLOR);

	// 키보드를 활성화
	if (mintActivateKeyboard() == TRUE) {
		mintPrintString(73, screenline, PassMsg, PASSCOLOR);
		mintChangeKeyboardLED(FALSE, FALSE, FALSE);
	} else {
		mintPrintString(73, screenline, FailMsg, FAILCOLOR);
		while (1);
	}

	screenline++;
	while (1) {
		if (mintIsOutputBufferFull() == TRUE) {
			// 출력 버퍼(포트 0x60)에서 스캔 코드를 읽어서 저장
			bTemp = mintGetKeyboardScanCode();

			if (mintConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]),
							   &bFlags) == TRUE) {
				if (bFlags & KEY_FLAGS_DOWN) {
					if (vcTemp[0] >= 0x21 &&
					    vcTemp[0] <= 0x7E) {
						mintPrintString(i++, screenline, vcTemp,
								0x07);
					}
					if (vcTemp[0] == '0') {
						bTemp = bTemp / 0;
					}
				}
			}
		}
	}
}


#else /** original */

/** Main.c */

#include "AssemblyUtility.h"
#include "Descriptor.h"
#include "Keyboard.h"
#include "Types.h"

#define SUCCESSCOLOR 0x0A
#define ERRORCOLOR 0x0C
#define PASSCOLOR 0x0A
#define FAILCOLOR 0x0C
#define TEXTCOLOR 0x0F

BOOL mintPrintString(int iX, int iY, const char *pcString, BYTE Attribute);
void itoa(int n, char str[]);

 const char SuccessMsg[] = "[Success]";
 const char ErrorMsg[] = "[Error]";
 const char PassMsg[] = "[Pass]";
 const char FailMsg[] = "[Fail]";

BYTE screenline;

void Main(void)
{
    screenline = *((BYTE *)0x90000);

    char vcTemp[2] = {
        0,
    };
    BYTE bFlags;
    BYTE bTemp;
    int i = 0;

#define SuccessMsg SuccessMsg


    mintPrintString(70, screenline, "[Success]", SUCCESSCOLOR);
    mintPrintString(1, 1, "DEBUG", 0x0A);


    mintPrintString(1, ++screenline, "IA-32e C Language Kernel Started",
            TEXTCOLOR);

    mintPrintString(1, ++screenline,
            "GDT Initialize And Switch For IA-32e Mode", TEXTCOLOR);


    mintInitializeGDTTableAndTSS();
    mintLoadGDTR(GDTR_STARTADDRESS);
    mintPrintString(73, screenline, PassMsg, PASSCOLOR);

    mintPrintString(1, ++screenline, "TSS Segment Load", TEXTCOLOR);
    mintLoadTR(GDT_TSSSEGMENT);
    mintPrintString(73, screenline, PassMsg, PASSCOLOR);

    mintPrintString(1, ++screenline, "IDT Initialize", TEXTCOLOR);
    mintInitializeIDTTable();

    mintLoadIDTR(IDTR_STARTADDRESS);
    mintPrintString(73, screenline, PassMsg, PASSCOLOR);


    mintPrintString(1, ++screenline, "Keyboard Activated", TEXTCOLOR);

    // 키보드를 활성화
    if (mintActivateKeyboard()) {
        mintPrintString(73, screenline, PassMsg, PASSCOLOR);
        mintChangeKeyboardLED(FALSE, FALSE, FALSE);
    } else {
        mintPrintString(73, screenline, FailMsg, FAILCOLOR);
        while (1)
            ;
    }
    while (1) {
        if (mintIsOutputBufferFull()) {
            // 출력 버퍼(포트 0x60)에서 스캔 코드를 읽어서 저장
            bTemp = mintGetKeyboardScanCode();

            if (mintConvertScanCodeToASCIICode(
                        bTemp
                        , &(vcTemp[0])
                        ,&bFlags)) {

                if (bFlags & KEY_FLAGS_DOWN) {
                    if (vcTemp[0] >= 0x21 &&
                            vcTemp[0] <= 0x7E)
                    {
                        mintPrintString(i++, 16, vcTemp,
                                0x07);
                    }

#if 0
                    if (vcTemp[0] == '0') {
                        bTemp = bTemp / 0;
                    }
#endif
                }
            }
        }
    }

#undef SuccessMsg
}

BOOL mintPrintString(int iX, int iY, const char *pcString, BYTE Attribute)
{

    CHARACTER *pstScreen = ((CHARACTER *)0xB8000) + (iY * 80) + iX;
    unsigned i = 0;

#if 0
    if ((iX) >= 80 || iY >= 25) {
        return FALSE;
    }
#endif

    while((iX < 80) && (iY < 25) && pcString[i]) {
        pstScreen[i].bCharactor = pcString[i];
        pstScreen[i].bAttribute = Attribute;
        ++i;
    }

    return !!i;
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
#endif /** original */
