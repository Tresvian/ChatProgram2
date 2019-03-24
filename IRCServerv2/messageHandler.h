#pragma once
#include "Client.h"
#include "IRC.h"

class Client;
class IRC;

class messageHandle
{
private:
	std::vector<Client*> clientPtrList;
	std::atomic<bool>* endIndicator;
	IRC* parentIRC;

public:
	messageHandle(std::vector<Client*> ptrList,
		std::atomic<bool>* endindi,
		IRC* parentIRC);
	~messageHandle();

	void run();
	bool checkForMessage(Client* client);
	std::string getClientMessage(Client* client);
};