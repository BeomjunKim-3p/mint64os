#include "ModeSwitch.h"
#include "Page.h"
#include "Types.h"

BOOL mintPrintString(int iX, int iY, const char *pcString, BYTE Attribute);
BOOL mintInitializeKernel64Area(void);
BOOL mintIsMemoryEnough(void);
void mintCopyKernel64ImageTo2MB(void);

void itoa(int n, char str[]);
BYTE screenline;

const char *SuccessMsg;
const char *ErrorMsg;
const char *PassMsg;
const char *FailMsg;
char TempBuffer[20] = {
        0,
};

// Main 함수
void Main(void)
{
	//__asm__ __volatile__("mov %%dx, %0" : "=m"(screenline) : : "dx");
	/* 심심해서 만든거
	BYTE* Screen = (BYTE*) 0xB8000;
	for ( int i = 0 ; i < 2 ; i++ )
	{
	    for ( int j = 0 ; j < 8 ; j++ )
	    {
	        Screen[( 80*17 + i * 80 + j ) * 2] = 0xB0;
	        Screen[( 80*17 + i * 80 + j ) * 2 + 1] = (BYTE)( i * 8 + j );
	    }
	}
	for ( int i = 0 ; i < 2 ; i++ )
	{
	    for ( int j = 0 ; j < 8 ; j++ )
	    {
	        Screen[( 80*19 + i * 80 + j ) * 2] = 0xB1;
	        Screen[( 80*19 + i * 80 + j ) * 2 + 1] = (BYTE)( i * 8 + j );
	    }
	}
	for ( int i = 0 ; i < 2 ; i++ )
	{
	    for ( int j = 0 ; j < 8 ; j++ )
	    {
	        Screen[( 80*21 + i * 80 + j ) * 2] = 0xB2;
	        Screen[( 80*21 + i * 80 + j ) * 2 + 1] = (BYTE)( i * 8 + j );
	    }
	}
	for ( int i = 0 ; i < 2 ; i++ )
	{
	    for ( int j = 0 ; j < 8 ; j++ )
	    {
	        Screen[( 80*23 + i * 80 + j ) * 2] = 0xDB;
	        Screen[( 80*23 + i * 80 + j ) * 2 + 1] = (BYTE)( i * 8 + j );
	    }
	}
	*/

	// DWORD i;
	DWORD dwEAX, dwEBX, dwECX, dwEDX;
	char vcVendorString[13] = {
	        0,
	};

	screenline = 4;
	mintPrintString(1, (int)(++screenline) /*line 5*/,
	                "Protected Mode C Language Kernel Started", 0x0F);

	// 최소 메모리 크기를 만족하는 지 검사
	mintPrintString(1, (int)(++screenline) /*line 6*/,
	                "Minimum Memory Size Check", 0x0F);
	if (mintIsMemoryEnough() == FALSE) {
		mintPrintString(73, (int)screenline /*line 6*/, FailMsg, 0x0C);
		mintPrintString(1, (int)(++screenline) /*line 7*/,
		                "Not Enough Memory. MINT64 OS Requires over "
		                "64MB. System Halted.",
		                0x0F);
		mintPrintString(1, (int)(++screenline) /*line 8*/,
		                "TODO: Make PM(<64MB) support", 0x0A);

		while (1)
			;
	} else {
		mintPrintString(73, (int)screenline /*line 6*/, PassMsg, 0x0A);
	}

	// IA-32e 모드의 커널 영역 초기화
	mintPrintString(1, (int)(++screenline) /*line 7*/,
	                "IA-32e Kernel Area Initializing", 0x0F);
	if (mintInitializeKernel64Area()) {
		mintPrintString(70, (int)screenline /*line 7*/, SuccessMsg,
		                0x0A);
	} else {
		mintPrintString(72, (int)screenline /*line 7*/, ErrorMsg, 0x0C);
		mintPrintString(
		        1, (int)(++screenline) /*line 8*/,
		        "Kernel Area Initialization Failed. System Halted.",
		        0x0F);
		while (1);
	}

	// IA-32e 모드 커널 위한 페이지 테이블 생성
	mintPrintString(1, (int)(++screenline) /*line 8*/,
	                "IA-32e Page Tables Initializing", 0x0F);
	mintInitializePageTables();
	mintPrintString(70, (int)screenline /*line 8*/, SuccessMsg, 0x0A);

	// 프로세서 제조사 정보 읽기
	mintReadCPUID(0x00, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	*((DWORD *)vcVendorString) = dwEBX;
	*((DWORD *)vcVendorString + 1) = dwEDX;
	*((DWORD *)vcVendorString + 2) = dwECX;

	TempBuffer[0] = '[';
	*(DWORD *)(TempBuffer + 1) = dwEBX;
	*(DWORD *)(TempBuffer + 5) = dwEDX;
	*(DWORD *)(TempBuffer + 9) = dwECX;
	TempBuffer[13] = ']';
	TempBuffer[14] = '\0'; // 문자열 종료

	mintPrintString(1, (int)(++screenline) /*line 9*/,
	                "Processor Vender String", 0x0F);
	mintPrintString(65, (int)screenline /*line 9*/, TempBuffer, 0x0F);

	// 64비트 지원 유무 확인
	mintReadCPUID(0x80000001, &dwEAX, &dwEBX, &dwECX, &dwEDX);
	mintPrintString(1, (int)(++screenline) /*line 10*/,
	                "64bit Mode Support Check", 0x0F);
	if (dwEDX & (1 << 29)) {
		mintPrintString(73, (int)screenline /*line 10*/, PassMsg, 0x0A);
	} else {
		mintPrintString(72, (int)screenline /*line 10*/, FailMsg, 0x0C);
		mintPrintString(1, (int)(++screenline) /*line 11*/,
		                "This processor does not support 64bit Mode. "
		                "System Halted.",
		                0x0F);
		while (1);
	}

	mintPrintString(1, (int)(++screenline) /*line 11*/,
	                "Copy IA-32e Kernel to 2MB Address", 0x0F);
	mintCopyKernel64ImageTo2MB();
	mintPrintString(70, (int)screenline /*line 11*/, SuccessMsg, 0x0A);

	// IA-32e 모드로 전환
	mintPrintString(1, (int)(++screenline) /*line 11*/,
	                "Switching To IA-32e Mode", 0x0F);
	BYTE *screenLineSwitchPtr = (BYTE *)0x90000;
	*screenLineSwitchPtr = screenline;
	mintSwitchAndExecute64bitKernel();
}

