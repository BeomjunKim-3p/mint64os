#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Types.h"

void mintReadCPUID(DWORD dwEAX, DWORD *pdwEAX, DWORD *pdwEBX, DWORD *pdwECX,
                   DWORD *pdwEDX);
void mintSwitchAndExecute64bitKernel(void);

#endif