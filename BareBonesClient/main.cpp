#include <iostream>
#include <ws2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std; 

void main()
{
	string ipAddress = "192.168.1.19"; // IP address of the server
	int port = 54000; // Listening port # of the server

	//Initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start winsock, Error # " << wsResult << endl;
		return;
	}

	//Create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Error # " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Error # " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	//Do-while loop to send and receive data
	char buf[4096];
	string userInput;

	do
	{
		// Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) // Make sure the user has typed in something 
		{

			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int byteReceived = recv(sock, buf, 4096, 0);
				if (byteReceived > 0)
				{
					// Echo response to console 
					cout << "SERVER> " << string(buf, 0, byteReceived) << endl;
				}
		}

	}

} while (userInput.size() > 0); 

		//Gracefully close down everything
			closesocket(sock); 
			WSACleanup(); 
	}

