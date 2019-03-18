#include "messageHandler.h"

messageHandle::messageHandle(
	std::map<int, Client*> ptrList, std::atomic<bool>* endindicator,
	IRC* const parentIRC)
	: endIndicator(endindicator), clientPtrList(ptrList),
	parentIRC(parentIRC)
{
	run();
}


messageHandle::~messageHandle()
{
}


void messageHandle::run()
{
	// This looks weird because its using a std::map
	for (auto& eachClient : clientPtrList)
	{
		if (checkForMessage(eachClient.second))
		{
			std::string temp = (eachClient.second)->getMessage();

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
	return client->getMessage;
}