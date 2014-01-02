#ifndef __NELIAN__
#define __NELIAN__

#include <stdbool.h>
#include <stdio.h>

#define NELIAN_ERROR_TABLE \
	MSG_X(NELIAN_OK,"OK") \
	MSG_X(NELIAN_CANT_OPEN_DEVICE,"can't open device") \
	MSG_X(NELIAN_TCGETATTR_FAIL,"tcgetattr fail") \
	MSG_X(NELIAN_TCSETATTR_FAIL,"tcsetattr fail") \
	MSG_X(NELIAN_CANT_RECEIVE_HEADER,"can't receive header") \
	MSG_X(NELIAN_CANT_SELECT_CHANNEL,"can't select channel")

struct Nelian {
	int fd;
	bool dump_enable;
	FILE *dump_fd;
	unsigned int received_bytes;
};

struct NelianData {
	unsigned int value_red;
	unsigned int value_blue;
};

enum NelianError {
	#define MSG_X(a,b) a,
	NELIAN_ERROR_TABLE
	#undef MSG_X
};

void NelianData_Clear(struct NelianData *data);
enum NelianError Nelian_Init(struct Nelian *nelian, const char *device,
	const char *dump_name, bool dump_enable);
void Nelian_Done(struct Nelian *nelian);
enum NelianError Nelian_GetData(struct Nelian *nelian, struct NelianData *data);
char *Nelian_StrErr(enum NelianError err);

#endif
