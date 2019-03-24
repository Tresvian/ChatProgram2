#pragma once
#include "boost/asio.hpp"
#include "IRC.h"
#include "messageHandler.h"
#include "acceptorHandler.h"

/*
Session holder for a threaded socket.

Starts up with initial params for asio, and links
to the parent IRC + the endIndicator bool.

setStatus(clientStatus) sets the programs occupancy.
getStatus() returns clientStatus status
requestMessage(string) puts chat messages into a queue.
	locks to prevent undefined behavior.
getMessage() returns a message out of the queue, and removes it.
getid() returns the id given to this Client (tracking purposes)
messageReady() returns a bool if queue has a message.
	locks and reads if empty()
netSendMessage(string) sends a message through network
	socket to the connected client
run() is the loop to keep this thread alive.
	sleeps when disconnected, receives when connected.
	upon error, disconnection, sets to disconnected and
	begins the error again.
*/
enum clientStatus { disconnected, connected};
class IRC;

class Client
{
private:
	boost::asio::ip::tcp::socket socket;
	clientStatus m_clientStatus;
	IRC* parentIRC;
	int id;

	std::queue<std::string> messageRequests;
	std::mutex messageLock;

	std::atomic<bool>* endIndicator;

public:
	Client(boost::asio::io_context& ioref, int id,
		IRC* parent, std::atomic<bool>* endIndicator);

	void setStatus(clientStatus x);
	const clientStatus getStatus() const;
	void requestMessage(std::string message);
	std::string getMessage();
	const int& getid() const;
	bool messageReady();
	void netSendMessage(std::string message);
	void run();
};