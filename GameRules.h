#pragma once
#pragma once
#include "Windows.h"
#include "GridDraw.h"

enum status { win = 1, lose, draw, inprocess };

const wchar_t Win1[] = L"Победа первого игрока!"; 
const wchar_t Win2[] = L"Победа второго игрока!";
const wchar_t Draw0[] = L"Ничья!";

const unsigned WM_FINISH = RegisterWindowMessage(L"WM_FINISH");
const unsigned WM_TURNNOTIFY = RegisterWindowMessage(L"WM_TURNNOTIFY");

void SetSign(int);
void SetupGame(int);

void GameWon();
void GameLost();

status statsResult(int, int, int);
status GetStatus(int, int[kNMax][kNMax]);

void CheckStatus(int, int[kNMax][kNMax]);

bool StartTurn();
void FinishTurn();

void SetTurn(const WPARAM);

bool SetValue(int, int, int[kNMax][kNMax]);

