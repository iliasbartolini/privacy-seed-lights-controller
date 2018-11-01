#include <Arduino.h>
#include <FastLED.h>

#define debug Serial

//pins 3 and 2 are inverted
#define TREE_BASE_DATA_PIN  4
#define TREE_BASE_CLOCK_PIN  5
#define TREE_BASE_NUM_PIXELS  53
CRGBArray<TREE_BASE_NUM_PIXELS> tree_base_led_strip;

#define TREE_TOP_DATA_PIN  6
#define TREE_TOP_CLOCK_PIN  7
#define TREE_TOP_NUM_PIXELS  25
CRGBArray<TREE_TOP_NUM_PIXELS> tree_top_led_strip;

//TREE_TOP = RING \ ROPE \ HEART
#define TREE_TOP_RING_NUM_PIXELS 7
#define TREE_TOP_ROPE_NUM_PIXELS 12
#define TREE_TOP_HEART_NUM_PIXELS 6

//TREE = RING \ ROPE \ HEART | BASE
#define TREE_NUM_PIXELS 78


void setup() {
  delay(1000);
  Serial.begin(115200);
  debug.println("-- start DotStar controller --");
  debug.flush();

  FastLED.addLeds<DOTSTAR, TREE_BASE_DATA_PIN, TREE_BASE_CLOCK_PIN, BGR>(tree_base_led_strip, TREE_BASE_NUM_PIXELS);

  FastLED.addLeds<DOTSTAR, TREE_TOP_DATA_PIN, TREE_TOP_CLOCK_PIN, BGR>(tree_top_led_strip, TREE_TOP_NUM_PIXELS);

}

void dimHeartAndRingPattern(){
  for (uint8_t i = 0; i < TREE_TOP_RING_NUM_PIXELS; i++) {
    tree_top_led_strip[i] = 0x200000;
  }
  for (uint8_t i = TREE_TOP_RING_NUM_PIXELS + TREE_TOP_ROPE_NUM_PIXELS; i < TREE_TOP_NUM_PIXELS; i++) {
    tree_top_led_strip[i] = 0x200000;
  }
}

uint8_t heartBeatPatternIndex = 0;
#define LIGHT_FADE_TAIL_NUM_PIXELS 6
void heartBeatPattern(){
  dimHeartAndRingPattern();

  uint8_t baseLedIndex = heartBeatPatternIndex / 2;
  uint8_t ropeLedIndex = heartBeatPatternIndex / 3;
  if (baseLedIndex < (TREE_BASE_NUM_PIXELS / 2) + 2  ){
    tree_base_led_strip[baseLedIndex] = CRGB::Red;
    tree_base_led_strip[TREE_BASE_NUM_PIXELS - baseLedIndex] = CRGB::Red;
  }
  if (ropeLedIndex <= TREE_TOP_ROPE_NUM_PIXELS){
    tree_top_led_strip[TREE_TOP_RING_NUM_PIXELS + TREE_TOP_ROPE_NUM_PIXELS - ropeLedIndex ] = CRGB::Red;
  }

  tree_base_led_strip.fadeToBlackBy(64);
  tree_top_led_strip.fadeToBlackBy(64);
  FastLED.show();

  //index reset condition
  heartBeatPatternIndex++;
  if (baseLedIndex > (TREE_BASE_NUM_PIXELS / 2) + LIGHT_FADE_TAIL_NUM_PIXELS) {
     heartBeatPatternIndex = 0;
  }
}


uint8_t randomPatternFadeIndex = 0;
void randomPatternTree(int cyclesToAddPixel, CRGB color){
  if (randomPatternFadeIndex < cyclesToAddPixel){
    randomPatternFadeIndex++;
  } else {
    uint8_t pixel = random(TREE_BASE_NUM_PIXELS + TREE_TOP_ROPE_NUM_PIXELS);
    if (pixel < TREE_BASE_NUM_PIXELS) {
      tree_base_led_strip[pixel] = color;
    } else {
      tree_top_led_strip[pixel - TREE_BASE_NUM_PIXELS + TREE_TOP_RING_NUM_PIXELS] = color;
    }
    randomPatternFadeIndex = 0;
  }
  tree_base_led_strip.fadeToBlackBy(1);
  tree_top_led_strip.fadeToBlackBy(1);
}

void idleLightsPattern(){
  randomPatternTree(100, CRGB::DarkRed);
  FastLED.show();
}

void sensorCoveredLightsPattern(){
  dimHeartAndRingPattern();
  randomPatternTree(20, CRGB::Red);
  FastLED.show();
}


void addGlitter( fract8 chanceOfGlitter)
{
 if( random8() < chanceOfGlitter) {
   tree_base_led_strip[random8(TREE_BASE_NUM_PIXELS)] = CRGB::White;
   tree_top_led_strip[random8(TREE_TOP_NUM_PIXELS)] = CRGB::White;
 }
}
uint8_t gHue = 0;
void rainbow(){
 fill_rainbow( tree_base_led_strip, TREE_BASE_NUM_PIXELS, gHue++, 7);
 fill_rainbow( tree_top_led_strip, TREE_TOP_NUM_PIXELS, gHue++, 7);
 addGlitter(20);
 FastLED.delay(2);
}


#define HRT_SENSOR_UNCOVERED  0x00
#define HRT_SENSOR_COVERED    0x01
#define HRT_SENSOR_BEAT_RATE  0x02
#define HRT_SENSOR_MAGIC      0x03 //don't try me... really :)

uint8_t lastI2cMessageType = HRT_SENSOR_UNCOVERED;
uint8_t lastBeatRateAvg = 0;

void handleNewMessage(){
  lastI2cMessageType = Serial.read();
  if (lastI2cMessageType == HRT_SENSOR_COVERED){
    uint8_t irValue = Serial.read();
    // debug.print(",NEW_IR: ");
    // debug.println(irValue);
    if (heartBeatPatternIndex != 0) {
      heartBeatPattern();
    } else {
      sensorCoveredLightsPattern();
    }
  } else if (lastI2cMessageType == HRT_SENSOR_BEAT_RATE){
    lastBeatRateAvg = Serial.read();
    // debug.print(",NEW_AVG: ");
    // debug.println(lastBeatRateAvg);
    heartBeatPatternIndex = 0;
    heartBeatPattern();
  } else if (lastI2cMessageType == HRT_SENSOR_UNCOVERED){
    lastBeatRateAvg = 0;
    // debug.println("*NC");
    idleLightsPattern();
  } else if (lastI2cMessageType == HRT_SENSOR_MAGIC){
    debug.println(". 　　 *　 * 　 　");
    debug.println("　　　　 * 　 ·");
    debug.println(" 　 ✧ 　.   · ˚  *");
    debug.println(" 　  +   + 　");
    rainbow();
  } else {
    debug.println("*ER");
    idleLightsPattern();
  }
}

uint8_t availableBytes = 0;
uint8_t cyclesUnavailable = 0;

#define TIMEOUT_LOOP_CYCLES 20
void loop() {
  availableBytes = Serial.available();
  // debug.print("a: ");
  // debug.print(availableBytes);
  // debug.print(" avg: ");
  // debug.print(lastBeatRateAvg);
  // debug.print(" ");
  uint8_t isDataAvailable = availableBytes != 0;
  uint8_t isConnectionNotExpired = cyclesUnavailable < TIMEOUT_LOOP_CYCLES;
  if (isDataAvailable || isConnectionNotExpired) {
    // if bytes were received
    if(isDataAvailable) {
      cyclesUnavailable = 0;
      // debug.print("OLD_AVG: ");
      // debug.print(lastBeatRateAvg);
      handleNewMessage();
      // debug.println();
    } else {
      cyclesUnavailable++;
      // debug.print(" - u: ");
      // debug.println(cyclesUnavailable);
      if (heartBeatPatternIndex != 0) {
        heartBeatPattern();
      } else {
        idleLightsPattern();
      }
    }
  } else {
    lastBeatRateAvg = 0;
    // debug.println("*NA");
    idleLightsPattern();
  }

  delay(5);
}
