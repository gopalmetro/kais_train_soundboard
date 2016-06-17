//Kailasa Metro's Train Soundboard for Christmas 2015
//Built on a Teensy 3.2 with a Teensy Audio Board by Gopal Metro

/* 
  16/3 = Blue/Green = White = intro_
  21/2 = White/Yellow = Green LED = chug_
  20/1 = Grey/Orange = Red LED = whis_
  17/0 = Purple/Red = Blue LED = bell_
  
*/


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Entropy.h>
#include <Bounce.h>

//Declaring and defining LED pins
int ledButton0 = 17;
int ledButton1 = 20;
int ledButton2 = 21;
int ledButton3 = 16;

bool b0State = 0;
bool b1State = 0;
bool b2State = 0;
bool b3State = 0;
bool counter = 1;

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=252,155
AudioPlaySdWav           playSdWav2;     //xy=252,192
AudioPlaySdWav           playSdWav3;     //xy=252,227
AudioPlaySdWav           playSdWav4;     //xy=251,262
AudioMixer4              mixer1;         //xy=533,205
AudioOutputI2S           i2s1;           //xy=706,205
AudioConnection          patchCord2(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord3(playSdWav2, 1, mixer1, 1);
AudioConnection          patchCord4(playSdWav3, 0, mixer1, 2);
AudioConnection          patchCord1(playSdWav4, 1, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=713,253
// GUItool: end automatically generated code

// Bounce objects to read pushbuttons 
Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);  // 15 ms debounce time
Bounce button2 = Bounce(2, 15);
Bounce button3 = Bounce(3, 15);

//Variables for Randomizer Function
uint8_t random_bell;
uint8_t random_chug;
uint8_t random_whistle;

void setup() {
  
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo and Due
//  }
  Entropy.Initialize();
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  
  pinMode(ledButton0, OUTPUT);
  pinMode(ledButton1, OUTPUT);
  pinMode(ledButton2, OUTPUT);
  pinMode(ledButton3, OUTPUT);
  
  digitalWrite(ledButton0, LOW);
  digitalWrite(ledButton1, LOW);
  digitalWrite(ledButton2, LOW);
  digitalWrite(ledButton3, LOW);
  
  AudioMemory(90);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);
  
  playSdWav4.play("intro_1.wav");
  Serial.println("Start playing random INTRO MUSIC sample");
  delay(10);
}

void loop() {
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  
//  float vol = analogRead(15);
//  vol = vol / 1024;
//  sgtl5000_1.volume(vol);
  
//BELL
  if (button0.fallingEdge()) { 
    String fileName = filenameGenerator(4, "bell_"); //make a random BELL sample play when button0 is pushed.  The integer should be the number of samples plus 1. The String should be the filename prefix for the given sample sound type. 
    playSdWav1.play(fileName.c_str());
    b0State = 1;
    delay(10);
  }
  
  if (playSdWav1.isPlaying() == true) {
    digitalWrite(ledButton0, HIGH);
  } 
  if (playSdWav1.isPlaying() == false && b0State == 1) {
    digitalWrite(ledButton0, LOW);
  }
    
    //WHISTLE
  if (button1.fallingEdge()) { //make a random WHISTLE sample play when button0 is pushed
    String fileName = filenameGenerator(6, "whis_");
    playSdWav2.play(fileName.c_str());
    b1State = 1;
    delay(10);
  }
  
  if (playSdWav2.isPlaying() == true)  {
    digitalWrite(ledButton1, HIGH);
  }
  if (playSdWav2.isPlaying() == false && b1State == 1) {
    digitalWrite(ledButton1, LOW);
    b1State = 0;
  }

  if (button2.fallingEdge()) { //make a random CHUG sample play when button0 is pushed
    String fileName = filenameGenerator(8, "chug_");
    playSdWav3.play(fileName.c_str());
    b2State = 1;
    delay(10);
  }
  
  if (playSdWav3.isPlaying() == true) {
    digitalWrite(ledButton2, HIGH);
  }
  if (playSdWav3.isPlaying() == false && b2State == 1) {
    digitalWrite(ledButton2, LOW);
    b2State = 0;
  }
  
  if (button3.fallingEdge()) { //make a random INTRO sample play when button0 is pushed
    String fileName = filenameGenerator(17, "intro_");
    playSdWav4.play(fileName.c_str());
    b3State = 1;
    delay(10);
  }
  
  if (playSdWav4.isPlaying() == true) {
    digitalWrite(ledButton3, HIGH);
  }
  if ((playSdWav4.isPlaying() == false && b3State == 1) || (playSdWav4.isPlaying() == false && counter == 1)) {
    digitalWrite(ledButton3, LOW);
    b3State = 0;
    counter = 0;
  }
}

// Randomizer Function
String filenameGenerator (int numFiles, String filePrefix) {
  int fileNumber = Entropy.random(1, numFiles);
  String fileName = filePrefix;
  fileName += fileNumber;
  fileName += ".wav";
 
  return fileName;
}
