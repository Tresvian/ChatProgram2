#include "IRC.h"
#include "Client.h"
#include "messageHandler.h"
#include "acceptorHandler.h"

int main(int argc, char* argv[])
{
	std::vector<std::string> argVector;
	std::cout << "Parasing args" << std::endl;
	for (int i = 1; i < argc; ++i)
	{
		argVector.push_back(argv[i]);
		std::cout << "Arg: " << i << argv[i];
	}

	// Changing this into something that has multiple instances
	// is fairly easy. It's all classes. Change server prompt
	// to be here instead of inside the ircInstance.
	try
	{
		IRC ircInstance(10, 5005);
	}
	catch (std::exception& e)
	{
		std::cout << "Exiting with failure" << std::endl;
		std::cout << e.what() << std::endl;
	}

	return 0;
};