#ifndef PACKET_H_
# define PACKET_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>

#include "global.h"
#include "PacketID.hh"
#include "SerializableBuffer.hpp"

typedef struct			s_packet_header
{
  unsigned short	packet_id;
  unsigned int      packet_len;
}__attribute__((__packed__))	t_packet_header;

typedef struct			s_packet_data
{
  unsigned short	packet_id;
  unsigned int      packet_len;
  char		data[CRING_BUFFER_SIZE - sizeof(t_packet_header)];
}__attribute__((__packed__))	t_packet_data;

namespace ACPPS
{
class Packet
{
public:
  Packet();
  virtual ~Packet();

public:
  virtual PacketID getPacketID();
  virtual void serialize(SerializableBuffer &dest);
  virtual void unserialize(SerializableBuffer &src);
};
}

#endif
