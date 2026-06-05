[BITS 64]

SECTION .text

global mintInPortByte
global mintOutPortByte
global mintLoadGDTR
global mintLoadTR
global mintLoadIDTR

; 포트로부터 1바이트를 읽음
;     PARAM: 포트 번호
mintInPortByte:
    push rdx

    mov rdx, rdi
    mov rax, 0
    in al, dx

    pop rdx
    ret

; 포트로 1바이트를 출력
;     PARAM: 포트 번호, 출력할 데이터
mintOutPortByte:
    push rdx
    push rax

    mov rdx, rdi
    mov rax, rsi
    out dx, al

    pop rax 
    pop rdx
    ret

; GDTR 레지스터에 GDT 테이블의 주소를 로드
;     PARAM: GDT 테이블의 주소
mintLoadGDTR:
    lgdt [rdi]
    ret

; TR 레지스터에 TSS 세그먼트 디스크립터를 로드
;     PARAM: TSS 세그먼트 디스크립터의 세그먼트 셀렉터
mintLoadTR:
    ltr di
    ret

; IDTR 레지스터에 IDT 테이블의 주소를 로드
;     PARAM: IDT 테이블의 주소
mintLoadIDTR:
    lidt [rdi]
    ret