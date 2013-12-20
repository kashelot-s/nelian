#include "ttydump_options.h"

enum ttyDump_Error ttyDumpOptions_Parse(struct ttyDumpOptions *options, 
	int argc, const char *argv[])
{
	options->device = "/dev/ttyUSB0";
	return OK;
}
