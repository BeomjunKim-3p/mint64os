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
		while (1)
			;
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
		while (1)
			;
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

	while (1);
}

const char *SuccessMsg = "[Success]";
const char *ErrorMsg = "[Error]";
const char *PassMsg = "[Pass]";
const char *FailMsg = "[Fail]";

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
	int start = (isNegative) ? 1 : 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

// 문자열 출력 함수
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

BOOL mintInitializeKernel64Area(void)
{
	DWORD *pdwCurrentAddress;

	// 초기화를 시작할 주소인 0x100000(1MB)를 설정
	pdwCurrentAddress = (DWORD *)0x100000;

	// 마지막 주소인 0x600000(6MB)까지 반복하면서 4바이트씩 0으로 채움
	while ((DWORD)pdwCurrentAddress < 0x600000) {
		*pdwCurrentAddress = 0x00;

		// 0으로 저장한 후 다시 읽었을 때 0이 나오지 않으면 해당 주소를
		// 사용하는데 문제가 생긴 것이므로 더이상 진행하지 않고 종료
		if (*pdwCurrentAddress != 0) {
			return FALSE;
		}

		// 다음 주소로 이동
		pdwCurrentAddress++;
	}
	return TRUE;
}

BOOL mintIsMemoryEnough(void)
{
	DWORD *pdwCurrentAddress;
#define MEMORY_TEST_PATTERN 0x15871308

	// 0x100000(1MB)부터 검사 시작
	pdwCurrentAddress = (DWORD *)0x100000;

	// 0x4000000(64MB)까지 검사
	while ((DWORD)pdwCurrentAddress < 0x4000000) {
		*pdwCurrentAddress = MEMORY_TEST_PATTERN;

		// 메모리에 정상적으로 저장되었는지 확인
		if (*pdwCurrentAddress != MEMORY_TEST_PATTERN) {
			return FALSE;
		}

		// 16바이트(4 DWORDs)씩 이동
		pdwCurrentAddress += (0x100000 / 4);
	}
	return TRUE;
}

void mintCopyKernel64ImageTo2MB(void)
{
	WORD wKernel32SectorCount, wTotalKernelSectorCount;
	DWORD *pdwSourceAddress, *pdwDestinationAddress;
	int i;

	// 0x7C05에 총 커널 섹터 수, 0x7C07에 보호 모드 커널 섹터 수가 들어있음
	wTotalKernelSectorCount = *((WORD *)0x7C05);
	wKernel32SectorCount = *((WORD *)0x7C07);

	pdwSourceAddress = (DWORD *)(0x10000 + (wKernel32SectorCount * 512));
	pdwDestinationAddress = (DWORD *)0x200000;
	// IA-32e 모드 커널 섹터 크기만큼 복사
	for (i = 0;
	     i < 512 * (wTotalKernelSectorCount - wKernel32SectorCount) / 4;
	     i++) {
		*pdwDestinationAddress = *pdwSourceAddress;
		pdwDestinationAddress++;
		pdwSourceAddress++;
	}
}
