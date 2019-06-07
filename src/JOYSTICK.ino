// nrf24_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_NRF24 class. RH_NRF24 class does not provide for addressing or
// reliability, so you should only use RH_NRF24 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example nrf24_server.
// Tested on Uno with Sparkfun NRF25L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <SPI.h>
#include <RH_NRF24.h>

// Singleton instance of the radio driver
RH_NRF24 nrf24(9,10);

uint8_t data[5] ; 
const uint8_t buffer_size = sizeof(data);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };//two pipes communication 

///////////////////////Joystick Left///////////////////////////////////////
const int potInPinLX = A0;// X axis input
const int potInPinLY = A1;//Y axis input

int potInPinLXval = 0; 
int potInPinLYval = 0;       
int InPinSWval = 0;
////////////////////////Joystick Right//////////////////////////////////////
const int potInPinRX = A2;// X axis input
const int potInPinRY = A3;//Y axis input

int potInPinRXval = 0; 
int potInPinRYval = 0;  
////////////////////////Potentiometer///////////////////////////////////////
const int potInPinM = A4;
int potInPinMval = 0; 


void setup() 
{
  Serial.begin(115200);
  while (!Serial) 
    ; // wait for serial port to connect. Needed for Leonardo only

  pinMode(potInPinLX,INPUT );
  pinMode(potInPinLY,INPUT );
  pinMode(potInPinRX,INPUT );
  pinMode(potInPinRY,INPUT );
  pinMode(potInPinM,INPUT );

  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(3))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");    
}


void loop()
{
  Serial.println("Sending to nrf24_server");

  ////////////////////////////////////////////////////////////////////////   
  potInPinLXval = analogRead(potInPinLX); 
  delay(15);
  data[0] = potInPinLXval/10;
  //////////////////////////////////////////////////////////////////////// 
  potInPinLYval = analogRead(potInPinLY); 
  delay(15);
  data[1] = potInPinLYval/10;//scaling 
  //////////////////////////////////////////////////////////////////////// 
  potInPinRXval = analogRead(potInPinRX); 
  delay(15);
  data[2] = potInPinRXval/10;
  //////////////////////////////////////////////////////////////////////// 
  potInPinRYval = analogRead(potInPinRY); 
  delay(15);
  data[3] = potInPinRYval/10;//scaling 
  //////////////////////////////////////////////////////////////////////// 
  potInPinMval = analogRead(potInPinM); 
  delay(15);
  data[4] = potInPinMval/10;//scaling  
  /////////////////////////////////////////////////////////////////////////

  // Send a message to nrf24_server
//  uint8_t data[] = "Hello Worffffffffld!";
  
  if ( nrf24.send(data, sizeof(data))){
  Serial.print("data[0]=");
  Serial.print(data[0]);
  Serial.print(" data[1]=");
  Serial.print(data[1]);
  Serial.print(" data[2]=");
  Serial.print(data[2]);
  Serial.print(" data[3]=");
  Serial.print(data[3]);
  Serial.print(" data[4]=");
  Serial.println(data[4]);
  }
  else {
    Serial.println ("Failed");
  }

  nrf24.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (nrf24.waitAvailableTimeout(500))
  { 
    // Should be a reply message for us now   
    if (nrf24.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("recv failed");
    }    
  }
  else
  {
    Serial.println("No reply, is nrf24_server running?");
  }  
  delay(400);
}