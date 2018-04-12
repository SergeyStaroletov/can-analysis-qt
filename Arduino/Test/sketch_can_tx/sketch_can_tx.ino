// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13
// sergey, 2018

#include <SPI.h>
#include "mcp_can.h"
#define DEBUG_MODE 1

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;


int num = 0;
int index = 0;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

    

START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS, MCP_8MHz))                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}


void loop()
{
  unsigned char len = 0;
  unsigned char buf[8];

  num = random (255);

  unsigned char stmp[8] = {'h', 'e', 'l', 'l', 'o', ' ', 's', 'e'};

  //send generated message
  CAN.sendMsgBuf(num, 0, 8, stmp);

  //delay(1000);

  index = random(8);

  stmp[index]++;
  if (stmp[index]++ > 255) stmp[index] = 0;
  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
