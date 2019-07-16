// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <string>
#include "zemu_env.h"
#include "labels.h"

std::list<s_LabelItem> labels;

void Labels_Load(const char* fname) {
    FileReaderPtr reader;

    try {
        reader = hostEnv->fileSystem()->path(fname)->fileReader();
    } catch (...) {
        printf("Error loading labels from \"%s\"\n", fname);
        return;
    }

    printf("Load labels \"%s\"\n", fname);

    while (!reader->isEof()) {
        std::string line = reader->readLine();

        if (line.length() < 9) {
            continue;
        }

        if (ishex(line[0])
            && ishex(line[1])
            && line[2] == ':'
            && ishex(line[3])
            && ishex(line[4])
            && ishex(line[5])
            && ishex(line[6])
            && line[7] == ' '
            && line[8] != ' '
        ) {
            int bank = (unhex(line[0]) * 0x10 + unhex(line[1])) & 0b11000111;
            int addr = unhex(line[3]) * 0x1000 + unhex(line[4]) * 0x100 + unhex(line[5]) * 0x10 + unhex(line[6]);

            switch (bank) {
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

            std::string label = line.substr(8);

            if (label.length() >= 6) {
                for (int i = 0, len = label.length() - 6; i <= len; i++) {
                    if (label[i] == '_'
                        && label[i + 1] == '_'
                        && label[i + 2] == 'b'
                        && label[i + 3] == 'p'
                        && label[i + 4] == '_'
                        && label[i + 5] == '_'
                    ) {
                        printf("Set breakpoint on 0x%04X bank %02X\n", addr, bank);
                        breakpoints[addr] = true;
                        break;
                    }
                }
            }

            if (label.length() > 14) {
                label = label.substr(0, 11) + "...";
            }

            s_LabelItem item;
            item.addr = addr;
            item.label.assign(label);

            labels.push_back(item);
        }
    }
}
