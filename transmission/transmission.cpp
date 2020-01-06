extern "C" {
#include "readin.h"
}

#include "transmission.h"


int Server::setup(char* ip, char* port) {
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);
}

int Server::shutdownServer() {
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}

int Server::recievemode(const char* path) {
	char* buf = (char*)malloc(sizeof(size_t) * 4000000);
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		this->iResult = recv(this->ClientSocket, buf, sizeof(size_t) * 4000000, 0);
		//printf("%lf\n", apple.price);
		//printf("%i", apple.weight);
		if (this->iResult > 0) {
			printf("Bytes received: %d\n", this->iResult);
			write_to_file(path, buf, this->iResult);				//запись поноса в файл с сокета
			// Echo the buffer back to the sender
			this->iSendResult = send(this->ClientSocket, this->recvbuf, this->iResult, 0);
			if (this->iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(this->ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", this->iSendResult);
		}
		else if (this->iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(this->ClientSocket);
			WSACleanup();
			return 1;
		}

		//system("PAUSE");

	} while (this->iResult > 0);
}

int Client::setup(char* ip, char* port) {
	// Validate the parameters
	//if (argc != 2) {
	//	printf("usage: %s server-name\n", argv[0]);
	//	return 1;
	//}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
}

int Client::sendfile(const char* path) {
	char* buf = (char*)malloc(sizeof(size_t) * 4000000);
	int in_obj;
	do {
		in_obj = read_from_file(path, buf, sizeof(size_t) * 4000000);

		this->iResult = send(this->ConnectSocket, buf, in_obj, 0);
		if (this->iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(this->ConnectSocket);
			WSACleanup();
			return 1;
		}
		this->iResult = recv(this->ConnectSocket, this->recvbuf, this->recvbuflen, 0);

		if (this->iResult > 0) { printf("Bytes received: %d\n", this->iResult); }
		else if (this->iResult == 0) { printf("Connection closed\n"); }
		else { printf("recv failed with error: %d\n", WSAGetLastError()); }

		//system("PAUSE");
		if (in_obj == 0) { break; }
	} while (this->iResult > 0);


	printf("Bytes Sent: %ld\n", this->iResult);
}

int Client::shutdownClient() {
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ConnectSocket);
	WSACleanup();
}