#include "covox.h"

C_SndRenderer C_Covox::sndRenderer;
bool C_Covox::enabled = false;

void C_Covox::Init(void)
{
	if (!config.GetBool("root/EnableCovox", &enabled)) enabled = false;

	if (enabled)
	{
		AttachZ80OutputHandler(OutputByteCheckPort, OnOutputByte);

		AttachFrameStartHandler(OnFrameStart);
		AttachAfterFrameRenderHandler(OnAfterFrameRender);

		RegisterSndRenderer(&sndRenderer);
	}
}

void C_Covox::Close(void)
{
}

bool C_Covox::OutputByteCheckPort(Z80EX_WORD port)
{
	return ((port & 0x07) == 0x03);
}

bool C_Covox::OnOutputByte(Z80EX_WORD port, Z80EX_BYTE value)
{
	if (SOUND_ENABLED)
	{
		unsigned vol = ((unsigned)value << 7);
		sndRenderer.Update(devClk, vol, vol);
	}

	return true;
}

void C_Covox::OnFrameStart(void)
{
	if (SOUND_ENABLED) {
		sndRenderer.StartFrame();
	}
}

void C_Covox::OnAfterFrameRender(void)
{
	if (SOUND_ENABLED) {
		sndRenderer.EndFrame(lastDevClk);
	}
}
