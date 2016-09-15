#include	"core/CServer.hh"

int main()
{
	ACPPS::CServer server(4000);
	server.loadService("service");
	server.run();
	return 0;
}
