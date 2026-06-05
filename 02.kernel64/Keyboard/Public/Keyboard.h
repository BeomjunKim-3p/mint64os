#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "Types.h"

// 매크로
// Pause 키 처리 위해 무시해야하는 스캔 코드 수
#define KEY_SKIPCOUNT_FOR_PAUSE 2

// 키 상태 플래그
#define KEY_FLAGS_UP 0x00
#define KEY_FLAGS_DOWN 0x01
#define KEY_FLAGS_EXTENDEDKEY 0x02

// 스캔코드 매핑 테이블 매크로
#define KEY_MAPPINGTABLE_MAXCOUNT 89

#define KEY_NONE 0x00
#define KEY_ENTER '\n'
#define KEY_TAB '\t'
#define KEY_ESCAPE 0x1B
#define KEY_BACKSPACE 0x08

#define KEY_LCTRL 0x80
#define KEY_RCTRL 0x81
#define KEY_LALT 0x82
#define KEY_RALT 0x83

#define KEY_LSHIFT 0x84
#define KEY_RSHIFT 0x85

#define KEY_CAPSLOCK 0x86
#define KEY_NUMLOCK 0x87
#define KEY_SCROLLLOCK 0x88

#define KEY_F1 0x89
#define KEY_F2 0x8A
#define KEY_F3 0x8B
#define KEY_F4 0x8C
#define KEY_F5 0x8D
#define KEY_F6 0x8E
#define KEY_F7 0x8F
#define KEY_F8 0x90
#define KEY_F9 0x91
#define KEY_F10 0x92
#define KEY_F11 0x93
#define KEY_F12 0x94

#define KEY_KP_HOME 0x95
#define KEY_KP_UP 0x96
#define KEY_KP_PGUP 0x97
#define KEY_KP_LEFT 0x98
#define KEY_KP_CENTER 0x99
#define KEY_KP_RIGHT 0x9A
#define KEY_KP_END 0x9B
#define KEY_KP_DOWN 0x9C
#define KEY_KP_PGDN 0x9D
#define KEY_KP_INS 0x9E
#define KEY_KP_DEL 0x9F
#define KEY_KP_7 0xA0
#define KEY_KP_8 0xA1
#define KEY_KP_9 0xA2
#define KEY_KP_4 0xA3
#define KEY_KP_5 0xA4
#define KEY_KP_6 0xA5
#define KEY_KP_1 0xA6
#define KEY_KP_2 0xA7
#define KEY_KP_3 0xA8
#define KEY_KP_0 0xA9
#define KEY_KP_DOT 0xAA
#define KEY_KP_MINUS 0xAB
#define KEY_KP_PLUS 0xAC
#define KEY_KP_DIVIDE 0xAD
#define KEY_KP_MULTIPLY 0xAE
#define KEY_KP_ENTER 0xAF

#define KEY_PAUSE 0xB0
#define KEY_PRINTSCREEN 0xB1

#pragma pack(push, 1)

// 스캔코드 매핑 테이블 구조체
typedef struct kKeyMappingEntryStruct {
	// shift 키나 Caps lock 키와 조합되지 않는 ASCII 코드
	BYTE bNormalCode;

	// shift 키나 Caps lock 키와 조합된 ASCII 코드
	BYTE bCombinedCode;
} KEYMAPPINGENTRY;

// 키보드 상태관리 구조체
typedef struct kKeyboardManagerStruct {
	// 조합 키 정보
	BOOL bShiftDown;
	BOOL bCapsLockDown;
	BOOL bNumLockDown;
	BOOL bScrollLockDown;

	// 확장 키 처리 위한 정보
	BOOL bExtendedCodeIn;
	int iSkipCountForPause;
} KEYBOARDMANAGER;

#pragma pack(pop)

// 함수
BOOL mintIsOutputBufferFull(void);
BOOL mintIsInputBufferFull(void);
BOOL mintActivateKeyboard(void);
BYTE mintGetKeyboardScanCode(void);
BOOL mintChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn,
			   BOOL bScrollLockOn);
void mintEnableA20Gate(void);
void mintReboot(void);
BOOL mintIsAlphabetScanCode(BYTE bScanCode);
BOOL mintIsNumberOrSymbolScanCode(BYTE bScanCode);
BOOL mintIsNumberPadScanCode(BYTE bScanCode);
BOOL mintIsUseCombinedCode(BYTE bScanCode);
void UpdateCombinationKeyStatusAndLED(BYTE bScanCode);
BOOL mintConvertScanCodeToASCIICode(BYTE bScanCode, BYTE *pbASCIICode,
				    BOOL *pbFlags);

#endif /* __KEYBOARD_H__ */