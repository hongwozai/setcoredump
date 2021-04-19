#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <tlhelp32.h>

#include <psapi.h>
#pragma comment(lib, "Psapi.lib")

#include "setcoredump.h"

BOOL SetCurrentPrivilege(BOOL bEnable, LPCTSTR lpszPrivilege)
{
    HANDLE hToken = 0;
    if(::OpenThreadToken(::GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken)
       || ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        TOKEN_PRIVILEGES tp;
        LUID luid;

        if(!::LookupPrivilegeValue(
               NULL,            // lookup privilege on local system
               lpszPrivilege,   // privilege to lookup
               &luid ) )        // receives LUID of privilege
        {
            ::CloseHandle(hToken);
            return FALSE;
        }
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = (bEnable) ?  SE_PRIVILEGE_ENABLED : 0;

        // Enable the privilege or disable all privileges.
        if(!::AdjustTokenPrivileges(
               hToken,
               FALSE,
               &tp,
               sizeof(TOKEN_PRIVILEGES),
               (PTOKEN_PRIVILEGES) NULL,
               (PDWORD) NULL)
            )
        {
            CloseHandle(hToken);
            return FALSE;
        }
        ::CloseHandle(hToken);
    }
    return TRUE;
}

int KillProcess(int processID)
{
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if(hProcess)
    {
        if(!SetCurrentPrivilege(TRUE, SE_DEBUG_NAME))
        {
            _tprintf(TEXT("Could not enable debug privilege\n"));
        }
        HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)1, NULL, 0, NULL);
        if(hThread)
        {
            ::CloseHandle(hThread);
        }
        else
        {
            _tprintf(TEXT("Error: %d\n"), GetLastError());
            ::CloseHandle(hProcess);
            return 1;
        }
        ::CloseHandle(hProcess);
    }
    return 0;
}

void ShowProcessInformation(std::list<ProcessInfo> &process_list)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hSnapshot) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if(Process32First(hSnapshot, &pe32)) {
            do {
                ProcessInfo info;
                info.pid = (int)pe32.th32ProcessID;
                info.name = std::string(pe32.szExeFile);
                process_list.push_back(info);
            } while(Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
}

#if 0
int __cdecl _tmain(int argc, _TCHAR *argv[])
{
    /* KillProcess(20420); */
    std::list<ProcessInfo> list;
    ShowProcessInformation(list);

    for (auto it : list) {
        printf("pid %d name %s\n", it.pid, it.name.c_str());
    }
    return 0;
}
#endif