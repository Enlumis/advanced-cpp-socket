#include "PacketDefault.hh"

PacketDefault::PacketDefault() {
	this->_data.uchar_test = 56;
	this->_data.ushort_test = 1324;
	this->_data.uint_test = 2543624;
	this->_data.short_test = 456;
	this->_data.int_test = 423452;
}
PacketDefault::~PacketDefault() {
	
}
PacketID PacketDefault::getPacketID() {
	return PACKET_DEFAULT;
}
void PacketDefault::serialize(SerializableBuffer &dest) {
	dest.writeToData<t_packet_data_default>(this->_data);
	dest.writeToData<int>(42);
	dest.writeToData<char>(42);
}
void PacketDefault::unserialize(SerializableBuffer &src) {
	this->_data = src.readToData<t_packet_data_default>();
}
