#ifndef CCLIENT_H_
# define CCLIENT_H_

# include	<iostream>
# include	<string>
# include	<exception>
# include	<map>

# include <sys/stat.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>

# include <netinet/in.h>
# include <arpa/inet.h>

#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>

#include "global.h"

#include "ByteBuffer.hh"
#include "CServer.hh"

namespace ACPPS
{
class CClient
{
	typedef bool (CClient::*PacketHandler)(t_packet_data *packet);
public:
	static std::map<PacketID, PacketHandler>	_packetsMap;

private:
	CServer*            _server;
	int                 _socket;
	struct sockaddr_in  _addr;
	std::string			_straddr;

	bool                _is_in_queue;
public:
	ByteBuffer        _read_buf;
	ByteBuffer        _write_buf;

public:
	CClient(CServer* server, const int socket, struct sockaddr_in addr);
	~CClient();
private:
	CClient(const CClient &);
	CClient &operator=(const CClient &);

public:
	bool handleDefautPacket(t_packet_data *packet_data);

public:
	
	std::string getIpAdress();
	bool sendPacket(Packet &p);
	void registerPacketHandler(PacketID p, PacketHandler h);

	int getSocket();
	void closeSocket();
	bool isInQueue();
	bool doWrite();

	bool handlePacket(t_packet_data *packet);
	bool doRead();
};
}

#endif
