#include "Client.h"

Client::Client(boost::asio::io_context& ioref, int id,
	IRC* parent, std::atomic_bool* endIndicator)
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

	//place a message in queue

	messageLock.lock();
	messageRequests.push(message);
	messageLock.unlock();
}


std::string Client::getMessage()
{
	// gets the front message
	messageLock.lock();

	std::string returnme;
	returnme = messageRequests.front();
	messageRequests.pop();

	messageLock.unlock();

	return returnme;
}


boost::asio::ip::tcp::socket* Client::getSocket()
{
	boost::asio::ip::tcp::socket* ptr = &socket;
	return ptr;
}


const int& Client::getid() const
{
	return id;
}


bool Client::messageReady()
{
	// check if message is available
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
		while (getStatus() == clientStatus::connected)
		{
			std::string buffer;
			socket.receive(boost::asio::buffer(buffer));
			requestMessage(buffer);
		}
	}
	catch (std::exception& e)
	{
		setStatus(clientStatus::disconnected);
		std::cout << e.what() << std::endl;
	}
	

	if (*endIndicator == false)
	{
		run();
	}
}