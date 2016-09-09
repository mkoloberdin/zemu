#ifdef _WIN32

#include "../defines.h"
#include "snd_backend_win32.h"

CSndBackendWIN32::CSndBackendWIN32(unsigned queue_size, unsigned frag_size) {
  if (queue_size < 1) queue_size = 1;
  if (queue_size > MAXWQSIZE) queue_size = MAXWQSIZE;
  WQSIZE = queue_size;
  hwo = 0;
}

void CSndBackendWIN32::Init() {
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

void CSndBackendWIN32::Write(uint8_t *data, unsigned len) {
  for (;;)
  {
    while ((wqtail != wqhead) && (wq[wqtail].dwFlags & WHDR_DONE))
    {
      waveOutUnprepareHeader(hwo, &wq[wqtail], sizeof(WAVEHDR));
      if (++wqtail == WQSIZE) wqtail = 0;
    }

    if ((wqhead + 1) % WQSIZE != wqtail) break;
    Sleep(10);
  }

  LPSTR bfpos = (LPSTR)(wbuffer + wqhead * MAXDSPIECE);
  memcpy(bfpos, data, len);

  wq[wqhead].lpData = bfpos;
  wq[wqhead].dwBufferLength = len;
  wq[wqhead].dwFlags = 0;

  waveOutPrepareHeader(hwo, &wq[wqhead], sizeof(WAVEHDR));
  waveOutWrite(hwo, &wq[wqhead], sizeof(WAVEHDR));

  if (++wqhead == WQSIZE) wqhead = 0;
}

CSndBackendWIN32::~CSndBackendWIN32() {
  if (hwo) {
    waveOutReset(hwo);
    waveOutClose(hwo);
  }
}

#endif // _WIN32
