#ifndef __ASSEMBLYUTILITY_H__
#define __ASSEMBLYUTILITY_H__

#include "Types.h"

// 함수
BYTE mintInPortByte(WORD wPort);
void mintOutPortByte(WORD wPort, BYTE bData);
void mintLoadGDTR(QWORD qwGDTRAddress);
void mintLoadTR(WORD wTSSSegmentOffset);
void mintLoadIDTR(QWORD qwIDTRAddress);

#endif /* __ASSEMBLYUTILITY_H__ */