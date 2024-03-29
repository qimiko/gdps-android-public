#ifndef __CCKEYBOARD_DELEGATE_H
#define __CCKEYBOARD_DELEGATE_H

#include <ccMacros.h>
NS_CC_BEGIN

enum enumKeyCodes {
    kCCEnumKeyCodesNone = 0x00,
    kCCEnumKeyCodesBackspace = 0x08,
    kCCEnumKeyCodesTab = 0x09,
    kCCEnumKeyCodesClear = 0x0C,
    kCCEnumKeyCodesEnter = 0x0D,
    kCCEnumKeyCodesShift = 0x10,
    kCCEnumKeyCodesControl = 0x11,
    kCCEnumKeyCodesAlt = 0x12,
    kCCEnumKeyCodesPause = 0x13,
    kCCEnumKeyCodesCapsLock = 0x14,
    kCCEnumKeyCodesEscape = 0x1B,
    kCCEnumKeyCodesSpace = 0x20,
    kCCEnumKeyCodesPageUp = 0x21,
    kCCEnumKeyCodesPageDown = 0x22,
    kCCEnumKeyCodesEnd = 0x23,
    kCCEnumKeyCodesHome = 0x24,
    kCCEnumKeyCodesLeft = 0x25,
    kCCEnumKeyCodesUp = 0x26,
    kCCEnumKeyCodesRight = 0x27,
    kCCEnumKeyCodesDown = 0x28,
    kCCEnumKeyCodesSelect = 0x29,
    kCCEnumKeyCodesPrint = 0x2A,
    kCCEnumKeyCodesExecute = 0x2B,
    kCCEnumKeyCodesPrintScreen = 0x2C,
    kCCEnumKeyCodesInsert = 0x2D,
    kCCEnumKeyCodesDelete = 0x2E,
    kCCEnumKeyCodesHelp = 0x2F,
    kCCEnumKeyCodesZero = 0x30,
    kCCEnumKeyCodesOne = 0x31,
    kCCEnumKeyCodesTwo = 0x32,
    kCCEnumKeyCodesThree = 0x33,
    kCCEnumKeyCodesFour = 0x34,
    kCCEnumKeyCodesFive = 0x35,
    kCCEnumKeyCodesSix = 0x36,
    kCCEnumKeyCodesSeven = 0x37,
    kCCEnumKeyCodesEight = 0x38,
    kCCEnumKeyCodesNine = 0x39,
    kCCEnumKeyCodesA = 0x41,
    kCCEnumKeyCodesB = 0x42,
    kCCEnumKeyCodesC = 0x43,
    kCCEnumKeyCodesD = 0x44,
    kCCEnumKeyCodesE = 0x45,
    kCCEnumKeyCodesF = 0x46,
    kCCEnumKeyCodesG = 0x47,
    kCCEnumKeyCodesH = 0x48,
    kCCEnumKeyCodesI = 0x49,
    kCCEnumKeyCodesJ = 0x4A,
    kCCEnumKeyCodesK = 0x4B,
    kCCEnumKeyCodesL = 0x4C,
    kCCEnumKeyCodesM = 0x4D,
    kCCEnumKeyCodesN = 0x4E,
    kCCEnumKeyCodesO = 0x4F,
    kCCEnumKeyCodesP = 0x50,
    kCCEnumKeyCodesQ = 0x51,
    kCCEnumKeyCodesR = 0x52,
    kCCEnumKeyCodesS = 0x53,
    kCCEnumKeyCodesT = 0x54,
    kCCEnumKeyCodesU = 0x55,
    kCCEnumKeyCodesV = 0x56,
    kCCEnumKeyCodesW = 0x57,
    kCCEnumKeyCodesX = 0x58,
    kCCEnumKeyCodesY = 0x59,
    kCCEnumKeyCodesZ = 0x5A,
    kCCEnumKeyCodesLeftWindowsKey = 0x5B,
    kCCEnumKeyCodesRightWindowsKey = 0x5C,
    kCCEnumKeyCodesApplicationsKey = 0x5D,
    kCCEnumKeyCodesSleep = 0x5F,
    kCCEnumKeyCodesNumPad0 = 0x60,
    kCCEnumKeyCodesNumPad1 = 0x61,
    kCCEnumKeyCodesNumPad2 = 0x62,
    kCCEnumKeyCodesNumPad3 = 0x63,
    kCCEnumKeyCodesNumPad4 = 0x64,
    kCCEnumKeyCodesNumPad5 = 0x65,
    kCCEnumKeyCodesNumPad6 = 0x66,
    kCCEnumKeyCodesNumPad7 = 0x67,
    kCCEnumKeyCodesNumPad8 = 0x68,
    kCCEnumKeyCodesNumPad9 = 0x69,
    kCCEnumKeyCodesMultiply = 0x6A,
    kCCEnumKeyCodesAdd = 0x6B,
    kCCEnumKeyCodesSeperator = 0x6C,
    kCCEnumKeyCodesSubtract = 0x6D,
    kCCEnumKeyCodesDecimal = 0x6E,
    kCCEnumKeyCodesDivide = 0x6F,
    kCCEnumKeyCodesF1 = 0x70,
    kCCEnumKeyCodesF2 = 0x71,
    kCCEnumKeyCodesF3 = 0x72,
    kCCEnumKeyCodesF4 = 0x73,
    kCCEnumKeyCodesF5 = 0x74,
    kCCEnumKeyCodesF6 = 0x75,
    kCCEnumKeyCodesF7 = 0x76,
    kCCEnumKeyCodesF8 = 0x77,
    kCCEnumKeyCodesF9 = 0x78,
    kCCEnumKeyCodesF10 = 0x79,
    kCCEnumKeyCodesF11 = 0x7A,
    kCCEnumKeyCodesF12 = 0x7B,
    kCCEnumKeyCodesF13 = 0x7C,
    kCCEnumKeyCodesF14 = 0x7D,
    kCCEnumKeyCodesF15 = 0x7E,
    kCCEnumKeyCodesF16 = 0x7F,
    kCCEnumKeyCodesF17 = 0x80,
    kCCEnumKeyCodesF18 = 0x81,
    kCCEnumKeyCodesF19 = 0x82,
    kCCEnumKeyCodesF20 = 0x83,
    kCCEnumKeyCodesF21 = 0x84,
    kCCEnumKeyCodesF22 = 0x85,
    kCCEnumKeyCodesF23 = 0x86,
    kCCEnumKeyCodesF24 = 0x87,
    kCCEnumKeyCodesNumlock = 0x90,
    kCCEnumKeyCodesScrollLock = 0x91,
    kCCEnumKeyCodesLeftShift = 0xA0,
    kCCEnumKeyCodesRightShift = 0xA1,
    kCCEnumKeyCodesLeftControl = 0xA2,
    kCCEnumKeyCodesRightControl = 0xA3,
    kCCEnumKeyCodesLeftMenu = 0xA4,
    kCCEnumKeyCodesRightMenu = 0xA5,
    kCCEnumKeyCodesBrowserBack = 0xA6,
    kCCEnumKeyCodesBrowserForward = 0xA7,
    kCCEnumKeyCodesBrowserRefresh = 0xA8,
    kCCEnumKeyCodesBrowserStop = 0xA9,
    kCCEnumKeyCodesBrowserSearch = 0xAA,
    kCCEnumKeyCodesBrowserFavorites = 0xAB,
    kCCEnumKeyCodesBrowserHome = 0xAC,
    kCCEnumKeyCodesVolumeMute = 0xAD,
    kCCEnumKeyCodesVolumeDown = 0xAE,
    kCCEnumKeyCodesVolumeUp = 0xAF,
    kCCEnumKeyCodesNextTrack = 0xB0,
    kCCEnumKeyCodesPreviousTrack = 0xB1,
    kCCEnumKeyCodesStopMedia = 0xB2,
    kCCEnumKeyCodesPlayPause = 0xB3,
    kCCEnumKeyCodesLaunchMail = 0xB4,
    kCCEnumKeyCodesSelectMedia = 0xB5,
    kCCEnumKeyCodesLaunchApp1 = 0xB6,
    kCCEnumKeyCodesLaunchApp2 = 0xB7,
    kCCEnumKeyCodesOEM1 = 0xBA,
    kCCEnumKeyCodesOEMPlus = 0xB8,
    kCCEnumKeyCodesOEMComma = 0xBC,
    kCCEnumKeyCodesOEMMinus = 0xBD,
    kCCEnumKeyCodesOEMPeriod = 0xBE,
    kCCEnumKeyCodesOEM2 = 0xBF,
    kCCEnumKeyCodesOEM3 = 0xC0,
    kCCEnumKeyCodesOEM4 = 0xDB,
    kCCEnumKeyCodesOEM5 = 0xDC,
    kCCEnumKeyCodesOEM6 = 0xDD,
    kCCEnumKeyCodesOEM7 = 0xDE,
    kCCEnumKeyCodesOEM8 = 0xDF,
    kCCEnumKeyCodesOEM102 = 0xE2,
    kCCEnumKeyCodesProcess = 0xE5,
    kCCEnumKeyCodesPacket = 0xE7,
    kCCEnumKeyCodesAttn = 0xF6,
    kCCEnumKeyCodesCrSel = 0xF7,
    kCCEnumKeyCodesExSel = 0xF8,
    kCCEnumKeyCodesEraseEOF = 0xF9,
    kCCEnumKeyCodesPlay = 0xFA,
    kCCEnumKeyCodesZoom = 0xFB,
    kCCEnumKeyCodesPA1 = 0xFD,
    kCCEnumKeyCodesOEMClear = 0xFE,
    kCCEnumKeyCodesArrowUp = 0x11B,
    kCCEnumKeyCodesArrowDown = 0x11C,
    kCCEnumKeyCodesArrowLeft = 0x11D,
    kCCEnumKeyCodesArrowRight = 0x11E,

	kCCEnumKeyCodesControllerA = 0x3E9,
	kCCEnumKeyCodesControllerB = 0x3EA,
	kCCEnumKeyCodesControllerY = 0x3EB,
	kCCEnumKeyCodesControllerX = 0x3EC,
	kCCEnumKeyCodesControllerStart = 0x3ED,
	kCCEnumKeyCodesControllerBack = 0x3EE,
	kCCEnumKeyCodesControllerRB = 0x3EF,
	kCCEnumKeyCodesControllerLB = 0x3F0,
	kCCEnumKeyCodesControllerRT = 0x3F1,
	kCCEnumKeyCodesControllerLT = 0x3F2,
	kCCEnumKeyCodesControllerUp = 0x3F3,
	kCCEnumKeyCodesControllerDown = 0x3F4,
	kCCEnumKeyCodesControllerLeft = 0x3F5,
	kCCEnumKeyCodesControllerRight = 0x3F6
};
class CC_DLL CCKeyboardDelegate {
public:
	virtual void keyDown(enumKeyCodes);
	virtual void keyUp(enumKeyCodes);
};

NS_CC_END
#endif