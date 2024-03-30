#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h> 
volatile UINT count;
void output_message()
{
    printf("A new process has been created!\n");
}

void thread()
{
    for (;;)
    {
        count++;
        Sleep(500);
        _cprintf("count = %d\n", count);
    }
}

int main()
{
    // имя нового процесса с пробелом
     WCHAR lpszComLine[80];
    // для символьного представления дескриптора
     WCHAR lpszHandle[20];
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    HANDLE hThread, hInheritThread, hThreadSecond;
    DWORD IDThread, IDThreadSecond;
    
    _cputs("Press any key to display text.\n");
    _cputs("Input 't' to terminate the new console process:\n");
    _cputs("After terminating the thread press any key to exit.\n\n\n");
    _getch();
    // запускаем поток вывода сообщения о создании дочернего процесса
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)output_message, NULL,
        0, &IDThread);
    hThreadSecond = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL,
        0, &IDThreadSecond);
    if (hThread == NULL && hThreadSecond == NULL)
        return GetLastError();
    // создаем наследуемый дубликат дескриптора потока
    if (!DuplicateHandle(
        GetCurrentProcess(), // дескриптор текущего процесса
        hThread, // исходный дескриптор потока
        GetCurrentProcess(), // дескриптор текущего процесса
        &hInheritThread, // новый дескриптор потока
        0, // этот параметр игнорируется
        TRUE, // новый дескриптор наследуемый
        DUPLICATE_SAME_ACCESS)) // доступ не изменяем
    {
        _cputs("The handle is not duplicated.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    // устанавливаем атрибуты нового процесса
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    // преобразуем наследуемый дескриптор в символьную строку
    _itow_s((int)hThread, lpszHandle, 10);
    wsprintf(lpszComLine, L"C:\\Users\\st310-04\\Documents\\Malugin\\Prakt10\\x64\\Debug\\ConsoleProcess.exe %s", lpszHandle);
    // запускаем новый консольный процесс
    if (!CreateProcess(
        NULL, // имя процесса
        lpszComLine, // адрес командной строки
        NULL, // атрибуты защиты процесса по умолчанию
        NULL, // атрибуты защиты первичного потока по умолчанию
        TRUE, // наследуемые дескрипторы текущего процесса
        // наследуются новым процессом
        CREATE_NEW_CONSOLE, // новая консоль
        NULL, // используем среду окружения процесса-предка
        NULL, // текущий диск и каталог, как и в процессе-предке
        &si, // вид главного окна - по умолчанию
        &pi // здесь будут дескрипторы и идентификаторы
        // нового процесса и его первичного потока
    )
        )
    {
        _cputs("The new process is not created.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    DWORD pId = GetProcessId(pi.hProcess);
    wprintf(L"pId: %d\n", pId);
   
    //Завершение процесса
    while (true)
    {
        char c;
        c = _getch();
        if (c == 't' || c == 'T')
        {
            _cputs("t\n");
            // завершаем новый процесс
            TerminateProcess(pi.hProcess, 1);
            _cputs("The child process was completed successfully!\n");
            break;
        }
    }
    // закрываем дескрипторы нового процесса
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    _getch();
    // закрываем дескриптор потока
    CloseHandle(hThread);
    CloseHandle(hThreadSecond);
    return 0;
}
   

