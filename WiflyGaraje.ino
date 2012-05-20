#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "WiFly.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"
//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>

//#define SALIDADIG_ON_OFF 2
#define PIN_PUERTA 7
#define PIN_ALUMBRADO 8
#define ADDR_Configuration 0
//#define SONDATEMP_PIN 0


Server server(80);
char c=NULL;
char cReadBuffer[50];
int iIndex=0;

  
void setup() 
{   
    Serial.begin(9600);
    Serial.println("Setting up");
  
    //WiFly.begin();
    WiFly.beginNoDHCP();

    Serial.print("IP: ");
    Serial.println(WiFly.ip());
  
    //Serial.print("Time: ");
    //Serial.println(WiFly.GetTime());

    server.begin();
}

void loop() 
{  
    Client client = server.available();
    if (client) 
    {
      while (client.connected()) 
      {
        if (client.available()) 
        {
          c = client.read();
          if (c != '\n')
          {
                cReadBuffer[iIndex]=c;
                iIndex++;
                cReadBuffer[iIndex]='\0';
                //Serial.println(cReadBuffer);
          }
          else
          {
            // if we've gotten to the end of the line (received a newline
            // character) and the line is blank, the http request has ended,
            // so we can send a reply
            // send a standard http response header
            //Descomentar la siguiente linea para ver lo que hemos leido
            Serial.println(cReadBuffer);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
          
            if(strncmp(cReadBuffer, "GET /?out=0&status=1", strlen("GET /?out=0&status=1")) == 0)
            {
    
            }
            
             /*
            if(strncmp(cReadBuffer, "GET /?out=all", strlen("GET /?out=all")) == 0)
            {
                //Serial.print("\n OUT ALL\n");
                
                client.print("{\"nombre\" : \"Calefaccion\", ");
                client.print("\"tipo\" : \"WiFi\", ");
                client.print("\"ubicacion\" : \"Bodega\", ");
                client.print("\"ip\" : \"192.168.0.110\", ");
                client.print("\"devices\" : ");
                client.print("[{ \"type\" : \"time\", \"name\" : \"Hora\", \"out\" : \"");
                client.print("15");
                client.print("\"}");
                client.print(",{ \"type\" : \"text\", \"name\" : \"Auto/Manual\", \"out\" : \"");
                client.print("16");
                client.print("\"}");
                client.print(",{ \"type\" : \"text\", \"name\" : \"Auto/Manual\", \"out\" : \"");
                client.print("17");
                client.print("\"}");
                client.print(",{ \"type\" : \"set\", \"name\" : \"Hora Inicio\", \"out\" : \"");
                client.print("18");
                client.print("\"}");
                client.print(",{ \"type\" : \"set\", \"name\" : \"Hora Fin\", \"out\" : \"");
                client.print("19");
                client.print("\"}");
                client.print(",{ \"type\" : \"time\", \"name\" : \"Hora Inicio\", \"out\" : \"");
                client.print("20");
                client.print("\"}");
                client.print(",{ \"type\" : \"time\", \"name\" : \"Hora Fin\", \"out\" : \"");
                client.print("21");
                client.print("\"}");
                //client.print(",{ \"type\" : \"text\", \"name\" : \"Estado\", \"out\" : \"");
                client.print("1");
                client.print("\"}");         
                //client.print(",{ \"type\" : \"plug\", \"name\" : \"ON/OFF\", \"out\" : \"");
                client.print("2");
                client.print("\"}");
                client.print("]}");
            }
            */
          
            //delay(5);
            iIndex = 0;
            cReadBuffer[iIndex] = '\0';
            client.stop();
        }
      }
    }
  }
}
