#ifndef ZEMU_SND_BACKEND_H
#define ZEMU_SND_BACKEND_H

class CSndBackend {
public:
	virtual void Init() = 0;
	virtual void Write(uint8_t* data, unsigned len) = 0;
	virtual ~CSndBackend() {}
};

enum eSndBackend {
	SND_BACKEND_SDL,
	SND_BACKEND_OSS,
	SND_BACKEND_WIN32
};

#endif // ZEMU_SND_BACKEND_H
