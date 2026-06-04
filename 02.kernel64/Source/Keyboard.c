#include "Keyboard.h"
#include "AssemblyUtility.h"
#include "Types.h"

BOOL mintIsOutputBufferFull(void)
{
	if (mintInPortByte(0x64) & 0x01) {
		return TRUE;
	}
	return FALSE;
}


BOOL mintIsInputBufferFull(void)
{
	if (mintInPortByte(0x64) & 0x02) {
		return TRUE;
	}
	return FALSE;
}

BOOL mintActivateKeyboard(void)
{
	int i;
	int j;

	mintOutPortByte(0x64, 0xAE);

	for (i = 0; i < 0xFFFF; i++) {
		if (mintIsInputBufferFull() == FALSE) {
			break;
		}
	}
	mintOutPortByte(0x60, 0xF4);

	for (j = 0; j < 100; j++) {
		for (i = 0; i < 0xFFFF; i++) {
			if (mintIsOutputBufferFull() == TRUE) {
				break;
			}
		}
		if (mintInPortByte(0x60) == 0xFA) {
			return TRUE;
		}
	}
	return FALSE;
}

BYTE mintGetKeyboardScanCode(void)
{
	while (mintIsOutputBufferFull() == FALSE) {
		;
	}
	return mintInPortByte(0x60);
}

#if 0 //origine
BOOL mintChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn,
			   BOOL bScrollLockOn)
{
	int i;
	int j;

	for (i = 0; i < 0xFFFF; i++) {
		if (mintIsInputBufferFull() == FALSE) {
			break;
		}
	}
	mintOutPortByte(0x64, 0xED);

	for (i = 0; i < 0xFFFF; i++) {
		if (mintIsInputBufferFull() == TRUE) {
			break;
		}
	}

	for (j = 0; j < 100; j++) {
		for (i = 0; i < 0xFFFF; i++) {
			if (mintIsOutputBufferFull() == TRUE) {
				break;
			}
		}
		if (mintInPortByte(0x60) == 0xFA) {
			break;
		}
	}
	if (j >= 100) {
		return FALSE;
	}
	mintOutPortByte(0x60,
			(bCapsLockOn << 2) | (bNumLockOn << 1) | bScrollLockOn);
	for (i = 0; i < 0xFFFF; i++) {
		if (mintIsInputBufferFull() == FALSE) {
			break;
		}
	}
	for (j = 0; j < 100; j++) {
		for (i = 0; i < 0xFFFF; i++) {
			if (mintIsOutputBufferFull() == TRUE) {
				break;
			}
		}
		if (mintInPortByte(0x60) == 0xFA) {
			break;
		}
	}
	if (j >= 100) {
		return FALSE;
	}
	return TRUE;
}
#endif
BOOL mintChangeKeyboardLED( BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn )
{
    int i, j;

    // 키보드에 LED 변경 커맨드 전송하고 커맨드가 처리될 때까지 대기
    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        // 출력 버퍼(포트 0x60)가 비었으면 커맨드 전송 가능
        if( mintIsInputBufferFull() == FALSE )
        {
            break;
        }
    }

    // 출력 버퍼(포트 0x60)로 LED 상태 변경 커맨드(0xED) 전송
    mintOutPortByte( 0x60, 0xED );
    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        // 입력 버퍼(포트 0x60)가 비어있으면 키보드가 커맨드를 가져간 것임
        if( mintIsInputBufferFull() == FALSE )
        {
            break;
        }
    }

    // 키보드가 LED 상태 변경 커맨드를 가져갔으므로 ACK가 올때까지 대기
    for( j = 0 ; j < 100 ; j++ )
    {
        for( i = 0 ; i < 0xFFFF ; i++ )
        {
            // 출력 버퍼(포트 0x60)가 차있으면 데이터를 읽을 수 있음
            if( mintIsOutputBufferFull() == TRUE )
            {
                break;
            }
        }

        // 출력 버퍼(포트 0x60)에서 읽은 데이터가 ACK(0xFA)이면 성공
        if( mintInPortByte( 0x60 ) == 0xFA )
        {
            break;
        }
    }
    if( j >= 100 )
    {
        return FALSE;
    }

    // LED 변경 값을 키보드로 전송하고 데이터가 처리가 완료될 때까지 대기
    mintOutPortByte( 0x60, ( bCapsLockOn << 2 ) | ( bNumLockOn << 1 ) | bScrollLockOn );
    for( i = 0 ; i < 0xFFFF ; i++ )
    {
        // 입력 버퍼(포트 0x60)가 비어있으면 키보드가 LED 데이터를 가져간 것임
        if( mintIsInputBufferFull() == FALSE )
        {
            break;
        }
    }

    // 키보드가 LED 데이터를 가져갔으므로 ACK가 올 때까지 대기함
    for( j = 0 ; j < 100 ; j++ )
    {
        for( i = 0 ; i < 0xFFFF ; i++ )
        {
            // 출력 버퍼(포트 0x60)가 차있으면 데이터를 읽을 수 있음
            if( mintIsOutputBufferFull() == TRUE )
            {
                break;
            }
        }

        // 출력 버퍼(포트 0x60)에서 읽은 데이터가 ACK(0xFA)이면 성공
        if( mintInPortByte( 0x60 ) == 0xFA )
        {
            break;
        }
    }
    if( j >= 100 )
    {
        return FALSE;
    }

    return TRUE;
}

void mintEnableA20Gate(void)
{
	BYTE bOutputPortData;
	int i;

	mintOutPortByte(0x64, 0xD0);
	for (i = 0; i < 0xFFFF; i++) {
		if (mintIsOutputBufferFull() == TRUE) {
			break;
		}
	}
	bOutputPortData = mintInPortByte(0x60);

	bOutputPortData |= 0x01;

	for (i = 0; i < 0xFFFF; i++) {
		if (mintIsInputBufferFull() == FALSE) {
			break;
		}
	}
	mintOutPortByte(0x64, 0xD1);

	mintOutPortByte(0x64, bOutputPortData);
}

void mintReboot(void)
{
	int i;

	for (i = 0; i < 0xFFFF; i++) {
		if (mintIsInputBufferFull() == FALSE) {
			break;
		}
	}
	mintOutPortByte(0x64, 0xD1);

	mintOutPortByte(0x60, 0x00);

	while (1) {
		;
	}
}

static KEYBOARDMANAGER gs_stKeyboardManager = {
	0,
};

static KEYMAPPINGENTRY gs_vstKeyMappingTable[KEY_MAPPINGTABLE_MAXCOUNT] = {
	/*0x00, 00*/ {KEY_NONE, KEY_NONE},
	/*0x01, 01*/ {KEY_ESCAPE, KEY_ESCAPE},
	/*0x02, 02*/ {'1', '!'},
	/*0x03, 03*/ {'2', '@'},
	/*0x04, 04*/ {'3', '#'},
	/*0x05, 05*/ {'4', '$'},
	/*0x06, 06*/ {'5', '%'},
	/*0x07, 07*/ {'6', '^'},
	/*0x08, 08*/ {'7', '&'},
	/*0x09, 09*/ {'8', '*'},
	/*0x0A, 10*/ {'9', '('},
	/*0x0B, 11*/ {'0', ')'},
	/*0x0C, 12*/ {'-', '_'},
	/*0x0D, 13*/ {'=', '+'},
	/*0x0E, 14*/ {KEY_BACKSPACE, KEY_BACKSPACE},
	/*0x0F, 15*/ {KEY_TAB, KEY_TAB},
	/*0x10, 16*/ {'q', 'Q'},
	/*0x11, 17*/ {'w', 'W'},
	/*0x12, 18*/ {'e', 'E'},
	/*0x13, 19*/ {'r', 'R'},
	/*0x14, 20*/ {'t', 'T'},
	/*0x15, 21*/ {'y', 'Y'},
	/*0x16, 22*/ {'u', 'U'},
	/*0x17, 23*/ {'i', 'I'},
	/*0x18, 24*/ {'o', 'O'},
	/*0x19, 25*/ {'p', 'P'},
	/*0x1A, 26*/ {'[', '{'},
	/*0x1B, 27*/ {']', '}'},
	/*0x1C, 28*/ {KEY_ENTER, KEY_ENTER},
	/*0x1D, 29*/ {KEY_LCTRL, KEY_LCTRL},
	/*0x1E, 30*/ {'a', 'A'},
	/*0x1F, 31*/ {'s', 'S'},
	/*0x20, 32*/ {'d', 'D'},
	/*0x21, 33*/ {'f', 'F'},
	/*0x22, 34*/ {'g', 'G'},
	/*0x23, 35*/ {'h', 'H'},
	/*0x24, 36*/ {'j', 'J'},
	/*0x25, 37*/ {'k', 'K'},
	/*0x26, 38*/ {'l', 'L'},
	/*0x27, 39*/ {';', ':'},
	/*0x28, 40*/ {'\'', '\"'},
	/*0x29, 41*/ {'`', '~'},
	/*0x2A, 42*/ {KEY_LSHIFT, KEY_LSHIFT},
	/*0x2B, 43*/ {'\\', '|'},
	/*0x2C, 44*/ {'z', 'Z'},
	/*0x2D, 45*/ {'x', 'X'},
	/*0x2E, 46*/ {'c', 'C'},
	/*0x2F, 47*/ {'v', 'V'},
	/*0x30, 48*/ {'b', 'B'},
	/*0x31, 49*/ {'n', 'N'},
	/*0x32, 50*/ {'m', 'M'},
	/*0x33, 51*/ {',', '<'},
	/*0x34, 52*/ {'.', '>'},
	/*0x35, 53*/ {'/', '?'},
	/*0x36, 54*/ {KEY_RSHIFT, KEY_RSHIFT},
	/*0x37, 55*/ {KEY_KP_MULTIPLY, KEY_KP_MULTIPLY},
	/*0x38, 56*/ {KEY_LALT, KEY_LALT},
	/*0x39, 57*/ {' ', ' '},
	/*0x3A, 58*/ {KEY_CAPSLOCK, KEY_CAPSLOCK},
	/*0x3B, 59*/ {KEY_F1, KEY_F1},
	/*0x3C, 60*/ {KEY_F2, KEY_F2},
	/*0x3D, 61*/ {KEY_F3, KEY_F3},
	/*0x3E, 62*/ {KEY_F4, KEY_F4},
	/*0x3F, 63*/ {KEY_F5, KEY_F5},
	/*0x40, 64*/ {KEY_F6, KEY_F6},
	/*0x41, 65*/ {KEY_F7, KEY_F7},
	/*0x42, 66*/ {KEY_F8, KEY_F8},
	/*0x43, 67*/ {KEY_F9, KEY_F9},
	/*0x44, 68*/ {KEY_F10, KEY_F10},
	/*0x45, 69*/ {KEY_NUMLOCK, KEY_NUMLOCK},
	/*0x46, 70*/ {KEY_SCROLLLOCK, KEY_SCROLLLOCK},
	/*0x47, 71*/ {KEY_KP_HOME, KEY_KP_7},
	/*0x48, 72*/ {KEY_KP_UP, KEY_KP_8},
	/*0x49, 73*/ {KEY_KP_PGUP, KEY_KP_9},
	/*0x4A, 74*/ {KEY_KP_MINUS, KEY_KP_MINUS},
	/*0x4B, 75*/ {KEY_KP_LEFT, KEY_KP_4},
	/*0x4C, 76*/ {KEY_KP_CENTER, KEY_KP_5},
	/*0x4D, 77*/ {KEY_KP_RIGHT, KEY_KP_6},
	/*0x4E, 78*/ {KEY_KP_PLUS, KEY_KP_PLUS},
	/*0x4F, 79*/ {KEY_KP_END, KEY_KP_1},
	/*0x50, 80*/ {KEY_KP_DOWN, KEY_KP_2},
	/*0x51, 81*/ {KEY_KP_PGDN, KEY_KP_3},
	/*0x52, 82*/ {KEY_KP_INS, KEY_KP_0},
	/*0x53, 83*/ {KEY_KP_DEL, KEY_KP_DOT},
	/*0x54, 84*/ {KEY_NONE, KEY_NONE},
	/*0x55, 85*/ {KEY_NONE, KEY_NONE},
	/*0x56, 86*/ {KEY_NONE, KEY_NONE},
	/*0x57, 87*/ {KEY_F11, KEY_F11},
	/*0x58, 88*/ {KEY_F12, KEY_F12},
};

BOOL mintIsAlphabetScanCode(BYTE bScanCode)
{
	if ('a' <= gs_vstKeyMappingTable[bScanCode].bNormalCode &&
	    gs_vstKeyMappingTable[bScanCode].bNormalCode <= 'z') {
		return TRUE;
	}
	return FALSE;
}

BOOL mintIsNumberOrSymbolScanCode(BYTE bScanCode)
{
	// 숫자 패드나 확장 키 범위를 제외한 범위(스캔코드 2~53)에서 영문자가
	// 아니면 숫자나 기호임
	if ((2 <= bScanCode) && (bScanCode <= 53) &&
	    (mintIsAlphabetScanCode(bScanCode) == FALSE)) {
		return TRUE;
	}
	return FALSE;
}

BOOL mintIsNumberPadScanCode(BYTE bScanCode)
{
	if ((71 <= bScanCode) && (bScanCode <= 83)) {
		return TRUE;
	}
	return FALSE;
}

BOOL mintIsUseCombinedCode(BYTE bScanCode)
{
	BYTE bDownScanCode;
	BOOL bUseCombinedKey = FALSE;  // 기본값 초기화

	bDownScanCode = bScanCode & 0x7F;

	// 알파벳 키라면 Shift 키와 Caps Lock의 영향을 받음
	if (mintIsAlphabetScanCode(bDownScanCode) == TRUE) {
		// 만약 Shift 키와 Caps Lock 키 중에 하나만 눌려있으면 조합된
		// 키를 되돌려 줌
		if (gs_stKeyboardManager.bShiftDown ^
		    gs_stKeyboardManager.bCapsLockDown) {
			bUseCombinedKey = TRUE;
		} else {
			bUseCombinedKey = FALSE;
		}
	}
	// 숫자와 기호키는 Shift 키의 영향만 받음
	else if (mintIsNumberOrSymbolScanCode(bDownScanCode) == TRUE) {
		// Shift 키가 눌려있으면 조합된 키를 되돌려 줌
		if (gs_stKeyboardManager.bShiftDown == TRUE) {
			bUseCombinedKey = TRUE;
		} else {
			bUseCombinedKey = FALSE;
		}
	}
	// 숫자 패드 키라면 Num Lock 키의 영향만 받음
	// 0xE0만 제외하면 확장 키 코드와 숫자패드 키코드가 겹치므로 확장
	// 키코드가 수신되지 않았을때만 처리 조합된 코드 사용
	else if ((mintIsNumberPadScanCode(bDownScanCode) == TRUE) &&
		 (gs_stKeyboardManager.bExtendedCodeIn == FALSE)) {
		// Num Lock 키가 켜져있으면 조합된 키를 되돌려 줌
		if (gs_stKeyboardManager.bNumLockDown == TRUE) {
			bUseCombinedKey = TRUE;
		} else {
			bUseCombinedKey = FALSE;
		}
	}
	return bUseCombinedKey;
}

void UpdateCombinationKeyStatusAndLED(BYTE bScanCode)
{
	BOOL bDown;
	BYTE bDownScanCode;
	BOOL bLEDStatusChanged = FALSE;

	// 눌림 또는 떨어짐 상태처리, 비트7이 1이면 키가 떨어졌음을 의미하고
	// 0이면 떨어졌음을 의미함
	if (bScanCode & 0x80) {
		bDown = FALSE;
		bDownScanCode = bScanCode & 0x7F;
	} else {
		bDown = TRUE;
		bDownScanCode = bScanCode;
	}

	// 조합 키 검색
	// Shift 키 스캔코드 (왼쪽 Shift 0x2A, 오른쪽 Shift 0x36)이면 Shift 키
	// 상태 변경
	if ((bDownScanCode == 0x2A) || (bDownScanCode == 0x36)) {
		gs_stKeyboardManager.bShiftDown = bDown;
	}
	// Caps Lock 키 스캔코드(0x3A)이면 Caps Lock 키 상태 변경 및 LED 상태
	// 변경
	else if ((bDownScanCode == 0x3A) && (bDown == TRUE)) {
		gs_stKeyboardManager.bCapsLockDown ^= TRUE;
		bLEDStatusChanged = TRUE;
	}
	// Num Lock 키 스캔코드(0x45)이면 Num Lock 키 상태 변경 및 LED 상태 변경
	else if ((bDownScanCode == 0x45) && (bDown == TRUE)) {
		gs_stKeyboardManager.bNumLockDown ^= TRUE;
		bLEDStatusChanged = TRUE;
	}
	// Scroll Lock 키 스캔코드(0x46)이면 Scroll Lock 키 상태 변경 및 LED
	// 상태 변경
	else if ((bDownScanCode == 0x46) && (bDown == TRUE)) {
		gs_stKeyboardManager.bScrollLockDown ^= TRUE;
		bLEDStatusChanged = TRUE;
	}

	// LED 상태가 변경되었으면 키보드에 변경된 상태를 반영
	if (bLEDStatusChanged == TRUE) {
		mintChangeKeyboardLED(gs_stKeyboardManager.bCapsLockDown,
				      gs_stKeyboardManager.bNumLockDown,
				      gs_stKeyboardManager.bScrollLockDown);
	}
}

BOOL mintConvertScanCodeToASCIICode(BYTE bScanCode, BYTE *pbASCIICode,
				    BOOL *pbFlags)
{
	BOOL bUseCombinedKey;

	// 이전에 Pause 키가 수신되었다면 Pause의 남은 스캔코드 무시
	if (gs_stKeyboardManager.iSkipCountForPause > 0) {
		gs_stKeyboardManager.iSkipCountForPause--;
		return FALSE;
	}

	// pause 키는 특수처리
	if (bScanCode == 0xE1) {
		*pbASCIICode = KEY_PAUSE;
		*pbFlags = KEY_FLAGS_DOWN;
		gs_stKeyboardManager.iSkipCountForPause =
			KEY_SKIPCOUNT_FOR_PAUSE;
		return TRUE;
	}
	// 확장 키코드 처리
	else if (bScanCode == 0xE0) {
		gs_stKeyboardManager.bExtendedCodeIn = TRUE;
		return FALSE;
	}
	// 조합된 키 반환 여부
	bUseCombinedKey = mintIsUseCombinedCode(bScanCode);

	// 키 값 설정
	if (bUseCombinedKey == TRUE) {
		*pbASCIICode =
			gs_vstKeyMappingTable[bScanCode & 0x7F].bCombinedCode;
		
	} else {
		*pbASCIICode =
			gs_vstKeyMappingTable[bScanCode & 0x7F].bNormalCode;
	}

	// 확장 키 유무 설정
	if (gs_stKeyboardManager.bExtendedCodeIn == TRUE) {
		*pbFlags = KEY_FLAGS_EXTENDEDKEY;
		gs_stKeyboardManager.bExtendedCodeIn = FALSE;
	} else {
		*pbFlags = 0;
	}

	// 눌림/떨어짐 플래그 설정
	if ((bScanCode & 0x80) == 0) {
		*pbFlags |= KEY_FLAGS_DOWN;
	}

	// 조합키 또는 떨어짐 상태 갱신
	UpdateCombinationKeyStatusAndLED(bScanCode);
	return TRUE;
}