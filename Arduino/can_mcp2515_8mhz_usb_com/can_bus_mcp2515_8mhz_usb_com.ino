// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13
// sergey, 2018

#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);

  byte bytes[4] = {0x19, 0x84, 0, 1};  // example
  int userCount = 0;
  byte speedy = CAN_500KBPS;

  // user code and setup
  do {
    userCount = Serial.readBytes(bytes, 4);
    if (userCount != 4)
      Serial.println("Please send 4 bytes: code, id bus, id speed!");
    if ((userCount == 4) && (bytes[0] != 0x19 && bytes[1] != 0x84)) {
      userCount = 0;
      Serial.println("Please send a different code!");
    }
  } while (userCount != 4);

  if (bytes[2] == 0) {
    CAN.init_CS(9);
    Serial.println("Init using CS = 9");
  } else {
    CAN.init_CS(10);
    Serial.println("Init using CS = 10");
  }

  if (bytes[3] == 0) {
    speedy = CAN_100KBPS;
    Serial.println("CAN_100KBPS");
  } else if (bytes[3] == 1) {
    speedy = CAN_125KBPS;
    Serial.println("CAN_125KBPS");
  } else if (bytes[3] == 2) {
    speedy = CAN_250KBPS;
    Serial.println("CAN_250KBPS");
  } else if (bytes[3] == 3) {
    speedy = CAN_500KBPS;
    Serial.println("CAN_500KBPS");
  } else if (bytes[3] == 4) {
    speedy = CAN_1000KBPS;
    Serial.println("CAN_1000KBPS");
  }

START_INIT:

  if (CAN_OK == CAN.begin(speedy, MCP_8MHz))  // init can bus : baudrate = 500k
  {
    Serial.println("CAN BUS Shield init ok!");
  } else {
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
    goto START_INIT;
  }
}

void loop() {
  unsigned char len = 0;
  unsigned char buf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive())  // check if data coming
  {
    CAN.readMsgBuf(&len, buf);              // read data, len: data length, buf: data buf

    unsigned short canId = CAN.getCanId();

    Serial.print(canId, HEX);
    Serial.print(" ");

    for (int i = 0; i < len; i++)  // print the data
    {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
