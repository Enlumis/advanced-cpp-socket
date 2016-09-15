#pragma once

# include	<list>
# include	<string>
# include	"Packet.hh"

namespace ACPPS
{
class ServiceManager;
class CClient;

class IService
{
public:
	virtual ~IService() {};
	virtual void		handlePacket(PacketID packetid, t_packet_data *buffer, CClient *user) = 0;
	virtual bool		startService(ServiceManager *serviceManager) = 0;
	virtual void		stopService() = 0;
	virtual std::string	getServiceName() const = 0;
	virtual void onServerEventClientDisconnected(CClient *user) = 0;
	virtual void onServerEventClientConnected(CClient *user) = 0;
};
}
