#include <IRremote.h>
#include <Adafruit_NeoPixel.h>

int ldrpin = A0;
int ledpin = 3; // LED pin
int NUMPIXELS = 58; // Count the number of leds in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledpin, NEO_GRB + NEO_KHZ800);

int delayval = 100; // timing delay in milliseconds

int redColor = 0;
int greenColor = 0;
int blueColor = 0;
int MEDIUM_BRIGHTNESS = 128;
int MAX_BRIGHTNESS = 255;

unsigned long previousMillis = 0;
const long interval = 100; // Interval for checking the LDR (100 milliseconds)

bool isIRControlActive = false; // Tracks if IR control is currently active

IRrecv IR(13);
decode_results results;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  IR.enableIRIn();
}

void colorflow(){
  redColor = random(0, 255);
  greenColor = random(0,255);
  blueColor = random(0, 255);
  
  for (int i=0; i < NUMPIXELS; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));

    // This sends the updated pixel color to the hardware.
    pixels.show();

    // Delay for a period of time (in milliseconds).
    delay(delayval);
  }
}

void solidColor(int Red, int Green, int Blue){
  
  for (int i=0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(Red, Green, Blue));
  	pixels.show();
    delay(delayval);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Check if an IR signal is received
  if (IR.decode()) {
    Serial.println(IR.decodedIRData.decodedRawData, HEX);
    translateIR();  // Execute the command from the IR remote
    IR.resume();    // Prepare to receive the next value
    delay(200);
  }
  
  // Check the ambient light only if IR control is not active
  if (!isIRControlActive) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      
      bool isAmbientHigh = check_ambient();  // Run check_ambient() once and store the result
      
      if (isAmbientHigh) { 
        colorflow();  // Turn on the LEDs if ambient light is above the threshold
        isIRControlActive = false; // Ensure IR control remains inactive
      } else {
        solidColor(0, 0, 0);  // Turn off the LEDs if the ambient light is below the threshold
      }
    }
  }
}

void translateIR(){
  switch(IR.decodedIRData.decodedRawData)
  {
    case 0xBA45FF00: solidColor(0, 0, 0); isIRControlActive = false; break;
    case 0xE916FF00: colorflow(); isIRControlActive = true; break; // left to right
    case 0xF30CFF00: solidColor(0, 183, 235); isIRControlActive = true; break; // solid Cyan
    case 0xE718FF00: solidColor(186, 17, 12); isIRControlActive = true; break; // solid Red
    case 0xA15EFF00: solidColor(2, 138, 15); isIRControlActive = true; break; // solid Green
    case 0xF708FF00: solidColor(255,255,0); isIRControlActive = true; break; // solid Yellow
    case 0xE31CFF00: solidColor(128,0,128); isIRControlActive = true; break; // solid Purple
    case 0xA55AFF00: solidColor(255,20,147); isIRControlActive = true; break; // solid Pink
    case 0xBD42FF00: solidColor(255, 244, 229); isIRControlActive = true; break; // warm light
  }
}

bool check_ambient(){
  int threshold = 200;
  int ldrStatus = analogRead(ldrpin);  // Read the LDR value
  
  return ldrStatus < threshold;  // Return true if the light level is above the threshold
}


// ---------- EXPERIMENTAL FEATURES FOR THE FURTURE ---------- // 
// void twilight() {
//   int baseRed = 148;
//   int baseGreen = 0;
//   int baseBlue = 211;

//   int ledRed[NUMPIXELS];
//   int ledGreen[NUMPIXELS];
//   int ledBlue[NUMPIXELS];

//   for (int i = 0; i < NUMPIXELS; i++) {
//     ledRed[i] = (baseRed * MEDIUM_BRIGHTNESS) / 255;
//     ledGreen[i] = (baseGreen * MEDIUM_BRIGHTNESS) / 255;
//     ledBlue[i] = (baseBlue * MEDIUM_BRIGHTNESS) / 255;
//     pixels.setPixelColor(i, pixels.Color(ledRed[i], ledGreen[i], ledBlue[i]));
//   }
//   pixels.show();
//   delay(delayval);

//   int brighteningInterval = 1000;
  
//   while (true) {
//     int randomLed = random(NUMPIXELS);
//     ledRed[randomLed] = (baseRed * MAX_BRIGHTNESS) / 255;
//     ledGreen[randomLed] = (baseGreen * MAX_BRIGHTNESS) / 255;
//     ledBlue[randomLed] = (baseBlue * MAX_BRIGHTNESS) / 255;

//     for (int i = 0; i < NUMPIXELS; i++) {
//       pixels.setPixelColor(i, pixels.Color(ledRed[i], ledGreen[i], ledBlue[i]));

//       if (i == randomLed) {
//         delay(delayval);
//         ledRed[i] = (baseRed * MEDIUM_BRIGHTNESS) / 255;
//         ledGreen[i] = (baseGreen * MEDIUM_BRIGHTNESS) / 255;
//         ledBlue[i] = (baseBlue * MEDIUM_BRIGHTNESS) / 255;
//       }
//     }

//     pixels.show();

//     // Wait for the brightening interval, and check for IR signal frequently
//     unsigned long startMillis = millis();
//     while (millis() - startMillis < brighteningInterval) {
//       if (IR.decode()) {
//         translateIR();  // Execute the command from the IR remote
//         IR.resume();    // Prepare to receive the next value
//         return;         // Exit the function to allow other IR commands
//       }
//     }
//   }
// }

// void fadeInOutLoop(int targetRed, int targetGreen, int targetBlue) {
//   int steps = 100;
//   int minBrightness = 10;
//   int fadeDelay = 50;
//   unsigned long previousMillis = 0;
  
//   for (int step = minBrightness; step <= steps; step++) {
//     unsigned long currentMillis = millis();
    
//     // Non-blocking delay check
//     if (currentMillis - previousMillis >= fadeDelay) {
//       previousMillis = currentMillis;
      
//       // Calculate the intermediate color for fading in
//       int intermediateRed = (targetRed * step) / steps;
//       int intermediateGreen = (targetGreen * step) / steps;
//       int intermediateBlue = (targetBlue * step) / steps;

//       // Set the color to all pixels
//       for (int i = 0; i < NUMPIXELS; i++) {
//         pixels.setPixelColor(i, pixels.Color(intermediateRed, intermediateGreen, intermediateBlue));
//       }

//       // Show the new color
//       pixels.show();
//     }

//     // Check for IR signal frequently
//     if (IR.decode()) {
//       translateIR();  // Execute the command from the IR remote
//       IR.resume();    // Prepare to receive the next value
//       return;         // Exit the function to allow other IR commands
//     }
//   }

//   for (int step = steps; step >= minBrightness; step--) {
//     unsigned long currentMillis = millis();
    
//     // Non-blocking delay check
//     if (currentMillis - previousMillis >= fadeDelay) {
//       previousMillis = currentMillis;
      
//       // Calculate the intermediate color for fading out
//       int intermediateRed = (targetRed * step) / steps;
//       int intermediateGreen = (targetGreen * step) / steps;
//       int intermediateBlue = (targetBlue * step) / steps;

//       // Set the color to all pixels
//       for (int i = 0; i < NUMPIXELS; i++) {
//         pixels.setPixelColor(i, pixels.Color(intermediateRed, intermediateGreen, intermediateBlue));
//       }

//       // Show the new color
//       pixels.show();
//     }

//     // Check for IR signal frequently
//     if (IR.decode()) {
//       translateIR();  // Execute the command from the IR remote
//       IR.resume();    // Prepare to receive the next value
//       return;         // Exit the function to allow other IR commands
//     }
//   }
// }