#ifndef _SND_BACKEND_WIN32_H_INCLUDED_
#define _SND_BACKEND_WIN32_H_INCLUDED_

#ifdef _WIN32

#include "snd_backend.h"

const unsigned SND_FQ = SOUND_FREQ;

const unsigned MAXWQSIZE = 32;
const unsigned MAXDSPIECE = (40000*4/20);

class CSndBackendWIN32 : public CSndBackend {
public:
	CSndBackendWIN32(unsigned queue_size, unsigned frag_size);
	~CSndBackendWIN32();
	void Init(void);
	void Write(uint8_t* data, unsigned len);
private:
	unsigned WQSIZE;
	HWAVEOUT hwo;
	WAVEHDR wq[MAXWQSIZE];
	uint8_t wbuffer[MAXWQSIZE*MAXDSPIECE];
	unsigned wqhead, wqtail;
};

#endif // !_WIN32

#endif
