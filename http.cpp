#include "main.h"

//Count number of bytes until a character starting at buffer
size_t countBytesUntil(char *buffer, char c)
{
	size_t len = strlen(buffer);
	size_t sz = 0;

	while (sz < len) {
		if (buffer[sz] == 0x0D)
			break;

		if (buffer[sz] == c)
			return sz;

		sz++;
	}
	return 0;
}

//Use file name to get extension. Will return 0 if file has no extension or given path ends with a directory.
char *getFileExtensionWithName(char *fileName)
{
	size_t off = strlen(fileName) - 1;

	while (off >= 0) {
		switch (fileName[off]) {
		case '.':
			return &fileName[off];
		case '/':
			return nullptr;
		default: break;
		}

		if (off == 0) break;

		off--;
	}

	return nullptr;
}

//Convert path from GET request to Windows.
void cleanPath(char *path)
{
	char val[3] = { 0 };
	char c = 0;

	size_t len = strlen(path) + 1; //include null char

	switch (*path) {

	case '/':
		*path = '\\'; //Replace '/' with '\'
		cleanPath((char *)(int(path) + 1));
		break;

	case '%':
		memcpy(val, (void *)(int(path) + 1), 2); //Copy hex into sperate buffer
		c = (char)strtol(val, NULL, 16); //Convert hex into character
		*path = c; //Replace the % symbol with the character

		memcpy((void *)(int(path) + 1), (void *)(int(path) + 3), len - 2);  //Move bytes from after the hex characters 2 spaces back.

		cleanPath((char *)(int(path) + 1));
		break;

	case 0: break; //Terminate if null byte
	default:
		cleanPath((char *)(int(path) + 1));
		break;
	}
}

//Send header of response.
size_t HTTP_SendHeader(SOCKET sock, int response_code, const char *ContentType, size_t ContentLength)
{
	if (strlen(ContentType) > 50) return 0; //Buffer overflow protection

	char *responseHeader = new char[256]();

	size_t sz = sprintf(responseHeader,
		"HTTP/1.1 %ld OK\r\n"
		"Server: EasyHTTPServer/1.0.0\r\n"
		"Accept-Ranges: bytes\r\n"
		"Connection: close\r\n"
		"Content-Type: %s\r\n"
		"Content-Length: %lu\r\n"
		"\r\n",
		response_code,
		ContentType,
		ContentLength
	);

	send(sock, responseHeader, sz, 0);

	delete[] responseHeader;

	return sz;
}

//Process request.
void HTTP_ProcessRequest(SOCKET sock, char *buffer, size_t sz)
{
	char *str = strstr(buffer, "GET /"); //Find GET request

	if (str) {
		size_t filenameLength = countBytesUntil(&str[4], ' ');
		size_t brkFound = countBytesUntil(&str[4], '?');

		if (brkFound)
			filenameLength = brkFound;


		//If the last character in the get request is a '/', then the user is accessing a directory
		//We must therfore load the index page.
		bool dirRequested = false;
		if (str[4 + filenameLength - 1] == '/') {
			filenameLength += strlen("index.html");
			dirRequested = true;
		}

		char *requestedFile = new char[filenameLength + 1]();
		memcpy(requestedFile, &str[4], dirRequested ? filenameLength - strlen("index.html") : filenameLength); //Start reading at the /

		if (dirRequested) {
			const char *index = "index.html";
			memcpy(&requestedFile[filenameLength - strlen(index)], index, strlen(index));
		}

		char *extension = getFileExtensionWithName(requestedFile);

		//Convert directory into Windows path
		cleanPath(requestedFile);

		wchar_t *convertedPath = new wchar_t[strlen(requestedFile) + 1];
		ZeroMemory(convertedPath, 2 * (strlen(requestedFile) + 1));

		std::mbstowcs(convertedPath, requestedFile, strlen(requestedFile));

		char *fileBuffer = nullptr;
		size_t fileSize = 0;

		TCHAR NPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, NPath);
		std::wstring filePath = NPath + std::wstring(HTML_ROOT_PATH) + convertedPath;
		bool bHeaderSent = false;
		
		//Read file once path is converted
		readFile(filePath.c_str(), &fileBuffer, &fileSize);

		//If the file exists, fileBuffer will be populated
		if (fileBuffer) {

			if (extension) {
				if (!strcmp(extension, ".html")) {

					HTTP_SendHeader(sock, 200, "text/html", fileSize + 2);
					bHeaderSent = true;
				}
				else if (!strcmp(extension, ".js")) {
					HTTP_SendHeader(sock, 200, "text/javascript", fileSize + 2);
					bHeaderSent = true;
				}
				else if (!strcmp(extension, ".css")) {
					HTTP_SendHeader(sock, 200, "text/css", fileSize + 2);
					bHeaderSent = true;
				}
				else if (!strcmp(extension, ".png")) {
					HTTP_SendHeader(sock, 200, "image/png", fileSize + 2);
					bHeaderSent = true;
				}
				else if (!strcmp(extension, ".jpeg") || !strcmp(extension, ".jpg")) {
					HTTP_SendHeader(sock, 200, "image/jpeg", fileSize + 2);
					bHeaderSent = true;
				}
				else if (!strcmp(extension, ".pdf")) {
					HTTP_SendHeader(sock, 200, "application/pdf", fileSize + 2);
					bHeaderSent = true;
				}
				else if (!strcmp(extension, ".txt")) {
					HTTP_SendHeader(sock, 200, "text/plain", fileSize + 2);
					bHeaderSent = true;
				}
				else {
					HTTP_SendHeader(sock, 200, "text/plain", fileSize + 2);
					bHeaderSent = true;
				}
			}
			else {
				//Handle no-extension files
				HTTP_SendHeader(sock, 200, "text/plain", fileSize + 2);
				bHeaderSent = true;
			}

			if (bHeaderSent) {
				char end[] = { 0x0D, 0x0A };

				send(sock, fileBuffer, fileSize, 0);
				send(sock, end, 2, 0);
			}

			delete[] fileBuffer;
		}

		else {
			//If fileBuffer is null, the page was not found
			HTTP_SendHeader(sock, 404, "text/html", 0); //Error 404
			//Normally, a server would give an 404 page


			/*
			* 
			* For custom commands, we can use a non-existant file with parameters
			
			if(!strcmp(requestedFile, "main.cmd") {

				size_t paramLen = countBytesUntil(&str[4 + brkFound], ' ');

				char *parameters = new char[paramLen - brkFound];
				memcpy(parameters, &str[4 + brkFound], paramLen);

				

				delete[] parameters;
			}
			*/


		}
		delete[] requestedFile;

	}
}

void HTTP_SocketThread(SOCKET *sockPtr)
{
	SOCKET sock = *sockPtr;

	char *buffer = new char[512];

	size_t sz = recv(sock, buffer, 512, 0);

	if (sz <= 512)
		HTTP_ProcessRequest(sock, buffer, sz);

	Sleep(1);

	closesocket(sock);

	delete[] buffer;

	//The thread will terminate
}
