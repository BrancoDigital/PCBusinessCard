# PCBusinessCard
A digital businesscard using an ESP01, PCF8574 and SSD1306 OLED.


## PURPOSE

## FEATURES

## PROJECT STRUCTURE

The project is organized as follows:

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

### LIBRARIES
This project uses the following libraries:

- Wire (Standard Arduino Library)
- [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit_PCF8574](https://github.com/adafruit/Adafruit_PCF8574)
- cstring (Standard C++ Library)
- [QRCode](https://github.com/ricmoo/qrcode/)

Each library is licensed under its own terms. See the individual library files for more details.

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
