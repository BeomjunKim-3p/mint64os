[BITS 32]

global mintReadCPUID, mintSwitchAndExecute64bitKernel

SECTION .text

; CPUID 반환
;     PARAM: DWORD dwEAX, DWORD* pdwEAX,* pdwEBX,* pdwECX, * pdwEDX
mintReadCPUID:
    push ebp         ; 베이스 포인터 레지스터 스택에 푸시
    mov ebp, esp    ; 베이스 포인터 레지스터에 스택 포인터 레지스터 값 설정
    push eax         
    push ebx
    push ecx
    push edx
    push esi

    ; EAX 레지스터 값으로 CPUID 실행
    mov eax, [ebp + 8]      ; dwEAX
    cpuid

    ; 반환된 값을 각 포인터 파라미터에 저장
    mov esi, [ebp + 12]     ; pdwEAX
    mov [esi], eax
    mov esi, [ebp + 16]     ; pdwEBX
    mov [esi], ebx
    mov esi, [ebp + 20]     ; pdwECX
    mov [esi], ecx
    mov esi, [ebp + 24]     ; pdwEDX
    mov [esi], edx
    
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret

; IA-32e 모드로 전환 후 64비트 커널 실행
;     PARAM: void
mintSwitchAndExecute64bitKernel:
    push edx
    ; CR4 컨트롤 레지스터 PAE 비트 1로 설정
    mov eax, cr4            ; CR4 컨트롤 레지스터의 값을 EAX 레지스터에 저장
    or eax, 0x20            ; PAE 비트(비트 5) 1로 설정
    mov cr4, eax            ; PAE 비트가 1로 설정된 값을 CR4 컨트롤 레지스터에 저장

    ; CR3 컨트롤 레지스터에 PML4 테이블 주소와 캐시 활성화
    mov eax, 0x100000       ; PML4 테이블 주소(0x100000(1MB))
    mov cr3, eax            ; CR3 컨트롤 레지스터에 PML4 테이블 주소(0x100000(1MB)) 설정

    ; IA32_EFER.LME을 1로 설정하여 IA-32e 모드 활성화
    mov ecx, 0xC0000080     ; IA32_EFER MSR 레지스터 주소 저장
    rdmsr                   ; MSR 레지스터 읽기

    or eax, 0x0100          ; EAX 레지스터에 저장된 IA32_EFER MSR의 하위 32비트에서 LME 비트(비트 8) 1로 설정
    wrmsr                   ; MSR 레지스터 쓰기

    ; CR0 컨트롤 레지스터를 NW 비트(비트 29) = 0, CD 비트(비트 30) = 0, PG 비트(비트 31) = 1로 설정하여 캐시 기능과 페이징 기능 활성
    mov eax, cr0            ; EAX 레지스터에 CR0 컨트롤 레지스터 저장
    or eax, 0xE0000000      ; NW 비트(비트 29), CD 비트(비트 30), PG 비트(비트 31) 모두 1로 설정
    xor eax, 0x60000000     ; NW 비트 = 0, CD 비트 = 0, PG 비트 = 1로 설정한 값을 
    mov cr0, eax            ; 다시 CR0 컨트롤 레지스터에 저장
    
    pop edx
    jmp 0x08:0x200000       ; CS 세그먼트 셀렉터를 IA-32e 모드용 코드 세그먼트 디스크립터로 교체하고 0x200000(2MB) 주소로 이동

    jmp $