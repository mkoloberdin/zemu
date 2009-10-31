#include "../defines.h"
#include "native.h"

#ifdef OS_LINUX

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/soundcard.h>
#include <math.h>

int audio;

void wav_start(unsigned frag_num, unsigned frag_size)
{
	int tmp;

	audio = open("/dev/dsp", O_WRONLY, 0);
	if (audio == -1) StrikeError("Unable to open /dev/dsp for writing.");

	tmp = (frag_num << 16) | (int)ceil(log2(frag_size));

	if (ioctl(audio, SNDCTL_DSP_SETFRAGMENT, &tmp) == -1) {close(audio); StrikeError("Unable to set audio fragment size.");}

	tmp = AFMT_S16_NE;
	if (ioctl(audio,SNDCTL_DSP_SETFMT,&tmp) == -1) {close(audio); StrikeError("setting SNDCTL_DSP_SETFMT on audiodev failed.");}

	tmp = 16;
	ioctl(audio, SNDCTL_DSP_SAMPLESIZE, &tmp);
	if (tmp != 16) StrikeError("Unable set samplesize = %d.", tmp);

	tmp = 1;
	if (ioctl(audio, SNDCTL_DSP_STEREO, &tmp) == -1) {close(audio); StrikeError("Unable to set stereo.");}

	tmp = SND_FQ;
	if (ioctl (audio, SNDCTL_DSP_SPEED, &tmp) == -1) {close(audio); StrikeError("Unable to set audio speed = %d.",SND_FQ);}
}

void wav_play(BYTE *buf, unsigned spbsize)
{
	if (write(audio, buf, spbsize) != (int)spbsize) printf("Write to soundcard device failed\n");
}

void wav_stop()
{
	close(audio);
}

#else

unsigned WQSIZE = 4;

const unsigned MAXWQSIZE = 32;
const unsigned MAXDSPIECE = (40000*4/20);

HWAVEOUT hwo = 0;
WAVEHDR wq[MAXWQSIZE];
unsigned char wbuffer[MAXWQSIZE*MAXDSPIECE];
unsigned wqhead, wqtail;

void wav_start(unsigned queue_size, unsigned frag_size)
{
	if (queue_size < 1) queue_size = 1;
	if (queue_size > MAXWQSIZE) queue_size = MAXWQSIZE;
	WQSIZE = queue_size;

	WAVEFORMATEX wf = { 0 };
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nSamplesPerSec = SND_FQ;
	wf.nChannels = 2;
	wf.wBitsPerSample = 16;
	wf.nBlockAlign = 4;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

	waveOutOpen(&hwo, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL);
	wqhead = 0, wqtail = 0;
}

void wav_play(BYTE *buf, unsigned spbsize)
{
	for (;;)
	{
		while ((wqtail != wqhead) && (wq[wqtail].dwFlags & WHDR_DONE))
		{
			waveOutUnprepareHeader(hwo, &wq[wqtail], sizeof(WAVEHDR));
			if (++wqtail == WQSIZE) wqtail = 0;
		}

		if ((wqhead+1) % WQSIZE != wqtail) break;
		Sleep(10);
	}

	LPSTR bfpos = (LPSTR)(wbuffer + wqhead*MAXDSPIECE);
	memcpy(bfpos, buf, spbsize);

	wq[wqhead].lpData = bfpos;
	wq[wqhead].dwBufferLength = spbsize;
	wq[wqhead].dwFlags = 0;

	waveOutPrepareHeader(hwo, &wq[wqhead], sizeof(WAVEHDR));
	waveOutWrite(hwo, &wq[wqhead], sizeof(WAVEHDR));

	if (++wqhead == WQSIZE) wqhead = 0;
}

void wav_stop()
{
	waveOutReset(hwo);
	waveOutClose(hwo);
}

#endif
