#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <exception>

class SerializableBufferException : public std::exception
{
  public:
  SerializableBufferException() {}
  virtual ~SerializableBufferException() throw() {};
  virtual const char* what() const throw()
  {
    return ("SerializableBuffer exception");
  }
};

class SerializableBuffer
{
  public:
  SerializableBuffer()
  {
    this->_size = 0;
  }

  SerializableBuffer(const char *data, size_t size)
  {
    this->_stream.write(data, size);
    this->_size = size;
  }

  ~SerializableBuffer(void){}

  public:
  // scalar read
  template <typename T>
  T readToData()
  {
    if (this->_size < sizeof(T))
      throw SerializableBufferException();
    T extractor;

    this->_stream.read(reinterpret_cast<char*>(&extractor), sizeof(T));
    this->_size -= sizeof(T);

    return (extractor);
  }

  // data read
  std::string readToData(size_t size)
  {
    if (this->_size < size)
      throw SerializableBufferException();
    std::string str;
    try{
      str.resize(size);
    } catch (std::bad_alloc&){
      throw SerializableBufferException();
    }
    this->_stream.read(&str[0], size);
    this->_size -= size;
    return (str);
  }

  // scalar write
  template <typename T>
  void writeToData(const T i)
  {
    this->_stream.write(reinterpret_cast<const char*>(&i), sizeof(T));
    this->_size += sizeof(T);
  }

  // data write
  void writeToData(char *cstr, size_t size)
  {
    this->_stream.write(cstr, size);
    this->_size += size;
  }

  // data write
  void writeToData(const char *cstr, size_t size)
  {
    this->_stream.write(cstr, size);
    this->_size += size;
  }

  void writeToData(const std::string &str, size_t size)
  {
    this->_stream.write(str.c_str(), size);
    this->_size += size;
  }

  size_t size()
  {
    return (this->_size);
  }

  void copyToBuffer(char *buffer) {
    this->_stream.rdbuf()->sgetn(buffer, this->_size);
  }

private:
  std::stringstream _stream;
  size_t _size;
};
