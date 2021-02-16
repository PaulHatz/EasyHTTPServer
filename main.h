#pragma once
#include <Windows.h>
#include <stdio.h>
#include <winsock.h>
#include <string>
#include <codecvt>
#include <locale>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#define HTML_ROOT_PATH L"\\html"
#define SRV_PORT 80 

void HTTP_SocketThread(SOCKET *sockPtr);

size_t getFileSize(FILE *FileHandle);
void readFile(const wchar_t *filePath, char **fileBuffer, size_t *fSize);