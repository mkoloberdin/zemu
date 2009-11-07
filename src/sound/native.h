#ifndef _NATIVE_H_INCLUDED_
#define _NATIVE_H_INCLUDED_

const unsigned SND_FQ = 44100;

void wav_start(unsigned frag_num, unsigned frag_size);
void wav_play(uint8_t *buf, unsigned spbsize);
void wav_stop();

#endif
