#pragma once
#include "Client.h"

class messageHandle
{
private:
	std::map<int, Client*> clientPtrList;
	std::atomic<bool>* endIndicator;
	IRC* const parentIRC;

public:
	messageHandle(std::map<int, Client*> ptrList,
		std::atomic<bool>* endindicator,
		IRC* const parentIRC);
	~messageHandle();

	void run();
	bool checkForMessage(Client* client);
	std::string getClientMessage(Client* client);
};