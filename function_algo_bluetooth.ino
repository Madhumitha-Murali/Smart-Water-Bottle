#include "HX711.h"  //You must have this library in your arduino library folder
#include "BluetoothSerial.h" 
#define DOUT  2
#define CLK  4
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
 
HX711 scale;
char junk = ' ';                     // to remove extra inputs from user
//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = -96650; //-106600 worked for my 40Kg max scale setup 
float goal;
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
float weight;
float workout_time;
float water_req_weight;
float water_req_work;
float water_required;
float threshold = 0.05;
int flag=0;
unsigned long StartTime;
unsigned long tempTime;
unsigned long CurrentTime;
int flag_w=0;
unsigned long StartTime1;
unsigned long tempTime1;
unsigned long CurrentTime1;
BluetoothSerial SerialBT;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void reset()
{
   scale.tare();
   scale.set_scale(-96650);
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Init_emptybottle()
{
   Serial.println("Place the empty bottle");
   SerialBT.println("Place the empty bottle");
   
   delay(5000);
   bottle_wt=scale.get_units(10);                                 //Getting the weight of empty bottle
   Serial.println(bottle_wt); 
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
void Init_fullbottle()
{
   Serial.println("Place the bottle with water");
   SerialBT.println("Place the bottle with water");
   delay(10000);
   water_curr=scale.get_units(5);                                //Getting the weight of bottle with water
   water_curr1=water_curr*12;
   water_curr2=water_curr1/15; 
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void water_to_be_consumed()
{
 Serial.println("Please enter your weight\n");
 SerialBT.println("Please enter your weight\n");
 
   while(SerialBT.available()==0)
   {} 
   weight = SerialBT.parseFloat();  
   Serial.print(weight);
   while (SerialBT.available() > 0)  // .parseFloat() can leave non-numeric characters
    { junk = SerialBT.read() ; }      // clear the keyboard buffer
    
    if(weight<=0.00 || weight > 650.01)
      {
        Serial.println("Please enter valid amount in numbers ");
        SerialBT.println("Please enter valid amount in numbers ");
        while(SerialBT.available()==0)
        {}
           weight = SerialBT.parseFloat();  
           Serial.print(weight);
           while (SerialBT.available() > 0)  // .parseFloat() can leave non-numeric characters
            { junk = SerialBT.read() ; }      // clear the keyboard buffer
      
      }
  

  Serial.println("Please enter your workout time in minutes per day \n");
  SerialBT.println("Please enter your workout time in minutes per day \n");
    while(SerialBT.available()==0)
   {}
   workout_time = SerialBT.parseFloat();  
   Serial.print(workout_time);
   while (SerialBT.available() > 0)  // .parseFloat() can leave non-numeric characters
    { junk = SerialBT.read() ; }      // clear the keyboard buffer
  
    //Serial.print(workout_time);
    if(workout_time<0 || workout_time>14460) 
      {
        Serial.println("Please enter valid data in digits");
        SerialBT.println("Please enter valid data in digits");
        
        while(SerialBT.available()==0)
        {}
        workout_time = SerialBT.parseFloat();  
        Serial.print(workout_time);
         while (SerialBT.available() > 0)  // .parseFloat() can leave non-numeric characters
        { junk = SerialBT.read() ; }      // clear the keyboard buffer
       }       
 

  water_req_weight = (weight*2.205*2)/3;
  water_req_work = (workout_time*12)/30;
  water_required = (water_req_weight+water_req_work)*0.0296;
  Serial.print("The amount of required for you is ");
  Serial.print(water_required);
  Serial.print("\n");
 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Inst_water()
{
   water_prev = water_curr2;                                    //Initialising water_prev 
   Serial.print("\n");
   Serial.println("Water prev:");
   Serial.println(water_prev); 
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
void water_consumed()
{
    if((water_prev-water_test2)>0.05 )          // To calculate the amount of water consumed
      {  
       drunk_water+=water_prev-water_curr2;
       water_prev=water_curr2;
       flag = 0;
       Serial.print("Amount of water consumed:");
       Serial.print(drunk_water);
       Serial.print("\n");
       SerialBT.print("Amount of water consumed:");
       SerialBT.print(drunk_water);
       //SerialBT.print("\n");
       delay(500);       
      }
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void set_goal()
{
  Serial.println("Please enter your goal for today in Litres");
  SerialBT.println("Please enter your goal for today in Litres");  
    while(SerialBT.available()==0)
   {
    
   }

   goal= SerialBT.parseFloat();  
   Serial.print(goal);
   while (SerialBT.available() > 0)  // .parseFloat() can leave non-numeric characters
    { junk = SerialBT.read() ; }      // clear the keyboard buffer
   
    if(goal<0.01 || goal>20.01) 
      {
        Serial.println("Please enter valid data in digits");
        SerialBT.println("Please enter valid data in digits");
        while(SerialBT.available()==0)
        {}
        goal= SerialBT.parseFloat();  
        Serial.print(goal);
        while (SerialBT.available() > 0)  // .parseFloat() can leave non-numeric characters
        { junk = SerialBT.read() ; }      // clear the keyboard buffer
   
      } 
     if(goal<water_required)
     {
      Serial.print("You can drink more water\n");
     }
 }
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
void water_refill()
{
   if(water_test2==bottle_wt || water_test2 < (bottle_wt+0.010))       // To checck if bottle is empty
      {
        Serial.print("Please refill \n");
        SerialBT.print("Please refill \n");
        
      }
      delay(2000);
  }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
void timer()
{
   if (flag == 0)                                                      // A timer to send notification
       {  
          StartTime = millis();
          tempTime = StartTime;
          flag = 1;
        }
         StartTime = millis();
         if(StartTime > tempTime + 30000)
             {                
              Serial.print("PLEASE DRINK  MORE WATER!\n");
              SerialBT.println("PLEASE DRINK  MORE WATER!");    
             }
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);
  scale.begin(DOUT,CLK); 
  //Serial.println("Press T to tare");
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  scale.set_scale(-96650);  //Calibration Factor obtained from first sketch
  scale.tare();
  //Reset the scale to 0 
  delay(2000);
   Serial.print(scale.get_units(20)); 
   delay(10000);
   bottle_wt=scale.get_units(20);
   water_prev=0;
   Serial.print("Beginning");                         
   Serial.print("\n");
   SerialBT.println("Beginning");
   //SerialBT.write("\n");
   reset();
   
  
   /////////////////////////////////////////////////////////////////////
 water_to_be_consumed();
   /////////////////////////////////////////////////////////////////////
 set_goal();
   /////////////////////////////////////////////////////////////////////
 Init_emptybottle();
  ///////////////////////////////////////////////////////////////////// 
 Init_fullbottle();
  /////////////////////////////////////////////////////////////////////
  Inst_water();
}

 
//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() 
{
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //Reset the scale to zero      
  }
     
    delay(2000);
    water_test=scale.get_units(5);                // Getting the instantaneous values  
    water_test1=water_test*12;
    water_test2=water_test1/15;
    Serial.print("The water test is ");
    Serial.print(water_test2);
    Serial.print("\n");

    if( water_test2 > threshold)                  // basic condition - to check if weight is more than the threshold
    { 
      flag_w=0;
      water_curr=scale.get_units(5);               // Initialiing the water_curr2
      water_curr1=water_curr*12;
      water_curr2=water_curr1/15;
      delay(2000);
      
      water_consumed();
      if (drunk_water > goal)
      {
        Serial.println("You have achieved today's goal successfuly");
        SerialBT.println("You have achieved today's goal successfuly");
      }
      
      water_refill();

      if(water_prev < water_test2)                                        // Swapping values while water is being filled 
       {
          water_prev = water_curr2;
       }
      timer();
      delay(1000);
    }
 
 else
  {
    //delay(500);
    if (flag_w ==0)                                                      // A timer to send notification
       {  
          StartTime1 = millis();
          tempTime1 = StartTime1;
          flag_w = 1;
        }
         StartTime1 = millis();
         if(StartTime1 > tempTime1 + 10000)
             {                
              Serial.print("Please place the bottle\n");
              SerialBT.println("Please place the bottle\n");    
             }
  }
// Serial.println(flag_w);  
}
