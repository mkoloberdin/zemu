#include "tape.h"
#include "tap_format.h"
#include "wav_format.h"
#include "voc_format.h"

uint64_t C_Tape::prevDevClkCounter = 0;
C_TapeFormat *C_Tape::currentFormat = nullptr;
C_SndRenderer C_Tape::sndRenderer;

void C_Tape::Init(void)
{
  AttachFrameStartHandler(OnFrameStart);
  AttachAfterFrameRenderHandler(OnAfterFrameRender);
  soundMixer.AddSource(&sndRenderer);
}

void C_Tape::Close(void)
{
  if (currentFormat != nullptr) delete currentFormat;
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

int C_Tape::getCurrBit(void)
{
  return (currentFormat == nullptr ? 1 : currentFormat->getCurrBit());
}

void C_Tape::process(void)
{
  if (currentFormat != nullptr)
  {
    if (SOUND_ENABLED)
    {
      unsigned val = (currentFormat->getCurrBit() ? MAX_TAPE_VOL : 0);
      sndRenderer.Update(devClk, val, val);
    }
  }

  if (currentFormat != nullptr) {
    if (currentFormat->processTicks(devClkCounter - prevDevClkCounter)) {
      prevDevClkCounter = devClkCounter;
    }
  }
}

bool C_Tape::isLoaded(void)
{
  return (currentFormat != nullptr);
}

bool C_Tape::isActive(void)
{
  return (currentFormat == nullptr ? false : currentFormat->isActive());
}

unsigned int C_Tape::getPosPerc(void)
{
  return (currentFormat == nullptr ? 0 : currentFormat->getPosPerc());
}

void C_Tape::eject(void)
{
  if (currentFormat != nullptr)
  {
    delete currentFormat;
    currentFormat = nullptr;
  }
}

bool C_Tape::isTapeFormat(const fs::path& fname)
{
  std::string ext = lowerCaseExtension(fname);
  if (ext == ".tap" ||
      ext == ".wav" ||
      ext == ".voc") {
    return true;
  }

  return false;
}

bool C_Tape::insert(const fs::path& fname)
{
  if (currentFormat != nullptr)
  {
    delete currentFormat;
    currentFormat = nullptr;
  }

  std::string ext = lowerCaseExtension(fname);
  
  if (ext == ".tap")
    currentFormat = new C_TapFormat();
  else if (ext == ".wav")
    currentFormat = new C_WavFormat();
  else if (ext == ".voc")
    currentFormat = new C_VocFormat();

  if (currentFormat == nullptr) return false;

  if (!currentFormat->load(fname))
  {
    delete currentFormat;
    currentFormat = nullptr;
    return false;
  }

  return true;
}

void C_Tape::start(void)
{
  if (currentFormat != nullptr)
  {
    prevDevClkCounter = devClkCounter;
    currentFormat->start();
  }
}

void C_Tape::stop(void)
{
  if (currentFormat != nullptr) {
    currentFormat->stop();
  }
}

void C_Tape::rewind(void)
{
  if (currentFormat != nullptr) {
    currentFormat->rewind();
  }
}
