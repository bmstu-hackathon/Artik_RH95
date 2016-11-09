//Include required lib so Arduino can talk with the Lora Shield
#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
const int BUZZER_PIN = 5;
const int BUZZER_FREQ = 100; // 1 kHg
int reset_lora = 9;
String dataString = "";

void setup() 
{
  Serial.begin(9600);
  Serial.println("Initialize...");  
  pinMode(BUZZER_PIN, OUTPUT); 
  pinMode(reset_lora, OUTPUT);     

  // reset lora module first. to make sure it will works properly
  digitalWrite(reset_lora, LOW);   
  delay(1000);
  digitalWrite(reset_lora, HIGH); 

  if (!rf95.init()) {
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    // Need to change to 868.0Mhz in RH_RF95.cpp 
    Serial.println("Init failed!");  
  }
}

void loop()
{
  dataString="";
  if (rf95.available()) {
    Serial.println("Get new message");
    // Should be a message for us now
    Serial.print("Max message len: ");
    Serial.println(RH_RF95_MAX_MESSAGE_LEN);   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    Serial.print("Initial len: ");
    Serial.println(len);
    if (rf95.recv(buf, &len)) {
      tone(BUZZER_PIN, BUZZER_FREQ);
      delay(1000);
      noTone(BUZZER_PIN);
      RH_RF95::printBuffer("request: ", buf, len);
      Serial.print("Length: ");
      Serial.println(len);
      Serial.print("Got message: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      //make a string that start with a timestamp for assembling the data to log:
      dataString += "  :  ";
      dataString += String((char*) buf);

      // Send a reply to client as ACK
      uint8_t data[] = "200 OK";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
    }
  }
}
