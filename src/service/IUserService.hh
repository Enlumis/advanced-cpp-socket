#pragma once

# include	<list>
# include	<string>
# include	"IService.hh"
# include	"ServiceManager.hh"

class IUserService : public ACPPS::IService
{
public:
  virtual void		handlePacket(PacketID packetid, t_packet_data *buffer, ACPPS::CClient *user) = 0;
  virtual bool		startService(ACPPS::ServiceManager *serviceManager) = 0;
  virtual void		stopService() = 0;
  virtual std::string	getServiceName() const = 0;
  virtual void onServerEventClientDisconnected(ACPPS::CClient *user) = 0;
  virtual void onServerEventClientConnected(ACPPS::CClient *user) = 0;

};
