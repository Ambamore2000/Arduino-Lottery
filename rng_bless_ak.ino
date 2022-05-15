#include <LEDMatrixDriver.hpp>

// This sketch will 'flood fill' your LED matrix using the hardware SPI driver Library by Bartosz Bielawski.
// Example written 16.06.2017 by Marko Oette, www.oette.info

// Define the ChipSelect pin for the led matrix (Dont use the SS or MISO pin of your Arduino!)
// Other pins are Arduino specific SPI pins (MOSI=DIN, SCK=CLK of the LEDMatrix) see https://www.arduino.cc/en/Reference/SPI
const uint8_t LEDMATRIX_CS_PIN = 9;

// Number of 8x8 segments you are connecting
const int LEDMATRIX_SEGMENTS = 1;
const int SEGMENT_WIDTH = 8;
const int LEDMATRIX_WIDTH    = LEDMATRIX_SEGMENTS * SEGMENT_WIDTH;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN, 4);

int inPin = 7;
bool val = false;
bool prev_val = false;

unsigned long randomseed;


void setup() {
  Serial.begin(9600);
  randomseed = analogRead(0);
  for(int i = 1; i < 10; i++){
    randomseed = randomseed * analogRead(0);
  }
  randomSeed(randomseed);
  Serial.println("Seed:");
  Serial.println(randomseed);
  delay(2000);
  // init the display
  lmd.setEnabled(true);
  lmd.setIntensity(5);   // 0 = low, 10 = high

  pinMode(inPin, INPUT_PULLUP);
}

long randNumber;

byte cube[8] = { 
  B11111111,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B11111111
};

byte printer[8] = {
  B00111100,
  B00100100,
  B11111111,
  B10000001,
  B10000001,
  B10111101,
  B11100111,
  B00111100
};

byte vr[8] = {
  B00000000,
  B01111110,
  B11111111,
  B11111111,
  B11100111,
  B01100110,
  B00000000,
  B00000000
};

byte sewing[8] = {
  B11111110,
  B11111111,
  B11100111,
  B01000011,
  B01000011,
  B00000011,
  B11111111,
  B11111111
};

byte camera[8] = {
  B11111111,
  B10000001,
  B10011001,
  B10100101,
  B10100101,
  B10011001,
  B10000001,
  B11111111,
};

byte drone[8] = {
  B00000000,
  B11100111,
  B01011010,
  B01111110,
  B01100110,
  B00100100,
  B00111100,
  B00000000,
};

byte computer[8] = {
  B11111111,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B11100111,
  B00100100,
  B11111111,
};
            
typedef struct {
  byte* symbol;
  int weight;
} sym_dict;

const sym_dict my_sym_dict[] {
  {cube, 15},
  {printer, 15},
  {vr, 15},
  {sewing, 15},
  {camera, 15},
  {drone, 10},
  {computer, 15}
};

byte* get_random_symbol() {
  randomSeed(analogRead(0) + millis());
  long rand_number = random(100);
  int start_weight = 0;
  int end_weight = 0;
  for(uint8_t i = 0; i < sizeof(my_sym_dict)/sizeof(sym_dict); ++i) {
      end_weight += my_sym_dict[i].weight;
      if (rand_number >= start_weight && rand_number < end_weight) {
        return my_sym_dict[i].symbol;
      }
      start_weight = end_weight;
  }
}

void drawSprite(byte* sprite, int width, int height )
{
  byte mask = B10000000;
  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      lmd.setPixel(ix, iy, (bool)(sprite[iy] & mask ));
      mask = mask >> 1;
    }
    mask = B10000000;
  }
}


void spammy_wammy() {
  int spam_int = 100;
  byte* random_sym = get_random_symbol();
  while(spam_int < 200) {
    for(uint8_t i = 0; i < sizeof(my_sym_dict)/sizeof(sym_dict); ++i) {
      drawSprite(my_sym_dict[i].symbol, 8, 8);
      lmd.display();
      delay(spam_int);
    }
    spam_int = spam_int * 1.1;
  }
  for(uint8_t i = 0; i < sizeof(my_sym_dict)/sizeof(sym_dict); ++i) {
    drawSprite(my_sym_dict[i].symbol, 8, 8);
    lmd.display();
    delay(spam_int);
    if (random_sym == my_sym_dict[i].symbol) {
      
      lmd.setIntensity(2);
      delay(500);
      lmd.setIntensity(10);
      delay(500);
      lmd.setIntensity(2);
      delay(500);
      lmd.setIntensity(10);
      delay(500);
      lmd.setIntensity(2);
      delay(500);
      lmd.setIntensity(10);
      delay(500);
      lmd.setIntensity(5);
      delay(10000);
      break;
    }
  }
}

void loop() {
  prev_val = val;
  val = !digitalRead(inPin);

  if ((val == 1) && (prev_val != val)) {
    spammy_wammy();
  } else {
    lmd.clear();
    lmd.display();
  }
  delay(1000);
}
