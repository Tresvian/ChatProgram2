#include "messageHandler.h"

messageHandle::messageHandle(
	std::vector<Client*> ptrList, std::atomic_bool* endindi,
	IRC* parentIRC)
	: endIndicator(endindi), clientPtrList(ptrList),
	  parentIRC(parentIRC)
{
	run();
}


void messageHandle::run()
{
	try
	{
		for (auto& eachClient : clientPtrList)
		{
			if (checkForMessage(eachClient))
			{
				std::string temp = eachClient->getMessage();

			}
		}
	}
	catch (std::exception& e)
	{
		// looks like we are gonna exit 
		// need exit handling
		std::cout << e.what() << std::endl;
	}
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(150ms);
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