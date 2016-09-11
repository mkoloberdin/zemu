#include "border.h"

uint8_t C_Border::portFB;
C_SndRenderer C_Border::sndRenderer;

void C_Border::Init(void)
{
  AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);

  AttachFrameStartHandler(OnFrameStart);				// [boo]
  AttachAfterFrameRenderHandler(OnAfterFrameRender);	// [boo]

  soundMixer.AddSource(&sndRenderer);
  portFB = 0;
}

void C_Border::Close(void)
{
}

bool C_Border::OutputByteCheckPort(uint16_t port)
{
  return (!(port & 1));
}

bool C_Border::OnOutputByte(uint16_t port, uint8_t value)
{
  if (SOUND_ENABLED)
  {
    unsigned vol = 0;

    if (value & 0x10) vol += MAX_SPK_VOL;
    if (value & 8) vol += MAX_TAPE_SAVE_VOL;

    sndRenderer.Update(devClk, vol, vol);
  }

  if (renderPtr && ((portFB & 7) != (value & 7))) {
    renderPtr(cpuClk + z80ex_op_tstate(cpu));
  }

  portFB = value;
  return true;
}

// [boo]
void C_Border::OnFrameStart(void)
{
  if (SOUND_ENABLED) {
    sndRenderer.StartFrame();
  }
}

void C_Border::OnAfterFrameRender(void)
{
  if (SOUND_ENABLED) {
    sndRenderer.EndFrame(lastDevClk);
  }
}
// [/boo]
