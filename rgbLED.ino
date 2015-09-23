
int potPin = A0; 
int potCol;

int switchPin = 8; 
int redPin = 11; 
int greenPin = 9; 
int bluePin = 10; 

int colR;
int colG;
int colB;

int hue;
int saturation;
int brightness;
int fadeAmount = 5;

int val;                        
int val2;                     

int switchValue;               
int mode = 0; 

// getRGB function stores RGB values in this array
// we use these values for the red, blue, green led. 
int rgb_colors[3]; 
const byte dim_curve[] = {
    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};

void setup() {
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  pinMode(switchPin, INPUT);
  
  switchValue = digitalRead(switchPin);
  
  colR = 0;
  colG = 0;
  colB = 0;
  
  hue = 0;
  saturation = 0;
  brightness = 0;

  // for debugging
  Serial.begin(9600); 
}
void loop() {
   val = digitalRead(switchPin);         // check button value and set val
   delay(10);                            // delay 10 ms 
   val2 = digitalRead(switchPin);        // check buton value again and set val2
 
   // this loop checks if button state has changed //
   
   if (val == val2) {                    // if button has been pressed for more than 10ms (for debounce)
      if (val != switchValue) {          // if val is not same as old value
      if (val == LOW) {                  // if button is not pressed
        if (mode == 0) {                 // if mode is 0
          mode = 1;                      // set mode to 1 
          //Serial.println("mode1");     // debug message           
        } else {
          mode = 0;                      // else set mode to 0 
          //Serial.println("mode0");     // debug message
        }
      }
     }
   }
   switchValue = val;                    // set old value to val 
   
   // here the button state change checking ends // 
  
   potCol = analogRead(potPin);                    // read potentiometer value
   hue = map(potCol,0, 1023,0, 359);               // map pot value to a value from 0 to 360
   saturation = 255;                               // saturation is fixed, but could be variable (0-255)
   getRGB(hue,saturation,brightness,rgb_colors);   // this function converts HSB to RGB - function at bottom of file
  
   if (mode == 0) {                                // if mode 0 - fade light to maximum and keep it at 255 - fixed brightness  
    //Serial.println("mode0");                     // debug message 
    int brightnessGoal = 255;                      // max goal of color is 255
    if (brightness < brightnessGoal) {             // if brightness is less than goal
      brightness = brightness + 5;                 // add 5 to brightness
      analogWrite(redPin, rgb_colors[0]);          // red value in index 0 of rgb_colors array
      analogWrite(greenPin, rgb_colors[1]);        // green value in index 1 of rgb_colors array
      analogWrite(bluePin, rgb_colors[2]);         // blue value in index 2 of rgb_colors array
      delay(20);                                   // delay to slow down fading
    } else { 
      brightness = 255;                            // if brightness is above 255 - set it to 255 for max brightness
      analogWrite(redPin, rgb_colors[0]);          // red value in index 0 of rgb_colors array
      analogWrite(greenPin, rgb_colors[1]);        // green value in index 1 of rgb_colors array
      analogWrite(bluePin, rgb_colors[2]);         // blue value in index 2 of rgb_colors array
      delay(20); // delay to slow down fading      // delay to slow down fading
   }
  }
   
  if (mode == 1) {                                 // if mode is 1, cycle fade in and out 
    brightness += fadeAmount;                      // add fadeAmount to brightness 
    if (brightness <= 0) {                         // if brightness less or equals to 0 i.e. dark
      fadeAmount = 5;                              // set fade amount to +5
    }   
    if (brightness >= 255) {                       // if brightness is more than 255
      fadeAmount = -5;                             // set fade amount to -5 for fade out 
    }
    
    analogWrite(redPin, rgb_colors[0]);            // red value in index 0 of rgb_colors array
    analogWrite(greenPin, rgb_colors[1]);          // green value in index 1 of rgb_colors array
    analogWrite(bluePin, rgb_colors[2]);           // blue value in index 2 of rgb_colors array
    
    delay(10);                                     // delay to slow down fading
    //Serial.println("mode1 | ");                  // debug message 
    //Serial.print(brightness);                    // debug message
  }
}   

/* 
 *  i did not write the math for converting rgb to hsb, but basically
 *  it uses a lokup table for changing the values and uses some math
 *  to calculate the rgb values from one potentiometer
 */
 
void getRGB(int hue, int sat, int val, int colors[3]) { 
  
  /* convert hue, saturation and brightness ( HSB/HSV ) to RGB
     The dim_curve is used only on brightness/value and on saturation (inverted).
     This looks the most natural.      
  */
  val = dim_curve[val];
  sat = 255-dim_curve[255-sat];

  int r;
  int g;
  int b;
  int base;

  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    colors[0]=val;
    colors[1]=val;
    colors[2]=val;  
  } else  { 

  base = ((255 - sat) * val)>>8;

  switch(hue/60) {
  case 0:
    r = val;
    g = (((val-base)*hue)/60)+base;
    b = base;
  break;

  case 1:
    r = (((val-base)*(60-(hue%60)))/60)+base;
    g = val;
    b = base;
  break;

  case 2:
    r = base;
    g = val;
    b = (((val-base)*(hue%60))/60)+base;
  break;

  case 3:
    r = base;
    g = (((val-base)*(60-(hue%60)))/60)+base;
    b = val;
  break;

  case 4:
    r = (((val-base)*(hue%60))/60)+base;
    g = base;
    b = val;
  break;

  case 5:
    r = val;
    g = base;
    b = (((val-base)*(60-(hue%60)))/60)+base;
  break;
    }

    colors[0]=r;
    colors[1]=g;
    colors[2]=b; 
  }   
}
