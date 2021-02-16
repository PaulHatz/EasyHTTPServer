#include "main.h"

bool bExit = false;
SOCKET SRV_sock = 0;

int initializeWinSock()
{
	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

}

bool initializeListener(int maxConnections)
{
	SRV_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SRV_PORT);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	unsigned long iMode = 1;

	if (setsockopt(SRV_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&iMode, 4))
		return false;

	if (bind(SRV_sock, (sockaddr *)&addr, sizeof(sockaddr_in)))
		return false;

	if (listen(SRV_sock, maxConnections))
		return false;

	return true;
}

int main()
{
	//Initialize WinSock
	if (initializeWinSock())
		return 1;

	printf("WinSock Initialized!\n");

	sockaddr_in addr = { 0 };
	int sz = sizeof(addr);

	//Initialize listener
	if (initializeListener(20)) {

		printf("HTTP Server Initialized.\n");
		printf("Open your browser to localhost:%ld\n", SRV_PORT);

		while (!bExit) {
			Sleep(1);

			SOCKET newSocket = accept(SRV_sock, (sockaddr *)&addr, &sz);
			if (newSocket) {
				if (newSocket == SOCKET_ERROR)
					continue;

				//Create thread every time there is a new connection

				DWORD ThreadID = 0;
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HTTP_SocketThread, &newSocket, 0, &ThreadID);
			}
		}
	}

	return 0;
}