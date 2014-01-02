#include <string.h>
#include "ttydump_options.h"

enum ttyDump_Error ttyDumpOptions_Parse(struct ttyDumpOptions *options, 
	int argc, const char *argv[])
{
	memset(options, 0, sizeof(struct ttyDumpOptions));
	options->device = "/dev/ttyUSB0";
	options->dump_enable = true;
	options->dump_name = "nelian.raw_data";
	options->x_count = 1000;
	return OK;
}
