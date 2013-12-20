#ifndef __WAVEFORM__
#define __WAVEFORM__

struct Waveform{
	int width;
	int current_pos;
};

enum WaveformError {
	WAVEFORM_OK
};

enum WaveformError Waveform_Init(struct Waveform *wave, int width,
	int *argc, const char *argv[]);
void Waveform_Done(struct Waveform *wave);
void Waveform_AddDataStep(struct Waveform *wave, 
	int data_old, int data_new, int color);
void Waveform_DoStep(struct Waveform *wave);

#endif