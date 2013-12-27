#ifndef __TTYDUMP_ERRORS__
#define __TTYDUMP_ERRORS__

#define TTYDUMP_ERRORS_TABLE \
	ERR_X(OK,"OK") \
	ERR_X(ERR_OPTIONS,"parsing options fail") \
	ERR_X(ERR_NELIAN,"nelian hardware error") \
	ERR_X(ERR_NELIAN_INIT,"can't init hardware") \
	ERR_X(ERR_CBUFFER_FAIL,"curcular buffer fail")

enum ttyDump_Error {
	#define ERR_X(a,b) a,
	TTYDUMP_ERRORS_TABLE
	#undef ERR_X
};

#endif
