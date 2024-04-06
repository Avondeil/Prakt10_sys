#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h> 
volatile UINT count;
bool start = false;
void output_message()
{
    for (;;)
    {
        count++;
        Sleep(900);
        _cprintf("count = %d\n", count);
    }
}

void thread()
{
    for (;;)
    {
        Sleep(800);
        _cprintf("COUNT * 2 = %d\n", count*2);
    }
}
// имя нового процесса с пробелом
WCHAR lpszComLine[80];
// для символьного представления дескриптора
WCHAR lpszHandle[20];
STARTUPINFO si;
PROCESS_INFORMATION pi;
HANDLE hThread, hThreadSecond;
DWORD IDThread, IDThreadSecond;

int main()
{
    setlocale(LC_ALL, "ru");
    printf("Нажмите '1', чтобы запустить первый поток.\n");
    printf("Нажмите '2', чтобы запустить второй поток.\n");
    printf("Нажмите 's', чтобы запустить запустить дочерний процесс.\n");
    printf("Нажмите 't', чтобы завершить дочерний процесс.\n");
    printf("Нажмите клавишу '0' для выхода.\n\n\n");
    Sleep(500);
    while (true)
    {
        char c = _getch();
        switch (c)
        {
        case 's':
        case 'S':
            if (start != false)
            {
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
                    printf("Дочерний процесс не создан.\n");
                    printf("Нажмите любую клавишу для завершения.\n");
                    _getch();
                    return GetLastError();
                }
                DWORD pId = GetProcessId(pi.hProcess);
                wprintf(L"pId: %d\n", pId);
                wprintf(L"Дескриптор: %p\n", pi.hProcess);
                wprintf(L"Дескриптор потока вывода сообщения (десятичное значение): %s\n", lpszHandle);
                start = false;
            }
            else
            {
                printf("Запустите первый поток на цифру 1 и попробуйте снова\n");
            }
            break;
        case 't':
        case 'T':
            printf("t\n");
            // завершаем новый процесс
            TerminateProcess(pi.hProcess, 1);
            printf("Дочерний процесс завершен!\n");
            break;
        case '1':
            if (hThread == NULL)
            {
                start = true;
                printf("Запуск потока count++:\n");
                hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)output_message, NULL,
                    0, &IDThread);
            }
            else
            {
                printf("Поток count++ уже запущен!\n");
            }
            break;
        case '2':
            if (hThreadSecond == NULL)
            {
                printf("Запуск потока, который умножает count на 2:\n");
                hThreadSecond = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL,
                    0, &IDThreadSecond);
            }
            else
            {
                printf("Поток, который умножает count на 2 уже запущен!\n");
            }
            break;
        case '0':
            printf("Программа завершена\n");
            TerminateProcess(pi.hProcess, 1);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            // закрываем дескриптор потока
            CloseHandle(hThread);
            CloseHandle(hThreadSecond);
            return 0;
        }
    }
}