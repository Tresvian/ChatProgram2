#pragma once
#include "boost/asio.hpp"
#include "IRC.h"
#include <queue>

enum clientStatus { disconnected, connected};

class Client
{
private:
	boost::asio::ip::tcp::socket socket;
	clientStatus m_clientStatus;
	IRC* const parentIRC;
	int id;

	std::queue<std::string> messageRequests;
	std::mutex messageLock;

	std::atomic<bool>* endIndicator;

public:
	Client(boost::asio::io_context& ioref, int id,
		IRC* const parent, std::atomic<bool>* endIndicator);

	void setStatus(clientStatus x);
	const clientStatus getStatus() const;
	void requestMessage(std::string message);
	std::string getMessage();
	const int& getid() const;
	bool messageReady();
	void netSendMessage(std::string message);
	void run();
};