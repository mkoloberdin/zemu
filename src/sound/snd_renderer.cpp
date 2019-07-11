// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//////////////////////////////////////////////////////////////////////
//
// Based on sound resampling core for Unreal Speccy project
// created under public domain license by SMT, jan.2006
//
//////////////////////////////////////////////////////////////////////

#include "snd_renderer.h"
#include "../zemu.h"

const unsigned TICK_FF = 6;
const unsigned TICK_F = (1 << TICK_FF);
const unsigned MULT_C = 12;

const double filterCoeff[TICK_F * 2] = {
   // filter designed with Matlab's DSP toolbox
   0.000797243121022152, 0.000815206499600866, 0.000844792477531490, 0.000886460636664257,
   0.000940630171246217, 0.001007677515787512, 0.001087934129054332, 0.001181684445143001,
   0.001289164001921830, 0.001410557756409498, 0.001545998595893740, 0.001695566052785407,
   0.001859285230354019, 0.002037125945605404, 0.002229002094643918, 0.002434771244914945,
   0.002654234457752337, 0.002887136343664226, 0.003133165351783907, 0.003391954293894633,
   0.003663081102412781, 0.003946069820687711, 0.004240391822953223, 0.004545467260249598,
   0.004860666727631453, 0.005185313146989532, 0.005518683858848785, 0.005860012915564928,
   0.006208493567431684, 0.006563280932335042, 0.006923494838753613, 0.007288222831108771,
   0.007656523325719262, 0.008027428904915214, 0.008399949736219575, 0.008773077102914008,
   0.009145787031773989, 0.009517044003286715, 0.009885804729257883, 0.010251021982371376,
   0.010611648461991030, 0.010966640680287394, 0.011314962852635887, 0.011655590776166550,
   0.011987515680350414, 0.012309748033583185, 0.012621321289873522, 0.012921295559959939,
   0.013208761191466523, 0.013482842243062109, 0.013742699838008606, 0.013987535382970279,
   0.014216593638504731, 0.014429165628265581, 0.014624591374614174, 0.014802262449059521,
   0.014961624326719471, 0.015102178534818147, 0.015223484586101132, 0.015325161688957322,
   0.015406890226980602, 0.015468413001680802, 0.015509536233058410, 0.015530130313785910,
   0.015530130313785910, 0.015509536233058410, 0.015468413001680802, 0.015406890226980602,
   0.015325161688957322, 0.015223484586101132, 0.015102178534818147, 0.014961624326719471,
   0.014802262449059521, 0.014624591374614174, 0.014429165628265581, 0.014216593638504731,
   0.013987535382970279, 0.013742699838008606, 0.013482842243062109, 0.013208761191466523,
   0.012921295559959939, 0.012621321289873522, 0.012309748033583185, 0.011987515680350414,
   0.011655590776166550, 0.011314962852635887, 0.010966640680287394, 0.010611648461991030,
   0.010251021982371376, 0.009885804729257883, 0.009517044003286715, 0.009145787031773989,
   0.008773077102914008, 0.008399949736219575, 0.008027428904915214, 0.007656523325719262,
   0.007288222831108771, 0.006923494838753613, 0.006563280932335042, 0.006208493567431684,
   0.005860012915564928, 0.005518683858848785, 0.005185313146989532, 0.004860666727631453,
   0.004545467260249598, 0.004240391822953223, 0.003946069820687711, 0.003663081102412781,
   0.003391954293894633, 0.003133165351783907, 0.002887136343664226, 0.002654234457752337,
   0.002434771244914945, 0.002229002094643918, 0.002037125945605404, 0.001859285230354019,
   0.001695566052785407, 0.001545998595893740, 0.001410557756409498, 0.001289164001921830,
   0.001181684445143001, 0.001087934129054332, 0.001007677515787512, 0.000940630171246217,
   0.000886460636664257, 0.000844792477531490, 0.000815206499600866, 0.000797243121022152
};

static unsigned filterDiff[TICK_F * 2];
const double filterSumFull = 1.0;
const double filterSumHalf = 0.5;
const unsigned filterSumFullU = (unsigned)(filterSumFull * 0x10000);
const unsigned filterSumHalfU = (unsigned)(filterSumHalf * 0x10000);

C_SndRenderer::C_SndRenderer() {
    static bool diffReady = false;

    samples = 0;
    mixL = 0;
    mixR = 0;
    SetTimings(SNDR_DEFAULT_SYSTICK_RATE, SNDR_DEFAULT_SAMPLE_RATE);

    if (!diffReady) {
        double sum = 0;

        for (unsigned i = 0; i < TICK_F * 2; i++) {
            filterDiff[i] = (int)(sum * 0x10000);
            sum += filterCoeff[i];
        }

        diffReady = true;
    }
}

void C_SndRenderer::SetTimings(unsigned clockRate, unsigned sampleRate) {
    this->sampleRate = sampleRate;
    this->clockRate = clockRate;

    tick = 0;
    startPos = nullptr;
    currPos = 0;
    passedSndTicks = 0;
    passedClkTicks = 0;

    this->multConst = (unsigned)(((uint64_t)sampleRate << (MULT_C + TICK_FF)) / clockRate);
}

void C_SndRenderer::StartFrame() {
    if (samples > (MIX_BUFFER_SIZE / 2)) {
        printf("Sound buffer overflow\n");
        samples = 0;
    }

    startPos = &mixBuffer[samples];
    currPos = startPos;
    baseTick = tick;
}

void C_SndRenderer::Update(unsigned clk, unsigned left, unsigned right) {
    if (!((left ^ mixL) | (right ^ mixR))) {
        return;
    }

    activeCnt = SNDR_ACTIVE_CNT_UPD;

    unsigned endTick = (clk * multConst) >> MULT_C;
    Flush(baseTick + endTick);

    mixL = left;
    mixR = right;
}

void C_SndRenderer::EndFrame(unsigned clk) {
    unsigned prevMixL = mixL;
    unsigned prevMixR = mixR;

    // don't use "((passedClkTicks + clk) * multConst) >> MULT_C", because it loose precision
    uint64_t endTick = ((passedClkTicks + clk) * (uint64_t)sampleRate * TICK_F) / clockRate;

    if (!activeCnt) {
        mixL = 0;
        mixR = 0;
    }

    Flush((unsigned)(endTick - passedSndTicks));

    if (!activeCnt) {
        mixL = prevMixL;
        mixR = prevMixR;
    }

    samples += currPos - startPos;

    tick -= (samples << TICK_FF);
    passedSndTicks += (samples << TICK_FF);
    passedClkTicks += clk;
}

void C_SndRenderer::Flush(unsigned endTick) {
    unsigned scale;

    if (!((endTick ^ tick) & ~(TICK_F - 1))) {
        scale = filterDiff[(endTick & (TICK_F - 1)) + TICK_F] - filterDiff[(tick & (TICK_F - 1)) + TICK_F];
        s2l += mixL * scale;
        s2r += mixR * scale;

        scale = filterDiff[endTick & (TICK_F - 1)] - filterDiff[tick & (TICK_F - 1)];
        s1l += mixL * scale;
        s1r += mixR * scale;

        tick = endTick;
    } else {
        scale = filterSumFullU - filterDiff[(tick & (TICK_F - 1)) + TICK_F];

        currPos->left += (mixL * scale + s2l) >> 16;
        currPos->right += (mixR * scale + s2r) >> 16;
        currPos++;

        scale = filterSumHalfU - filterDiff[tick & (TICK_F - 1)];
        s2l = s1l + mixL * scale;
        s2r = s1r + mixR * scale;

        tick = (tick | (TICK_F - 1)) + 1;

        if ((endTick ^ tick) & ~(TICK_F - 1)) {
            unsigned valL = mixL * filterSumHalfU;
            unsigned valR = mixR * filterSumHalfU;

            do {
                currPos->left += (s2l + valL) >> 16;
                currPos->right += (s2r + valR) >> 16;
                currPos++;

                tick += TICK_F;
                s2l = valL;
                s2r = valR;
            } while ((endTick ^ tick) & ~(TICK_F - 1));
        }

        tick = endTick;

        scale = filterDiff[(endTick & (TICK_F - 1)) + TICK_F] - filterSumHalfU;
        s2l += mixL * scale;
        s2r += mixR * scale;

        scale = filterDiff[endTick & (TICK_F - 1)];
        s1l = mixL * scale;
        s1r = mixR * scale;
    }
}
