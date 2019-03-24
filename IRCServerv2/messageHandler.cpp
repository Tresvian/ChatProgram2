#include "messageHandler.h"

messageHandle::messageHandle(
	std::vector<Client*> ptrList, std::atomic<bool>* endindi,
	IRC* parentIRC)
	: endIndicator(endindi), clientPtrList(ptrList),
	parentIRC(parentIRC)
{
	run();
}


messageHandle::~messageHandle()
{
}


void messageHandle::run()
{
	for (auto& eachClient : clientPtrList)
	{
		if (checkForMessage(eachClient))
		{
			std::string temp = eachClient->getMessage();

		}
	}

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(100ms);
	if (*endIndicator == false)
	{
		run();
	}
}


bool messageHandle::checkForMessage(Client* client)
{
	return client->messageReady();
}


std::string messageHandle::getClientMessage(Client* client)
{
	return client->getMessage();
}