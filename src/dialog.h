#ifndef _DIALOG_H_INCLUDED_
#define _DIALOG_H_INCLUDED_ 1

#include <defines.h>

bool DlgConfirm(const char *message);
void FileDialog(void);
void fileDialogInit(void);
void RunDebugger(void);

extern std::vector<fs::path> OldFileName;

#endif
