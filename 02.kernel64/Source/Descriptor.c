#include "Descriptor.h"
#include "Utility.h"


// GDT 및 TSS

// GDT 테이블 초기화
void mintInitializeGDTTableAndTSS(void)
{
        GDTR* pstGDTR;
        GDTENTRY8* pstEntry;
        TSSSEGMENT* pstTSS;
        int i;

        // GDTR 설정
        pstGDTR = (GDTR*)GDTR_STARTADDRESS;
        pstEntry = (GDTENTRY8*)(GDTR_STARTADDRESS + sizeof(GDTR));
        pstGDTR->wLimit = GDT_TABLESIZE - 1;
        pstGDTR->qwBaseAddress = (QWORD)pstEntry;

        // TSS 설정
        pstTSS = (TSSSEGMENT*)((QWORD)pstEntry + GDT_TABLESIZE);
        // NULL, 64비트 Code/Data, tss 위해 4개 세그먼트 생성
        mintSetGDTEntry8(&(pstEntry[0]), 0, 0, 0, 0, 0); // NULL
        mintSetGDTEntry8(&(pstEntry[1]), 0, 0xFFFFF, GDT_FLAGS_UPPER_CODE,
                         GDT_FLAGS_LOWER_KERNELCODE, GDT_TYPE_CODE); // Code
        mintSetGDTEntry8(&(pstEntry[2]), 0, 0xFFFFF, GDT_FLAGS_UPPER_DATA,
                         GDT_FLAGS_LOWER_KERNELDATA, GDT_TYPE_DATA); // Data
        mintSetGDTEntry16((GDTENTRY16*)&(pstEntry[3]), (QWORD)pstTSS,
                          sizeof(TSSSEGMENT) - 1, GDT_FLAGS_UPPER_TSS,
                          GDT_FLAGS_LOWER_TSS, GDT_TYPE_TSS); // TSS

        // TSS 초기화; GDT 이하 영역 사용
        mintInitializeTSSSegment(pstTSS);
}

// 8바이트 크기 GDT 엔트리 설정
void mintSetGDTEntry8(GDTENTRY8 *pstEntry, DWORD dwBaseAddress, DWORD dwLimit,
                       BYTE bUpperFlags, BYTE bLowerFlags, BYTE bType) 
{
        pstEntry->wLowerLimit = dwLimit & 0xFFFF;
        pstEntry->wLowerBaseAddress = dwBaseAddress & 0xFFFF;
        pstEntry->bUpperBaseAddress1 = (dwBaseAddress >> 16) & 0xFF;
        pstEntry->bTypeAndLowerFlag = bType | bLowerFlags;
        pstEntry->bUpperLimitAndUpperFlag =
            ((dwLimit >> 16) & 0x0F) | bUpperFlags;
        pstEntry->bUpperBaseAddress2 = (dwBaseAddress >> 24) & 0xFF;
}

// 16바이트 크기 GDT 엔트리 설정
void mintSetGDTEntry16(GDTENTRY16 *pstEntry, QWORD qwBaseAddress,
                       DWORD dwLimit, BYTE bUpperFlags, BYTE bLowerFlags,
                       BYTE bType)
{
        pstEntry->wLowerLimit = dwLimit & 0xFFFF;
        pstEntry->wLowerBaseAddress = (QWORD)qwBaseAddress & 0xFFFF;
        pstEntry->bMiddleBaseAddress1 = (qwBaseAddress >> 16) & 0xFF;
        pstEntry->bTypeAndLowerFlag = bType | bLowerFlags;
        pstEntry->bUpperLimitAndUpperFlag =
            ((dwLimit >> 16) & 0x0F) | bUpperFlags;
        pstEntry->bMiddleBaseAddress2 = (qwBaseAddress >> 24) & 0xFF;
        pstEntry->dwUpperBaseAddress = (qwBaseAddress >> 32);
        pstEntry->dwReserved = 0;
}

// TSS 세그먼트 정보 초기화
void mintInitializeTSSSegment(TSSSEGMENT *pstTSS)
{
        mintMemSet(pstTSS, 0, sizeof(TSSSEGMENT));
        pstTSS->qwIST[0] = IST_STARTADDRESS + IST_SIZE;
        // IO를 TSS limit보다 크게 설정해 IO Map 사용 안하게 함
        pstTSS->wIOMapBaseAddress = 0xFFFF;
}


// IDT

// IDT 테이블 초기화
void mintInitializeIDTTable(void)
{
        IDTR* pstIDTR;
        IDTENTRY* pstEntry;
        int i;

        // IDTR 설정
        pstIDTR = (IDTR*)IDTR_STARTADDRESS;
        pstEntry = (IDTENTRY*)(IDTR_STARTADDRESS + sizeof(IDTR));
        pstIDTR->qwBaseAddress = (QWORD)pstEntry;
        pstIDTR->wLimit = IDT_TABLESIZE - 1;

        // 모든 벡터를 더미 핸들러로 설정
        for (i = 0; i < IDT_MAXENTRYCOUNT; i++) {
                mintSetIDTEntry(&(pstEntry[i]), mintDummyHandler,
                                GDT_KERNELCODESEGMENT, IDT_FLAGS_IST1,
                                IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
        }
}

// IDT 게이트 디스크립터 설정
void mintSetIDTEntry(IDTENTRY *pstEntry, void *pvHandler, WORD wSelector,
                     BYTE bIST, BYTE bFlags, BYTE bType)
{
        pstEntry->wLowerBaseAddress = (QWORD)pvHandler & 0xFFFF;
        pstEntry->wSegmentSelector = wSelector;
        pstEntry->bIST = bIST & 0x3;
        pstEntry->bTypeAndFlags = bType | bFlags;
        pstEntry->wMiddleBaseAddress = ((QWORD)pvHandler >> 16) & 0xFFFF;
        pstEntry->dwUpperBaseAddress = ((QWORD)pvHandler >> 32);
        pstEntry->dwReserved = 0;
}

// 더미 핸들러
void mintDummyHandler(void)
{
        mintPrintString(0, 0, "********************************************************************************", 0x07);
        mintPrintString(0, 1, "                                                                                ", 0x07);
        mintPrintString(0, 2, "                         Dummy Interrupt Handler Called                         ", 0x07);
        mintPrintString(0, 3, "                                                                                ", 0x07);
        mintPrintString(0, 4, "********************************************************************************", 0x07);
        // 아무것도 하지 않음
        while(1);
}