#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h> 
#include <termios.h> 
#include <unistd.h>
#include "nelian.h"

//  Protocol:
// 5500     - 2 bytes
// value 0  - 2 bytes
// 5520     - 2 bytes
// value 1  - 2 bytes

#define NELIAN_HEAD 0x55
#define NELIAN_CHANNEL_BIT 5

char *nelian_error_strings[] = {
	#define MSG_X(a,b) b,
	NELIAN_ERROR_TABLE
	#undef MSG_X
};

static unsigned char reverse_byte(unsigned char in)
{
	unsigned char out = 0, in_bit;
	int i;

	for (i = 0; i < 8; i++) {
		in_bit = (in >> i) & 0x01;
		out |= in_bit << (8 - i);
	}
	return out;
}

static void Nelian_DumpByte(struct Nelian *nelian, unsigned char buffer)
{
	fprintf(nelian->dump_fd, "%02X ", buffer);
	if (0 == nelian->received_bytes % 16)
		fprintf(nelian->dump_fd, "\n");
	fflush(nelian->dump_fd);
}

static unsigned char Nelian_ReadByte(struct Nelian *nelian)
{
	unsigned char buffer;
	read(nelian->fd, &buffer, 1);
	nelian->received_bytes += 1;
	if (nelian->dump_enable)
		Nelian_DumpByte(nelian, buffer);
	return buffer;
}

static unsigned int Nelian_ReadWord(struct Nelian *nelian)
{
	unsigned char byte1, byte2;
	unsigned int value;

	//byte1 = reverse_byte(Nelian_ReadByte(nelian));
	//byte2 = reverse_byte(Nelian_ReadByte(nelian));
	byte1 = Nelian_ReadByte(nelian);
	byte2 = Nelian_ReadByte(nelian);
	value = byte2 << 8 | byte1; 
	return value;
}

static enum NelianError Nelian_OpenDevice(struct Nelian *nelian, 
	const char *device)
{
	struct termios tty;

	nelian->fd = open(device, O_RDONLY| O_NOCTTY);
	if (0 == nelian->fd)
		return NELIAN_CANT_OPEN_DEVICE;
	memset(&tty, 0, sizeof tty);
	if (0 != tcgetattr(nelian->fd, &tty))
		return NELIAN_TCGETATTR_FAIL;
	cfsetospeed(&tty, (speed_t)B38400);
	cfsetispeed(&tty, (speed_t)B38400);
	cfmakeraw(&tty);
	tcflush(nelian->fd, TCIFLUSH);
	if (0 != tcsetattr(nelian->fd, TCSANOW, &tty))
		return NELIAN_TCSETATTR_FAIL;
	return NELIAN_OK;
}

static enum NelianError Nelian_ProtocolHeader(struct Nelian *nelian)
{
	int i;
	unsigned char byte;

	for (i = 0; i < 5; i++) {
		byte = Nelian_ReadByte(nelian);
		if (NELIAN_HEAD == byte)
			break;
		if (i == 3)
			NELIAN_CANT_RECEIVE_HEADER;
	}
	return NELIAN_OK;
}

static enum NelianError Nelian_ProtocolSyncro(struct Nelian *nelian, int channel)
{
	int i;
	unsigned char byte;
	static enum NelianError err;
	int channel_nelian;

	for (i = 0; i < 2; i++) {
		err = Nelian_ProtocolHeader(nelian);
		if (NELIAN_OK != err)
			return err;
		byte = Nelian_ReadByte(nelian);
		channel_nelian = (byte & (1 << NELIAN_CHANNEL_BIT)) >> 
			NELIAN_CHANNEL_BIT;
		if (channel == channel_nelian)
			return NELIAN_OK;
	}
	return NELIAN_CANT_SELECT_CHANNEL;
}

enum NelianError Nelian_Init(struct Nelian *nelian, const char *device,
	const char *dump_name, bool dump_enable)
{
	enum NelianError err;

	memset(nelian, 0, sizeof(struct Nelian));
	err = Nelian_OpenDevice(nelian, device);
	if (NELIAN_OK != err)
		return err;

	nelian->dump_enable = dump_enable;
	if (dump_enable) {
		assert(dump_name != NULL);
		nelian->dump_fd = fopen(dump_name, "w");
		if (NULL == nelian->dump_fd)
			printf("[ERROR]: error opening dump file %s, because %s\n",
				dump_name, strerror(errno));
	} else {
		nelian->dump_fd = NULL;
	}

	return NELIAN_OK;
}

void Nelian_Done(struct Nelian *nelian)
{
	close(nelian->fd);
	if (nelian->dump_enable)
		fclose(nelian->dump_fd);
}

enum NelianError Nelian_GetData(struct Nelian *nelian, struct NelianData *data)
{
	unsigned int value;
	int i;
	enum NelianError err;

	for (i = 0; i < 2; i++) {
		err = Nelian_ProtocolSyncro(nelian, i);
		if (NELIAN_OK != err)
			return err;
		value = Nelian_ReadWord(nelian);
		if (i == 0) 
			data->value_red = value;
		if (i == 1) 
			data->value_blue = value;
	}
	return NELIAN_OK;
}

char *Nelian_StrErr(enum NelianError err)
{
	return nelian_error_strings[err];
}

void NelianData_Clear(struct NelianData *data)
{
	memset(data, 0, sizeof(struct NelianData));
}
