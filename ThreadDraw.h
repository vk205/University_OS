#pragma once
#pragma once

#include "Windows.h"
#include "Color.h"
#include "Properties.h"
#include "Config.h"
#include <thread>

// Возможные приоритеты потоков
const int ThreadPriorities[] = {
	THREAD_PRIORITY_LOWEST,
	THREAD_PRIORITY_BELOW_NORMAL,
	THREAD_PRIORITY_NORMAL,
	THREAD_PRIORITY_ABOVE_NORMAL,
	THREAD_PRIORITY_HIGHEST
};

void ChangeBGR(HWND, COLORREF);
void ThreadFunc(HWND);
void Launch(HWND);
void Resume();
void Suspend();
void СhangeState();
void SetPriority(int);
void Stop();
