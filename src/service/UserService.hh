#pragma once

# include	<list>
# include	<string>
# include	"IUserService.hh"
# include	"ServiceManager.hh"


class UserService : public IUserService
{
public:
  static const std::string	m_userServiceName;

public:
  UserService();
  ~UserService() {}

  void				handlePacket(PacketID packetId, t_packet_data *packet, ACPPS::CClient *user);
  bool				startService(ACPPS::ServiceManager *serviceManager);
  void				stopService();
  std::string			getServiceName() const;
  void				onServerEventClientDisconnected(ACPPS::CClient *user);
  void				onServerEventClientConnected(ACPPS::CClient *user);



private:
  UserService(const UserService &);
  UserService &operator=(const UserService &);

};

