#include "ThreadDraw.h"
#include <string>
#include <iostream>

std::thread workThread; // ����� ��� �������� ����
bool isSuspended = false; // ��������� ������ (������������� ��)

// �������, ����������� ����� ����
void ChangeBGR(HWND hwnd, COLORREF clr)
{
	HBRUSH hBrush = CreateSolidBrush(clr);
	SetClassLongPtr(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush);
}

// ������� ������
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

// ������� �������� ������ ������
void Launch(HWND hwnd)
{
	workThread = std::thread(ThreadFunc, hwnd);
}

// ������� ������������� ������ ������
void Resume()
{
	isSuspended = false;
	ResumeThread(workThread.native_handle());
}

// ������� ������������ ������ ������
void Suspend()
{
	isSuspended = true;
	SuspendThread(workThread.native_handle());
}

// ������� ��������� ��������� ������
void �hangeState()
{
	// ���� ��� �������������, �� ����������� ������
	if (isSuspended)
	{
		Resume();
	}
	// ����� �������������
	else
	{
		Suspend();
	}
}

// ������� ��������� ����������
void SetPriority(int pr)
{
	int priority = ThreadPriorities[pr];
	MessageBox(NULL, L"��������� ����������", L"��������� ������", MB_OK);
	SetThreadPriority(workThread.native_handle(), priority);
}

// ���������� ������ ������
void Stop()
{
	TerminateThread(workThread.native_handle(), 0);
	workThread.join();
}