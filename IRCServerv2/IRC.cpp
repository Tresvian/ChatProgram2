#include "IRC.h"


IRC::IRC(int socketCount, int port)
	: socketCount(socketCount),
	  acceptor(io_context,
	  boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
	  endIndicator(new std::atomic_bool(false))
{
	for (int i = 0; i < socketCount; ++i)
	{
		// Construct data for the client socket thread.
		clientStruct data{io_context, i, this, endIndicator};

		// Spawns threads, and lets them start updating back.
		std::thread thread(&newClient, data);

		// Move them into a manageable list.
		clientListThreads.push_back(std::move(thread));
	}
	std::cout << "Spawning threads, please wait..." << std::endl;

	// The clients need to report back into another list for
	// the other threads to use.
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(30s);


	// Start handlers. Struct data first.
	handleStruct handleData{clientList, endIndicator, this};


	messageHandler = std::thread(&newMessageHandler, handleData);


	acceptorHandler = std::thread(&newAcceptorHandler, handleData);
	// this is causing abort() for some reason. maybe error code not
	// being used somewhere
	std::cout << "Starting Command prompt:" << std::endl;
	run();
}


IRC::~IRC()
{
	// Join all threads once endIndicator is true.
	// If it's not true, this will get stuck.
	messageHandler.join();
	acceptorHandler.join();
	for (auto eachThread = clientListThreads.begin();
		eachThread != clientListThreads.end();
		++eachThread)
	{
		eachThread->join();
	}
}


void IRC::newClient(clientStruct data)
{
	// param IRC* parent is supposed to be "this"
	Client(data.io, data.id, data.parent, data.endIndicator);
}


void IRC::newMessageHandler(handleStruct handleData)
{
	// param IRC* parent is supposed to be "this"
	messageHandle(handleData.clientList,
		handleData.endIndicator, handleData.parent);
}


void IRC::newAcceptorHandler(handleStruct handleData)
{
	// param IRC* parent is supposed to be "this"
	acceptorHandle(handleData.clientList,
		handleData.endIndicator, handleData.parent);
}


void IRC::updateClientList(int id, Client* clientPtr)
{
	// This is reached by the socket threads. Adds themselves
	// to the list.
	clientListmutex.lock();
	clientList.push_back(clientPtr);
	clientListmutex.unlock();
}


void IRC::netSendMessage(std::string message)
{
	// Sends message out of ALL sockets.
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
	/*
	Just for the purpose of the command console.
	Splits the line up so it can be parsed.
	*/
	using namespace std;
	vector<string> returnResult;
	string::iterator i = theString.begin();
	while (i != theString.end())
	{
		while (i != theString.end() && isspace(*i))
		{
			++i;
		}
		string::iterator j = i;
		while (j != theString.end() && !isspace(*j))
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


boost::asio::ip::tcp::acceptor* IRC::getAcceptor()
{
	boost::asio::ip::tcp::acceptor* myptr = &acceptor;
	return myptr;
}


void IRC::run()
{
	// a loop to keep alive
	// can put this into main instead of IRC so 
	// multiple IRCs can be run at diff ports

	std::string command;
	while (std::getline(std::cin,command))
	{
		if (command.empty())
		{
			continue;
		}
		std::vector<std::string> commandVector;
		commandVector = split(command);

		if (commandVector[0] == std::string("/help"))
		{
			std::cout << "Available commands:\n"
				<< "-------\n"
				<< "/help\n"
				<< "/clear\n"
				<< "/quit\n"
				<< std::endl;
		}
		else if (commandVector[0] == std::string("/clear"))
		{
			system("cls");
		}
		else if (commandVector[0] == std::string("/quit"))
		{
			*endIndicator = true;
		}
		else
		{
			std::cout << "Unknown command (/help)" << std::endl;
		}
	}
	if (*endIndicator == false)
	{
		std::cout << "Did you exit weirdly? (/help)\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		run();
	}
}