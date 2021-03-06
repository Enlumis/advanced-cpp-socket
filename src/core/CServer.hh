#ifndef CSERVER_H_
# define CSERVER_H_

# include	<iostream>
# include	<string>
# include	<exception>
# include <list>

# include <sys/stat.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>

# include <netinet/in.h>
# include <arpa/inet.h>

#include <stdlib.h>
#include <netdb.h>
#include <signal.h>

#include "Packet.hh"
#include "global.h"
#include "ServiceLoader.hpp"

namespace ACPPS 
{

class ServiceManager;

class CServerException : public std::exception
{
private:
  std::string   m_error;

public:
  CServerException(const std::string &error);
  virtual ~CServerException() throw();

public:
  virtual const char* what() const throw();
};

class CServer
{
public:
  ServiceManager*       _serviceManager;
private:
  ServiceLoader         _serviceLoader;
  int                   _port;
  std::list<std::string> _servicesList;
  int                   _socket;

  std::list<CClient*>    _clientsList;

  fd_set                _read_set;
  fd_set                _write_set;
  int                   _sockmax;

public:
  CServer(const int port, const std::list<std::string> &serviceNames);
  ~CServer();

public:
  void run();
  void stop();
  void acceptClient();
  void disconnectClient(CClient *c);
  void clientAddWriteListening(CClient *c);
  void clientRemoveWriteListening(CClient *c);

private:
  void loadService(const std::string &serviceName);

};

}

#endif
