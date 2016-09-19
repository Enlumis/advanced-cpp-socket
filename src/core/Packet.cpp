#include "Packet.hh"

namespace ACPPS
{
Packet::Packet() {
	
}
Packet::~Packet() {
	
}
PacketID Packet::getPacketID() {
	return PACKET_DEFAULT;
}
void Packet::serialize(SerializableBuffer &dest) {
	(void)dest;
}
void Packet::unserialize(SerializableBuffer &src) {
	(void)src;
}
}
