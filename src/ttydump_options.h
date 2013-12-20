#ifndef __TTYDUMP_OPTIONS__
#define __TTYDUMP_OPTIONS__

#include "ttydump_error.h"

struct ttyDumpOptions {
	char *device;
};

enum ttyDump_Error ttyDumpOptions_Parse(struct ttyDumpOptions *options,
	int argc, const char *argv[]);

#endif