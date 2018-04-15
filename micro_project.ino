#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial mySerial(9, 10);

int sensor = 7;
int gas_value, Gas_alert_val, Gas_shut_val;
int Gas_Leak_Status;
int sms_count = 0;

void setup()
{
  pinMode(sensor, INPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2);
  delay(500);
}

void loop()
{
  CheckGas();
  CheckShutDown();
}

void CheckGas()
{

  lcd.setCursor(0, 0);
  lcd.print("Gas Scan - ON");
  Gas_alert_val = ScanGasLevel();
  if (Gas_alert_val == LOW)
  {
    SetAlert(); // Function to send SMS Alerts
  }
}

int ScanGasLevel()
{
  gas_value = digitalRead(sensor); // reads the sensor output (Vout of LM35)
  Serial.println(gas_value);
  return gas_value; // returns temperature value in degree celsius
}

void SetAlert()
{
  while (sms_count < 1) //Number of SMS Alerts to be sent
  {
    SendTextMessage(); // Function to send AT Commands to GSM module
  }
  Gas_Leak_Status = 1;
  lcd.setCursor(0, 1);
  lcd.clear();
  lcd.print("Gas Alert! SMS Sent!");
  delay(10000);
}

void CheckShutDown()
{
  if (Gas_Leak_Status == 1)
  {

    Gas_shut_val = ScanGasLevel();
    if (Gas_shut_val == HIGH)
    {

      lcd.setCursor(0, 1);
      lcd.clear();
      lcd.print("No Gas Leaking");
      delay(2000);
      lcd.clear();
      sms_count = 0;
      Gas_Leak_Status = 0;
    }
  }
}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1"); //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+911234567890\"\r");
  delay(1000);
  mySerial.println("Gas leaking!"); //the content of the message
  delay(200);
  mySerial.println((char)26); //the stopping character
  delay(1000);
  sms_count++;
}
