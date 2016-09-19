#include	<list>
#include	<typeinfo>
#include	"ServiceManager.hh"

namespace ACPPS {

ServiceManager*	ServiceManager::m_instance = NULL;
ServiceManager::ServiceManager()
{}

ServiceManager::~ServiceManager()
{}

ServiceManager	*ServiceManager::getInstance()
{
  if (!m_instance)
    m_instance = new ServiceManager();
  return m_instance;
}

void ServiceManager::bindService(IService *service)
{
  this->_services.push_back(service);
}

IService *ServiceManager::getService(const std::string &name)
{
  std::list<IService *>::iterator it = this->_services.begin();

  while (it != this->_services.end())
  {
    std::string serviceName = (*it)->getServiceName();
    if (name.compare(serviceName) == 0)
      return (*it);
    ++it;
  }
  return NULL;
}

void ServiceManager::notifyServerEventClientConnected(CClient *user)
{
  std::list<IService *>::iterator it = this->_services.begin();

  while (it != this->_services.end())
    {
      (*it)->onServerEventClientConnected(user);
      ++it;
    }
}

void ServiceManager::notifyServerEventClientDisconnected(CClient *user)
{
  std::list<IService *>::iterator it = this->_services.begin();

  while (it != this->_services.end())
    {
      (*it)->onServerEventClientDisconnected(user);
      ++it;
    }
}

void ServiceManager::bindAtRuntimeService(IService *service)
{
  if (service->startService(this))
    {
      std::cout << coutprefix << "'" << service->getServiceName() << "' started at runtime" << std::endl;
      this->_services.push_back(service);
    }else{
      std::cout << coutprefix << "Failed to start '" << service->getServiceName() << "'" << std::endl;
    }
}

bool ServiceManager::startServices()
{
  std::list<IService *>::iterator it = this->_services.begin();

  while (it != this->_services.end())
  {
    if (!(*it)->startService(this))
    {
      std::cout << coutprefix << "Failed to start '" << (*it)->getServiceName() << "'" << std::endl;
      it = this->_services.erase(it);
      return false;
    }else{
      addPacketIdsFromService((*it));
      std::cout << coutprefix << "'" << (*it)->getServiceName() << "' started" << std::endl;
      ++it;
    }
  }
  return true;
}

void ServiceManager::stopServices()
{
  std::list<IService *>::iterator it = this->_services.begin();

  while (it != this->_services.end())
    {
      delPacketIdsFromService((*it));
      (*it)->stopService();
      std::cout << coutprefix << "'" << (*it)->getServiceName() << "' stopped" << std::endl;
      ++it;
    }
}

void  ServiceManager::addPacketIdsFromService(IService *service)
{
  const std::list<PacketID>  &packetIds = service->getPacketIds();

  for (std::list<PacketID>::const_iterator it = packetIds.begin();
       it != packetIds.end();
       ++it)
    {
      registerPacket(service, (*it));
    }
}

void  ServiceManager::delPacketIdsFromService(IService *service)
{
  const std::list<PacketID>  &packetIds = service->getPacketIds();

  for (std::list<PacketID>::const_iterator it = packetIds.begin();
       it != packetIds.end();
       ++it)
    {
      unregisterPacket(service, (*it));
    }
}

void ServiceManager::registerPacket(IService *service, PacketID packetID)
{
  std::list<IService *> serviceList;
  std::map<PacketID, std::list<IService *> >::iterator it = this->_mapRegister.find(packetID);

  if (it == this->_mapRegister.end()){
    serviceList = std::list<IService * >();
    serviceList.push_back(service);
  }
  else
  {
    serviceList = (*it).second;
    serviceList.push_back(service);
  }
  this->_mapRegister.insert(std::make_pair(packetID, serviceList));
  std::cout << coutprefix << "'" << service->getServiceName() << "' has registered packet ID '" << packetID << "'" << std::endl;
}

void ServiceManager::unregisterPacket(IService *service, PacketID packetID)
{
  std::list<IService *> serviceList;
  std::map<PacketID, std::list<IService *> >::iterator it = this->_mapRegister.find(packetID);

  if (it != this->_mapRegister.end())
  {
    serviceList = (*it).second;
    serviceList.remove(service);
    if (serviceList.size() == 0)
      this->_mapRegister.erase(packetID);
    std::cout << coutprefix << "'" << service->getServiceName() << "' has unregistered packet ID '" << packetID << "'" << std::endl;
  }
}

bool ServiceManager::handlePacket(uint16_t packetID, t_packet_data *packet_data, CClient *user)
{
  std::map<PacketID, std::list<IService *> >::iterator it = this->_mapRegister.find(static_cast<PacketID>(packetID));

  if (it != this->_mapRegister.end()){
    std::list<IService *>::iterator serviceIt = (*it).second.begin();
    while (serviceIt != (*it).second.end())
      {
        SerializableBuffer buffer(packet_data->data, packet_data->packet_len);
        if (!(*serviceIt)->handlePacket(static_cast<PacketID>(packetID), buffer, user)){
          return false;
        }
        ++serviceIt;
      }
    return true;
  }
  std::cout << coutprefix << "Packet ID '" << packetID << "' packet forcing from the client "<< user->getIpAdress() << std::endl;
  return false;
}
}
