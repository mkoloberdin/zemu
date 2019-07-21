// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifdef __unix__

#include <string>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/soundcard.h>
#include "sound_driver_oss.h"

SoundDriverOss::SoundDriverOss(int soundFreq, int maxFragments, int sizeSelector) {
    if (maxFragments < 0 || maxFragments > 1024) {
        throw std::logic_error("\"maxFragments\" should be 0 (default value) or between 1 and 1024 inclusive");
    }

    if (sizeSelector < 0 || sizeSelector > 1024) {
        throw std::logic_error("\"sizeSelector\" should be 0 (default value) or between 1 and 1024 inclusive");
    }

    if (maxFragments == 0) {
        maxFragments = 8;
    }

    if (sizeSelector == 0) {
        sizeSelector = 10;
    }

    audioDescriptor = open("/dev/dsp", O_WRONLY, 0);

    if (audioDescriptor == -1) {
        throw std::runtime_error("Unable to open /dev/dsp for writing");
    }

    ioctlApply(SNDCTL_DSP_SETFRAGMENT, (maxFragments << 16) | sizeSelector, "Unable to set fragment size (SNDCTL_DSP_SETFRAGMENT failed)");
    ioctlApply(SNDCTL_DSP_SETFMT, AFMT_S16_NE, "Unable to set format (SNDCTL_DSP_SETFMT failed)");
    ioctlApply(SNDCTL_DSP_SAMPLESIZE, 16, "Unable to set sample size (SNDCTL_DSP_SAMPLESIZE failed)");
    ioctlApply(SNDCTL_DSP_STEREO, 1, "Unable to set stereo (SNDCTL_DSP_STEREO failed)");
    ioctlApply(SNDCTL_DSP_SPEED, soundFreq, "Unable to set frequency (SNDCTL_DSP_SPEED failed)");
}

SoundDriverOss::~SoundDriverOss() {
    if (audioDescriptor != -1) {
        close(audioDescriptor);
    }
}

void SoundDriverOss::ioctlApply(unsigned long request, int value, const char* errorMessage) {
    if (ioctl(audioDescriptor, request, &value) == -1) {
        throw std::runtime_error(errorMessage);
    }
}

void SoundDriverOss::render(uint32_t* buffer, int samples) {
    write(audioDescriptor, (uint8_t*)buffer, samples * sizeof(uint32_t));
}

#endif
