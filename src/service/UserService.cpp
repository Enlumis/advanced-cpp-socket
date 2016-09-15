#include	<iostream>
#include	"UserService.hh"

const std::string UserService::m_userServiceName = std::string("UserService");

UserService::UserService()
{
	// register handler
}

bool		UserService::startService(ACPPS::ServiceManager *serviceManager)
{
 // IService *databaseService = serviceManager->getService("DatabaseService");
 // if (!databaseService || !(m_database = dynamic_cast<IDatabase *>(databaseService)))
  //  {
   //   std::cout << "[" << m_userServiceName << "]  : "
//		<< "Warning, DatabaseService not found." << std::endl;
  //    return false;
   // }

  return true;
}

std::string	UserService::getServiceName() const
{
  return UserService::m_userServiceName;
}

void UserService::stopService()
{

}

void		UserService::handlePacket(PacketID packetId, t_packet_data *buffer, ACPPS::CClient *user)
{
/*  std::cout << "[UserService] : executeTask..." << std::endl;
  if (!user->isAuthentificated())
    {
      std::cout << "[UserService] : warning an user is not authenticate and he try to communicate with unauthorized service..." << std::endl;
      user->disconnect();
      return ;
    }
  void (UserService::*service)(SerializerBuffer *buffer, UserController *user);

  if ((service = m_services[packetId]))
    (this->*service)(buffer, user); */
}

void UserService::onServerEventClientDisconnected(ACPPS::CClient *user)
{}

void UserService::onServerEventClientConnected(ACPPS::CClient *user)
{}
