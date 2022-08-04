#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  21  // ESP32 pin GPIO21
#define RST_PIN 22 // ESP32 pin GPIO22

MFRC522 rfid(SS_PIN, RST_PIN);    // Objeto mfrc522 enviando pines de slave select y reset

void setup() {
  Serial.begin(115200);       // inicializa comunicacion por monitor serie a 115200 bps
  SPI.begin();                // init SPI bus
  rfid.PCD_Init();            // init MFRC522

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      //MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      //Serial.print("RFID/NFC Tag Type: ");
      //Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10){     // si el byte leido es menor a 0x10
        Serial.print(" 0");                   // imprime espacio en blanco y numero cero
      }
    
      else{                                   // sino
        Serial.print(" ");                      // imprime un espacio en blanco
      }
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();

      rfid.PICC_HaltA(); // halt PICC
      //rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}
