#ifndef __TTYDUMP_OPTIONS__
#define __TTYDUMP_OPTIONS__

#include <stdbool.h>
#include "ttydump_error.h"

struct ttyDumpOptions {
	char *device;
	bool dump_enable;
	char *dump_name;
	int x_count;
};

enum ttyDump_Error ttyDumpOptions_Parse(struct ttyDumpOptions *options,
	int argc, const char *argv[]);

#endif
