#pragma once

#include <stdio.h>
#include <string>
#include <codecvt>
#include <locale>
#include <unordered_map>
#include <string_view>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;

#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <Windows.h>
    #include <winsock.h>

    #pragma comment(lib, "Ws2_32.lib")
    #pragma warning(disable:4996)
    
    using SOCKET_TYPE = SOCKET;
    #define IS_VALIDSOCKET(s) ((s) != INVALID_SOCKET)
    #define CLOSESOCKET(s) closesocket(s)
    #define GETSOCKETERRNO() (WSAGetLastError())
#else
    #define PLATFORM_LINUX
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>
    
    using SOCKET_TYPE = int;
    #define SOCKET int
    #define SOCKET_ERROR (-1)
    #define IS_VALIDSOCKET(s) ((s) >= 0)
    #define closesocket(s) close(s)
    #define GETSOCKETERRNO() (errno)
#endif

#define HTML_ROOT_PATH L"\\html"
#define SRV_PORT 80 

namespace fs = std::filesystem;


void HTTP_SocketThread(int sock);

size_t getFileSize(FILE *FileHandle);
std::string readFile(const std::filesystem::path& file_path);

bool IsWithinRoot(const fs::path& root, const fs::path& requested);