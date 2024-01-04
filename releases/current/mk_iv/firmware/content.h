// content.h
#ifndef CONTENT_H
#define CONTENT_H

//// SUB PAGES ////

// Define the contents of each subpage as a C-style string (char array)
// You need to ensure that the array size is enough to hold your largest subpage, including the null-terminator.
// In this example, we assume that 1000 is sufficient.

#define MAX_SUBPAGE_LENGTH 1000

char subPages[][MAX_SUBPAGE_LENGTH]  = {

    // Contact Info
    "CONTACT INFO\n\n[Your Name]\n[Your Title]\n[Your Company]\n\n[Your Email]\n[Your Phone Number]",
    // CV
    "CURRICULUM VITAE\n\nThis section can include your professional experience, education, skills, etc. Replace this text with your own CV details.",
    // Services
    "SERVICES\n\nDescribe the services you offer here. You can list various services like design, consulting, etc.",
    // Awards
    "AWARDS\n\nList your awards and recognitions here. Replace this text with details of your achievements.",
    //LIBRARIES
    "LIBRARIES\n\nThis project uses the following libraries:\n\nADAFRUIT_GFX\n1.11.5\nSoftware License Agreement (BSD License)\nCopyright (c) 2012 Adafruit Industries.\nAll rights reserved.\n\nADAFRUIT_PCF8574\n1.1.0\nSoftware License Agreement (BSD License)\nCopyright (c) 2020 Bryan Siepert for Adafruit Industries.\nAll rights reserved.\n\nADAFRUIT_SSD1306\n2.5.7\nSoftware License Agreement (BSD License)\nCopyright (c) 2012 Adafruit Industries.\nAll rights reserved.\n\nCSTRING\nStandard C++ Library\n\nWIRE\nStandard Arduino Library\n\nQRCODE\nThe MIT License (MIT)\nThis library is written and maintained by Richard Moore.\nMajor parts were derived from Project Nayuki's library.\nCopyright (c) 2017 Richard Moore\nCopyright (c) 2017 Project Nayuki",
    // About Me
    "HI,\nI'M [Your Name].\n\nI am a [Your Profession]. I work with [Types of Clients/Projects] to [Your Value Proposition].\n\nContact me at: [Your Email]",
    // ... more subpages ...
  };

#endif
