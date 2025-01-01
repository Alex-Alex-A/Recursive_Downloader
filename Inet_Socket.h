#pragma once

#define UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <wininet.h>
#include <string>
#include <iostream>
#include <fstream>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "wininet.lib")


bool Write_File (char*, char*, int);

std::string Do_Inet (const std::string&);