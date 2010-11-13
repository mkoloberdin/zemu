#ifndef _DIALOG_H_INCLUDED_
#define _DIALOG_H_INCLUDED_ 1

#include "zemu.h"

bool DlgConfirm(const char *message);
void FileDialog(void);
void FileDialogInit(void);
void RunDebugger(void);

extern char oldFileName[4][MAX_PATH];

#endif
