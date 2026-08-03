//USE 460800 BAUD RATE WHEN OBTAINING DATA, THIS IS WHAT SAMPLE SPEED ON PLAYBACK CODE IS SYNCED TO, ANOTHER BAUD RATE WONT WORK

#define micPin A3
#define samples 120000 //About 15 seconds of audio, so 2 is about 30 seconds
//Do 2 120000 audio recordings, and stack them in byte array on playback code, for longest audio that can fit in DUE board 1/2MB flash memory
//Don't do them all at once, or Arduino serial monitor starts deleting older characters from buffer.
int sample = 0;
int line = 0;
int values = 0;

void setup() {
  Serial.begin(460800);
  Serial.flush(); //Clean serial buffer
  Serial.println("Copy and paste these numbers directly into the playback samples array.");
  Serial.println("If you want a longer audio sample, do multiple, and paste them into playback array consecutively.");
  analogReadResolution(12);
}

void loop() {
  sample = analogRead(micPin);

  Serial.print(sample);
  if (values != samples - 1) {Serial.print(", ");} //Ensure last value doesn't get a comma, or it breaks playback code
  line++;
  values++;
  if (line == 20) {Serial.println(); line = 0;}
  if (values == samples) {while(1);}
}