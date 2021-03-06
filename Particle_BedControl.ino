/**
 * CC2500 Sample transceiver code
 * This will send a packet containing the length and "Hello"
 * Every 400ms, then go into receive mode.
 * Adapted from https://github.com/yasiralijaved/Arduino-CC2500-Library
 * Written by Zohan SonZohan@gmail.com
 * Customized for serta bed by JeffLester@gmail.com
 *
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCLK/SCK -> 13
 * CSN/SS - > 10
 */
#include "cc2500_REG.h"
#include "cc2500_VAL.h"

#include <SPI.h>

#define CC2500_IDLE    0x36      // Exit RX / TX, turn
#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC2500_SWOR    0x38
#define CC2500_TXFIFO  0x3F
#define CC2500_RXFIFO  0x3F

#define TX_TIMEOUT 10 // Timeouts are added
long previousTXTimeoutMillis = 0;
long previousMillis = 0;
long sendInterval = 2000; // in milliseconds
int led2 = D7;
int REPEATTIMES = 50;

void setup(){
  Serial.begin(9600);
   pinMode(led2, OUTPUT);
  delay(10);
  
  Particle.function("ExecBedCmd",ExecBedCmd);
  
  // Setup 
  pinMode(SS,OUTPUT);
  SPI.begin();
  digitalWrite(SS,HIGH);
  Serial.println("Initializing Wireless..");
  Spark.publish("initializing Wireless Module","");
  
  init_CC2500();
  //SendCommand(0x33);
  //SendEnd();
  //Read_Config_Regs(); 
  sendPacket(BED_FLAT);
}

void loop(){
  unsigned long currentMillis = millis();
   digitalWrite(led2, LOW);
  if(currentMillis - previousMillis > sendInterval) {
    previousMillis = currentMillis;   
    //SendCommand(0x33);
    
    
    
  }
  //listenForPacket();
}

void listenForPacket() {
  SendStrobe(CC2500_RX);
   
  // Switch MISO to output if a packet has been received or not
  WriteReg(REG_IOCFG1,0x01);
  delay(20);
  unsigned long currentMillis = millis();
  if (digitalRead(MISO)) {      
    char PacketLength = ReadReg(CC2500_RXFIFO);
    char recvPacket[PacketLength];
    Serial.print("PacketLength: ");
    Serial.println(PacketLength,DEC);
    
    if(PacketLength >= -550) {
      //Serial.println("Packet Received!");
      //Serial.print("Packet Length: ");
      //Serial.println(PacketLength, DEC);
      Serial.print("Data: '");
      for(int i = 0; i < PacketLength; i++){
        recvPacket[i] = ReadReg(CC2500_RXFIFO);
        Serial.print(recvPacket[i], HEX);
        Serial.print("_");
      }
      Serial.println("' ");
      // Print quality information
      byte rssi = ReadReg(CC2500_RXFIFO);
      byte lqi = ReadReg(CC2500_RXFIFO);
      Serial.print("RSSI: ");
      Serial.println(rssi);
      Serial.print("LQI: ");
      Serial.println(lqi);
    }
    
    // Make sure that the radio is in IDLE state before flushing the FIFO
    // (Unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point) 
    SendStrobe(CC2500_IDLE);
    // Flush RX FIFO
    SendStrobe(CC2500_FRX);
  } else {
    
  }
}

void sendPacket(char commandCode) {
  
  WriteReg(REG_IOCFG1,0x06);
  // Make sure that the radio is in IDLE state before flushing the FIFO
  SendStrobe(CC2500_IDLE);
  // Flush TX FIFO
  SendStrobe(CC2500_FTX);
  // prepare Packet
  int length = 3;
  unsigned char packet[length];
  // First Byte = Length Of Packet
  //packet[0] = length;
  packet[0] = 0x02; // packet length is 2 
  packet[1] = 0x5F; // same for all packets
  packet[2] = commandCode; // keypress code

  
  // SIDLE: exit RX/TX
  SendStrobe(CC2500_IDLE);
  
  Serial.println("Transmitting ");
  for(int i = 0; i < length; i++)
  {	  
    // SendStrobe(packet[i]);
      WriteReg(CC2500_TXFIFO,packet[i]);
  }
  // STX: enable TX
  SendStrobe(CC2500_TX);
  // Wait for GDO0 to be set -> sync transmitted
  previousTXTimeoutMillis = millis();
  while (!digitalRead(MISO) && (millis() - previousTXTimeoutMillis) <= TX_TIMEOUT) {
     //Serial.println("GDO0 = 0");
  }
   
  // Wait for GDO0 to be cleared -> end of packet
  previousTXTimeoutMillis = millis();
  while (digitalRead(MISO) && (millis() - previousTXTimeoutMillis) <= TX_TIMEOUT) {
      //Serial.println("GDO0 = 1");
  }
  Serial.println("Finished sending");
  SendStrobe(CC2500_IDLE);
}

void WriteReg(char addr, char value){
  digitalWrite(SS,LOW);
  
  while (digitalRead(MISO) == HIGH) {
    };
    
  SPI.transfer(addr);
  delay(10);
  SPI.transfer(value);
  digitalWrite(SS,HIGH);
}

char ReadReg(char addr){
  addr = addr + 0x80;
  digitalWrite(SS,LOW);
  while (digitalRead(MISO) == HIGH) {
    };
  char x = SPI.transfer(addr);
  delay(10);
  char y = SPI.transfer(0);
  digitalWrite(SS,HIGH);
  return y;  
}

char SendStrobe(char strobe){
  digitalWrite(SS,LOW);
  
  while (digitalRead(MISO) == HIGH) {
  };
    
  char result =  SPI.transfer(strobe);
  digitalWrite(SS,HIGH);
  delay(10);
  return result;
}
void SendEnd(){
  SendStrobe(0x39);
}
void SendCommand(char BedCommand){
    
  SendStrobe(0x7E); // Start
  SendStrobe(0xFE); // 8 packetes
  SendStrobe(0xFE); 
  SendStrobe(0xFE); 
  SendStrobe(0xFE); 
  SendStrobe(0xFE); 
  SendStrobe(0xFE); 
  SendStrobe(0xFE); 
  SendStrobe(0xFE); 

  SendStrobe(0x7F);
  SendStrobe(0x02);  
  SendStrobe(0x5F); 
  SendStrobe(BedCommand); 
  SendStrobe(REG_MARCSTATE); 

}

void init_CC2500(){
  SendStrobe(0x30); 
  delay(10);
  WriteReg(REG_PARTNUM,0x00);
  WriteReg(REG_VERSION,0xFF);
  
//  Serial.println("REG_IOCFG0 ");
  WriteReg(REG_IOCFG0,0x06); //02 06
  
//  Serial.println("REG_PKTLEN ");
  WriteReg(REG_PKTLEN,0x03); //06 03
  
//  Serial.println("REG_PKTCTRL1 ");
  WriteReg(REG_PKTCTRL1,0x84);
//  Serial.println("REG_PKTCTRL0 ");
  WriteReg(REG_PKTCTRL0, 0x04);
//  Serial.println("REG_FSCTRL1 ");
  WriteReg(REG_FSCTRL1,0x09);
//  Serial.println("REG_FREQ2 ");
  WriteReg(REG_FREQ2,0x5C);
//  Serial.println("REG_FREQ1 ");
  WriteReg(REG_FREQ1,0x4E);
//  Serial.println("REG_FREQ0 ");
  WriteReg(REG_FREQ0,0xC4);
//  Serial.println("REG_MD ");
  WriteReg(REG_MDMCFG4,0x2A);
//  Serial.println("REG_MDMCFG3 ");
  WriteReg(REG_MDMCFG3,0x3B);
///  Serial.println("REG_MDMCFG2 ");
  WriteReg(REG_MDMCFG2,0x73);
//  Serial.println("REG_MDMCFG1 ");
  WriteReg(REG_MDMCFG1,0xD2);
//  Serial.println("REG_MDMCFG0 ");
  WriteReg(REG_MDMCFG0,0xF8);
//  Serial.println("REG_DEVIATN ");
  WriteReg(REG_DEVIATN,0x01);
//  Serial.println("REG_MCSM2 ");
  WriteReg(REG_MCSM2,0x07);
//  Serial.println("REG_MCSM1 ");
  WriteReg(REG_MCSM1,0x30);
//  Serial.println("REG_MCSM0 ");
  WriteReg(REG_MCSM0,0x18);
//  Serial.println("REG_FOCCFG ");
  WriteReg(REG_FOCCFG,0x1D);
//  Serial.println("REG_BSCFG ");
  WriteReg(REG_BSCFG,0x1C);
//  Serial.println("REG_AGCCTRL2 ");
  WriteReg(REG_AGCCTRL2,0xC7);
//  Serial.println("REG_AGCCTRL1 ");
  WriteReg(REG_AGCCTRL1,0x00);
//  Serial.println("REG_AGCCTRL0 ");
  WriteReg(REG_AGCCTRL0,0xB2);
//  Serial.println("REG_FREND1 ");
  WriteReg(REG_FREND1,0xB6);
//  Serial.println("REG_FREND0 ");
  WriteReg(REG_FREND0,0x10);
//  Serial.println("REG_FSCAL3 ");
  WriteReg(REG_FSCAL3,0xCA);
//  Serial.println("REG_FSCAL0 ");
  WriteReg(REG_FSCAL0,0x11);
//  Serial.println("REG_SYNC1 ");
  WriteReg(REG_SYNC1,0x7A);
//  Serial.println("REG_SYNC0 ");
  WriteReg(REG_SYNC0,0xA7);
//  Serial.println("REG_CHANNR ");
  WriteReg(REG_CHANNR,0x00);
  

    SendStrobe(0x7E); // Start
    SendStrobe(0xFE); // 8 packetes
    SendStrobe(0xFE); 
    SendStrobe(0xFE); 
    SendStrobe(0xFE); 
    SendStrobe(0xFE); 
    SendStrobe(0xFE); 
    SendStrobe(0xFE); 
    SendStrobe(0xFE); 
    
    SendStrobe(0x3B); 
    



}

void Read_Config_Regs(void){ 
  Serial.println(ReadReg(REG_IOCFG2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_IOCFG1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_IOCFG0),HEX);
   delay(10);
 Serial.println(ReadReg(REG_FIFOTHR),HEX);
   delay(10);
  Serial.println(ReadReg(REG_SYNC1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_SYNC0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PKTLEN),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PKTCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PKTCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_ADDR),HEX);
   delay(10);
  Serial.println(ReadReg(REG_CHANNR),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREQ2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREQ1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREQ0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG4),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG3),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MDMCFG0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_DEVIATN),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MCSM2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MCSM1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_MCSM0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FOCCFG),HEX);
   delay(10);

  Serial.println(ReadReg(REG_BSCFG),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCCTRL2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_WOREVT1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_WOREVT0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_WORCTRL),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREND1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FREND0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL3),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSCAL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_RCCTRL1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_RCCTRL0),HEX);
   delay(10);
  Serial.println(ReadReg(REG_FSTEST),HEX);
   delay(10);
  Serial.println(ReadReg(REG_PTEST),HEX);
   delay(10);
  Serial.println(ReadReg(REG_AGCTEST),HEX);
   delay(10);
  Serial.println(ReadReg(REG_TEST2),HEX);
   delay(10);
  Serial.println(ReadReg(REG_TEST1),HEX);
   delay(10);
  Serial.println(ReadReg(REG_TEST0),HEX);
   delay(10);
 /*
  Serial.println(ReadReg(REG_PARTNUM),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_VERSION),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_FREQEST),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_LQI),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RSSI),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_MARCSTATE),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_WORTIME1),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_WORTIME0),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_PKTSTATUS),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_VCO_VC_DAC),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_TXBYTES),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RXBYTES),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RCCTRL1_STATUS),HEX);
   delay(1000);
  Serial.println(ReadReg(REG_RCCTRL0_STATUS),HEX);
   delay(1000);
  */
}
  









int ExecBedCmd(String command) {
    
      Spark.publish("Sending Command",command);
    
    if (command=="FLAT"){
      sendPacket(BED_FLAT);
    }
    if (command=="TVPC"){
      sendPacket(BED_TVPC);
    }
    if (command=="LOUNGE"){
      sendPacket(BED_LOUNGE);
    }
    if (command=="ZEROG"){
      sendPacket(BED_ZEROG);
    }
    if (command=="HEADUP"){
      for(int i=0;i<REPEATTIMES;i++){sendPacket(BED_HEADUP);}
      
    }
    if (command=="HEADDOWN"){
      for(int i=0;i<REPEATTIMES;i++){sendPacket(BED_HEADDOWN);}
      
    }
    if (command=="FOOTUP"){
        for(int i=0;i<REPEATTIMES;i++){sendPacket(BED_FOOTUP);}
      
       
    }
    if (command=="FOOTDOWN"){
      for(int i=0;i<REPEATTIMES;i++){sendPacket(BED_FOOTDOWN);}
       
    }
    digitalWrite(led2, HIGH);
    delay(250);
    digitalWrite(led2, LOW);
}










