#include "Page.h"

// IA-32e 모드 커널 페이지테이블 생성
void mintInitializePageTables(void)
{
	PML4TENTRY *pstPML4TEntry;
	PDPTENTRY *pstPDPTEntry;
	PDENTRY *pstPDEntry;
	DWORD dwMapingAddress;
	int i;

	// PML4테이블 생성
	// 첫번째 엔트리 외 나머지 모두 초기화
	pstPML4TEntry = (PML4TENTRY *)0x100000;
	mintSetPageEntryData(&(pstPML4TEntry[0]), 0x00, 0x101000,
	                     PAGE_FLAGS_DEFAULT, 0);
	for (i = 1; i < PAGE_MAXENTRYCOUNT; i++) {
		mintSetPageEntryData(&(pstPML4TEntry[i]), 0, 0, 0, 0);
	}
	// 페이지 디렉토리 포인터 테이블 생성
	// 하나의 PDPT로 512GB 매핑 가능
	// 512개 엔트리 설정해 512GB까지 매핑
	pstPDPTEntry = (PDPTENTRY *)0x101000;
	for (i = 0; i < 512; i++) {
		mintSetPageEntryData(&(pstPDPTEntry[i]), 0x00,
		                     0x102000 + (i * PAGE_TABLESIZE),
		                     PAGE_FLAGS_DEFAULT, 0);
	}
	for (i = 512; i < PAGE_MAXENTRYCOUNT; i++) {
		mintSetPageEntryData(&(pstPDPTEntry[i]), 0, 0, 0, 0);
	}

	// 페이지 디렉토리 테이블 생성
	// 하나의 페이지 디렉토리 1GB 매핑 가능
	// 512개 페이지 디렉토리 생성; 512GB지원
	pstPDEntry = (PDENTRY *)0x102000;
	dwMapingAddress = 0;
	for (i = 0; i < PAGE_MAXENTRYCOUNT * 512; i++) {
		// 32비트로는 상위 주소 표현 불가능, MB단위로 계산한다음 최종
		// 결과 4KB로 나눠 32비트 이상 주소 계산
		mintSetPageEntryData(
		        &(pstPDEntry[i]), (i * (PAGE_DEFAULTSIZE >> 20)) >> 12,
		        dwMapingAddress, PAGE_FLAGS_DEFAULT | PAGE_FLAGS_PS, 0);
		dwMapingAddress += PAGE_DEFAULTSIZE;
	}
}

void mintSetPageEntryData(PTENTRY *pstEntry, DWORD dwUpperBaseAddress,
                          DWORD dwLowerBaseAddress, DWORD dwLowerFlags,
                          DWORD dwUpperFlags)
{
	pstEntry->dwAttributeAndLowerBaseAddress =
	        dwLowerBaseAddress | dwLowerFlags;
	pstEntry->dwUpperBaseAddressAndEXB =
	        (dwUpperBaseAddress & 0x000000FF) | dwUpperFlags;
}