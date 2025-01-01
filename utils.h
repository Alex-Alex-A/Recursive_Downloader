#pragma once

#include <windows.h>

#include <Shlwapi.h>
#pragma comment( lib,"Shlwapi.lib" )

#include <iostream>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <assert.h>

#pragma warning(disable : 4996)

#define  ANY_WHITESPACE   1
#define  ONLY_SPACES      2

#define NOT_FOUND  std::string::npos



std::pair<std::string, std::string> host;

long Get_File_Size (char* ofile)
{
	long size = 0;
	std::fstream fp (ofile, std::fstream::in);
	fp.seekp(0, std::ios::end);
	size = (long)fp.tellp();
	fp.close();
	return size;
}


bool Delete_File (char* ofile)
{
	char fname[280];

	std::memset(fname, 0, sizeof(fname));
	strcpy_s(fname, ofile);
	if (remove(fname))
		return true;

	return false;
}


bool FileExists (char* fname)
{
	struct _stat buf;

	if (0 != _stat(fname, &buf))
		return false;

	return true;
}


bool Write_File (char* ofile, char* data, int mode)
{
	bool b = false;
	if (mode == 1)
	{
		setlocale(LC_ALL, "");
		std::ofstream fout(ofile, std::ios::out);

		for (int i = 0; i < 1000; i++) {
			if (fout.is_open()) {
				b = true;
				break;
			}
			else
				fout.open(ofile, std::ios::out);
		}
		if (!b)
			return false;

		fout << data;
		fout.close();
		fout.clear();
	}
	else if (mode == 2)
	{
		setlocale(LC_ALL, "");
		std::ofstream fout(ofile, std::ios::out | std::ios::app);

		for (int i = 0; i < 1000; i++) {
			if (fout.is_open()) {
				b = true;
				break;
			}
			else
				fout.open(ofile, std::ios::out | std::ios::app);
		}
		if (!b)
			return false;

		fout << data;
		fout.close();
		fout.clear();
	}
	return true;
}


bool Read_File_All (char* ofile, char* data)
{
	int i = 0;
	char letter;
	bool b = false;
	std::ifstream fin(ofile, std::ifstream::in);

	for (int i = 0; i < 1000; i++) {
		if (fin.is_open()) {
			b = true;
			break;
		}
		else
			fin.open(ofile, std::ifstream::in);
	}
	if (!b)
		return false;

	while (!fin.eof())
	{
		letter = fin.get();
		data[i] = letter;
		++i;
	}

	data[i - 1] = '\0';
	fin.close();
	return true;
}


std::string str_tolower (std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
	return s;
}


std::string urlDecode (const std::string& str) { 
	std::string ret = "";                        
	char ch;                           
	int len = str.length();
	unsigned int ii;

	for (int i = 0; i < len; i++) {
		if (str[i] != '%') {
			if (str[i] == '+')
				ret += ' ';
			else
				ret += str[i];
		}
		else if (len - i > 10 &&                         // фиксим проблему с русской буквой "й"
			     str.substr(i + 1, 2) == "D0" && 
				 str.substr(i + 4, 2) == "B8" &&   
			 	 str.substr(i + 7, 2) == "CC" &&
				 str.substr(i + 10, 2) == "86"
				)
		{
				ret += 0xD0;
				ret += 0xB9;
				i += 11;	
		}
		else {
			sscanf (str.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i += 2;
		}
	}

	return ret;
}


wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	int res = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, charArray, -1, wString, 4096);
	if (res == 0)
		printf("\n MultiByteToWideChar failed!!!!!!!!!!! str: <<%S>>\n\n", charArray);

	return wString;
}


std::string win16_win1251 (const std::string& str) {
	std::string ret;
	char ch;
	unsigned char uc1, uc2 = 0;
	unsigned short ush1, ush2;
	int len = str.length();
	char ch2[2];

	for (int i = 0; i < len; i++) {
		uc1 = static_cast<unsigned char>(str.at(i));
		if (i < len - 1)
			uc2 = static_cast<unsigned char>(str.at(i + 1));
		ush1 = uc1;
		ush2 = uc2;

		if (ush1 == 208 || ush1 == 209) {
			ush1 <<= 8;
			ush1 = ush1 | ush2;
			ch = static_cast<char>(ush1);
			if (ush1 == 0xD090)				ret += 'А';
			if (ush1 == 0xD091)				ret += 'Б';
			if (ush1 == 0xD092)				ret += 'В';
			if (ush1 == 0xD093)				ret += 'Г';
			if (ush1 == 0xD094)				ret += 'Д';
			if (ush1 == 0xD095)				ret += 'Е';
			if (ush1 == 0xD096)				ret += 'Ж';
			if (ush1 == 0xD097)				ret += 'З';
			if (ush1 == 0xD098)				ret += 'И';
			if (ush1 == 0xD099)				ret += 'Й';
			if (ush1 == 0xD09A)				ret += 'К';
			if (ush1 == 0xD09B)				ret += 'Л';
			if (ush1 == 0xD09C)				ret += 'М';
			if (ush1 == 0xD09D)				ret += 'Н';
			if (ush1 == 0xD09E)				ret += 'О';
			if (ush1 == 0xD09F)				ret += 'П';
			if (ush1 == 0xD0A0)				ret += 'Р';
			if (ush1 == 0xD0A1)				ret += 'С';
			if (ush1 == 0xD0A2)				ret += 'Т';
			if (ush1 == 0xD0A3)				ret += 'У';
			if (ush1 == 0xD0A4)				ret += 'Ф';
			if (ush1 == 0xD0A5)				ret += 'Х';
			if (ush1 == 0xD0A6)				ret += 'Ц';
			if (ush1 == 0xD0A7)				ret += 'Ч';
			if (ush1 == 0xD0A8)				ret += 'Ш';
			if (ush1 == 0xD0A9)				ret += 'Щ';
			if (ush1 == 0xD0AA)				ret += 'Ъ';
			if (ush1 == 0xD0AB)				ret += 'Ы';
			if (ush1 == 0xD0AC)				ret += 'Ь';
			if (ush1 == 0xD0AD)				ret += 'Э';
			if (ush1 == 0xD0AE)				ret += 'Ю';
			if (ush1 == 0xD0AF)				ret += 'Я';

			if (ush1 == 0xD0B0)				ret += 'а';
			if (ush1 == 0xD0B1)				ret += 'б';
			if (ush1 == 0xD0B2)				ret += 'в';
			if (ush1 == 0xD0B3)				ret += 'г';
			if (ush1 == 0xD0B4)				ret += 'д';
			if (ush1 == 0xD0B5)				ret += 'е';
			if (ush1 == 0xD0B6)				ret += 'ж';
			if (ush1 == 0xD0B7)				ret += 'з';
			if (ush1 == 0xD0B8)				ret += 'и';
			if (ush1 == 0xD0B9)				ret += 'й';
			if (ush1 == 0xD0BA)				ret += 'к';
			if (ush1 == 0xD0BB)				ret += 'л';
			if (ush1 == 0xD0BC)				ret += 'м';
			if (ush1 == 0xD0BD)				ret += 'н';
			if (ush1 == 0xD0BE)				ret += 'о';
			if (ush1 == 0xD0BF)				ret += 'п';
			if (ush1 == 0xD180)				ret += 'р';
			if (ush1 == 0xD181)				ret += 'с';
			if (ush1 == 0xD182)				ret += 'т';
			if (ush1 == 0xD183)				ret += 'у';
			if (ush1 == 0xD184)				ret += 'ф';
			if (ush1 == 0xD185)				ret += 'х';
			if (ush1 == 0xD186)				ret += 'ц';
			if (ush1 == 0xD187)				ret += 'ч';
			if (ush1 == 0xD188)				ret += 'ш';
			if (ush1 == 0xD189)				ret += 'щ';
			if (ush1 == 0xD18A)				ret += 'ъ';
			if (ush1 == 0xD18B)				ret += 'ы';
			if (ush1 == 0xD18C)				ret += 'ь';
			if (ush1 == 0xD18D)				ret += 'э';
			if (ush1 == 0xD18E)				ret += 'ю';
			if (ush1 == 0xD18F)				ret += 'я';
			if (ush1 == 0xD191)				ret += 'Ё';
			if (ush1 == 0xD192)				ret += 'ё';

			++i;
		}
		else {
			ch2[1] = 0;
			ch2[0] = str[i];
			ret += str[i];
		}
	}
	return ret;
}


inline bool IsWhitespace (char c) {

	switch (c) {
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	case '\f':
		return true;
	default:
		return false;
	}
}


int Is_Substr (char* haystack, char* needle, int start) {
	int out = -1;
	int haystack_l = strlen(haystack);
	int needle_l = strlen(needle);

	for (int i = start; i <= haystack_l - needle_l; i++) {
		out = i;
		for (int j = 0; j < needle_l; j++) {
			if (haystack[i + j] != needle[j]) {
				out = -1;
				break;
			}
		}
		if (out > -1)
			return out;
	}
	return out;
}


inline int What_Near_Right_Symbol (char* p, int pos_, int mode, int full_len) {

	for (int i = pos_ + 1; i < full_len; i++) {
		if (mode == ANY_WHITESPACE && IsWhitespace(p[i]))
			continue;
		else if (mode == ONLY_SPACES && p[i] == ' ')
			continue;

		return i;
	}

	return 0;
}


inline int What_Near_Left_Symbol (char* p, int pos_, int mode) { 

	for (int i = pos_ - 1; i >= 0; i--) {
		if (mode == ANY_WHITESPACE && IsWhitespace(p[i])) 
			continue;
		else if (mode == ONLY_SPACES && p[i] == ' ')  
			continue;

		return i;
	}

	return 0;
}


inline int Find_Left_a_tag (char* p, int pos_) {
	                                         
	int i = 0;
	bool is_a = false, is_angle_bracket = false;

	for (i = pos_ - 1; i >= 0; i--) {
		if (p[i] == '<')
			break;
		else if (p[i] == 'a' && IsWhitespace(p[i + 1])) {       //
			is_a = true;
			break;
		}
	}

	if (is_a) {
		--i;

		for (; i >= 0; i--) {
			if (p[i] == '<') {
				return i;
			}
			else if (IsWhitespace(p[i])) {
				continue;
			}
			else
				break;
		}
	}

	return -1;
}


std::pair<int, int> Find_Link (char* p, int start, int full_len) {  
	int from = -1, to = -1;

	int i = Is_Substr (p, (char*)"href", start);

	if (i >= 0) {
		to = i + 4;
		int ll = Find_Left_a_tag (p, i);

		if (ll > 0) {
			int rr = What_Near_Right_Symbol (p, i+3, ANY_WHITESPACE, full_len);

			if (rr > 0 && p[rr] == '=') {
				rr = What_Near_Right_Symbol (p, rr, ANY_WHITESPACE, full_len);
				if (rr > 0 && p[rr] == '"') {
					from = rr + 1;
				}
			}
		}
	}

	if (from >= 0) {
		for (int j = from; j < full_len; j++) {
			if (p[j] == '"') {
				to = j - 1;
				break;
			}
		}
	}

	return std::make_pair (from, to);
}


bool is_Book (const std::string& s) {
	std::string tmp = str_tolower(s);
	if (tmp.length() > 4 && tmp.substr(tmp.length() - 4, 4) == ".pdf")		return true;
	if (tmp.length() > 5 && tmp.substr(tmp.length() - 5, 5) == ".djvu")		return true;
	if (tmp.length() > 4 && tmp.substr(tmp.length() - 4, 4) == ".fb2")		return true;
	if (tmp.length() > 5 && tmp.substr(tmp.length() - 5, 5) == ".epub")		return true;

	if (tmp.length() > 5 && tmp.substr(tmp.length() - 4, 4) == ".zip")		return true;

	return false;
}


bool is_file (const std::string& s) {
	std::string tmp = str_tolower(s);
	if (tmp.length() > 4 && tmp.substr(tmp.length() - 5, 5) == ".html")	return false;
	if (tmp.length() > 4 && tmp.substr(tmp.length() - 4, 4) == ".htm")	return false;
	if (tmp.length() > 4 && tmp.substr(tmp.length() - 4, 4) == ".php")	return false;

	if (s.length() > 4 && s.substr(s.length() - 3, 1) == ".")		return true;
	if (s.length() > 4 && s.substr(s.length() - 3, 1) == ".")		return true;
	if (s.length() > 4 && s.substr(s.length() - 4, 1) == ".")		return true;
	if (s.length() > 5 && s.substr(s.length() - 5, 1) == ".")		return true;

	return false;
}


bool is_Garbige (const std::string& s) {
	std::string tmp = str_tolower(s);
	if (tmp.length() > 8 && tmp.find(".css") != NOT_FOUND)		       return true;
	if (tmp.length() > 8 && tmp.find(".png") != NOT_FOUND)		       return true;
	if (tmp.length() > 8 && tmp.find(".net") != NOT_FOUND)		       return true;
	if (tmp.length() > 8 && tmp.find(".json") != NOT_FOUND)		       return true;
	if (tmp.length() > 8 && tmp.find("/css/") != NOT_FOUND)		       return true;
	if (tmp.length() > 8 && tmp.find(".ico") != NOT_FOUND)		       return true;
	if (tmp.length() > 8 && tmp.find("?") != NOT_FOUND)		           return true;
	if (tmp.length() > 8 && tmp.find(".svg") != NOT_FOUND)		       return true;
	if (tmp.length() > 8 && tmp.find("javascript:") != NOT_FOUND)      return true;

	return false;
}


template <class T>
bool is_In_Vector (std::vector<T> &v, T val) {
	for (size_t i = 0; i < v.size(); i++) {
		if (val == v[i])
			return true;
	}
	return false;
}


std::string Make_Dir_Name (const std::string& s) {
	std::string out = s;
	size_t find, len = s.length();

	if ((find = s.rfind("/")) != NOT_FOUND)
	{
		out = s.substr (find + 1);
	}

	return out;
}


std::pair<std::string, std::string> get_host_name (std::string& s) {   
	std::string host, full_host;
	if (s.length() > 10) {
		if (s.substr(0, 8) == "https://") {
			host = s.substr(8, s.length() - 8);
			full_host = "https://";
		}
		else if (s.substr(0, 7) == "http://") {
			host = s.substr(7, s.length() - 7);
			full_host = "http://";
		}

		size_t found = host.find('/');
		if (found != NOT_FOUND)
			host = host.substr(0, found);

		full_host += host;
	}
	return make_pair(host, full_host);
}


std::string Clear_First_Last_Slash (const std::string& s) {
	std::string tmp, out = s;
	size_t len = out.length();

	if (len > 1 && out[0] == '/') {
		assert(len - 1 < len);
		tmp = out.substr(1, len - 1);
		out = tmp;
		len = out.length();
	}
	if (len > 1 && out[0] == '/') {
		assert(len - 1 < len);
		tmp = out.substr(1, len - 1);
		out = tmp;
		len = out.length();
	}

	if (len > 0 && out[len - 1] == '/') {
		assert(len - 1 < len);
		out = out.substr(0, len - 1);
		len = out.length();
	}
	if (len > 0 && out[len - 1] == '/') {
		assert(len - 1 < len);
		out = out.substr(0, len - 1);
		len = out.length();
	}

	return out;
}


std::string get_url_without_hostname (const std::string& url) {
	std::string out = "";
	size_t len_hostname = host.second.length();
	size_t len_url = url.length();

	if (len_url >= len_hostname && url.find(host.second) != NOT_FOUND) {
		assert(len_hostname >= 0);
		if (len_url > len_hostname) {
			assert(len_url - len_hostname > 0);
			assert(len_url - len_hostname < len_url);
			out = url.substr(len_hostname, len_url - len_hostname);
		}
	}

	if (out == "") {
		len_hostname = host.first.length();
		if (len_url >= len_hostname && url.find(host.first) != NOT_FOUND) {
			assert(len_hostname >= 0);
			assert(len_hostname < len_url);
			assert(len_url - len_hostname > 0);
			assert(len_url - len_hostname < len_url);
			out = url.substr(len_hostname, len_url - len_hostname);
			out = Clear_First_Last_Slash(out) + "/";
		}
	}

	return out;
}


bool is_Stop_Words (const std::string& s) {
	size_t len_needle, len = s.length();
	std::vector<std::string> stops;

	stops.push_back("/login/");
	stops.push_back("/login");
	stops.push_back("/password_recover");

	for (size_t i = 0; i < stops.size(); i++) {
		len_needle = stops[i].length();

		if (len > len_needle && s.find(stops[i]) != NOT_FOUND) {
			return true;
		}
	}

	return false;
}


std::string Get_File_Name_only (const std::string& s) {
	std::string out = "";
	for (int i = s.length() - 1; i >= 0; i--) {
		if (s[i] == '/')
			break;
		else
		    out = s[i] + out;
	}
	return out;
}


void Create_Dirs (const std::string& s) {
	std::vector<std::string> ss;
	std::vector<size_t> ii;
	ii.push_back(0);

	std::string tmp = Clear_First_Last_Slash (s);

	for (size_t i = 0; i < tmp.length(); i++) {
		if (tmp[i] == '/')
			ii.push_back(i + 1);
	}

	ii.push_back (tmp.length() + 1);

	for (size_t i = 0; i < ii.size() - 1; i++) {
		std::string tmp2 = tmp.substr (ii[i], ii[i + 1] - ii[i] - 1);

		if (!is_Garbige(tmp2) ) {
			ss.push_back(tmp2);
		}
	}

	std::string parent_dir2, dir_str = "";
	for (size_t i = 0; i < ss.size(); i++) {

		parent_dir2 = urlDecode (ss[i]);
		dir_str += parent_dir2 + "/";

		LPCWSTR lpc = convertCharArrayToLPCWSTR (dir_str.c_str());

		bool b = CreateDirectory (lpc, NULL);

		if (i == ss.size() - 1) {
			setlocale(LC_ALL, ""); 
			std::wcout << L"           Creating catalog       <<< " << lpc << L" >>>       ";

			if (b)
				std::wcout << L"    ====    OK\n";
			else
				std::wcout << L" !!!!!!!!!!!!!!!!!!       Can't create catalog!   <<< " << lpc << L" >>>\n\n";
		}

		if (lpc)	delete lpc;
	}
}


std::string Url_Prepare (const std::string& s, const std::string& prev_url) {
	size_t found_first, found_second;
	bool is_first_slash;

	if (s[0] == '/')		is_first_slash = true;
	else            		is_first_slash = false;

	std::string out = Clear_First_Last_Slash(s);

	if (out == "" || out == "." || out.find("..") != NOT_FOUND)
		return "";

	found_first = out.find  (host.first);
	found_second = out.find (host.second);

	if (found_second != NOT_FOUND)      
		return out;
	else if (found_first != NOT_FOUND) {
		out = host.second + "/" + Clear_First_Last_Slash(out.substr(found_first + host.first.length()));
		return out;
	}
	else {              
		if (!is_first_slash)
			out = Clear_First_Last_Slash ( Clear_First_Last_Slash(prev_url) + "/" + out );
		else
		    out = host.second + "/" + Clear_First_Last_Slash (out);    
	}

	return out;
}

