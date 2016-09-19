#pragma once

# include	<list>
# include	<string>
# include	"Service.hpp"
# include	"ServiceManager.hh"


class UserService : public ACPPS::Service<UserService>
{
public:
  UserService();
  ~UserService() {}

  bool        handlePacketDefault(SerializableBuffer &buffer, ACPPS::CClient *user);

  bool				startService(ACPPS::ServiceManager *serviceManager);
  void				stopService();
  void				onServerEventClientDisconnected(ACPPS::CClient *user);
  void				onServerEventClientConnected(ACPPS::CClient *user);

private:
  UserService(const UserService &);
  UserService &operator=(const UserService &);

};

