
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 38
#define RST_PIN 39
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte checkArray[9][4]={
  {117,18,113,28},
  {101,90,64,28},
  {101,42,213,28},
  {101,200,207,28},
  {101,222,168,28},
  {117,18,28,28},
  {117,9,121,28},
  {117,15,56,28},
  {42,32,60,121}
 };

void setup() 
{ 
  
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  pinMode(52, OUTPUT); 
  pinMode(51, OUTPUT);
  rfid.PCD_Init(); // Init MFRC522 

  Serial.println("starting rock");

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  //pinMode(SS_PIN, OUTPUT);
  //digitalWrite(SS_PIN, LOW);

  //rfid.PCD_SetAntennaGain(rfid.RxGain_max);
}
 
void loop() 
{
  delay(100);

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  Serial.print(F("In hex: "));
  for (int i = 0; i < 4; i++)
  {
    Serial.print(rfid.uid.uidByte[i]);
    Serial.print(" ");
  }
  Serial.println();

  for (int i = 0; i < 9; i++)
  {

    if (rfid.uid.uidByte[0] == checkArray[i][0] &&
      rfid.uid.uidByte[1] == checkArray[i][1] &&
      rfid.uid.uidByte[2] == checkArray[i][2] &&
      rfid.uid.uidByte[3] == checkArray[i][3])
      {
        Serial.println("Success!");
        //flashLights();
        break;
      }
    
  }

  // Halt PICC
  //rfid.PICC_HaltA();

  // Stop encryption on PCD
  //rfid.PCD_StopCrypto1();
  return;

}
