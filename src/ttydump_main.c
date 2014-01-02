#include <stdio.h>
#include <string.h>
#include "nelian.h"
#include "ttydump_options.h"
#include "waveform.h"

char *errors[] = {
	#define ERR_X(a,b) b,
	TTYDUMP_ERRORS_TABLE
	#undef ERR_X
};

void ErrorDecode(enum ttyDump_Error err)
{
	if (OK == err)
		return;
	printf("%s\n", errors[err]);
}

int main(int argc, const char *argv[])
{
	enum ttyDump_Error err = OK;
	struct ttyDumpOptions options;
	enum NelianError err_nelian;
	struct Nelian nelian;
	struct NelianData data_old;
	struct NelianData data_new;
	struct Waveform wave;

	err = ttyDumpOptions_Parse(&options, argc, argv);
	if (OK != err)
		return err;
	err_nelian = Nelian_Init(&nelian, options.device, 
		options.dump_name, options.dump_enable);
	if (NELIAN_OK != err_nelian) {
		printf("[ERROR:NELIAN] %s\n", Nelian_StrErr(err_nelian));
		err = ERR_NELIAN_INIT;
		goto out1;
	}
	Waveform_Init(&wave, options.x_count, &argc, argv);

	NelianData_Clear(&data_old);
	NelianData_Clear(&data_new);
	while (1) {
		err_nelian = Nelian_GetData(&nelian, &data_new);
		if (NELIAN_OK != err_nelian) {
			if (NELIAN_CANT_SELECT_CHANNEL == err_nelian) {
				printf("[WARNING] missing point\n");
				continue;
			} else{
				printf("[ERROR] %s\n", 
					Nelian_StrErr(err_nelian));
				break;
			}
		}
		Waveform_AddDataStep(&wave, 
			data_old.value_red, data_new.value_red, 1);
		Waveform_AddDataStep(&wave,
			data_old.value_blue, data_new.value_blue, 2);
		Waveform_DoStep(&wave);
		memcpy(&data_old, &data_new, sizeof(struct NelianData));
		// TODO: check for ESC key
	}
out2:	Waveform_Done(&wave);
out1:	Nelian_Done(&nelian);

	// TODO: diagnose error
	ErrorDecode(err);
	return (int)err;
}
