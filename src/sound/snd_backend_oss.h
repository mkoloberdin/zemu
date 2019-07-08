#ifndef _SND_BACKEND_OSS_H_INCLUDED_
#define _SND_BACKEND_OSS_H_INCLUDED_

#if !defined(_WIN32) && !defined(__APPLE__)

#include "snd_backend.h"

const unsigned SND_FQ = SOUND_FREQ;

class CSndBackendOSS : public CSndBackend {
public:
	CSndBackendOSS(unsigned frag_num, unsigned frag_size);
	~CSndBackendOSS();
	void Init(void);
	void Write(uint8_t* data, unsigned len);
private:
	int audio;
	int frag;
};

#endif // !_WIN32 && !__APPLE__

#endif
