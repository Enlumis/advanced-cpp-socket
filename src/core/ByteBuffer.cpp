#include <cstring>
#include	"ByteBuffer.hh"

namespace ACPPS
{

ByteBuffer::ByteBuffer()
{
  this->_realbufferSize = 0;
}

ByteBuffer::~ByteBuffer()
{
}

size_t ByteBuffer::getBufferLength() {
  return this->_realbufferSize;
}

bool ByteBuffer::pushPacket(Packet &p) {
  t_packet_header h;
  h.packet_id = p.getPacketID();
  SerializableBuffer buffer;
  p.serialize(buffer);
  h.packet_len = buffer.size();
  if (this->_realbufferSize + h.packet_len + sizeof(t_packet_header) >= CRING_BUFFER_SIZE) {
    std::cout << coutprefix << " [Server][FATAL ERROR] send buffer overflow" << std::endl;  
    return (false);
  }
  memcpy(this->_realbuffer + this->_realbufferSize, (char*)&h, sizeof(t_packet_header));
  this->_realbufferSize += sizeof(t_packet_header);
  buffer.copyToBuffer(this->_realbuffer + this->_realbufferSize);
  //p.serialize(this->_realbuffer + this->_realbufferSize);

#ifndef SILENT
  std::cout << coutprefix << "Packet inserted is the buffer (ID: "<< p.getPacketID() 
    << ", PacketLength: " << (h.packet_len  + sizeof(t_packet_header)) 
    << ", DataLength: " << h.packet_len
    << ")"<< std::endl;
#endif
  this->_realbufferSize += h.packet_len;
  return true;
}

int ByteBuffer::sendSocket(int socket) {
  int need_send = this->_realbufferSize;
  int wd = send(socket, this->_realbuffer, need_send, 0);
  if (wd == -1) {
    return (-1);
  }
  memcpy(this->_realbuffer, this->_realbuffer + wd, CRING_BUFFER_SIZE - wd);     
  this->_realbufferSize -= wd;
#ifndef SILENT
  std::cout << coutprefix << "Bytes sended: "<< wd << ", bytes rest: " << this->_realbufferSize << std::endl;
#endif
  return wd;
}

int ByteBuffer::readSocket(int socket) {
  int need_read = CRING_BUFFER_SIZE - this->_realbufferSize;
  if (need_read == 0)
    return -2;
  int rd = recv(socket, this->_realbuffer + this->_realbufferSize, need_read, 0);
  this->_realbufferSize += rd;
#ifndef SILENT
  std::cout << coutprefix << "Bytes read :"<< rd << std::endl;
#endif
  return rd;
}

t_packet_data* ByteBuffer::extractPacket() {
  t_packet_data* packet = (t_packet_data*)this->_realbuffer;

  if (this->_realbufferSize >= sizeof(t_packet_header) && 
      this->_realbufferSize - sizeof(t_packet_header) >= packet->packet_len) {

    int packetSize = sizeof(t_packet_header) + packet->packet_len;
    memcpy(this->_realtmpbuffer, this->_realbuffer, packetSize);
    packet = (t_packet_data*)this->_realtmpbuffer;

    this->_realbufferSize -= packetSize;

    if (this->_realbufferSize > 0) {
      // shift rest packet
      memcpy(this->_realbuffer, this->_realbuffer + packetSize, CRING_BUFFER_SIZE - packetSize);      
    }
    return packet;
  }
  return NULL;
}
}
