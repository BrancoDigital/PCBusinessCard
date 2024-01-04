// qr_codes.h
#ifndef QR_CODES_H
#define QR_CODES_H

//// QR CODE ARRAY ////

const char* qrContents[][2] = {
  { "https://example.com", "Your Website" },
  { "https://github.com/yourrepository", "Your GitHub Repo" },
  { "https://yourcontactform.com", "Your Contact Form" },
  { "https://linkedin.com/in/yourprofile", "Your LinkedIn Profile" },
};

int numberOfQRCodes = sizeof(qrContents) / sizeof(qrContents[0]);

#endif
