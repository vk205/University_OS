#include "GameRules.h"

int ourSign;
int opSign;
bool isOurTurn;

// ������� ������������� ������ ������
void SetSign(int n)
{
	if (n == 0)
	{
		ourSign = 1;
		opSign = -1;
	}
	else
	{
		ourSign = -1;
		opSign = 1;
	}
}


// �������, ���������� �� ������ ����
void SetupGame(int n)
{
	SetSign(n);

	if (ourSign == 1)
		isOurTurn = true;
	else 
		isOurTurn = false;
}

// �������, �������������� �������� ������ �������� ������
void GameWon()
{
	if(ourSign==1)
		MessageBox(nullptr, Win1, L"������!", MB_OK );
	else
		MessageBox(nullptr, Win2, L"������!", MB_OK);
	SendNotifyMessage(HWND_BROADCAST, WM_FINISH, 0, 0);
	PostQuitMessage(0);
}

// �������, �������������� �������� ��������� �������� ������
void GameLost()
{
	if (ourSign == 1)
		MessageBox(nullptr, Win2, L"������!", MB_OK | MB_ICONASTERISK);
	else
		MessageBox(nullptr, Win1, L"������!", MB_OK | MB_ICONASTERISK);
	SendNotifyMessage(HWND_BROADCAST, WM_FINISH, 0, 0);
	PostQuitMessage(0);
}

// �������, �������������� �������� ���������� ���� ������
void Draw()
{
	MessageBox(nullptr, Draw0, L"�����!", MB_OK);
	SendNotifyMessage(HWND_BROADCAST, WM_FINISH, 0, 0);
	PostQuitMessage(0);
}

// �������, ������������� ������ ����
status statsResult(int ours, int ops, int dim)
{
	// ���� ������/ �������/ ���� ��������� �� ����� ������
	if (ours == dim)
	{
		// ������
		return win;
	}
	// ���� ������/ �������/ ���� ��������� �� ������ ����������
	if (ops == dim)
	{
		// ���������
		return lose;
	}
	// ����� ���� ��� � ��������
	return inprocess;
}

// �������, ������������ ������ ����
status GetStatus(int dim, int field[kNMax][kNMax])
{
	int our = 0, op = 0;
	status result;

	{
		// �������� ���������� ���� �����
		int count = 0;

		for (int row = 0; row < dim; ++row)
		{
			for (int col = 0; col < dim; ++col)
			{
				if (field[row][col] != 0)
				{
					++count;
				}
			}

		}
		if (count == dim * dim)
		{
			return draw;
		}
		return inprocess;
	}
	
		
		// �������� ����� 
		for (int row = 0; row < dim; ++row)
		{
			int ours = 0;
			int ops = 0;

			for (int col = 0; col < dim; ++col)
			{
				if (field[row][col] == ourSign)
				{
					++ours;
				}
				else if (field[row][col] == opSign)
				{
					++ops;
				}
			}

			result = statsResult(ours, ops, dim);

			if (result != inprocess)
			{
				return result;
			}
		}

	// �������� �������
	for (int col = 0; col < dim; ++col)
	{
		int ours = 0;
		int ops = 0;

		for (int row = 0; row < dim; ++row)
		{
			if (field[row][col] == ourSign)
			{
				++ours;
			}
			else if (field[row][col] == opSign)
			{
				++ops;
			}
		}

		result = statsResult(ours, ops, dim);

		if (result != inprocess)
		{
			return result;
		}
	}

	{
		// �������� ������� ���������
		int ours = 0;
		int ops = 0;
		for (int row = 0; row < dim; ++row)
		{
			const int col = row;
			if (field[row][col] == ourSign)
			{
				++ours;
			}
			else if (field[row][col] == opSign)
			{
				++ops;
			}
		}

		result = statsResult(ours, ops, dim);
		if (result != inprocess)
		{
			return result;
		}
	}

	{
		// �������� �������� ���������
		int ours = 0;
		int ops = 0;
		for (int row = 0; row < dim; ++row)
		{
			const int col = dim - row - 1;
			if (field[row][col] == ourSign)
			{
				++ours;
			}
			else if (field[row][col] == opSign)
			{
				++ops;
			}
		}

		result = statsResult(ours, ops, dim);

		if (result != inprocess)
		{
			return result;
		}
	}
}

// ������� �������� ������� ���������� ����
void CheckStatus(int dim, int field[kNMax][kNMax])
{
	status st = GetStatus(dim, field);
	switch (st)
	{
	case win:
		GameWon();
	case lose:
		GameLost();
	case draw:
		Draw();
	}
}

// �������, �������� ����������� ������ ���� �������� ������
bool StartTurn()
{
	if (!isOurTurn)
	{
		MessageBox(nullptr, L"��� ����������� ������� ������", L"��������",
			MB_OK | MB_ICONASTERISK);
		return false;
	}	
	return true;
}

// �������, ���������� �� ���������� ���� �������� ������
void FinishTurn()
{
	isOurTurn = false;
	// ��������� � ����� ������������ ����
	PostMessage(HWND_BROADCAST, WM_TURNNOTIFY, GetCurrentProcessId(), 0);
}

// �������, ���������� �� ����� �����������
void SetTurn(const WPARAM wParam)
{
	if (wParam != GetCurrentProcessId())
	{
		isOurTurn = true;
	}
}

// �������, ��������������� ������ �������� ������ � ������ ����
bool SetValue(int iIndex, int jIndex, int field[kNMax][kNMax])
{
	if (field[iIndex][jIndex] != 0)
	{
		MessageBox(nullptr, L"������ ��� ������.", L"������", MB_OK | MB_ICONASTERISK);
		return false;
	}
	field[iIndex][jIndex] = ourSign;
	return true;
}