#include "HX711.h"  //You must have this library in your arduino library folder
 
#define DOUT  2
#define CLK  4
 
HX711 scale;//(DOUT, CLK);
 
//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = -96650; //-106600 worked for my 40Kg max scale setup 

float drunk_water=0;
float water_init;
float water_curr;
float water_curr1;
float water_curr2;
float water_prev;
float bottle_wt;
float water_test;
float water_test1;
float water_test2;
float water_posneg;
//float water_alert;
//float water_alert1;
//float water_alert2;
float threshold = 0.025;
int flag=0;
int i;
float w;
float t;

unsigned long StartTime;
unsigned long tempTime;
unsigned long CurrentTime;


//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);
  scale.begin(DOUT,CLK); 
  //Serial.println("Press T to tare");
  scale.set_scale(-96650);  //Calibration Factor obtained from first sketch
  scale.tare();
  //Reset the scale to 0 
  delay(2000);
  Serial.print(scale.get_units(20)); 
  //if(scale.get_units(20)>=0.010)
  //{
   delay(10000);
   bottle_wt=scale.get_units(20);
   //water_init=scale.get_units(10);
   //water_curr=water_init;
   water_prev=0;
   Serial.print("Beginning");
   Serial.print("\n");
   Serial.print("Threshold value:  ");
   Serial.println(threshold);
   
   scale.tare();
   scale.set_scale(-96650);
   Serial.println("Place the empty bottle");
   delay(5000);
   bottle_wt=scale.get_units(10);
   Serial.println(bottle_wt);

   Serial.println("Place the bottle with water");
   delay(10000);
   water_curr=scale.get_units(5);
   water_curr1=water_curr*12;
   water_curr2=water_curr1/15; 

   water_prev = water_curr2;
   Serial.print("Initial amount consumed is  ");
   Serial.print(drunk_water);
   Serial.print("\n");
   Serial.println("Water prev:");
   Serial.println(water_prev);
   
  //}   
}

 
//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  
  //Serial.print("Weight: ");
  //Serial.print((scale.get_units(5)* 12)/15, 3);  //Up to 3 decimal points
  //Serial.println(" kg"); //Change this to kg and re-adjust the calibration factor if you follow lbs
  //delay(1000);
  //Serial.println(scale.get_units(5)* 12/(15),3);
  //drunk_water=0;
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //Reset the scale to zero      
  }

  
   //Serial.print("BFR TAKING BOTTLE  ");
   //Serial.print(water_curr2);
  
    water_test=scale.get_units(5);
    water_test1=water_test*12;
    water_test2=water_test1/15;
    Serial.print("Water test:  ");
    Serial.print(water_test2);
    Serial.print("\n");
    if( water_test2 > threshold)
    { 
      //delay(2000);
      //Serial.println(" flag= 1");
      water_curr=scale.get_units(5);
      water_curr1=water_curr*12;
      water_curr2=water_curr1/15;
      Serial.print("Water_prev:  ");
      Serial.print(water_prev);
      Serial.print("\n");
      Serial.print("Water_current:  ");
      Serial.print(water_curr2);
      Serial.println("\n");
      delay(2000);

     
      if((water_prev-water_test2)>0.020 )   //water_prev-water_test2)>0.020
      { 
        Serial.print(water_prev - water_curr2);
        //if((water_prev - water_curr2) > 0.02)
      //{
        //Serial.print("Water has been filled");
        //t = water_prev;
        //water_prev = water_test2;
        //water_test2=t;
        //flag = 0;
       //}
      
        
      drunk_water+=water_prev-water_curr2;
      water_prev=water_curr2;
      flag = 0;
      Serial.print("Amount of water consumed:");
      Serial.print(drunk_water);
      Serial.print("\n");
      delay(500); 
      //flag=0;
      }
     
       
       
      if(water_test2==bottle_wt || water_test2 < (bottle_wt+0.010))
      {
        Serial.print("Please refill \n");
      }
      delay(2000);
       //water_alert=scale.get_units(5);
       //water_alert1=water_alert*12;
       //water_alert2=water_alert1/15;
       //Serial.print("Intermediate value: "); 
       //Serial.print(water_alert2);
       //Serial.print("\n");
       w=water_prev-water_test2;
       //water_posneg = water_prev - water_test2;
       //Serial.print("W is ");
       //Serial.print(w);
       //Serial.print("\n ");
       //Serial.print(flag);
       
       // if((-0.02 < w)  && (w < 0.02))
      // { 

        if(water_prev < water_test2)
        {
          water_prev = water_curr2;
          }
        if (flag == 0)
         {  StartTime = millis();
            tempTime = StartTime;
            flag = 1;
         }
         StartTime = millis();
         //Serial.print("Checking time\t");
         //Serial.print(StartTime);
         
           if(StartTime > tempTime + 10000)
             {                
              Serial.print("PLEASE DRINK  MORE WATER!\n");
              
             }
            
       //}
       //if(flag%6==0)
       //{
        //flag=1;
//        Serial.print("PLEASE DRINK  MORE WATER!\n");
      // }
       //if(water_prev<water_curr2)
      //{
        //drunk_water+=water_curr2;
      //}
      }
   
   

    //if(water_curr2 < threshold)
   //{
    //flag= 1;
    //water_prev = water_curr2;
    //Serial.println("flag = 0");
  // }
   
    
    delay(1000);
   
}
