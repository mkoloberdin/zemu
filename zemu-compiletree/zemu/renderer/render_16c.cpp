#include "render_16c.h"
#include "../zemu.h"
#include "../devs.h"

#define LINE_SHIFT 3

unsigned long Render16c(SDL_Surface *surf, int ptch, unsigned long lastClk, unsigned long clk)
{
	unsigned long rnClk, clMod;
	int line, scrLine, pos, cl, zxLine, *scr;
	int scrA, scrB, b1, b2, b3, b4;

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

					if (dev_mman.port7FFD & 8)
					{
						scrA = RAM_BANK7;
						scrB = RAM_BANK6;
					}
					else
					{
						scrA = RAM_BANK5;
						scrB = RAM_BANK4;
					}

					b1 = dev_mman.ram[ scrB + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
					b2 = dev_mman.ram[ scrA + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
					b3 = dev_mman.ram[ scrB + 0x2000 + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];
					b4 = dev_mman.ram[ scrA + 0x2000 + ((zxLine & 0xC0) << 5) + ((zxLine & 7) << 8) + ((zxLine & 0x38) << 2) + pos ];

					// IiGRBgrb

					*(scr++) = colors[((b1 & 0x40) >> 3) | (b1 & 7)];
					*(scr++) = colors[((b1 & 0x80) >> 4) | ((b1 & 0x38) >> 3)];
					*(scr++) = colors[((b2 & 0x40) >> 3) | (b2 & 7)];
					*(scr++) = colors[((b2 & 0x80) >> 4) | ((b2 & 0x38) >> 3)];
					*(scr++) = colors[((b3 & 0x40) >> 3) | (b3 & 7)];
					*(scr++) = colors[((b3 & 0x80) >> 4) | ((b3 & 0x38) >> 3)];
					*(scr++) = colors[((b4 & 0x40) >> 3) | (b4 & 7)];
					*(scr++) = colors[((b4 & 0x80) >> 4) | ((b4 & 0x38) >> 3)];

					if (clMod <= 160) rnClk += 4;
					else rnClk++;
				}
			} else rnClk++;
		} else rnClk++;
	}

	return rnClk;
}
