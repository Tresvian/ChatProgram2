#include "Client.h"

Client::Client(boost::asio::io_context& ioref, int id,
	IRC* const parent, std::atomic<bool>* endIndicator)
	: socket(ioref),
	m_clientStatus(clientStatus::disconnected),
	parentIRC(parent),
	id(id),
	endIndicator(endIndicator)
{
	parentIRC->updateClientList(id, this);
	run();
}


void Client::setStatus(clientStatus x)
{
	m_clientStatus = x;
}


const clientStatus Client::getStatus() const
{
	return m_clientStatus;
}


void Client::requestMessage(std::string message)
{
	messageLock.lock();
	messageRequests.push(message);
	messageLock.unlock();
}


std::string Client::getMessage()
{
	messageLock.lock();

	std::string temp = messageRequests.front();
	messageRequests.pop();

	messageLock.unlock();
	return temp;
}


const int& Client::getid() const
{
	return id;
}


bool Client::messageReady()
{
	messageLock.lock();
	if (messageRequests.empty())
	{
		messageLock.unlock();
		return false;
	}
	messageLock.unlock();
	return true;
}


void Client::netSendMessage(std::string message)
{
	socket.send(boost::asio::buffer(message));
}


void Client::run()
{
	while (getStatus() == clientStatus::disconnected)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);
		if (*endIndicator == true)
		{
			break;
		}
	}


	try
	{
		std::string buffer;
		socket.receive(boost::asio::buffer(buffer));
		messageLock.lock();
		messageRequests.push(buffer);
		messageLock.unlock();
	}
	catch (std::exception& e)
	{
		setStatus(clientStatus::disconnected);
	}
	

	if (*endIndicator == false)
	{
		run();
	}
}