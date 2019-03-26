#include "acceptorHandler.h"

acceptorHandle::acceptorHandle(
	std::vector<Client*> ptrList,
	std::atomic_bool* endindi,
	IRC* parentIRC)
	: clientPtrList(ptrList),
	  endIndicator(endindi),
	  parentIRC(parentIRC),
	  acceptorPtr(parentIRC->getAcceptor())
{
	run();
}


void acceptorHandle::run()
{
	while (*endIndicator == false)
	{
		for (auto eachClient : clientPtrList)
		{
			if (eachClient->getStatus() == clientStatus::disconnected)
			{
				std::cout << "Listening on ID:" << eachClient->getid()
					<< std::endl;
				connect(eachClient);
			}
		}

		// throttle
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(150ms);
	}
}


void acceptorHandle::connect(Client* client)
{
	// little confusing. grabs client socket, dereference,
	// then accept with the acceptor.
	// acceptorPtr->accept(socket)
	boost::system::error_code ignoredErr;
	acceptorPtr->accept(*(client->getSocket()), ignoredErr);
}


void acceptorHandle::disconnect(Client* client)
{
	client->setStatus(clientStatus::disconnected);
}