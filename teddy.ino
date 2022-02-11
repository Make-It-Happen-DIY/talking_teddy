#include <SD.h>                           //include SD module library
#include <TMRpcm.h>                       //include speaker control library

#define SD_ChipSelectPin 10                //define CS pin

TMRpcm wazzup;                            //crete an object for speaker library
const char *wav_files[10] = { "1.wav", "2.wav", 
                             "3.wav", "4.wav", "5.wav", 
                             "6.wav", "7.wav", "8.wav", 
                             "9.wav", "10.wav" };
                             
const int button = 2;
int n = 0;
bool first_time = 0; // different random numbers every power up					/// shouldnt this be 1

//debounce variables
int debounce = 500;       // wanted debounce time in milliseconds
long last_updated = 0;     //timestamp of previous interrupt
long curr_time;      // current time in as reported by millis()

// change lighting mode on button click
void button_ISR(){
  curr_time = millis();
  
  if (first_time){
    randomSeed(curr_time);
    first_time = 1;																/// shouldnt this be 0
    }
    
  if ((curr_time - last_updated) > debounce){
      wazzup.stopPlayback();
      n = random(0,9);
      Serial.println(n);
      wazzup.play(wav_files[n]);         // play file
    }
  last_updated = curr_time;
}

void setup(){

    //interrupt setup
  pinMode(button, INPUT);
  attachInterrupt(0, button_ISR, FALLING);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  
  wazzup.speakerPin = 9;                  //define speaker pin. 
                                          //you must use pin 9 of the Arduino Uno and Nano
                                          //the library is using this pin
                                          
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_ChipSelectPin)) {      //see if the card is present and can be initialized
    Serial.println("Initialization failed...");
    return;                               //don't do anything more if not
  }
  else {
      Serial.println("Initialization done...");
  }



  wazzup.quality(1);
  wazzup.setVolume(4);                    //0 to 7. Set volume level
 
  /*
  wazzup.play("10.wav");

  if(wazzup.isPlaying()){
    Serial.println("Wazzup is playing");
    }
  else{
    Serial.println("Playback failed...");
  }
  */
}

void loop(){
  delay(5);
  if(!wazzup.isPlaying()) {
   digitalWrite(wazzup.speakerPin, LOW);
 }
}
