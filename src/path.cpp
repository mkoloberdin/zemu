#include "path.h"

#ifdef __WIN32__

#include <shlwapi.h>
#define PATH_SEP '\\'
#define PATH_SEP_S "\\"

#else

#define PATH_SEP '/'
#define PATH_SEP_S "/"

#endif

string path_append(const string &p1, const string &p2) {
	if (p1.empty()) {
		return p2;
	} else if (p2.empty()) {
		return p1;
	}
#ifdef __WIN32__
	TCHAR buffer[MAX_PATH];
	strcpy(buffer, p1.c_str());
	PathAppend(buffer, p2.c_str());
	return string(buffer);
#else
	size_t offset = 0;
	while(!p2.empty() > 0 && offset < p2.size() && p2[offset] == '/') {
		offset++;
	}
	if (p1[p1.size() - 1] == '/') {
		return p1 + p2.substr(offset);
	} else {
		return p1 + '/' + p2.substr(offset);
	}
#endif
}
