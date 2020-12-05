// MSGEQ7 I/O pins
#define msg7RESET 6
#define msg7Strobe 7
#define msg7DCout A0
#define potentiometer 1

// WS2811 compatible NeoPixel library is used
#include <Adafruit_NeoPixel.h>
#define LED_COUNT 100
#define PIN 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// Defining RGB colors, to be used later
uint32_t blue = strip.Color(0, 0, 255);
uint32_t red = strip.Color(0, 255, 0);
uint32_t white = strip.Color(50, 50, 50);
uint32_t light_blue = strip.Color(50, 50, 255);

// The two frequency measure variables are the 'bands' that are to be read by the msgeq7;
// bands 1 and 0 are the lowest frequency bands, with bands 6 and 7 being the highest frequency bands.
int frequencyMeasure = 1;
int frequencyMeasure2 = 0;

// Counter variables used to create the 'moving' effects of the lights changing color
int counter_index = 0;
int time_index = 0;


//SETUP//
void setup() {       
  // Set the I/O pins to output         
  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show();
  }


//LOOP//
void loop() {
  
  digitalWrite(msg7RESET, HIGH);      // reset the MSGEQ7's counter
  delay(5);
  digitalWrite(msg7RESET, LOW);       // send the reset signal to the MSGEQ7
  int magnitudes[7];                  // array that stores the 7 frequency band values


  //READING THE MSGEQ7//
  for (int x = 0; x < 7; x++)         // save each frequency band value to its respective array location
  {
    digitalWrite(msg7Strobe, LOW);    // output each DC value for each freq band
    delayMicroseconds(35);            // to allow the output to settle
    int spectrumRead = analogRead(msg7DCout);
    digitalWrite(msg7Strobe, HIGH);
    magnitudes[x]=spectrumRead;
    //Serial.print(spectrumRead);       // print the frequency band value to the serial monitor
    //Serial.print(" ");                // print a space inbetween each value to the serial monitor 
  }

  //CALCULATING HOW MANY LIGHTS NEED TO LIGHT UP
 int n = pow(((magnitudes[frequencyMeasure] + magnitudes[frequencyMeasure2]) / 2), 2) * 25 / pow(1023.0, 2); //this transforms the magnitude of the signal into a number between 0 and 25 which is also the number of lights on either side of the center


 //TRANSFORMING USING POTENTIOMETER
  double potentReading = analogRead(potentiometer);
  potentReading *= (3.0 / 1024);
  n *= potentReading;

  
  // Format time_index, which determines if any white lights should show up (is there music, or not??)
  if (n > 3) {
    time_index = 0;
  }
  if (time_index < 500) {
      n += 3;
  }

  
  //CHECKING VALUES TO ENSURE THE DOMAIN IS RESPECTED
  if (n > 25) {
    n = 25;
  }
  else if ( n < 1) {
    n = 0;
  }

  
  //SET LIGHTS TO NEW VALUES
   for (int i = 0; i < 60 - n; i++) {
    if (sin(3 * i - (counter_index / 30.0)) > 0){
      strip.setPixelColor(i, 50, 50, 255);
    }
    else {
      strip.setPixelColor(i, 0, 0, 255);
    }
  }

  strip.fill(white, 60 - n, n * 2);
  
  for (int i = 60 + n; i < 100; i ++) {
    if (sin(3 * i - (counter_index / 30.0)) > 0) {
       strip.setPixelColor(i, 50, 50, 255);
    }
    else {
        strip.setPixelColor(i, 0, 0, 255);
    }
  }
  
  //UPDATING THE LIGHTS
   strip.show();
  
   
  //PRINT TO SERIAL MONITOR//
  //Serial.print(redVar);
  //Serial.print(greenVar);
 // Serial.println(blueVar);
 // Serial.println(potentReading);
  Serial.println(n);

  
  // Increment our time variables
  time_index++;
  if (time_index < 550) {
    time_index++;
  }
  counter_index++;
  if (counter_index > 2000000) {
    counter_index = 0;
  }

}
