#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <cctype>
#include <exception>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>
#include "boost/asio.hpp"
#include "Client.h"
#include "messageHandler.h"
/*
This is the main parent for all client instances.

Once endIndicator = true, all childs will detect and exit threads.

Upon construction with a port and amount of sockets,
	it will start spawning threads with newly made
	clients. Each client makes its own thread and communicates
	back to the parent IRC. Then, messageHandler spawns to 
	start looking for data from each client, and the 
	acceptorHandler spawns to direct connections to each
	client.

Todo: Add in a history
	  Finish acceptorHandler
	  announce server join.
	  make IRC client :(
	  Provide IRC master server commands.

*/
class IRC
{
private:
	int socketCount;
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::endpoint endpoint;

	std::thread messageHandler;
	std::thread acceptorHandler; //todo, last one.

	std::vector<std::thread> clientListThreads;
	std::map<int, Client*> clientList;
	std::mutex clientListmutex;

	std::atomic<bool>* endIndicator;

public:
	IRC(int socketCount, int port);
	~IRC();

	void updateClientList(int id, Client* const clientPtr);
	void netSendMessage(std::string message);
	void run();
};
