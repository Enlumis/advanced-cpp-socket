#include <list>
#include "core/CServer.hh"

int main()
{
	std::list<std::string> services;
	
	services.push_back("service");

	ACPPS::CServer server(4000, services);
	server.run();
	
	return 0;
}
