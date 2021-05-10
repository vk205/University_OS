#include "ThreadDraw.h"
#include <string>
#include <iostream>

std::thread workThread; // поток для анимации фона
bool isSuspended = false; // состояние потока (приостановлен ли)

// Функция, реализующая смену фона
void ChangeBGR(HWND hwnd, COLORREF clr)
{
	HBRUSH hBrush = CreateSolidBrush(clr);
	SetClassLongPtr(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush);
}

// Функция потока
void ThreadFunc(HWND hwnd)
{
	COLORREF clr = GetRandomColor();
	for (;;)
	{
		{
			clr = GetNextColor(clr, -1);
			ChangeBGR(hwnd, clr);
			InvalidateRect(hwnd, NULL, TRUE);
			Sleep(5);

		}
	}
}

// Функция создания нового потока
void Launch(HWND hwnd)
{
	workThread = std::thread(ThreadFunc, hwnd);
}

// Функция возобновление работы потока
void Resume()
{
	isSuspended = false;
	ResumeThread(workThread.native_handle());
}

// Функция приостановки работы потока
void Suspend()
{
	isSuspended = true;
	SuspendThread(workThread.native_handle());
}

// Функция изменения состояния потока
void СhangeState()
{
	// Если был приостановлен, то возобновить работу
	if (isSuspended)
	{
		Resume();
	}
	// Иначе приостановить
	else
	{
		Suspend();
	}
}

// Функция установки приоритета
void SetPriority(int pr)
{
	int priority = ThreadPriorities[pr];
	MessageBox(NULL, L"Приоритет установлен", L"Приоритет потока", MB_OK);
	SetThreadPriority(workThread.native_handle(), priority);
}

// Завершение работы потока
void Stop()
{
	TerminateThread(workThread.native_handle(), 0);
	workThread.join();
}