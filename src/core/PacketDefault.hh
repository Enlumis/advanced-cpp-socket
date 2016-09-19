#pragma once

#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "Packet.hh"

typedef struct			s_packet_data_default
{
  unsigned char		uchar_test;
  unsigned short	ushort_test;
  unsigned int		uint_test;
  short				short_test;
  int				int_test;
}__attribute__((__packed__))	t_packet_data_default;

class PacketDefault: public ACPPS::Packet
{
public:
	PacketDefault();
	~PacketDefault();

public:
	PacketID getPacketID();
	void serialize(SerializableBuffer &dest);
	void unserialize(SerializableBuffer &src);
	
public:
	t_packet_data_default _data;
};
