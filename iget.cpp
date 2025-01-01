
#include <iostream>

#include "Inet_Socket.h"

#include <windows.h>
#include <cstring>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <set>
#include <assert.h>

#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

#define NOT_FOUND  std::string::npos

#include "utils.h"

static int n_opened = 0;

//==============================================================================================================

std::set<std::string> visited;
std::set<std::string> visited_dirs;

void Get_Page_Links (std::string& url, std::string& parent_dir) {

	std::string parent_dir2, html_text;
	std::vector<std::string> files, links;
	char tmp[1000];
	int j = 0;
	bool b = true;

	++n_opened;

	auto ret = visited.insert ( Clear_First_Last_Slash(url) );
	if (ret.second == false) {
		--n_opened;
		return;
	}

	parent_dir2 = get_url_without_hostname (url);
	parent_dir2 = Clear_First_Last_Slash (parent_dir2);
	parent_dir2 = parent_dir + "/" + parent_dir2 + "/";
	parent_dir2 = Clear_First_Last_Slash (parent_dir2) + "/";

	ret = visited_dirs.insert (Make_Dir_Name(url));
	if (ret.second == false) {
		--n_opened;
		printf(";;;;; Opened items: %d (catalog exists)\n", n_opened);
		return;
	}

	if (is_Stop_Words(url)) {
		--n_opened;
		printf(";;;;; Opened items: %d (stop word)\n", n_opened);
		return;
	}

	LPCWSTR lpc_url = convertCharArrayToLPCWSTR (url.c_str());

	while (1) {

		html_text = Do_Inet (url);

		if (html_text != "") {
			std::wcout << "\nRequest success!         Processing link: " << lpc_url << "\n" << std::endl;
			break;
		}
		else {
			std::wcout << "\nFailed... Trying one more time.  " << lpc_url << "\n";
			Sleep (1200);
			j++;

			if (j > 7) {
				return;
			}
		}
	}

	if (lpc_url)	delete lpc_url;



	size_t fsize = html_text.length();

	char* p = new char[fsize];
	if (!p) {
		printf ("\n\nCan't allocate memory!!!\n\n");
		exit(1);
	}

	for (size_t i = 0; i < fsize; i++)      	    p[i] = html_text[i];
	p[fsize - 1] = '\0';
	html_text.clear();

	int i = 0;
	while (b) {

		std::pair<int, int> link_bounds = Find_Link (p, i, fsize);

1		if (link_bounds.second <= 0) {
			b = false;
		}
		else
		{
			i = link_bounds.second;
			if (link_bounds.first <= 0)
				continue;

			std::string Str (p + link_bounds.first, p + link_bounds.second + 1);

			Str = Url_Prepare (Str, url);

			if ( is_Book(Str) ) {
				files.push_back (Str);
			}
			else if (Str != "")
			{
				std::string link = Str;

				if ( !is_In_Vector(links, link) ) {

					links.push_back (link);
				}
			}
		}
	}

	Create_Dirs (parent_dir2);

	for (int i = 0; i < files.size(); i++) {

		std::string from = files[i];
		LPCWSTR lpc_file_from = convertCharArrayToLPCWSTR (from.c_str());
		                                                                   
		printf("\nlink for download: %S \n\n", lpc_file_from);

		std::string to = Get_File_Name_only (files[i]);
		to = parent_dir2 + to;
		to = urlDecode (to);
		LPCWSTR lpc_file_to = convertCharArrayToLPCWSTR (to.c_str());
		printf("hard disk path:    %S \n", lpc_file_to);

		std::string fl = urlDecode (files[i]);
		LPCWSTR lpc_fl = convertCharArrayToLPCWSTR (fl.c_str());

		HANDLE hFile = CreateFile (lpc_file_to,  // file to open
						GENERIC_READ,            // open for reading
						FILE_SHARE_READ,         // share for reading
						NULL,                    // default security
						OPEN_EXISTING,           // existing file only
						FILE_FLAG_OVERLAPPED,    // overlapped operation
						NULL);                   // no attr. template

		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Loading    ----    %S    ", lpc_fl);

			int f = 0;
			int tout = 5200;
			while (1) {
				HRESULT hr = URLDownloadToFile (0, lpc_file_from, lpc_file_to, 0, 0);

				if (hr != S_OK) {
					if (f > 7) {
						std::wcout << L"\n                       Can not load file!\n\n";
						break;
					}
					std::wcout << L"  ====   Failed to download  ,       Timeout...\n";
					Sleep(tout);
					std::wcout << L" Trying one more time...     ";
					++f;
					tout += 300;
				}
				else {
					std::wcout << L"  ====   OK  \n\n\n";
					break;
				}
			}
		}
		else
		{
			printf("File   %S    already exist... \n", lpc_file_to);
		}

		CloseHandle (hFile);
		if (lpc_file_from)	delete lpc_file_from;
		if (lpc_file_to)	delete lpc_file_to;
		if (lpc_fl)			delete lpc_fl;
	}

	files.clear();

	if (p) {
		delete[] p;
		p = NULL;
	}

	for (int i = 0; i < links.size(); i++) {

		Get_Page_Links (links[i], parent_dir);
	}

	--n_opened;
	printf(";;;;; Opened items: %d (end function) \n", n_opened);
}

//==============================================================================================================


int main(int argc, char* argv[])
{
	char buf[200];
	std::string args[15];
	std::string url = "";

	int i = 0;
	for (i = 1; i < argc; i++) {
		ZeroMemory (buf, sizeof(buf));
		strcpy_s (buf, argv[i]);
		args[i - 1] = buf;
	}

	for (int o = 0; o < i; o++) {
		if (args[o] == "-i" && o + 1 < i)
			url = args[o + 1];
	}

	if (url == "") {
		printf("You should use programm somehow like this:         iget -i http://site.com/catalog/ ...\n");
		std::wcout << L"\nPress any button." << std::endl;
		int c = _getch();
		return 0;
	}

	std::string root_folder = "books";
	std::string tmp = urlDecode (root_folder);
	tmp = win16_win1251 (tmp);
	LPCWSTR lpc = convertCharArrayToLPCWSTR (tmp.c_str());

	CreateDirectory (lpc, NULL);

	setlocale (LC_ALL, "");
	std::wcout << L"       Creating root catalog   <<< " << lpc << L" >>>\n\n";

	if (lpc)			delete lpc;


	host = get_host_name (url);

	Get_Page_Links (url, root_folder);

	std::wcout << L"\nFinish!  Press any button." << std::endl;
	int c = _getch();

	return 0;
}







