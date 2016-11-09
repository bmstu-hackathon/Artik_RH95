// Singleton instance of the radio driver
#include <SPI.h>
#include <RH_RF95.h>
RH_RF95 rf95;

float data;
String datastring="";
char databuf[] = "buf";
uint8_t dataoutgoing[251];
void setup() 
{
  Serial.begin(9600);
  if (!rf95.init()) {
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    // Need to change to 868.0Mhz in RH_RF95.cpp 
    Serial.println("init failed");
  }
}

void loop()
{
  
  strcpy((char *) dataoutgoing, databuf);
  Serial.println(databuf);
  Serial.println(sizeof(dataoutgoing));
  rf95.send(dataoutgoing, sizeof(dataoutgoing));
  rf95.waitPacketSent();
  
  // Now wait for a reply
  uint8_t indatabuf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(indatabuf);

  if (rf95.waitAvailableTimeout(3000)) { 
    // Should be a reply message for us now   
    if (rf95.recv(indatabuf, &len)) {
      // Serial print "got reply:" and the reply message from the server
      Serial.print("got reply: ");
      Serial.println((char*)indatabuf);
    } else {
      Serial.println("No message");
    }
  } else {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(2000);
}
