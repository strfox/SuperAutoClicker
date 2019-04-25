#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>

#include "beep.h"
#include "click.h"

#define MS_DIGITS_MAX 5

/**
 * Forward declarations
 */
void NUMPADProc(DWORD vkCode);

void K_MAINProc();

void K_SETProc();

void ErrorHandler(LPTSTR lpszFunction);

void K_SWITCHProc();

/*
 * State of the SuperAutoClicker.
 */
BOOL isClicking;
BOOL isListening;

/*
 * Buffer for storing numbers entered by the user while setting a millisecond timer.
 */
int msInput[MS_DIGITS_MAX];
int digitsAdded;

/*
 * Key bindings
 */
int K_MAIN = VK_ADD;
int K_SET = VK_SUBTRACT;
int K_SWITCH = VK_DIVIDE;

/*
 * This array is used to store the status of the keyboard and the key at position x.
 *
 * For example, keysPressed[VK_ENTER] will be set to 'true' if Enter is pressed.
 * I assume there aren't more than 256 keys.
 */
BOOL keysPressed[256];


HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    kbdStruct = *((KBDLLHOOKSTRUCT *) lParam);
    DWORD vkCode = kbdStruct.vkCode;
    assert(vkCode < 256);

    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN && !keysPressed[vkCode]) {

            // Mark key as pressed
            keysPressed[vkCode] = TRUE;

            if (vkCode == K_MAIN) {
                K_MAINProc();
            } else if (vkCode == K_SET) {
                K_SETProc();
            } else if (vkCode == K_SWITCH) {
                K_SWITCHProc();
            } else if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) {
                NUMPADProc(vkCode);
            }
        } else if (wParam == WM_KEYUP) {
            // Mark key as released
            keysPressed[vkCode] = FALSE;
        }
    }

    // Call the next hook on the hook chain.
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void CreateHook() {
    _hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
    if (!_hook) {
        MessageBox(NULL, "Failed to create hook", "Error", MB_ICONERROR);
    }
}

void ReleaseHook() {
    UnhookWindowsHookEx(_hook);
}

int GetEnteredMs() {
    int ms = 0;

    for (int i = 0; i < digitsAdded; i++) {
        ms *= 10;
        ms += msInput[i];
    }
    return ms;
}

void StartPerforming() {
    assert(!isClicking);

    if (isListening) {
        isListening = FALSE;
    }

    int ms = GetEnteredMs();
    if (ms == 0) {
        BeepError();
        printf("Please set the milliseconds before starting. Press - to start listen mode.\n");
    } else {
        BeepStart();
        printf("\nOK, clicking at %d milliseconds interval. Press + to toggle off.\n", ms);
        isClicking = TRUE;
        StartClickThread(ms);
    }
}

void StopPerforming() {
    assert(isClicking);
    isClicking = FALSE;

    BeepStop();
    printf("Autoclick off.\n");
    StopClickThread();
}

void K_SETProc() {
    isListening = !isListening;

    if (isListening) {
        BeepListenStart();
        printf("Listen mode ON: Use your numpad to enter milliseconds.\n");
        memset(msInput, 0, sizeof(msInput));
        digitsAdded = 0;
    } else {
        BeepListenStop();
        int ms = GetEnteredMs();
        printf("\nOK, interval is set to %d ms. Press + to start autoclick.\n", ms);
        SetMsInterval(ms);
    }
}

void K_MAINProc() {
    if (isClicking) {
        StopPerforming();
    } else {
        StartPerforming();
    }
}

void NUMPADProc(DWORD vkCode) {
    assert(vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9);

    if (isListening) {
        int number;
        // Honestly, I can't find a better way to do this without macros.
        // And I don't want to use macros.
        switch (vkCode) {
            case VK_NUMPAD0:
                number = 0;
                break;
            case VK_NUMPAD1:
                number = 1;
                break;
            case VK_NUMPAD2:
                number = 2;
                break;
            case VK_NUMPAD3:
                number = 3;
                break;
            case VK_NUMPAD4:
                number = 4;
                break;
            case VK_NUMPAD5:
                number = 5;
                break;
            case VK_NUMPAD6:
                number = 6;
                break;
            case VK_NUMPAD7:
                number = 7;
                break;
            case VK_NUMPAD8:
                number = 8;
                break;
            case VK_NUMPAD9:
                number = 9;
                break;
            default:
                assert(FALSE);
        }

        assert(number >= 0 && number <= 9);

        if (digitsAdded < MS_DIGITS_MAX) {
            msInput[digitsAdded++] = number;
            BeepType();
            printf("%d", number);
        } else {
            BeepInputLimit();
        }

        assert(digitsAdded < sizeof(msInput));
    }
}

////-50+
void K_SWITCHProc() {
    mousebtn_t current = GetMouseButton();
    switch (current) {
        case MOUSEBTN_LEFT:
            SetMouseButton(MOUSEBTN_RIGHT);
            printf("Using right mouse button.\n");
            break;
        case MOUSEBTN_RIGHT:
            SetMouseButton(MOUSEBTN_LEFT);
            printf("Using left mouse button.\n");
            break;
        default:
            assert(FALSE);
    }
    BeepSwitchButton();
}

int main() {
    printf("%s version %s by Michel Faria\n"
           "\n"
           "[[ Use your numpad to control %s ]]\n"
           "/ -> Switch between left and right mouse buttons.\n"
           "- -> Start millisecond input mode. (MAX 5 DIGITS)\n"
           "+ -> Toggle autoclick.\n"
           "\n"
           "Press - and then type the click interval you want to use in milliseconds, then press + to begin autoclicking.\n"
           "You can toggle the autoclick with the + key, and you don't have to set the milliseconds every time you toggle.\n",
           PROGRAM_TITLE, PROGRAM_VERSION, PROGRAM_TITLE);

    CreateHook();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        // Empty loop just to keep the hook from timing out
    }

    ReleaseHook();
    return 0;
}

//void ErrorHandler(LPTSTR lpszFunction) {
//    // Retrieve the system error message for the last-error code.
//
//    LPVOID lpMsgBuf;
//    LPVOID lpDisplayBuf;
//    DWORD dw = GetLastError();
//
//    FormatMessage(
//            FORMAT_MESSAGE_ALLOCATE_BUFFER |
//            FORMAT_MESSAGE_FROM_SYSTEM |
//            FORMAT_MESSAGE_IGNORE_INSERTS,
//            NULL,
//            dw,
//            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//            (LPTSTR) &lpMsgBuf,
//            0, NULL);
//
//    // Display the error message.
//
//    lpDisplayBuf = (LPVOID) LocalAlloc(LMEM_ZEROINIT,
//                                       (lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) lpszFunction) + 40) *
//                                       sizeof(TCHAR));
//    snprintf((LPTSTR) lpDisplayBuf,
//             LocalSize(lpDisplayBuf) / sizeof(TCHAR),
//             TEXT("%s failed with error %d: %s"),
//             lpszFunction, (int) dw, lpMsgBuf);
//    MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK);
//
//    // Free error-handling buffer allocations.
//
//    LocalFree(lpMsgBuf);
//    LocalFree(lpDisplayBuf);
//}