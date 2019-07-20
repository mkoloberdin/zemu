// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
#ifdef __unix__

#include "defines.h"
#include "snd_backend_oss.h"

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/soundcard.h>
#include <cmath>

CSndBackendOSS::CSndBackendOSS(unsigned frag_num, unsigned frag_size) {
    // int frag = (max_fragments << 16) | (size_selector);
    // ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &frag);

    frag = (frag_num << 16) | (int)ceil(std::log2(frag_size));
}

void CSndBackendOSS::Init() {
    audio = open("/dev/dsp", O_WRONLY, 0);

    if (audio == -1) {
        StrikeError("Unable to open /dev/dsp for writing.");
    }

    if (ioctl(audio, SNDCTL_DSP_SETFRAGMENT, &frag) == -1) {
        close(audio);
        StrikeError("Unable to set audio fragment size.");
    }

    int tmp = AFMT_S16_NE;

    if (ioctl(audio, SNDCTL_DSP_SETFMT, &tmp) == -1) {
        close(audio);
        StrikeError("setting SNDCTL_DSP_SETFMT on audiodev failed.");
    }

    tmp = 16;

    if (ioctl(audio, SNDCTL_DSP_SAMPLESIZE, &tmp) == -1) {
        // TODO: close(audio)?
        StrikeError("Unable set samplesize = %d.", tmp);
    }

    // TODO: we really need this check?
    if (tmp != 16) {
        // TODO: close(audio)?
        StrikeError("Unable set samplesize = %d.", tmp);
    }

    tmp = 1;

    if (ioctl(audio, SNDCTL_DSP_STEREO, &tmp) == -1) {
        close(audio);
        StrikeError("Unable to set stereo.");
    }

    tmp = SND_FQ;

    if (ioctl(audio, SNDCTL_DSP_SPEED, &tmp) == -1) {
        close(audio);
        StrikeError("Unable to set audio speed = %d.", SND_FQ);
    }
}

void CSndBackendOSS::Write(uint8_t* buf, unsigned spbsize) {
    if (write(audio, buf, spbsize) != (int)spbsize) {
        printf("Write to soundcard device failed\n");
    }
}

CSndBackendOSS::~CSndBackendOSS() {
    close(audio);
}

#endif
*/
