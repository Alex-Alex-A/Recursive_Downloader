
#include "Inet_Socket.h"

#include <urlmon.h>

std::string Do_Inet (const std::string& url) {

	HINTERNET hUrlDump, hInternetSession;
	std::string out = "";

#define LEN_WSTR   1024

	wchar_t* wString = new wchar_t [LEN_WSTR];
	if (!wString) {
		std::wcout << L"Can't allocate memory!!!" << std::endl;
		return  out;
	}

	DWORD dwBytesRead = 0;
	const DWORD size = 1024;
	char* buffer = new char[size + 1];
	if (!buffer) {
		delete[] wString;
		std::wcout << L"Can't allocate memory!!!" << std::endl;
		return  out;
	}

	MultiByteToWideChar (CP_ACP, 0, url.c_str(), -1, wString, LEN_WSTR);

	hInternetSession = InternetOpen (L"User-Agent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (!hInternetSession) {
		std::wcout << L"Internet Error!" << std::endl;
		return out;
	}

	hUrlDump = InternetOpenUrl (hInternetSession, wString, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	if (!hUrlDump) {
		std::wcout << L"Internet Error!" << std::endl;
		return out;
	}

	int readbyte = 0;

	while (true)
	{
		memset (buffer, 0, size);
		bool b = InternetReadFile (hUrlDump, buffer, size, &dwBytesRead);
		if (!b)
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				std::wcout << L"Read error" << std::endl;
				break;
			}
		}

		if (dwBytesRead == 0)
			break;
		
		readbyte += dwBytesRead;
		out.append (buffer, dwBytesRead);
	}

	InternetCloseHandle(hUrlDump);
	InternetCloseHandle(hInternetSession);

	delete[] buffer;
	delete[] wString;

	return out;
}






