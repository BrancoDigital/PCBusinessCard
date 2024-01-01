# PCBusinessCard
A digital businesscard using an ESP01, PCF8574 and SSD1306 OLED.


## PURPOSE

The main purpose of this project is to realize a novel idea for a business card that I had floating around for a while.
The secondary purpose was for me to get familiarized with microcontroller programming, circuit design and pcb development.
Please keep the experimental nature of this project in mind if you build your own. 
Suggestions and improvements are welcome!

## IDEA

The idea behind this project is to find a novel way to introduce myself and my skills/services to potential clients.
Since most introductions happen virtually nowadays, I was looking to find a more interesting and personal way to get in touch. 
Business Cards have been around for generations and are unmatched in their simplicity, but they are also pretty boring and old school.
So I decided to combine my growing interest in electronics and microcontroller programming and industrial design into something that I felt more closely represents my current interests and skills.

In January of 2023, I started working with microcontrollers and electronics for the first time and quickly fell down that rabbithole.
As a consequence, I saw a lot of really cool ideas one can build with simply electronics modules. After hearing about the concept of a PCB based business card for the first time, the idea stuck and I really wanted to make my own "digital businesscard".

Since my skillset in this area was (and is) relatively limited, and I am not much of a programmer, I decided to set some requirements and limitations for this project. I wanted to push myself out of my comfort zone and learn something new, while pushing towards a relatively well defined goal.

As such, I came up with the following rough guidelines:

REQUIREMENTS:
- Primary function: The microdevice still needs to act as a business card, i.e. include name, contact data, position, etc.
- Interactivity: the business card should be as interactive as possible within its inherent limitations
- Enhanced functionality: the business card should utilize its nature as a microdevice to enhance the content and interaction
- Ease of use: The business card needed to be intuitive enough to use after the initial learning curve
- Readability: The business card needed to display content in a way that's easy to digest (within hardware limitations)

LIMITATIONS:
- Skillset: While part of the goal was to expand my knowledge, I still needed to be realistic about my capabilities
- Hardware: The chosen hardware needed to be capable of covering my requirements, despite low processing power and storage space
- Power Supply: The hardware needed to be powered in a way that didn't require an external power supply
- Cost per Unit: The cost per unit needed to be relatively low in order to ensure I could produce enough units to give away

FUTURE CONSIDERATIONS:
- While this is not the case at the moment, future iterations might include a way to keep the business card up to date, to reflect newly acquired skills or milestones. This would make it the "first" business card that keeps itself updated. In order to enable this functionality, I will need to use the ESP01's WIFI & WPS capabilities, but this might have severe implications to the current power supply solution.


## FEATURES

The current version of the PCBusiness Card (MKIV) is built around an ESP01 module, using a PCF8574 I/O Expander to address an SSD1306 black and white OLED display, as well as 4 push buttons for user interaction.

The code and chosen display type supports different types of content such as plain text, bitmaps as byte arrays and QR codes.
The content is accesssed through a menu system, controlled through the push buttons included on the PCB.

- Splash Screen Animation: Displays a logo bitmap at startup
- Control Animation: Displays two blinking bitmaps highlighting the push buttons on the PCB
- Menu System: Overview of all the content and subpages
- Content Types: Scrollable plain text, Bitmaps and QR codes which are generated on the fly

Power requirements and energy conservation solution:

The ESP01 microcontroller usually requires a voltage of 3.3V, and the current consumption can vary between 70 mA to over 200 mA.
This can rise significantly during boot up or WIFI transmissions, and peak around 300-400 mA. 

The OLED type used (SSD1306) increases this requirement by another 10-20 mA (or more) in active current, while also requiring a voltage of around 3.3V. 

Since I wanted to keep the business card as compact as possible, I needed to find a solution to these requirements.
The initial prototype (MK0) was powered through a breadboard connector, so this was not an issue during initial development. 
The first handheld version (MKI) was powered using 2xAA batteries, which in hindsight was overkill, but was an important step to figuring out subsequent iterations.

The solution to the power draw issue was ultimately achieved through a combination of software and hardware implementations.
In order to keep the processing load as low as possible and preserve the battery lifetime on the software side, two main solutions were implemented:

- Disable ESP01 WIFI during startup: This has helped reduce the initial power draw of the microcontroller significantly
- Reduce the processing cycles: By only updating the display after a button press, the processing load was further reduced significantly

Those two software solutions combined have helped reduce the power requirements tremendously. Since the SSD1306 OLED has an internal buffer we can write our updated display data to, there is no need to constantly push display updates every processing cycle. This allows us to run an "empty" loop on every processing cycle, effectively reducing the power draw of the ESP01 as much as possible.

Using the ESP01's Deep Sleep Mode between cycles would have theoretically reduced the power draw even more, however this was not practical to implement since the ESP01 does not have a built-in wake up timer or GPIO for this purpose. 

On the hardware side, I quickly discovered that the decoupling capacitor pairs for the ESP01 and the PCF8574 module were enough to maintain a sufficiently high current, especially during boot up. 

These combined solutions above allow us to run the entire circuit for its intended purpose on 3V from a single CR2032.
I have nevertheless initially added 3xCR2032 holders in the first few iterations, reducing this to 2xCR2032 with the MKIV to further reduce hardware cost and waste.

## CODEBASE STRUCTURE




## PROJECT STRUCTURE

This structure is outdated and will need to be updated.

```
PCBusinessCard Project Folder       // main project folder
│
├── firmware                        // main firmware folder
│   ├── ver_1                       // version specific folder
│   │   ├── main.ino                // main script
│   │   ├── libraries               // own libraries - Please note: you will need to acquire third party libraries yourself, see links down below
│   │   │   └── library_name_subfolder // subfolder for own libraries (if applicable)
│   │   ├── assets                  // assets folder 
│   │   │  └── asset_type_subfolder // asset type subfolder (images, sound effects, etc.)
│   │   └── ...                     // other subfolders
│   │
│   ├── ver_2                       // additional version folders
│   └── ver_3
└── hardware                        // main hardware folder
    ├── mk_i                        // version specific folder
    │   ├── schematics              // schematics for this version
    │   ├── pcb                     // pcb layout files
    │   ├── manufacturing           // gerber exports, drill files, BOM, pick and place, etc.
    │   └── ...                     // other subfolders
    │
    ├── mk_ii                       // additional version folders
    └── mk_iii
```

## FIRMWARE

### MAIN CODE

### LIBRARIES
This project uses the following libraries:

- Wire (Standard Arduino Library)
- [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit_PCF8574](https://github.com/adafruit/Adafruit_PCF8574)
- cstring (Standard C++ Library)
- [QRCode](https://github.com/ricmoo/qrcode/)

Each library is licensed under its own terms. See the individual library files for more details.

Bitmap Byte Arrays converted using "Image to byte array online converter" by Renzo Mischianti
- [Image to Byte Array Online Converter](https://mischianti.org/images-to-byte-array-online-converter-cpp-arduino/)

## HARDWARE

### SCHEMATICS

### PCB LAYOUT

### MANUFACTURING

## RELEASES
Each release of this project contains a self-contained firmware and hardware configuration. These configurations should function "out of the box" in most cases, but please be aware that compatibility between different releases is not guaranteed. Modifications may be necessary if you're attempting to integrate aspects from different releases.

Regarding licensing, it's crucial to note that each release may be subject to different licensing terms. Always review the license accompanying each release, as the terms may vary. Typically, the most recent release is published primarily for reference and might not immediately come with a license. Always check the license status of each release to understand the permissions and restrictions that apply.

## DISCLAIMER
The code and designs in this repository are provided "as is", without warranty of any kind, express or implied. They are intended for educational and experimental purposes only, and should not be used in critical or unsafe contexts.

The author(s) and contributors to this repository cannot be held liable for any damages arising from the use of this code and designs, including, but not limited to: 

1. direct, indirect, special, incidental, consequential, or punitive damages; 
2. lost profits, lost revenue, loss of use or data, or costs of substitute goods or services; 
3. claims by third parties, or other similar costs.

Always use good judgment and take safety precautions when experimenting with electronics. This project is not guaranteed to be free of errors or other defects, and should be used responsibly. 

Please remember that this project is open source, and contributions are welcome. However, please respect the terms of the included license(s) and the licenses of any included third-party libraries.
