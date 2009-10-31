#include "render_multicolor.h"
#include "../zemu.h"
#include "../devs.h"

#define LINE_SHIFT 3

unsigned long RenderMulticolor(SDL_Surface *surf, int ptch, unsigned long lastClk, unsigned long clk)
{
	unsigned long rnClk, clMod;
	int line, scrLine, pos, cl;
	int *scr;
	int zxLine, zxScreen, ci, cp, bt;

	for (rnClk = lastClk; rnClk < clk;)
	{
		line = rnClk / 224;

		if (line>=40 && line<280)	// in 320x240 screen (vertical)
		{
			scrLine = line - 40;
			clMod = rnClk % 224;

			if (clMod>=(20-LINE_SHIFT) && clMod<(180-LINE_SHIFT))	// in 320x240 screen (horizontal)
			{
				if (line<64 || line>=256 || clMod<(36-LINE_SHIFT) || clMod>=(164-LINE_SHIFT))	// border
				{
					cl = colors[dev_border.portFB & 7];
					pos = (clMod - (20-LINE_SHIFT)) * 2;
					scr = (int *)surf->pixels + ptch*scrLine + pos;
					*(scr++) = cl;
					*(scr) = cl;
					rnClk++;
				}
				else
				{
					pos = (clMod - (36-LINE_SHIFT)) / 4;
					zxLine = line - 64;
					scr = (int *)surf->pixels + ptch*scrLine + 32 + pos*8;

					zxScreen = (dev_mman.port7FFD & 8) ? RAM_BANK7 : RAM_BANK5;
					bt = dev_mman.ram[ zxScreen + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
					cl = dev_mman.ram[ zxScreen + 0x2000 + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];

					if ((frames&32) && (cl&128))
					{
						cp = colors[((cl & 64) >> 3) | (cl & 7)];
						ci = colors[((cl & 64) >> 3) | ((cl >> 3) & 7)];
					}
					else
					{
						ci = colors[((cl & 64) >> 3) | (cl & 7)];
						cp = colors[((cl & 64) >> 3) | ((cl >> 3) & 7)];
					}

					if (attributesHack)
					{
						ci = DRGB(0,0,0);
						cp = DRGB(255,255,255);
					}

					*(scr++) = (bt & 128 ? ci : cp);
					*(scr++) = (bt &  64 ? ci : cp);
					*(scr++) = (bt &  32 ? ci : cp);
					*(scr++) = (bt &  16 ? ci : cp);
					*(scr++) = (bt &   8 ? ci : cp);
					*(scr++) = (bt &   4 ? ci : cp);
					*(scr++) = (bt &   2 ? ci : cp);
					*(scr)   = (bt &   1 ? ci : cp);

					if (clMod <= 160) rnClk += 4;
					else rnClk++;
				}
			} else rnClk++;
		} else rnClk++;
	}

	return rnClk;
}
