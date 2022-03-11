#include "mainwindow.h"
#include <math.h>
#include <QApplication>
HHOOK hHook = nullptr;
int dummy = 0;
int test  = 0;
QApplication a(dummy,nullptr);
MainWindow mainwindow;
void UpdateKeyState(BYTE *keystate, int keycode)
{
    keystate[keycode] = GetKeyState(keycode);
}
LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    //WPARAM is WM_KEYDOWn, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP
    //LPARAM is the key information

    if (wParam == WM_KEYDOWN || wParam == WM_KEYUP)
    {
        //Get the key information
        KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);

        wchar_t buffer[5];

        //get the keyboard state
        BYTE keyboard_state[256];
        GetKeyboardState(keyboard_state);
//        UpdateKeyState(keyboard_state, VK_SHIFT);
//        UpdateKeyState(keyboard_state, VK_CAPITAL);
//        UpdateKeyState(keyboard_state, VK_CONTROL);
//        UpdateKeyState(keyboard_state, VK_MENU);

        //Get keyboard layout
        HKL keyboard_layout = GetKeyboardLayout(0);

        //Get the name
        char lpszName[0X100] = {0};

        DWORD dwMsg = 1;
        dwMsg += cKey.scanCode << 16;
        dwMsg += cKey.flags << 24;

        //int i = GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);

        //Try to convert the key information
        //int result = ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
        //buffer[4] = L'\0';

        //Print the output
        //qDebug() << "Key: " << cKey.vkCode << " " << QString::fromUtf16((ushort*)buffer) << " "<< QString::fromUtf16((ushort*)lpszName);
        QString keyname = QString::fromUtf16((ushort*)lpszName);

        //qDebug() << "test=" << ++test;
        QEvent::Type keypressType = (wParam == WM_KEYDOWN) ? QKeyEvent::KeyPress : QKeyEvent::KeyRelease;
        QKeyEvent event(keypressType, cKey.vkCode, Qt::NoModifier,0,0,0,keyname);
        QApplication::sendEvent(&mainwindow, &event);
        //mainwindow.KeyPressAction(wParam, cKey.vkCode, keyname);
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}
int main(int argc, char *argv[])
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
    if (hHook == nullptr)
        qDebug("Hook Failed");

    mainwindow.show();
    return a.exec();
}
