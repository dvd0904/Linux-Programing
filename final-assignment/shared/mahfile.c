#include "shared.h"

int IsFile(const char *file)
{
    struct stat buf;
	return (!stat(file, &buf) && S_ISREG(buf.st_mode)) ? 0 : -1;
}