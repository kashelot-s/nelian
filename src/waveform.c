#include <assert.h>
#include <stdlib.h>
#include <plplot/plplot.h>
#include "waveform.h"

static void Waveform_CleanUpWindow(struct Waveform *wave)
{
	plclear();
	plcol0(7);
	pllab("x", "y", "nelian");
}

void Waveform_DoStep(struct Waveform *wave)
{
	wave->current_pos += 1;
	if (wave->current_pos > wave->width) {
		wave->current_pos = 0;
		Waveform_CleanUpWindow(wave);
	}
}

enum WaveformError Waveform_Init(struct Waveform *wave, int width,
	int *argc, const char *argv[])
{
	wave->width = width;
	wave->current_pos = 0;
	plparseopts(argc, argv, PL_PARSE_FULL);
	plsdev("xwin");
	plinit();
	plenv(0, width, 0, 1 << 16, 0, 0);
	Waveform_CleanUpWindow(wave);
	return WAVEFORM_OK;
}

void Waveform_Done(struct Waveform *wave)
{
	plend();
}

void Waveform_AddDataStep(struct Waveform *wave, 
	int data_old, int data_new, int color)
{
	PLFLT x[2], y[2];

	x[0] = (PLFLT)(wave->current_pos);
	x[1] = (PLFLT)(wave->current_pos + 1);
	y[0] = (PLFLT)data_old;
	y[1] = (PLFLT)data_new;

	plcol0(color);
	plline(2, x, y);
}
