#pragma once
#include "IRC.h"
#include "Client.h"
#include "messageHandler.h"

class Client;
class IRC;

class acceptorHandle
{
private:
	std::vector<Client*> clientPtrList;
	std::atomic_bool* endIndicator;
	IRC* parentIRC;
	boost::asio::ip::tcp::acceptor* acceptorPtr;

public:
	acceptorHandle(
		std::vector<Client*> ptrList,
		std::atomic_bool* endindi,
		IRC* parentIRC);

	void run();
	void connect(Client* client);
	void disconnect(Client* client);
};

