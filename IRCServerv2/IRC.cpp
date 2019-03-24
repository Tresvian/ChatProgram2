#include "IRC.h"

IRC::IRC(int socketCount, int port)
	: socketCount(socketCount),
	acceptor(io_context),
	endpoint(boost::asio::ip::tcp::v4(), port)
{
	std::atomic<bool>* endIndicator;
	*endIndicator = new bool(false);
	for (int i; i < socketCount; ++i)
	{
		// Spawns threads, and lets them start updating back.
		std::thread thread(newClient, io_context, i, this, endIndicator);

		clientListThreads.push_back(std::move(thread));
	}
	std::cout << "Spawning threads, please wait..." << std::endl;

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(30s);
	// Controller for messages.
	messageHandler = std::thread(
		messageHandle(clientList, endIndicator, this));

	run();
}


IRC::~IRC()
{
	messageHandler.join();
	acceptorHandler.join();
	for (auto eachThread = clientListThreads.begin();
		eachThread != clientListThreads.end();
		++eachThread)
	{
		eachThread->join();
	}
}


void IRC::newClient(boost::asio::io_context& ioref, int id,
	IRC* parent, std::atomic<bool>* endIndicator)
{
	// param IRC* parent is supposed to be "this"
	Client(ioref, id, parent, endIndicator);
}



void IRC::updateClientList(int id, Client* clientPtr)
{
	clientListmutex.lock();
	clientList.push_back(clientPtr);
	clientListmutex.unlock();
}


void IRC::netSendMessage(std::string message)
{
	for (auto& eachClient : clientList)
	{
		if (eachClient->getStatus() == clientStatus::connected)
		{
			boost::system::error_code ignoredError;
			eachClient->netSendMessage(message);
		}
	}
}


static std::vector<std::string> split(std::string theString)
{
	using namespace std;
	vector<string> returnResult;
	string::iterator i = theString.begin();
	while (i != theString.end())
	{
		while (isspace(*i) && i != theString.end())
		{
			++i;
		}
		string::iterator j = i;
		while (!isspace(*j) && j != theString.end())
		{
			++j;
		}

		if (i != j)
		{
			returnResult.push_back(string(i, j));
			i = j;
		}
	}
	return returnResult;
}


void IRC::run()
{
	std::string command;
	while (std::getline(std::cin, command))
	{
		std::vector<std::string> commandVector;
		commandVector = split(command);

		if (commandVector[0] == std::string("/help"))
		{
			std::cout << "Available commands:\n"
				<< "/help\n"
				<< "/quit\n"
				<< std::endl;
		}
		if (commandVector[0] == std::string("/quit"))
		{
			*endIndicator = true;
		}
	}
	if (*endIndicator == false)
	{
		std::cout << "Did you exit weirdly? (/help)" << std::endl;
		run();
	}
}