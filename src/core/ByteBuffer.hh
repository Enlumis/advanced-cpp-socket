#pragma once

# include	<iostream>
# include	<string>
# include	<exception>

# include <sys/stat.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>

# include <netinet/in.h>
# include <arpa/inet.h>

#include <stdlib.h>
#include <netdb.h>

#include "global.h"
#include "Packet.hh"

namespace ACPPS
{

class ByteBuffer 
{
public:
  char			_realbuffer[CRING_BUFFER_SIZE];
  char			_realtmpbuffer[CRING_BUFFER_SIZE];
  size_t		_realbufferSize;

public:
  ByteBuffer();
  ~ByteBuffer();

  int readSocket(int sock);
  int sendSocket(int sock);

  bool pushPacket(Packet &p);
  t_packet_data* extractPacket();
  size_t getBufferLength();

};
}
