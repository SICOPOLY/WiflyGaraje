#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "WiFly.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


#define PIN_PUERTA 7
#define PIN_ALUMBRADO 8
#define ADDR_Configuration 0



Server server(80);
char c=NULL;
char cReadBuffer[50];
int iIndex=0;

  
void setup() 
{   
    Serial.begin(9600);
    Serial.println("Setting up");
    
    // initialize the lcd
    lcd.init();   
    
    // Print a message to the LCD.
    lcd.backlight(); //Activa la retroiluminacion
    lcd.print("Starting...");
      
    //WiFly.begin();
    WiFly.beginNoDHCP();

    lcd.clear();
    lcd.print("Conectado con IP");
    lcd.setCursor(0, 1);
    lcd.print(WiFly.ip());
  
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
            
            //Encendido de alumbrado
            if(strncmp(cReadBuffer, "GET /?out=8&status=1", strlen("GET /?out=8&status=1")) == 0)
            {
                digitalWrite(PIN_ALUMBRADO, LOW);           
                Serial.print("\nLOW - Encendido\n");
                client.print("{\"status\" : \"1\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_ALUMBRADO);
                client.print("\"}");
            }
            
            //Apagado de alumbrado
            if(strncmp(cReadBuffer, "GET /?out=8&status=0", strlen("GET /?out=8&status=0")) == 0)
            {
                digitalWrite(PIN_ALUMBRADO, HIGH);           
                Serial.print("\nHIGH - Apagado\n");
                client.print("{\"status\" : \"1\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_ALUMBRADO);
                client.print("\"}");
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
