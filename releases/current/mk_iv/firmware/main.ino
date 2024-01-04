#include <Wire.h>
#include <Adafruit_GFX.h> //version 1.11.15
#include <Adafruit_SSD1306.h> //version 2.5.7
#include <Adafruit_PCF8574.h> //version 1.1.0
#include <cstring>
#include <qrcode.h> //version 0.0.1
#include "bitmaps.h"
#include "content.h"
#include "qr_codes.h"

//// DEFINE PINS & OBJECTS ////
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Double-Check your setup! (Adapter version: SDA=2 / SCL=0)
#define SDA_PIN 0  // GPIO2
#define SCL_PIN 2  // GPIO0

Adafruit_PCF8574 pcf;

// Create a QR code object
QRCode qrcode;

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
// #define OLED_RESET pcf8574.pin[0] // Reset pin is first pin on PCF8574 //Only if OLED has reset pin
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//Define Buttons
#define btnBACK    1  // on the GPIO expander!
#define btnCONFIRM 0  // on the GPIO expander!
#define btnDOWN      3  // on the GPIO expander!
#define btnUP    2  // on the GPIO expander!


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//// MENU ITEMS ////

//Enumerate Menu Entry Types
enum PageType {
  PAGE_TEXT,
  PAGE_BITMAP,
  PAGE_QRCODE
};

// Add a qrCodeIndex to the MenuItem structure
struct MenuItem {
  String label;
  PageType type; // We now use the type instead of isBitmap
  union {
    int subpageIndex; // Index into subPages[] for text content
    int bitmapIndex; // Index into bitmaps[] for bitmap content
    int qrCodeIndex; // Index into qrContents[] for QR code content
  };
};

// Define the menu
MenuItem menuItems[] = {
  {"About me", PAGE_TEXT, .subpageIndex = 6},
  {"Contact Info", PAGE_TEXT, .subpageIndex = 1},
  {"Services", PAGE_TEXT, .subpageIndex = 3},
  {"Curriculum Vitae", PAGE_TEXT, .subpageIndex = 2},
  {"Awards", PAGE_TEXT, .subpageIndex = 4},
  {"Visit Website", PAGE_QRCODE, .qrCodeIndex = 0}, // QR code with URL
  {"Follow on LinkedIn", PAGE_QRCODE, .qrCodeIndex = 3}, // QR code with URL
  //{"Controls", PAGE_BITMAP, .bitmapIndex = 0},
  {"Contact Form", PAGE_QRCODE, .qrCodeIndex = 2}, // QR code with URL
  {"Star on Github", PAGE_QRCODE, .qrCodeIndex = 1}, // QR code with URL
  {"Libraries", PAGE_TEXT, .subpageIndex = 5},

  // ... more items ...
};

//// BITMAP ARRAY FOR MENU/CONTENT ////

//First item = 0, Second item = 1, etc.
//Bitmaps need to be defined as individual byte arrays, use their names to reference them here
const uint8_t* bitmaps[] = {
  CONTROLS02,
  //bitmap_01,
  //bitmap_02,
  // ... more bitmaps ...
};

//// VARIABLES AND STATE MACHINE ////

int currentScreen = 0; //Track which screen is currently active. If 0, it's the menu, if any other number load the respective subpage
int menuSelection = 0; // Track current selected item in the menu, if it's 0 it's none
int scrollDepth = 0; // Track current scrollDepth on the subpages, if it's 0 it's the top
int maxScrollDepth = 0; //Track maxScrollDepth for the current subpage
int totalMenuItems = sizeof(menuItems) / sizeof(menuItems[0]); // Get total menu items
int menuScrollDepth = 0; // Tracks the first menu item to be displayed


//bool to check whether the screen should be completely wiped to prevent artefacts
bool shouldClearScreen = false;
//bool to check whether the screen should be updated, to only do so when necessary and conserve power otherwise
bool shouldUpdateScreen = true;


const int MAX_LINES_PER_SCREEN = 4;
const int FONT_WIDTH = 7; // Width of the font in pixels, including spacing

const int MAX_CHARACTERS_PER_LINE = 20; // adjust this according to your display width

unsigned long lastButtonPress = 0; // timestamp of the last button press
unsigned int buttonPressInterval = 200; // time interval in ms between two button presses

// Scroll Depth Definitions

#define MAX_SCROLL_DEPTH 100
int** scrollDepths;


int scrollPosition = 0;



void setup() {
  //Serial.begin(9600);

  Wire.begin(SDA_PIN, SCL_PIN);

  // Setup PCF8574. It's important to make sure it starts in a known state
  pcf.begin(0x20);
  
  pcf.pinMode(btnBACK, INPUT);
  pcf.pinMode(btnCONFIRM, INPUT);
  pcf.pinMode(btnUP, INPUT);
  pcf.pinMode(btnDOWN, INPUT);

// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Allocate memory for scrollDepths
  scrollDepths = (int**)malloc(MAX_SCROLL_DEPTH * sizeof(*scrollDepths));
  for (int i = 0; i < MAX_SCROLL_DEPTH; ++i) {
    scrollDepths[i] = (int*)malloc(2 * sizeof(int));
  }


// Splash/Logo

  display.clearDisplay();
  display.fillScreen(BLACK);
  display.display();
  delay(150);

  display.drawBitmap(0,0, SplashWHITE, 128, 64, WHITE);
  display.display();
  delay(150);
  display.clearDisplay();


  display.drawBitmap(0,0, SplashBLACK, 128, 64, WHITE);
  display.display();
  delay(1000);

/*
  // Use this if you want your splash screen to feature a title
  
  // Print centered text
  int textWidth = 6 * strlen("YOUR TEXT"); // Calculate the width of the text in pixels
  int textX = max((SCREEN_WIDTH - textWidth) / 2, 0); // Calculate the x-position to center the text

  display.setTextColor(WHITE); // Set the text color to black
  display.setCursor(textX, SCREEN_HEIGHT - 8); // Position the cursor to center the text at the bottom of the screen
  display.println("YOUR TEXT");
  display.display();
  */

  delay(3000);

  display.clearDisplay();

  controlAnim(CONTROLS01, CONTROLS02, 3, 500);
  display.clearDisplay();
  delay(500);

  }

void controlAnim (const unsigned char* bitmap1, const unsigned char* bitmap2, int loops, int delayMs) {
  for (int i = 0; i < loops; i++) {
    // Display first bitmap
    display.clearDisplay();
    display.drawBitmap(0, 0, bitmap1, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    display.display();
    delay(delayMs);

    // Display second bitmap
    display.clearDisplay();
    display.drawBitmap(0, 0, bitmap2, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    display.display();
    delay(delayMs);
  }
}


void drawMenuItem(String label, bool selected, int index) {
  int y = index * 15; // 15 pixel spacing between each item
  display.setTextSize(1); // Set the text size
  display.setCursor(0, y); // Set the cursor to the top-left of the item's area
  
  if (selected) {
    // Draw item as selected (white background, black text)
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw black text on a white background
  } else {
    // Draw item as not selected (black background, white text)
    display.setTextColor(SSD1306_WHITE); // Draw white text
  }

  display.println(label);
}

void drawMenu() {
  int maxMenuIndex = menuScrollDepth + MAX_LINES_PER_SCREEN;
  if (maxMenuIndex > totalMenuItems) maxMenuIndex = totalMenuItems;
  
  for (int i = menuScrollDepth; i < maxMenuIndex; ++i) {
    // Determine whether the current menu item is selected
    bool isSelected = (i == menuSelection);
    // Call drawMenuItem() with the label, selected state, and index of the current menu item
    drawMenuItem(menuItems[i].label, isSelected, i - menuScrollDepth); // Adjusted index to fit within screen boundaries
  }
}


void drawSubPage(int subpageIndex) {
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  char* subPage = subPages[subpageIndex - 1];

  int start = 0, end = 0;
  char lineBuffer[MAX_CHARACTERS_PER_LINE + 1];
  int lastSpacePos = -1;

  int lineIndex = 0;
  int displayedLines = 0;

  while (subPage[end] != '\0' && displayedLines < MAX_LINES_PER_SCREEN) {
    while (subPage[end] != '\0' && subPage[end] != '\n' && end - start < MAX_CHARACTERS_PER_LINE) {
      if (subPage[end] == ' ') {
        lastSpacePos = end;
      }
      ++end;
    }

    if (end - start == MAX_CHARACTERS_PER_LINE && subPage[end] != ' ' && subPage[end] != '\n') {
      if (lastSpacePos != -1) {
        end = lastSpacePos;
        lastSpacePos = -1;
      } else {
        // We're in a long word that's longer than MAX_CHARACTERS_PER_LINE, so we force a break
        start = end - MAX_CHARACTERS_PER_LINE; // Move the start backwards
        lastSpacePos = -1; // Not necessary, but for clarity
      }
    }

    if (lineIndex >= scrollDepth) {
      int copyStart = (subPage[start] == ' ') ? start + 1 : start;
      strncpy(lineBuffer, &subPage[copyStart], end - copyStart);
      lineBuffer[end - copyStart] = '\0'; 
      display.setCursor(0, 15 * displayedLines);
      display.println(lineBuffer);
      displayedLines++;
    }

    if (subPage[end] == '\n') {
      ++end;
    }

    start = end;

    lineIndex++;
  }

  maxScrollDepth = lineIndex;
}


void checkButtons() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastButtonPress > buttonPressInterval) {  // Debouncing
    if (!pcf.digitalRead(btnBACK)) {
      currentScreen = 0; // Navigate back to Menu
      scrollDepth = 0;   // Reset scroll depth
      shouldClearScreen = true;
      shouldUpdateScreen = true;

        // Reset ScrollDepth coordinates
        for (int i = 0; i < MAX_SCROLL_DEPTH; ++i) {
          scrollDepths[i][0] = 0;
          scrollDepths[i][1] = 0;
        }


    } else if (!pcf.digitalRead(btnDOWN)) {
      shouldClearScreen = true;
      shouldUpdateScreen = true;
      if (currentScreen == 0) { // On Menu
        if (menuSelection < totalMenuItems - 1) {
          menuSelection++;
          if (menuSelection - menuScrollDepth >= MAX_LINES_PER_SCREEN) {
            menuScrollDepth++;
          }
        }
      } else { // On Subpage
        if (scrollDepth < maxScrollDepth - 3) { // Adjust LINE OFFSET here
          scrollDepth++;
        }
      }
      lastButtonPress = currentMillis;
    } else if (!pcf.digitalRead(btnUP)) {
      shouldClearScreen = true;
      shouldUpdateScreen = true;

      if (currentScreen == 0) { // On Menu
        if (menuSelection > 0) {
          menuSelection--;
          if (menuSelection < menuScrollDepth) {
            menuScrollDepth--;
          }
        }
      } else { // On Subpage
        if (scrollDepth > 0) {
          scrollDepth--;
        }
      }
      lastButtonPress = currentMillis;
    } else if (!pcf.digitalRead(btnCONFIRM)) {
      if (currentScreen == 0) { // On Menu
        currentScreen = menuSelection + 1;
        scrollDepth = 0; // Reset scroll depth on entering a subpage
        shouldClearScreen = true;
        shouldUpdateScreen = true;
      }
      lastButtonPress = currentMillis;
    }
  }
}




void drawBitmap(int bitmapIndex) {
  //display.clearDisplay();
  display.drawBitmap(0, 0, bitmaps[bitmapIndex], 128, 64, WHITE);
  //display.display();
}


void drawQRCode(int index) {
  uint8_t qrcodeData[qrcode_getBufferSize(2)];

  const char* qrContent = qrContents[index][0];
  const char* qrReadableText = qrContents[index][1];

  qrcode_initText(&qrcode, qrcodeData, 2, ECC_LOW, qrContent);

  int scale = min(SCREEN_WIDTH / qrcode.size, SCREEN_HEIGHT / qrcode.size);
  int qrX = max((SCREEN_WIDTH - (qrcode.size * scale)) / 2, 0);
  
  // Subtract 3 from the y-position to move the QR code up a bit
  int qrY = max((SCREEN_HEIGHT - (qrcode.size * scale)) / 2, 0) - 3;

  // Fill the screen with a white rectangle
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        display.fillRect(qrX + x * scale, qrY + y * scale, scale, scale, BLACK);
      }
    }
  }

  // Print the human-readable text under the QR code
  int textWidth = 6 * strlen(qrReadableText); // Calculate the width of the text in pixels
  int textX = max((SCREEN_WIDTH - textWidth) / 2, 0); // Calculate the x-position to center the text

  display.setTextColor(BLACK); // Set the text color to black
  display.setCursor(textX, SCREEN_HEIGHT - 8); // Position the cursor to center the text at the bottom of the screen
  display.println(qrReadableText);
}


void loop() {
  checkButtons();

  if (shouldUpdateScreen) {
    if (shouldClearScreen) {
      display.clearDisplay();
      shouldClearScreen = false;
    }

    if (currentScreen == 0) {
      // Draw the menu
      drawMenu();
    } else {
      if (menuItems[currentScreen - 1].type == PAGE_TEXT) {
        drawSubPage(menuItems[currentScreen - 1].subpageIndex);
      } else if (menuItems[currentScreen - 1].type == PAGE_BITMAP) {
        drawBitmap(menuItems[currentScreen - 1].bitmapIndex);
      } else if (menuItems[currentScreen - 1].type == PAGE_QRCODE) {
        drawQRCode(menuItems[currentScreen - 1].qrCodeIndex);
      }
    }

    // Update the display
    display.display();

    // Set the flag back to false after the screen update
    shouldUpdateScreen = false;
  }
}

