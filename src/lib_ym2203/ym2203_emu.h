#include "sound/snd_renderer.h"

void* YM2203Init(int baseclock, int rate);
void YM2203Shutdown(void* chip);
void YM2203ResetChip(void* chip);
void YM2203UpdateOne(void* chip, C_SndRenderer* sndr, unsigned devClk);
int YM2203Write(void* chip, int a, unsigned char v);
void YM2203SetMute(void* chip, int mask);
void YM2203GetAllTL(void* chip, int* levels);
unsigned char YM2203Read(void* chip, int a);
