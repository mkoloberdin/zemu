#ifndef ZEMU_SND_BACKEND_OSS_H
#define ZEMU_SND_BACKEND_OSS_H

#ifndef _WIN32

#include "snd_backend.h"

const unsigned SND_FQ = 44100;

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

#endif // !_WIN32

#endif // ZEMU_SND_BACKEND_OSS_H
