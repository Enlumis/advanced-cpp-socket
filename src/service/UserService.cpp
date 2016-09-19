#include	<iostream>
#include "global.h"
#include	"UserService.hh"
#include "PacketDefault.hh"

UserService::UserService() : Service("service")
{
	this->registerPacketHandler(PACKET_DEFAULT, &UserService::handlePacketDefault);
}

bool		UserService::startService(ACPPS::ServiceManager *serviceManager)
{
	(void)serviceManager;
 // IService *databaseService = serviceManager->getService("DatabaseService");
 // if (!databaseService || !(m_database = dynamic_cast<IDatabase *>(databaseService)))
  //  {
   //   std::cout << "[" << m_userServiceName << "]  : "
//		<< "Warning, DatabaseService not found." << std::endl;
  //    return false;
   // }

  return true;
}

bool		UserService::handlePacketDefault(SerializableBuffer &buffer, ACPPS::CClient *user) {
	std::cout << coutprefix << "UserService::handlePacketDefault OK" << std::endl;

	PacketDefault testRead;
	try {
		testRead.unserialize(buffer);
	}catch (PacketBufferException &packet_exception) {
		std::cout << coutprefix << "Serialization exception the client has send a bad packet (FORCE DISCONNECT)" << std::endl;
		return false;
	}
	std::cout << coutprefix << "testRead { uchar:" << testRead._data.uchar_test 
	        << "ushort:" << testRead._data.ushort_test 
	        << "uint:" << testRead._data.uint_test 
	        << "short:" << testRead._data.short_test 
	        << "int:" << testRead._data.int_test 
	        << std::endl;

	PacketDefault test;
	user->sendPacket(test);
	return true;
}

void UserService::stopService()
{}

void UserService::onServerEventClientDisconnected(ACPPS::CClient *user)
{
	(void)user;
}

void UserService::onServerEventClientConnected(ACPPS::CClient *user)
{
	(void)user;
}
