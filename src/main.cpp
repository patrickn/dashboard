/*
 Example animated analogue meters using a ILI9341 TFT LCD screen

 Needs Font 2 (also Font 4 if using large scale label)

 Comment out lines 153 and 197 to reduce needle flicker and
 to remove need for Font 4 (which uses ~5k of FLASH!)

 */

#include <TFT_ILI9341.h> // Hardware-specific library
#include <SPI.h>

void background();
void analogCompass();


TFT_ILI9341 tft = TFT_ILI9341();       // Invoke custom library

#define TFT_GREY 0x5AEB

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

uint32_t updateTime = 0;       // time for next update
byte omm = 99;
unsigned xcolon = 0;
bool startup = true;

void setup(void) {
  tft.init();
  tft.setRotation(3);
  // Serial.begin(57600); // For debug

  background();
  analogCompass(); // Draw analog compass

  updateTime = millis(); // Next update time
}


void loop() {
   if (updateTime <= millis()) {
      updateTime = millis() + 1000;
      ss++;              // Advance second
      if (ss==60) {
         ss=0;
         omm = mm;
         mm++;            // Advance minute
         if(mm>59) {
            mm=0;
            hh++;          // Advance hour
            if (hh>23) {
               hh=0;
            }
         }
      }

   if (ss==0 || startup) {
      startup = false;
      tft.setTextColor(TFT_GREEN, TFT_GREY);
      tft.drawString(__DATE__, 5, 221, 2);

      // tft.setTextColor(TFT_BLUE, TFT_GREY);
      // tft.drawRightString("v0.01", 170, 210, 2);

      tft.setTextColor(TFT_BLUE, TFT_GREY);
      tft.setCursor(140, 210);
      tft.print("v0.01"); // This uses the standard ADAFruit small font

      tft.setTextColor(TFT_BLUE, TFT_GREY);
      tft.drawRightString("milhouse.uk", 170, 220, 2);
   }


   // Update digital time
   unsigned xpos = 178;
   unsigned ypos = 190;
   if (omm != mm) { // Only redraw every minute to minimise flicker
     tft.setTextColor(0x39C4, TFT_GREY);
     tft.drawString("88:88", xpos, ypos, 7); // Overwrite the text to clear it
     tft.setTextColor(0xFBE0); // Orange
     omm = mm;

     if (hh<10) xpos += tft.drawChar('0', xpos, ypos, 7);
     xpos += tft.drawNumber(hh, xpos, ypos, 7);
     xcolon = xpos;
     xpos += tft.drawChar(':', xpos, ypos, 7);
     if (mm<10) xpos += tft.drawChar('0', xpos, ypos, 7);
     tft.drawNumber(mm, xpos, ypos, 7);
   }

   if (ss%2) { // Flash the colon
     tft.setTextColor(0x39C4, TFT_GREY);
     xpos += tft.drawChar(':', xcolon, ypos, 7);
     tft.setTextColor(0xFBE0, TFT_GREY);
   }
   else {
     tft.drawChar(':', xcolon, ypos, 7);
   }



   //  tft.drawRightString("85.2", xpos, ypos, 7);
   //  tft.drawRightString("88:88", xpos, ypos+60, 7);

   //  Serial.print("Height: ");
   //  Serial.println(tft.height());
   //  Serial.print("Width: ");
   //  Serial.print(tft.width());
  }
}


void background()
{
   tft.fillScreen(TFT_GREY);
   tft.drawRect(  0,   0, 320, 240, TFT_GREEN);
   tft.drawRect(176,   0, 144, 51, TFT_GREEN);     // 88:88
   tft.drawRect(211,  62, 109, 51, TFT_GREEN);     // 88.8
   tft.drawRect(211, 128, 109, 51, TFT_GREEN);     // Temperature
   tft.drawRect(176, 188, 144, 51, TFT_GREEN);     // Time
}

void analogCompass()
{
   // Draw clock face
   tft.fillCircle(100, 120, 95, TFT_GREEN);
   tft.fillCircle(100, 120, 90, TFT_BLACK);

}
