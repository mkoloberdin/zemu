#ifndef _DIALOG_H_INCLUDED_
#define _DIALOG_H_INCLUDED_

#include "zemu.h"

bool DlgConfirm(const char* message);
void FileDialog(void);
void FileDialogInit(void);
void RunDebugger(void);

extern std::string oldFileName[4];

#endif
