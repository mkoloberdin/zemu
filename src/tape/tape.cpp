#include "tape.h"
#include "../dirwork.h"
#include "tap_format.h"
#include "wav_format.h"
#include "voc_format.h"

uint64_t C_Tape::prevDevClkCounter = 0;
C_TapeFormat *C_Tape::currentFormat = NULL;
C_SndRenderer C_Tape::sndRenderer;

void C_Tape::Init(void)
{
	AttachFrameStartHandler(OnFrameStart);
	AttachAfterFrameRenderHandler(OnAfterFrameRender);
	SoundMixer.AddSource(&sndRenderer);
}

void C_Tape::Close(void)
{
	if (currentFormat != NULL) delete currentFormat;
}

void C_Tape::OnFrameStart(void)
{
	if (SOUND_ENABLED) {
		sndRenderer.StartFrame();
	}
}

void C_Tape::OnAfterFrameRender(void)
{
	if (SOUND_ENABLED) {
		sndRenderer.EndFrame(lastDevClk);
	}
}

int C_Tape::GetCurrBit(void)
{
	return (currentFormat == NULL ? 1 : currentFormat->GetCurrBit());
}

void C_Tape::Process(void)
{
	if (currentFormat != NULL)
	{
		if (SOUND_ENABLED)
		{
			unsigned val = (currentFormat->GetCurrBit() ? MAX_TAPE_VOL : 0);
			sndRenderer.Update(devClk, val, val);
		}
	}

	if (currentFormat != NULL) {
		if (currentFormat->ProcessTicks(devClkCounter - prevDevClkCounter)) {
			prevDevClkCounter = devClkCounter;
		}
	}
}

bool C_Tape::IsLoaded(void)
{
	return (currentFormat != NULL);
}

bool C_Tape::IsActive(void)
{
	return (currentFormat == NULL ? false : currentFormat->IsActive());
}

unsigned int C_Tape::GetPosPerc(void)
{
	return (currentFormat == NULL ? 0 : currentFormat->GetPosPerc());
}

void C_Tape::Eject(void)
{
	if (currentFormat != NULL)
	{
		delete currentFormat;
		currentFormat = NULL;
	}
}

bool C_Tape::IsTapeFormat(const char *fname)
{
	if (!stricmp(C_DirWork::ExtractExt(fname), "tap")) return true;
	if (!stricmp(C_DirWork::ExtractExt(fname), "wav")) return true;
	if (!stricmp(C_DirWork::ExtractExt(fname), "voc")) return true;

	return false;
}

bool C_Tape::Insert(const char *fname)
{
	if (currentFormat != NULL)
	{
		delete currentFormat;
		currentFormat = NULL;
	}

	if (!stricmp(C_DirWork::ExtractExt(fname), "tap")) {
		currentFormat = new C_TapFormat();
	} else if (!stricmp(C_DirWork::ExtractExt(fname), "wav")) {
		currentFormat = new C_WavFormat();
	} else if (!stricmp(C_DirWork::ExtractExt(fname), "voc")) {
		currentFormat = new C_VocFormat();
	}

	if (currentFormat == NULL) return false;

	if (!currentFormat->Load(fname))
	{
		delete currentFormat;
		currentFormat = NULL;
		return false;
	}

	return true;
}

void C_Tape::Start(void)
{
	if (currentFormat != NULL)
	{
		prevDevClkCounter = devClkCounter;
		currentFormat->Start();
	}
}

void C_Tape::Stop(void)
{
	if (currentFormat != NULL) {
		currentFormat->Stop();
	}
}

void C_Tape::Rewind(void)
{
	if (currentFormat != NULL) {
		currentFormat->Rewind();
	}
}
