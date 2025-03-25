#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SCK_PIN 18
#define MISO_PIN 16
#define MOSI_PIN 17
#define SS_PIN 14

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

bool validateUser(fs::FS &fs, const char *path, const char *userToken) {
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return false;
    }

    char fileToken[64];  // Buffer for reading tokens
    int i = 0;

    while (file.available()) {
        char ch = file.read();

        // Check for token delimiter (newline assumed)
        if (ch == '\n' || ch == '\r' || i >= sizeof(fileToken) - 1) {
            fileToken[i] = '\0';  // Null-terminate token

            // Compare with userToken
            if (strcmp(userToken, fileToken) == 0) {
                Serial.println("MATCH FOUND!");
                file.close();
                return true;  // Found a match, exit early
            }

            // Reset buffer for next token
            i = 0;
            continue;
        }

        // Store character in buffer
        fileToken[i++] = ch;
    }

    file.close();
    Serial.println("No match found.");
    return false;  // No match found in the entire file
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // Wait for Serial to initialize
  }

  // Initialize SPI bus
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, LOW);

  // Initialize SD card
  //Serial.println(SD.begin(SS_PIN));
  if (!SD.begin(SS_PIN)) {
    Serial.println("Card Mount no worky!!!");
    //testSPI();
    return;
  }
  Serial.println("Card Mount worked!!!");

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  Serial.println("Create successful directory:");
  createDir(SD, "/testdir");
  Serial.println("Demonstrate duplicate directory creation:");
  createDir(SD, "/testDir");
  removeDir(SD, "/testdir");
  //if two duplicate directories are made the creatDir() function simply ensures the directory exists
  //it does not overwrite anything inside or erase files

  writeFile(SD, "/manualTesting.txt", "I assume this is ");
  appendFile(SD, "/manualTesting.txt", "working!\n");
  readFile(SD, "/manualTesting.txt");
  deleteFile(SD, "/manualTesting.txt");

  writeFile(SD, "/testValidateUser.txt", "axy657\nbnh123\n");
  validateUser(SD, "/testValidateUser.txt", "plyOOOO");
  validateUser(SD, "/testValidateUser.txt", "bnh123");
  deleteFile(SD, "/testValidateUser.txt");
}

void loop() {

}
