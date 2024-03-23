#include <windows.h>
#include <iostream>
#include <stdio.h>
volatile UINT count;
void thread()
{
    for (;;)
    {
        count++;
        Sleep(100);
    }
}
int main()
{
    WCHAR lpszAppName[] = L"C:\\Users\\st310-04\\Documents\\Malugin\\Prakt10\\x64\\Debug\\ConsoleProcess.exe";
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    // создаем новый консольный процесс
    if (!CreateProcess(lpszAppName, NULL, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        printf("The new process is not created.\n");
        printf("Check a name of the process.\n");
        printf("Press any key to finish.\n");
        getchar();
        return 0;
    }
    printf("The new process is created.\n");
    while (true)
    {
        char c;
        printf("Input 't' to terminate the new console process: ");
        c = getchar();
        if (c == 't')
        {
            printf("t\n");
            // завершаем новый процесс
            TerminateProcess(pi.hProcess, 1);
            break;
        }
    }
    // закрываем дескрипторы нового процесса в текущем процессе
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}
   

