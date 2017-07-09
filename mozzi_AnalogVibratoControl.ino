/*  Example playing a sinewave at a set frequency,
    using Mozzi sonification library.
  
    Demonstrates the use of Oscil to play a wavetable.
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa.
*/

//#include <ADC.h>  // Teensy 3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/sin256_int8.h> // sine table for oscillator

#include <WavePacket.h>
#include <RollingAverage.h>
#include <AutoMap.h>

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil <SIN256_NUM_CELLS, AUDIO_RATE> aSin(SIN256_DATA);
Oscil <SIN256_NUM_CELLS, CONTROL_RATE> kVib(SIN256_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

float centre_freq = 440.0;
float depth = 0.25;


const int KNOB_PIN = 0; // set the input for the knob to analog pin 0
const int LDR1_PIN=1; // set the analog input for fm_intensity to pin 1
const int LDR2_PIN=2; // set the analog input for mod rate to pin 2

// min and max values of synth parameters to map AutoRanged analog inputs to
const int MIN_F = 5;
const int MAX_F = 100;

const int MIN_BW = 1;
const int MAX_BW = 1000;

const int MIN_CF = 60;
const int MAX_CF = 2000;

// for smoothing the control signals
// use: RollingAverage <number_type, how_many_to_average> myThing
RollingAverage <int, 16> kAverageF;
AutoMap kMapF(0,1023,MIN_F,MAX_F);

WavePacket <DOUBLE> wavey; // DOUBLE selects 2 overlapping streams

void setup(){
  Serial.begin(9600);
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  //aSin.setFreq(400); // set the frequency
  kVib.setFreq(3.5f); // set the frequency
}



void updateControl(){

  int fundamental = mozziAnalogRead(KNOB_PIN)+1;
  Serial.print(fundamental);
  Serial.print("  ");
  fundamental = kMapF(fundamental);
  Serial.print(fundamental);
  Serial.println();

  kVib.setFreq(fundamental/5 ); // set the frequency
  float vibrato = depth * kVib.next();
  aSin.setFreq(centre_freq + vibrato);
}


int updateAudio(){
  return aSin.next(); // return an int signal centred around 0
}


void loop(){
  audioHook(); // required here
}



