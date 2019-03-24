#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <cctype>
#include <exception>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include "boost/asio.hpp"
#include "Client.h"
#include "messageHandler.h"
#include "acceptorHandler.h"

/*
This is the main parent for all client instances.

Once endIndicator = true, all childs will detect and exit threads.

Constructs its own io_context, socket amount passed, then
	port number passed.
	Constructs socket threads(socket#), and endIndicator.
	Waits for the threads to spawn and reach back, then passes
	the threads locations/IDs into the messageHandler to start
	monitoring. TODO//acceptorhandler.
	run() loops the instance to stay alive (actually used for
	starting up the cmd prompt).

	/quit to turn endIndicator=true
	/help to display commands available.

Todo: Add in a history
	  Finish acceptorHandler
	  announce server join.
	  make IRC client :(
	  Provide IRC master server commands.

updateClientList(int,Client* const) used for socket threads
	to reach back and update socket locations.
netSendMessage(string) broadcast out to all sockets.
run() keepalive. currently cmd prompt too.
*/
class Client;



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
	std::vector<Client*> clientList;
	std::mutex clientListmutex;

	std::atomic<bool> *endIndicator;

public:
	IRC(int socketCount, int port);
	~IRC();

	static void newClient(boost::asio::io_context& ioref, int id,
		IRC* parent, std::atomic<bool>* endIndicator);
	void updateClientList(int id, Client*  clientPtr);
	void netSendMessage(std::string message);
	void run();
};
