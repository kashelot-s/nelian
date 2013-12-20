#ifndef __NELIAN__
#define __NELIAN__

struct Nelian {
	int fd;
};

struct NelianData {
	unsigned int value_red;
	unsigned int value_blue;
};

#define NELIAN_ERROR_TABLE \
	MSG_X(NELIAN_OK,"OK") \
	MSG_X(NELIAN_CANT_OPEN_DEVICE,"can't open device") \
	MSG_X(NELIAN_TCGETATTR_FAIL,"tcgetattr fail") \
	MSG_X(NELIAN_TCSETATTR_FAIL,"tcsetattr fail") \
	MSG_X(NELIAN_CANT_RECEIVE_HEADER,"can't receive header") \
	MSG_X(NELIAN_CANT_SELECT_CHANNEL,"can't select channel")

enum NelianError {
	#define MSG_X(a,b) a,
	NELIAN_ERROR_TABLE
	#undef MSG_X
};

void NelianData_Clear(struct NelianData *data);
enum NelianError Nelian_Init(struct Nelian *nelian, const char *device);
void Nelian_Done(struct Nelian *nelian);
enum NelianError Nelian_GetData(struct Nelian *nelian, struct NelianData *data);
char *Nelian_StrErr(enum NelianError err);

#endif