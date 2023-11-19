#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
int calibrationTime = 10;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 6;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;


/////////////////////////////
//SETUP
void setup(){
  lcd.begin(16,2);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  lcd.print("calibrtng ");
  delay(1000);
  lcd.clear();
    for(int i = 1; i < calibrationTime; i++){
      lcd.print(i);
      delay(1000);
      lcd.clear();
      }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DEVICE");
    lcd.setCursor(0,1);
    lcd.print("ACTIVE");
    delay(1000);
    lcd.clear();
  }

////////////////////////////
//LOOP
void loop(){

     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false; 
         lcd.setCursor(0,0);
         lcd.print("START ");        
         lcd.print(millis()/1000);
         lcd.print(" sec"); 
         delay(50);
         
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           lcd.setCursor(0,1);
           lcd.print("END ");
           lcd.print((millis() - pause)/1000);
           lcd.print(" sec");
           delay(50);
           }
       }
  }