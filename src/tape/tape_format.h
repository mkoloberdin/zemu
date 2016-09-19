#ifndef _TAPE_FORMAT_H_
#define _TAPE_FORMAT_H_

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class C_TapeFormat
{
public:

  C_TapeFormat() {}
  virtual ~C_TapeFormat() {}

  virtual bool load(const fs::path& fname) = 0;
  virtual bool processTicks(uint64_t ticks) = 0;
  virtual bool getCurrBit(void) = 0;
  virtual void stop(void) = 0;
  virtual void start(void) = 0;
  virtual void rewind(void) = 0;
  virtual unsigned int getPosPerc(void) = 0;
  virtual bool isActive(void) = 0;
};

#endif /* _TAPE_FORMAT_H_ */
