#include <GSM.h>
#include <LiquidCrystal.h>
#define PINNUMBER ""
GSM gsmAccess;
GSM_SMS sms;
LiquidCrystal lcd(6,8,9,10,11,12);
float sensorValue = 0;
int i_sms = 0 ;
char remoteNumber[20]="24022442";
char txtMsg[200]="Warning danger at home...!";

void setup() 
{
    lcd.begin(16, 2);
    pinMode(1, OUTPUT); // for the ventulator 
    pinMode(4, OUTPUT); // for the buzzer
    pinMode(5, INPUT); // for the MQ2 sensor 
    pinMode(13,INPUT); // for selection the working mode ( mode 0 without gsm shield ) ( mode 1 with gsm shield )
}

void loop() 
{
  lcd.clear();
  sensorValue = (analogRead(A0)*5.0/1023.0);
  float tempC = ((sensorValue ) * 100.0);  // calculation of the temperature value  
  lcd.setCursor(2,0);
  lcd.print("Temperature:");
  lcd.setCursor(4,1);
  lcd.print(tempC);
  lcd.print((char)223);
  lcd.print("C");
  
  int gsm_existence = digitalRead(13); // to select the working mode mode0 or mode1
  if((tempC > 60) || (not digitalRead(5)) ) // if there is a danger at home do {...}
  {
        digitalWrite(4,HIGH);
        digitalWrite(1,HIGH);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Warning danger!");
        delay(500);
        lcd.clear();
        digitalWrite(4,LOW);

        if((i_sms == 0) and gsm_existence) //sending the sms if there was gsm shield
        {
           boolean notConnected = true;
           while (notConnected) 
           {
              if (gsmAccess.begin(PINNUMBER) == GSM_READY) 
              {
                notConnected = false;
              } 
              else 
              {
                delay(1000);
              }
           }
           sms.beginSMS(remoteNumber);
           sms.print(txtMsg);
           sms.endSMS();
           i_sms++;
        }
  delay(100);
  }
  else 
  {
  digitalWrite(4,LOW);
  digitalWrite(1,LOW);
  }
  
  delay(1000);
}
