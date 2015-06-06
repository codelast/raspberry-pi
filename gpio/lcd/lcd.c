#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <string.h>
#include <time.h>

#include <wiringPi.h>
#include <lcd.h>

/**
 * A program to display something on a LCD1602 screen using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

typedef enum { false, true } bool;

// LCD row number & column number
const int ROWS = 2;
const int COLUMNS = 16;

// Raspberry Pi GPIO pin number connected to the LCD1602 RS pin and Strobe(E) pin
const int GPIO_PIN_NUMBER_RS = 11;
const int GPIO_PIN_NUMBER_E = 10;

static int lcdHandle;

void scrollMessage(int line, int width, const char* message);
void waitForEnter();

int main(int argc, char *argv[]) {
  struct tm *t;
  time_t tim;
  char buf[32];

  const char* programName = argv[0];
  int dataBits = atoi(argv[1]);
  const char* displayMessage = argv[2];

  if(argc < 3) {
    fprintf(stderr, "Usage: %s data_bits display_message\n", programName);
    return -1;
  }

  if (-1 == wiringPiSetup()) {  // initialize WiringPi
    printf("Setup wiringPi failed!\n");
    return 1;
  }

  if(4 == dataBits) {
    lcdHandle = lcdInit(ROWS, COLUMNS, dataBits, GPIO_PIN_NUMBER_RS, GPIO_PIN_NUMBER_E, 4, 5, 6, 7, 0, 0, 0, 0);
  } else if (8 == dataBits) {
    lcdHandle = lcdInit(ROWS, COLUMNS, dataBits, GPIO_PIN_NUMBER_RS, GPIO_PIN_NUMBER_E, 0, 1, 2, 3, 4, 5, 6, 7);
  } else {
    fprintf(stderr, "data bits must be set to 4 OR 8\n");
    return -1;
  }

  if(lcdHandle < 0) {
    fprintf(stderr, "lcdInit failed\n");
    return -1;
  }

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
