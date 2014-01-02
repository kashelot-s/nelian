#ifndef __WAVEFORM__
#define __WAVEFORM__

struct Waveform {
	int width;
	int current_pos;
};

enum WaveformError {
	WAVEFORM_OK,
	WAVEFORM_CANT_INIT_SDL_WINDOW
};

enum WaveformError Waveform_Init(struct Waveform *wave, int width,
	int *argc, const char *argv[]);
void Waveform_Done(struct Waveform *wave);
void Waveform_AddDataStep(struct Waveform *wave, int data_old, int data_new, 
	int wave_color);
void Waveform_DoStep(struct Waveform *wave);

#endif
