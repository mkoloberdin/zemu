#include "labels.h"
#include "file.h"
#include "exceptions.h"
#include "bin.h"
#include <stdio.h>

std::list<s_LabelItem> labels;

void Labels_Load(const char *fname)
{
	C_File fl;
	char buf[0x100];

	try
	{
		fl.Read(fname);
	}
	catch (C_E &e)
	{
		printf("Error loading labelf from \"%s\"\n", fname);
		return;
	}

	printf("Load labels \"%s\"\n", fname);

	while (!fl.Eof())
	{
		fl.GetS(buf, sizeof(buf));
		if (strlen(buf) < 9) continue;

		if (ishex(buf[0]) && ishex(buf[1]) && buf[2]==':' &&
				ishex(buf[3]) && ishex(buf[4]) && ishex(buf[5]) &&
				ishex(buf[6]) && buf[7]==' ' && buf[8]!=' ')
		{
			int bank = (unhex(buf[0]) * 0x10 + unhex(buf[1])) & BIN(0x11000111);
			int addr = unhex(buf[3]) * 0x1000 + unhex(buf[4]) * 0x100 + unhex(buf[5]) * 0x10 + unhex(buf[6]);

			switch (bank)
			{
				case 5:
					addr += 0x4000;
					break;

				case 2:
					addr += 0x8000;
					break;

				default:
					addr += 0xC000;
					break;
			}

			char *label = (buf + 8);

			if (strlen(label) > 14)
			{
				label[11] = '.';
				label[12] = '.';
				label[13] = '.';
				label[14] = 0;
			}

			s_LabelItem item;

			item.addr = addr;
			item.label.assign(label);

			labels.push_back(item);

			if (strlen(label) >= 6)
			{
				if (label[0]=='_' && label[1]=='_' &&
						label[2]=='b' && label[3]=='p' &&
						label[4]=='_' && label[5]=='_')
				{
					printf("Set breakpoint on 0x%04X bank %02X\n", addr, bank);
					breakpoints[addr] = true;
				}
			}
		}
	}

	fl.Close();
}
