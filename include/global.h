#ifndef CGLOBAL_H_
# define CGLOBAL_H_

#include <iomanip>

#define coutprefix std::left << std::setw(34) << std::setfill(' ')<< (std::string(__FILE__)+":"+std::to_string(__LINE__)+"> ")

static bool  gg_exit = false;

#define CRING_BUFFER_SIZE 512

#endif
