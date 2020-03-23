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



std::wstring Decrypt(std::wstring str)
{
    std::wstring res = L"";
    for (int i = 0; i < str.length(); ++i)
    {
        res += (WCHAR)str[i] ^ 0x4242;
    }
    return res;
}

int main()
{
    WCHAR buffer[1000];
    DWORD nread;
    HANDLE hFile = CreateFile(L"C:\\Users\\tutq9\\AppData\\Roaming\\Ser1ce Host\\23032020-144232.dat", FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    ReadFile(hFile, buffer, 1000, &nread, NULL);

    
    std::wcout << Decrypt(buffer);

    return 0;
}