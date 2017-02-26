#include "cbuffer.h"

unsigned long Index = 0;
unsigned long TimerCount = 0;
volatile unsigned char PlayingSound;


#define AUDIOPIN_1 FPGA_J2_6
#define AUDIOPIN_2 FPGA_J2_7
#define FRAME_SIZE  64
SmallFSFile audiofile;

struct AudioData {
	unsigned char count;
	unsigned char sample[FRAME_SIZE];
};

CircularBuffer<AudioData, 2> audioBuffer;
AudioData currentData;

void DAC_SetOutput(unsigned char data)
{
	unsigned int level0 = ((unsigned int)data) << 8;
	unsigned int level1 = ((unsigned int)data) << 8;
	unsigned int level = (level1 << 16) | level0;

	SIGMADELTADATA = level;
}

void SoundInit(void)
{
	// Enable channel 0 and 1
	SIGMADELTACTL = 0x03;

	// Clear timer counter.
	TMR0CNT = 0;

	// Set up timer , no prescaler.
	TMR0CMP = 0;
	TMR0CTL = (0 << TCTLENA) | (1 << TCTLCCM) | (1 << TCTLDIR) | (1 << TCTLIEN);

	// Enable timer 0 interrupt on mask.
	INTRMASK |= (1 << INTRLINE_TIMER0);

	// Globally enable interrupts.
	INTRCTL = (1 << 0);
}

void SoundOff(void)
{
	DAC_SetOutput(0);

	// Disable timer 0
	TMR0CTL &= ~(_BV(TCTLENA));
}

void AudioFillBuffer()
{
	int r;

	AudioData d;
	while (!audioBuffer.isFull()) {
		r = audiofile.read(&d.sample[0], FRAME_SIZE);
		if (r != 0) {
			d.count = r; // 1 sample per byte
			audioBuffer.push(d);
		} else {
			audiofile.seek(0, SEEK_SET);
		}
	}

}

void SoundPlay(const char *fileName)
{
	unsigned int sampleCount;
	unsigned int frameRate;

	audiofile = SmallFS.open(fileName);
	audiofile.read(&sampleCount, sizeof(unsigned int));
	audiofile.read(&frameRate, sizeof(unsigned int));

	Index = 0;
	PlayingSound = 1;
	AudioFillBuffer();
	currentData = audioBuffer.pop();

	TMR0CNT = 0;
	TMR0CMP = (CLK_FREQ / frameRate) - 1;
	TMR0CTL |= 1 << TCTLENA;
}

void _zpu_interrupt ()
{
	if ( TMR0CTL & (1 << TCTLIF)) {
		unsigned char sample, hasData = 1;

		if (Index >= currentData.count) {
			if (audioBuffer.hasData()) {
				Index = 0;
				currentData = audioBuffer.pop();
			} else {
				hasData = 0;
			}
		}

		if (hasData) {
			sample = currentData.sample[Index];
			DAC_SetOutput(sample);
			Index++;
		} else {
			SoundOff();
			PlayingSound = 0;
		}

		/* Clear the interrupt flag on timer register */
		TMR0CTL &= ~(1 << TCTLIF);
		TMR0CNT = 0;
	}
}

void SoundSetup()
{
	SoundInit();
	pinMode(AUDIOPIN_1, OUTPUT);
	pinModePPS(AUDIOPIN_1, HIGH);
	outputPinForFunction(AUDIOPIN_1, 0);

	pinMode(AUDIOPIN_2, OUTPUT);
	pinModePPS(AUDIOPIN_2, HIGH);
	outputPinForFunction(AUDIOPIN_2, 0);

	if (SmallFS.begin() < 0) {
		Serial.println("No SmalLFS found, aborting.");
		while (1) {};
	}

	SoundPlay("ecstasyoflife.chip");
}