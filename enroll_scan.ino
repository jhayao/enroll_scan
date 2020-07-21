/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial3);

uint8_t id,scans;
int s,m;
void setup()  
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("Adafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
    Serial.println("Enter Number :###");
    id= readnumber();
    Serial.println(id);
    if(id==1){
      finger.getTemplateCount();
      if(finger.templateCount > 0){
      Serial.print("Scanning");
        while(scan());
        Serial.print("DONE");
      }
      else
      {
        Serial.println("Empty Templates");
      }
    }
    else if(id==2){
        enroll();
        
    }
    else if(id==3){
        Serial.println("Enter ID");
        int num=0;
        scans=readnumber();
        match(scans);
    }
    else if(id==4){
        Serial.println("Deleting");
        finger.emptyDatabase();
        Serial.println("Done");
        finger.getTemplateCount();
        Serial.print(finger.templateCount);
    }
    else if(id == 5){
        Serial.print("Template Count:" );
        finger.getTemplateCount();
        Serial.print(finger.templateCount);
    }
    
//    switch(id)
//    {
//      case 0:
//        return;
//        break;
//      case 1:
//        Serial.print("Scanning");
//        while(scan());
//        Serial.print("DONE");
//        break;
//      case 2: 
//        enroll();
//        break;
//     
//      case 4:
//        Serial.println("Deleting");
//        finger.emptyDatabase();
//        break;
//
//      case 5: 
//        Serial.println(finger.templateCount);
//      case 3: 
//        Serial.println("Enter ID");
//        int num=0;
//        while (num == 0) {
//        while (! Serial.available());
//        scans = Serial.parseInt();
//        }
//        match(scans);
//        break;
//      default:
//        Serial.print("Invalid Input");
//        break;
//    }
}
uint8_t match(int x){
  loadFlashMemory(x);
  
  getFingerprintEnroll2();
}
uint8_t scan(){
   s= finger.getImage();
    switch (s) {
    case FINGERPRINT_OK:
      Serial.println("\nImage taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.print(".");
      return s;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return s;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return s;
    default:
      Serial.println("Unknown error");
      return s;
  }
  
  return getFingerprintID();
}
uint8_t enroll(){
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("scanning");
//      delay(500);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}

uint8_t getFingerprintID() {
  

  // OK success!
  
  s = finger.image2Tz();
  switch (s) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return s;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return s;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return s;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return s;
    default:
      Serial.println("Unknown error");
      return s;
  }
  
  // OK converted!
  Serial.print("==> ");
  s= finger.fingerFastSearch();
  if (s == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (s == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return s;
  } else if (s == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return s;
  } else {
    Serial.println("Unknown error");
    return s;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  delay(500);
  return false;
}

uint8_t loadFlashMemory( uint16_t scaned) {  
  Serial.print("==> Load FlashMemory "); Serial.print(id); Serial.println(" to CharBuffer");
  m = finger.loadModel(scaned);   // ==> Move Templete to CharBuffer 1
  switch (m) {
    case FINGERPRINT_OK:
      Serial.print("Template "); Serial.print(scaned); Serial.println(" loaded");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return m;
    default:
      Serial.print("[ UNKNOWN ERROR ]"); Serial.println(m);
      return m;
  }
}

uint8_t getFingerprintEnroll2() {

  m = -1;
  Serial.print("Waiting for valid finger as ID #"); Serial.println(scans);
  while (m != FINGERPRINT_OK) {
    m = finger.getImage();
    switch (m) {
    case FINGERPRINT_OK:
      Serial.println("\nImage taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(",");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  m = finger.image2Tz(0x02);              //   Simpan di Char Buffer 02 (0x02)
  if (m != FINGERPRINT_OK)  return -1;
  Serial.println("Succes image2Tz");
    
   m = finger.matchUpload();  
  if (m != FINGERPRINT_OK)  
  {
    Serial.print("\t ===??? [ DOESN'T Match ] as ID #");Serial.println(scans); Serial.println();
    delay(500);
    return -1;  // fingerprint doesn't match
  }
  
  else
  {
    Serial.print("\t ===>>> [ MATCH !!! ] as ID #");Serial.println(scans);
    delay(500);
  }
  Serial.println();
  return finger.fingerID; 
}
