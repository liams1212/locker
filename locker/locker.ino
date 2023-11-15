#include <LiquidCrystal_I2C.h>

//Viral Science www.youtube.com/c/viralscience
//RFID Solenoid Lock

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define RELAY 3 //relay pin
#define BUZZER 2 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, LOW);
  Serial.println("Put your card to the reader...");
  Serial.println();

  lcd.init();
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.backlight();
  lcd.print("Present Your Tag");
  lcd.setCursor(5,1);
  lcd.print("Please!");
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "E9 00 DA D9") //change here the UID of the card/cards that you want to give access
  {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("ACCESS APPROVED");
    lcd.setCursor(0,1);
    lcd.print("");
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Present Your Tag");
    lcd.setCursor(5,1);
    lcd.print("Please!");
  }
 
 else   {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("ACCESS DENIED");
    lcd.setCursor(0,1);
    lcd.print("");
    delay(ACCESS_DELAY);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Present Your Tag");
    lcd.setCursor(5,1);
    lcd.print("Please!");
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}
