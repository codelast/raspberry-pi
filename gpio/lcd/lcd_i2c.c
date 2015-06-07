#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <string.h>
#include <time.h>

#include <wiringPi.h>
#include <lcd.h>
#include <pcf8574.h>

/**
 * A program to display something on a LCD1602 screen through I2C using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

typedef enum { false, true } bool;

// LCD row number & column number
const int ROWS = 2;
const int COLUMNS = 16;

static int lcdHandle;

// define new pins
#define AF_BASE 100

#define AF_RS   (AF_BASE + 0)
#define AF_RW   (AF_BASE + 1)
#define AF_E    (AF_BASE + 2)

#define AF_BL   (AF_BASE + 3)

#define AF_D1   (AF_BASE + 4)
#define AF_D2   (AF_BASE + 5)
#define AF_D3   (AF_BASE + 6)
#define AF_D4   (AF_BASE + 7)

void scrollMessage(int line, int width, const char* message);
void waitForEnter();

int main(int argc, char *argv[]) {
  struct tm *t;
  time_t tim;
  char buf[32];

  const char* programName = argv[0];
  const char* displayMessage = argv[1];

  if(argc < 2) {
    fprintf(stderr, "Usage: %s display_message\n", programName);
    return -1;
  }

  /* initialize WiringPi */
  wiringPiSetupSys();

  /* initalize LCD */
  lcdHandle = lcdInit(ROWS, COLUMNS, 4, AF_RS, AF_E, AF_D1, AF_D2, AF_D3, AF_D4, 0, 0, 0, 0);
  if(lcdHandle < 0) {
    fprintf(stderr, "lcdInit failed\n");
    return -1;
  }

  /* initalize PCF8574 module, NOTE that the device address must be right & can be acquired by "i2cdetect -y 1" command */
  pcf8574Setup(AF_BASE, 0x3f);

  /* turn on LCD backlight */
  pinMode(AF_BL, OUTPUT);
  digitalWrite(AF_BL, 1);

  /* set LCD into write mode */
  pinMode(AF_RW, OUTPUT);
  digitalWrite(AF_RW, 0);

  /* then we start to display various messages on the LCD screen */
  lcdClear(lcdHandle);             // clear the screen
  lcdPosition(lcdHandle, 3, 0);    // set the cursor on the the 4th column & 1st row
  lcdPuts(lcdHandle, "RPi");       // print the text on the LCD at current cursor postion
  lcdPosition(lcdHandle, 4, 1);    // set the cursor on the the 5th column & 2nd row
  lcdPuts(lcdHandle, "codelast");

  waitForEnter();

  lcdClear(lcdHandle);             // clear the screen
  lcdPosition(lcdHandle, 0, 0);    // set the cursor on the 1st column & 1st row
  lcdCursor(lcdHandle, true);      // turn the cursor on
  lcdCursorBlink(lcdHandle, true); // turn the cursor blink on

  waitForEnter();

  lcdCursor(lcdHandle, false);       // turn the cursor off
  lcdCursorBlink(lcdHandle, false);  // turn the cursor blink off
  lcdClear(lcdHandle);               // clear the screen
  lcdPuts(lcdHandle, "Will scroll...");

  waitForEnter();

  while(true) {
    scrollMessage(0, COLUMNS, displayMessage);  // scroll the specified message on the 1st row

    /* display current time on LCD */
    tim = time(NULL);
    t = localtime(&tim);
    sprintf(buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

    lcdPosition(lcdHandle, (COLUMNS - 8) / 2, 1);
    lcdPuts(lcdHandle, buf);
  }
  
  return 0;
}

void waitForEnter() {
  printf("Press ENTER to continue: ");
  (void) fgetc(stdin);
}

void scrollMessage(int row, int width, const char* message) {
  char buf[32] = {0};
  memset(buf, 0, sizeof(buf));

  static int position = 0;
  static int timer = 0;

  if(millis() < timer) {
    return;
  }

  timer = millis() + 200;

  strncpy(buf, &message[position], width);
  buf[width] = 0;

  lcdPosition(lcdHandle, 0, row);
  lcdPuts(lcdHandle, buf);

  if (++position == (strlen(message) - width)) {
    position = 0;
  }
}
