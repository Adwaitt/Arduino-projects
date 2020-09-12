

#include <HX711_ADC.h>
HX711_ADC LoadCell(9, 10);
#define btn5 A5

#include <Wire.h>//For I2C distplay 

#include <LiquidCrystal_I2C.h>//For LCD

LiquidCrystal_I2C lcd(0x27, 16, 2);//setting the mode for I2C




#include "DHT.h"
#define dht_Signal A0//pin for dht sensor
#define btn1 A1//pin for dht sensor
DHT dht;;//object for dht sensor 
float *Z;float *P;


#define Airpin A10//pin for air quality sensor
#define btn3 A2
//from here

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define SO 8
#define btn2 A3
float i;
int R,G,B;

//till here is for color sensing

#define analogIn A9//for pH
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

void setup(){lcd.begin();//for turning on the LCD for read/write mode
             lcd.backlight();//for turning on the backlight LCD 
             pinMode(btn1,INPUT);//button which initializes the dht function
             pinMode(btn2,INPUT);//button which initializes the colour sensing function
             pinMode(btn3,INPUT);//button which intitializes the air quality function
             pinMode(btn5,INPUT);//button which intitializes the wght function
             pinMode(S0,OUTPUT);//declaration of pinmode
             pinMode(S1,OUTPUT);//decalration of pinmode
             pinMode(S2,OUTPUT);//declaration of pinmode
             pinMode(S3,OUTPUT);//declaration of pinmode
             pinMode(SO,INPUT);//declaration of pinmode
             digitalWrite(S0,HIGH);//setting the frequency to 20%
             digitalWrite(S1,LOW);
             LoadCell.begin(); // start connection to HX711
             LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
             LoadCell.setCalFactor(428.8);// calibration factor for load cell => strongly dependent on your individual setup
             dht.setup(A0);
             Serial.begin(9600);}

void comp_color(){lcd.clear();lcd.setCursor(0,0);
                 int y1R,y1G,y1B;//for first sample
                 int y2R,y2G,y2B;//for second sample
                 digitalWrite(S2,LOW);digitalWrite(S3,LOW);
                 y1R=pulseIn(SO,LOW);//Reading Red colour of first sample
                 digitalWrite(S2,HIGH);digitalWrite(S3,HIGH);
                 y1G=pulseIn(SO,LOW);//Reading Green colour of first sample
                 digitalWrite(S2,LOW);digitalWrite(S3,HIGH);
                 y1B=pulseIn(SO,LOW);//Reading Blue colour of first sample
                 lcd.print(y1R);
                 lcd.print(",");
                 lcd.print(y1G);
                 lcd.print(",");
                 lcd.print(y1B);
                 delay(3000);lcd.clear();
                 Serial.println("Enter sample 2");
                 delay(3000);
                 digitalWrite(S2,LOW);digitalWrite(S3,LOW);
                 y2R=pulseIn(SO,LOW);//Reading Red colour of second sample
                 digitalWrite(S2,HIGH);digitalWrite(S3,HIGH);
                 y2G=pulseIn(SO,LOW);//Reading Green colour of second sample
                 digitalWrite(S2,LOW);digitalWrite(S3,HIGH);
                 y2B=pulseIn(SO,LOW);//Reading Blue colour of second sample
                 lcd.print(y2R);
                 lcd.print(",");
                 lcd.print(y2G);
                 lcd.print(",");
                 lcd.print(y2B);
                 delay(3000);lcd.clear();}
                 

void Dht(){lcd.clear();lcd.setCursor(0,1);
           delay(dht.getMinimumSamplingPeriod());

           float humidity = dht.getHumidity();
           float T = dht.getTemperature();

           Serial.print(dht.getStatusString());
           Serial.print("\t");
           Serial.print(humidity, 1);
           Serial.print("\t\t");
           Serial.print(T, 1);
           Serial.print("\t\t");
           float X=T;
           float Y=X-4;
           float Q=Y*0.0002+1;
           float E=Y*0.000082+1;
           Z=&Q;
           P=&E;
           Serial.print(*Z);
           delay(1000);
           lcd.print(T);
           lcd.print(",");
           lcd.print(humidity);
           delay(3000);lcd.clear();}
       
void AirQual(){lcd.clear();
  lcd.setCursor(0,0);
  int Asens;
  Asens=analogRead(Airpin);//reading the pin to print value in PPM
  lcd.setCursor(0,0);
  lcd.print("AQI=");
  lcd.print(Asens,DEC);
  lcd.print("PPM");
  delay(3000);lcd.clear();}
                                    

void pH(){lcd.clear();

lcd.setCursor(1,0);
int sensorValue=0;
unsigned long int avgValue;
int buf[10],temp;
float b;
for(int i=0;i<10;i++) 
{
buf[i]=analogRead(analogIn);
 delay(10);}
for(int i=0;i<9;i++)
{
 for(int j=i+1;j<10;j++)
 {
  if(buf[i]>buf[j])
  {
   temp=buf[i];
   buf[i]=buf[j];
   buf[j]=temp;
  }
}
}
 

avgValue=0;
for(int i=2;i<8;i++)
avgValue+=buf[i];
float pHVol=(float)avgValue*5.0/1024/6;
float phValue = -5.70 *pHVol + 21.34;
lcd.print("sensor=");
lcd.print(phValue);

delay(20);
delay(3000);lcd.clear();


}
void wght(){float Time=0;float arr[6];float arr1[4];float m_wtr;float arr2[6];float arr3[4];float m_fat;int C=0;
m_wtr=(25)*(*Z);
m_fat=(25)*(*P);
lcd.setCursor(0,1);lcd.print("Processing...");
while(Time<500){Time=C;LoadCell.update(); // retrieves data from the load cell
i = LoadCell.getData();delay(20);C++;} // get output value  // print out to LCD
 lcd.clear();
 lcd.setCursor(0,1);
 arr[0]=i+m_wtr;
 for(int B=1;B<6;B++)
 {arr[B]=arr[B-1]+m_wtr;
  arr2[B]=arr[B-1]+m_fat;}
 for(int E=0;E<=3;E++)
 {arr1[E]=arr[E]/arr[E+1];
  arr3[E]=arr2[E]/arr2[E+1];}
 float X=(arr3[0]+arr3[1]+arr3[2]+arr3[3])/4;
 float N=(arr1[0]+arr1[1]+arr1[2]+arr1[3])/4;
 float T=N*100;
 float Q=X*100;
 lcd.setCursor(0,0);lcd.print("%water");lcd.setCursor(1,1);lcd.print(T);delay(3000);lcd.clear();
 lcd.setCursor(0,0);lcd.print("%fat");lcd.setCursor(1,1);lcd.print(Q);delay(3000);lcd.clear();
 lcd.setCursor(0,0);lcd.print("%SNF");lcd.setCursor(1,1);lcd.print(100-T-Q);delay(3000);lcd.clear();} 


 


              




            
void loop(){int btnst1=0,btnst2=0,btnst3=0,btnst4=0,btnst5=0;
btnst1=digitalRead(btn1);//A1
btnst2=digitalRead(btn2);//A2
btnst3=digitalRead(btn3);//A3
btnst5=digitalRead(btn5);//A5
if(btnst1==1){Dht();btnst1=0;}
if(btnst2==1){comp_color();}
if(btnst3==1){AirQual();pH();}
if(btnst5==1){wght();}}             
