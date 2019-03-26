#pragma once
#include "Client.h"
#include "IRC.h"

class Client;
class IRC;

class messageHandle
{
private:
	std::vector<Client*> clientPtrList;
	std::atomic_bool* endIndicator;
	IRC* parentIRC;

public:
	messageHandle(std::vector<Client*> ptrList,
		std::atomic_bool* endindi,
		IRC* parentIRC);

	void run();
	bool checkForMessage(Client* client);
	std::string getClientMessage(Client* client);
};