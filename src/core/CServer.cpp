#include "CServer.hh"
#include "CClient.hh"
#include "ServiceManager.hh"

static void	exit_server(int sig)
{
  (void)sig;
  gg_exit = true;
}

namespace ACPPS
{
CServer::CServer(const int port, const std::list<std::string> &serviceNames) : 
  _serviceManager(ServiceManager::getInstance()), 
  _serviceLoader("."), 
  _servicesList(serviceNames), 
  _port(port)
{
	int					sock;
	struct sockaddr_in	sin;
	struct protoent		*protocol;
	int					boole = 1;
	int  				maxconnectionsocket = 6;

	std::cout << coutprefix << "{** ACPPS **}" <<std::endl;
	std::cout << coutprefix << "[Server]" <<std::endl;

 	signal(SIGINT, exit_server);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	protocol = getprotobyname("TCP");
	if (protocol == NULL)
		throw CServerException("get protocol check file /etc/protocols");
	if ((sock = socket(PF_INET, SOCK_STREAM, protocol->p_proto)) == -1)
		throw CServerException("socket method error");
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &boole, sizeof(boole)) == -1)
		throw CServerException("setsockopt method error");
	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1)
		throw CServerException("bind method error");
	if (listen(sock, maxconnectionsocket) == -1)
		throw CServerException("listen method error");

	_socket = sock;
	_sockmax = sock;

	FD_ZERO(&_write_set);
	FD_ZERO(&_read_set);
	FD_SET(sock, &_read_set);

	std::cout << coutprefix << "Config:" <<std::endl;
	std::cout << coutprefix << "- Port: "<< port <<std::endl;
	std::cout << coutprefix << "- ByteBuffer size: "<< CRING_BUFFER_SIZE <<std::endl;

	std::cout << coutprefix << "[ServiceLoader]" <<std::endl;
	std::list<std::string>::iterator it = this->_servicesList.begin();
	while (it != this->_servicesList.end()){
		this->loadService(*it);
		++it;
	}

	std::cout << coutprefix << "[ServiceManager]" <<std::endl;
	if (!this->_serviceManager->startServices())
	{
		this->_serviceManager->stopServices();
	}

	gg_exit = false;
}

void CServer::loadService(const std::string &serviceName) {
	try {
		IService *service = this->_serviceLoader.loadService<IService>(serviceName);
		this->_serviceManager->bindService(service);
	}catch(ClassException &e){
		std::cout << e.what() << std::endl;
	}
}

CServer::~CServer()
{
	close(_socket);
}

void CServer::stop() {
	gg_exit = true;
}

void CServer::acceptClient() {
	int			csock;
	struct sockaddr_in	caddr;
	socklen_t		clen = sizeof(caddr);

	if ((csock = accept(this->_socket, (struct sockaddr*)&caddr, &clen)) != -1)
	{
		
		this->_clientsList.push_front(new CClient(this, csock, caddr));
		FD_SET(csock, &this->_read_set);

		if (csock > this->_sockmax) {
			this->_sockmax = csock;
		} else {
			std::list<CClient*>::iterator it = this->_clientsList.begin();
			int sockmax = this->_socket;
			while (it != this->_clientsList.end())
			{
				if (sockmax < (*it)->getSocket())
					sockmax = (*it)->getSocket();
				++it;
			}
			this->_sockmax = sockmax;
		}
	}
	else
	{
		std::cout << coutprefix << "[FATAL ERROR] accept method" << std::endl;
	}
}
void CServer::disconnectClient(CClient *c) {
	this->_clientsList.remove(c);
#ifndef SILENT
	std::cout << coutprefix << c->getIpAdress() << " Disconnected" << std::endl;
#endif
	c->closeSocket();
	delete c;
}

void CServer::clientRemoveWriteListening(CClient *c) {
	FD_CLR(c->getSocket(), &this->_write_set);
}
void CServer::clientAddWriteListening(CClient *c) {
	FD_SET(c->getSocket(), &this->_write_set);
}
void CServer::run()
{

	std::cout << coutprefix << "Listening new client ... " << std::endl;
	fd_set	cp_read;
	fd_set	cp_write;

	while (!gg_exit)
	{
		cp_write = this->_write_set;
		cp_read = this->_read_set;

		if (select(this->_sockmax + 1, &cp_read, &cp_write, NULL, NULL) == -1)
		{
			if (errno == EINTR)
			{
				gg_exit = true;
				break;				
			}
			throw CServerException("select method error");
		}
		if (FD_ISSET(this->_socket, &cp_read))
			this->acceptClient();

		std::list<CClient*>::iterator it = this->_clientsList.begin();
		while (it != this->_clientsList.end())
		{
			CClient* client = *it;
			++it;
			if (FD_ISSET(client->getSocket(), &cp_write)
			&& !client->doWrite())
			{
				FD_CLR(client->getSocket(), &this->_write_set);
				FD_CLR(client->getSocket(), &this->_read_set);
				this->disconnectClient(client);
			}
			else if (FD_ISSET(client->getSocket(), &cp_read)
			&& !client->doRead())
			{
				if (client->isInQueue())
					FD_CLR(client->getSocket(), &this->_write_set);
				FD_CLR(client->getSocket(), &this->_read_set);
				this->disconnectClient(client);
			}
		} 
	}
	this->_serviceManager->stopServices();
	std::cout << coutprefix << "Server Terminated" << std::endl;
}

CServerException::CServerException(const std::string &error) : m_error(error){}
CServerException::~CServerException() throw(){}
const char* CServerException::what() const throw()
{
  return m_error.c_str();
}

}
