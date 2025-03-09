#include "BluetoothSerial.h"
#include "Arduino.h"
#include "ELMduino.h"

BluetoothSerial SerialBT;

#define DEBUG_PORT Serial
#define ELM_PORT   SerialBT
char protocol = ISO_15765_11_BIT_500_KBAUD;

ELM327 myELM327;
#define ELM_PIN "1234"
#define NAME "CarThing"

uint8_t mac[6] = {0x66, 0x1E, 0x32, 0x30, 0x1B, 0xD0};

void printError()
{
  Serial.print("Received: ");
  for (byte i = 0; i < myELM327.recBytes; i++)
    Serial.write(myELM327.payload[i]);
  Serial.println();
  
  if (myELM327.nb_rx_state == ELM_SUCCESS)
    Serial.println(F("\tELM_SUCCESS"));
  else if (myELM327.nb_rx_state == ELM_NO_RESPONSE)
    Serial.println(F("\tERROR: ELM_NO_RESPONSE"));
  else if (myELM327.nb_rx_state== ELM_BUFFER_OVERFLOW)
    Serial.println(F("\tERROR: ELM_BUFFER_OVERFLOW"));
  else if (myELM327.nb_rx_state == ELM_UNABLE_TO_CONNECT)
    Serial.println(F("\tERROR: ELM_UNABLE_TO_CONNECT"));
  else if (myELM327.nb_rx_state== ELM_NO_DATA)
    Serial.println(F("\tERROR: ELM_NO_DATA"));
  else if (myELM327.nb_rx_state == ELM_STOPPED)
    Serial.println(F("\tERROR: ELM_STOPPED"));
  else if (myELM327.nb_rx_state == ELM_TIMEOUT)
    Serial.println(F("\tERROR: ELM_TIMEOUT"));
  else if (myELM327.nb_rx_state == ELM_TIMEOUT)
    Serial.println(F("\tERROR: ELM_GENERAL_ERROR"));

  delay(100);
}

void setup()
{
  //pinMode(LED_BUILTIN, OUTPUT);

  DEBUG_PORT.begin(115200);
  ELM_PORT.begin(NAME, true);
  ELM_PORT.setPin(ELM_PIN);

  DEBUG_PORT.println("Attempting to connect to ELM327...");

  while (!ELM_PORT.connect(mac))
  {
    DEBUG_PORT.println("Couldn't connect to OBD scanner");
    delay(1000);
  }

  DEBUG_PORT.println("Connected to ELM327");
  DEBUG_PORT.println("Ensure your serial monitor line ending is set to 'Carriage Return'");
  DEBUG_PORT.println("Type and send commands/queries to your ELM327 through the serial monitor");
  DEBUG_PORT.println();
  //digitalWrite(LED_BUILTIN, HIGH);
  myELM327.begin(ELM_PORT, protocol);
  delay(1000);

  myELM327.sendCommand(DISP_CURRENT_PROTOCOL);
  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    Serial.print("PROTOCOL: ");
    Serial.println(myELM327.payload);
  }
  else
  {
    printError();
  }
}

float rpm = 0;

//void loop()
//{
//  rpm = myELM327.rpm();
//  if (myELM327.nb_rx_state == ELM_SUCCESS){
//    DEBUG_PORT.print("RPM: ");
//    DEBUG_PORT.println(rpm);
//  }
//  else{
//    DEBUG_PORT.println("Error reading RPM");
//  }

//  delay(1000);
//}


void loop()
{
  float tempRPM = myELM327.rpm();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    rpm = (uint32_t)tempRPM;
    Serial.print("RPM: "); Serial.println(rpm);
  }
  else
    printError();
  delay(100);
}

