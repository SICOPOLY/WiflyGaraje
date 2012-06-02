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



#define PIN_ALUMBRADO 7
#define PIN_APERTURA_PUERTA 8
#define PIN_CERRADO_PUERTA 9
#define PIN_HORA 15
#define ADDR_Configuration 0



Server server(80);
char c=NULL;
char cReadBuffer[50];
int iIndex=0;
unsigned long currentMillis = millis();
long previousMillis = 0;
long interval = 5000;
char cTimestamp[8]="";
char *pTimestamp="";


  
void setup() 
{   
    Serial.begin(9600);
    Serial.println("Setting up");
    
    // initialize the lcd
    lcd.init();   
    
    // Print a message to the LCD.
    lcd.backlight(); //Activa la retroiluminacion
    lcd.print("Starting...");
    
    pinMode(PIN_ALUMBRADO, OUTPUT);
    digitalWrite(PIN_ALUMBRADO, HIGH);
    pinMode(PIN_APERTURA_PUERTA, OUTPUT);
    digitalWrite(PIN_APERTURA_PUERTA, HIGH);
    pinMode(PIN_CERRADO_PUERTA, OUTPUT);
    digitalWrite(PIN_CERRADO_PUERTA, HIGH);
      
    //WiFly.begin();
    WiFly.beginNoDHCP();
 
    server.begin();  
    
    
    delay(3000);
  
    lcd.clear();
    lcd.print("Hora:");
    lcd.setCursor(0, 1);
    lcd.print(WiFly.GetTime());
    
    delay(2000);
    
    lcd.clear();
    lcd.print("Conectado:  "); 
    lcd.print(WiFly.GetSignalQuality());
    lcd.setCursor(0, 1);
    lcd.print(WiFly.ip());
    
    delay(4000);
    lcd.clear();
    lcd.noBacklight();
   
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
            if(strncmp(cReadBuffer, "GET /?out=7&status=1", strlen("GET /?out=7&status=1")) == 0)
            {
                digitalWrite(PIN_ALUMBRADO, LOW);           
                Serial.print("\nLOW - Encendido\n");
                client.print("{\"status\" : \"1\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_ALUMBRADO);
                client.print("\"}");
                lcd.backlight();
                lcd.clear();
                lcd.print("Conectado:  "); 
                lcd.print(WiFly.GetSignalQuality());
                //delay(2000);
            }
            
            //Apagado de alumbrado
            if(strncmp(cReadBuffer, "GET /?out=7&status=0", strlen("GET /?out=7&status=0")) == 0)
            {
                digitalWrite(PIN_ALUMBRADO, HIGH);           
                Serial.print("\nHIGH - Apagado\n");
                client.print("{\"status\" : \"0\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_ALUMBRADO);
                client.print("\"}");
            }
            
            //Apertura de la puerta
            if(strncmp(cReadBuffer, "GET /?out=8&status=1", strlen("GET /?out=8&status=1")) == 0)
            {
                digitalWrite(PIN_APERTURA_PUERTA, LOW);           
                Serial.print("\nLOW - Encendido\n");
                client.print("{\"status\" : \"1\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_APERTURA_PUERTA);
                client.print("\"}");
            }
            
            //Paro de apertura de la puerta
            if(strncmp(cReadBuffer, "GET /?out=8&status=0", strlen("GET /?out=8&status=0")) == 0)
            {
                digitalWrite(PIN_APERTURA_PUERTA, HIGH);           
                Serial.print("\nHIGH - Apagado\n");
                client.print("{\"status\" : \"0\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_APERTURA_PUERTA);
                client.print("\"}");
            }
            
            //Cerrado de la puerta
            if(strncmp(cReadBuffer, "GET /?out=9&status=1", strlen("GET /?out=9&status=1")) == 0)
            {
                digitalWrite(PIN_CERRADO_PUERTA, LOW);           
                Serial.print("\nLOW - Encendido\n");
                client.print("{\"status\" : \"1\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_CERRADO_PUERTA);
                client.print("\"}");
            }
            
            //Paro de cerrado de la puerta
            if(strncmp(cReadBuffer, "GET /?out=9&status=0", strlen("GET /?out=9&status=0")) == 0)
            {
                digitalWrite(PIN_CERRADO_PUERTA, HIGH);           
                Serial.print("\nHIGH - Apagado\n");
                client.print("{\"status\" : \"0\" , \"type\" : \"plug\", \"out\" : \"");
                client.print(PIN_CERRADO_PUERTA);
                client.print("\"}");
            }
            
            //Consultar hora
            if(strncmp(cReadBuffer, "GET /?out=15&status=1", strlen("GET /?out=15&status=1")) == 0)
            {     
                pTimestamp=WiFly.GetTime();
                strncpy(cTimestamp, pTimestamp, strlen(pTimestamp)+1); //El +1 en el strlen es para que coja el final de cadena tambien en la copia
                client.print("{\"status\" : \"1\" , \"type\" : \"time\", \"value\" : \"");
                client.print(cTimestamp);
                client.print("\"}");
            }
            
             
            if(strncmp(cReadBuffer, "GET /?out=all", strlen("GET /?out=all")) == 0)
            {
                //Serial.print("\n OUT ALL\n");
                
                client.print("{\"nombre\" : \"Garaje\", ");
                client.print("\"tipo\" : \"WiFi\", ");
                client.print("\"ubicacion\" : \"Garaje\", ");
                client.print("\"ip\" : \"192.168.0.111\", ");
                client.print("\"devices\" : ");
                client.print("[{ \"type\" : \"plug\", \"name\" : \"Alumbrado\", \"out\" : \"");
                client.print(PIN_ALUMBRADO);
                client.print("\"}");
                client.print(",{ \"type\" : \"plug\", \"name\" : \"Abrir puerta\", \"out\" : \"");
                client.print(PIN_APERTURA_PUERTA);
                client.print("\"}");
                client.print(",{ \"type\" : \"plug\", \"name\" : \"Cerrar puerta\", \"out\" : \"");
                client.print(PIN_CERRADO_PUERTA);
                client.print("\"}");
                client.print(",{ \"type\" : \"time\", \"name\" : \"¿Hora?\", \"out\" : \"");
                client.print(PIN_HORA);
                client.print("\"}");
                client.print("]}");
            }
            
          
            //delay(5);
            iIndex = 0;
            cReadBuffer[iIndex] = '\0';
            client.stop();
        }
      }
    }
  }
  
  currentMillis = millis();
  
  if(currentMillis - previousMillis > interval) 
  {
      previousMillis = currentMillis;  

      lcd.backlight();
      lcd.clear();
      lcd.print("Conectado:  "); 
      lcd.print(WiFly.GetSignalQuality());
  }
}



void ShowOnLCDMessage(char* sMessage, int iSegundos)
{
    lcd.backlight();
    lcd.print(sMessage);
    
    delay(iSegundos*1000);
    lcd.clear();
    lcd.noBacklight();
}
