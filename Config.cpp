#include "Config.h"


const wchar_t configFile[] = L"config.ini";

void WriteConfigUsingWinApi(const Properties* prefs)
{
	const HANDLE hFile = CreateFile(configFile,
		GENERIC_WRITE,
		0,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	const TCHAR* buffer = PropertiesToString(prefs);
	const UINT len = wcslen(buffer) * sizeof(wchar_t);
	WriteFile(hFile, buffer, len, nullptr, nullptr);

	CloseHandle(hFile);
	delete[] buffer;
}

Properties* ReadConfigUsingWinApi()
{
	const HANDLE hFile = CreateFile(configFile,
		GENERIC_READ,
		0,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	const UINT bufferSize = GetFileSize(hFile, nullptr);
	TCHAR* buffer = new TCHAR[bufferSize];
	ReadFile(hFile, buffer, bufferSize, nullptr, nullptr);

	CloseHandle(hFile);

	Properties* prefs = StringToProperties(buffer);

	delete[] buffer;
	return prefs;
}

Properties* ReadConfigUsingCommandLine()
{
	int nArgs;
	LPWSTR *Arglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	Properties* prefs = new Properties();

	if (nArgs == 1)
	{
		return nullptr;
	}

	if (nArgs != 1)
	{
		prefs->GridSize = _wtoi(Arglist[1]);
	}
	LocalFree(Arglist);

	prefs->WindowHeight = 280;
	prefs->WindowWidth = 320;

	prefs->BackgroundColor = 0xff0000;
	prefs->GridColor = 0x0000ff;

	return prefs;
}

Properties* GetProperties()
{
	Properties * prefs = ReadConfigUsingCommandLine();
	if (!prefs)
		return ReadConfigUsingWinApi();
	else return prefs;
}



