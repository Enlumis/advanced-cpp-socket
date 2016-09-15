#pragma once

#include "Packet.hh"
#include "IService.hh"
#include "CClient.hh"
#include <map>
#include <list>

namespace ACPPS
{
class ServiceManager
{
private:
  ServiceManager();
  ~ServiceManager();

public:

  static ServiceManager		*getInstance();

  void bindService(IService *service);
  void bindAtRuntimeService(IService *service);
  bool startServices();
  void stopServices();

  IService *getService(const std::string &name);
  void registerPacket(IService *service, PacketID packetID);
  void unregisterPacket(IService *service, PacketID packetID);

  bool handlePacket(uint16_t packetID, t_packet_data *buffer, CClient *user);
  void notifyServerEventClientConnected(CClient *user);
  void notifyServerEventClientDisconnected(CClient *user);

private:
  void	addPacketIdsFromService(IService *service);
  void	delPacketIdsFromService(IService *service);

private:
  std::map<PacketID, std::list<IService *> > _mapRegister;
  std::list<IService *> _services;

private:
  static ServiceManager		*m_instance;
};
}
