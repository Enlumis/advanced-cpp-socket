#pragma once

# include	<list>
# include	<map>
# include	<string>
# include	"Packet.hh"
# include	"IService.hh"

namespace ACPPS
{
class ServiceManager;
class CClient;

template <class T>
class Service : public IService
{
public:
	typedef bool (T::*ServicePacketHandler)(SerializableBuffer &buffer, CClient *user);
protected:
	std::string _serviceName;
	std::list<PacketID>	_packetIdsMap;
	std::map<PacketID, ServicePacketHandler>	_handlersMap;
public:
	Service(const std::string &serviceName) : _serviceName(serviceName) {};
	virtual ~Service() {};

	void registerPacketHandler(PacketID packetID, ServicePacketHandler handler) {
		this->_packetIdsMap.push_front(packetID);
		this->_handlersMap[packetID] = handler;
	}

	bool handlePacket(PacketID packetid, SerializableBuffer &buffer, CClient *user) {
		ServicePacketHandler handler = this->_handlersMap[packetid];

		if (handler)
			return ((static_cast<T*>(this)->*handler)(buffer, user));
		return false;
	}
	std::string	getServiceName() const {
		return (this->_serviceName);
	}
 	const std::list<PacketID>	&getPacketIds() const {
 		return this->_packetIdsMap;
 	}


	virtual bool startService(ServiceManager *serviceManager) = 0;
	virtual void stopService() = 0;
	virtual void onServerEventClientDisconnected(CClient *user) = 0;
	virtual void onServerEventClientConnected(CClient *user) = 0;
};
}
