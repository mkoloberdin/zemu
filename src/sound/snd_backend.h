#ifndef _SND_BACKEND_H_INCLUDED_
#define _SND_BACKEND_H_INCLUDED_

#include "../params.h"

class CSndBackend
{
	public:

	virtual void Init() = 0;
	virtual void Write(uint8_t* data, unsigned len) = 0;
	virtual ~CSndBackend() {}
};

enum eSndBackend
{
	SND_BACKEND_DEFAULT,
	SND_BACKEND_OSS,
	SND_BACKEND_WIN32
};

#endif
