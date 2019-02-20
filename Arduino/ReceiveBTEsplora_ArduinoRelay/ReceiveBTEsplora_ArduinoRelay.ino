// The code to upload on Arduino B ( Relay H-Bridge controller ) 
#include "MeccaBrain.h"

MeccaBrain head(11);
MeccaBrain leftArm(12);
MeccaBrain rightArm(13);


// COMMUNICATION PATTERNS
byte startPatternByte = 'a';
byte endPatternByte = 'z';

// COMMUNICATION
int startByteIndex = 0;
int endByteIndex = 0;
int messageByteIndex = 0;

int checkReceiveSerial = 0;
int checkReceiveSerialLimit = 50;

bool messageIsReady = false;
bool messageIsOver = false;
int bytesExpected = 4; // direction / animation / slider_state / speed
bool debugMessageProcess = false;

//RELAY
int relayPinA = 2;
int relayPinB = 4;
int relayPinC = 6;
int relayPinD = 8;
bool debugRelay = false;

//PWM SPEED - mosfet
int mosfetPinA = 3;
int mosfetPinB = 5;
int mosfetPinC = 10;
int mosfetPinD = 11;


// ANIMATION
bool playAnimation = false;
int buttonState = 0;
int sliderState = 0; // Slider state is defining animation state
//byte buttonState = 'x';
int animationCycle = 500;
int frameIndex = 0;

// LEDS
int led = 13;
int ledState = HIGH;

// DEBUG LEDs
int redLed = A0;
int greenLed = A1;
int yellowLed = 9;



char saveDirection = 'x';

// %ECCA MOVE : a simple keyframe servoPos interpolation
struct MeccaMove{
  
  byte leftArmAPosition[8];
  byte leftArmBPosition[8];
  byte leftArmCPosition[8];
  byte rightArmAPosition[8];
  byte rightArmBPosition[8];
  byte rightArmCPosition[8];

  byte headAPosition[8];
  byte headBPosition[8];

  byte headColor[4];
};

MeccaMove animations[12] = {

  // TALK : NON EXPRESSIVE . YELLOW
 {
    { 0 , 0, 0 ,0 , 0, 0, 0 , 0 }, // leftA
    { 220 , 220 , 220 , 220, 220 , 220 , 220 , 220 }, // leftB
    { 0 , 10 , 20 , 0 , 20, 0 , 40 , 0} , // leftC
    { 0 , 0 , 0 , 0, 0, 0, 0 ,0 }, // rightA
    { 220 , 220 , 220, 220, 220, 220 , 220 , 220 }, // rightB
    { 220 , 230 , 220, 200,220 , 180 ,210 , 220}, // rightC
    { 120 , 120 , 120 , 120, 120 ,120 ,120,120 }, // hA
    { 120 , 120 , 120 , 120 , 120 , 130 , 20 , 20},  // hB
    { 0 , 0 , 0 , 0 }  
  },

  // 1b TALK 2 : EXPRESSIVE   YELLOW
  {
    { 20 , 10, 0 ,20 , 0, 0, 0 , 0 },
    { 230 , 220 , 220 , 220, 220 , 220 , 220 , 220 }, 
    { 0 , 10 , 20 , 0 , 20, 0 , 40 , 0}, 
    { 0 , 20 , 10 , 0, 20, 0, 0 ,0 }, 
    { 220 , 220 , 220, 220, 220, 220 , 220 , 220 }, 
    { 220 , 230 , 220, 200,220 , 180 ,210 , 220}, 
    { 120 , 110 , 120 , 120, 220 ,120 ,120,120 }, 
    { 120 , 120 , 120 , 120 , 120 , 130 , 20 , 20},
    { 3 , 3 , 3 , 2 }  
  },


  // 1c TALK 3 : WORRIED   YELLOW
  {
    { 20 , 10, 50 ,20 , 0, 40, 30 , 0 }, 
    { 230 , 180 , 220 , 220, 220 , 180 , 220 , 220 }, 
    { 0 , 10 , 20 , 0 , 20, 0 , 40 , 0}, 
    { 0 , 20 , 10 , 0, 20, 0, 0 ,0 }, 
    { 220 , 170 , 220, 220, 180, 220 , 220 , 220 }, 
    { 220 , 230 , 220, 200,220 , 180 ,210 , 220}, 
    { 120 , 110 , 140 , 120, 220 ,120 ,20,120 }, 
    { 120 , 120 , 120 , 180 , 120 , 130 , 110 , 110},
    { 4 , 4 , 4 , 4 }  
  },

  // 2a EXPLAIN STH BLUE
  {
    { 20 , 10, 50 ,20 , 60, 40, 30 , 0 }, // headA
    { 230 , 180 , 220 , 220, 220 , 180 , 220 , 220 }, // headB
    { 0 , 10 , 20 , 0 , 20, 0 , 40 , 0}, // lArmA
    { 40 , 20 , 10 , 0, 20, 60, 0 ,0 }, // lArmB
    { 180 , 170 , 180, 180, 180, 220 , 220 , 220 }, // lArmC
    { 220 , 230 , 220, 200,220 , 180 ,210 , 220}, // rArmA
    { 120 , 120 , 120 , 120, 120 ,120 ,120,120 }, // rArmB
    { 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120},  // rArmC
    {3,2,7,6}
  },

  // 2b : SAY HELLO   BLUE
  {
    { 80 , 80, 80 ,80 , 80, 80, 80 , 60 },
    { 120 , 120 , 120 , 120, 120 , 120 , 120 , 220 },
    { 0 , 10 , 20 , 0 , 20, 0 , 40 , 0},
    { 100 , 100 , 100 , 100, 100, 100, 100 ,100 },
    { 120 , 120 , 120, 120, 120, 120 , 120 , 220 },
    { 220 , 230 , 220, 200,220 , 180 ,210 , 220},
    { 120 , 120 , 120 , 120, 120 ,120 ,120,120 },
    { 120 , 120 , 120 , 120 , 120 , 120 , 120 , 120},
    {7,3,7,3}
  },

  // 2c : DANCE : HEADBANG     BLUE
  {
    { 180 , 80, 80 ,80 , 80, 80, 80 , 60 },
    { 120 , 120 , 120 , 120, 120 , 120 , 120 , 220 },
    { 0 , 10 , 20 , 0 , 20, 0 , 40 , 0},
    { 10 , 10 , 180 , 10, 100, 100, 100 ,100 },
    { 120 , 120 , 120, 120, 120, 120 , 120 , 220 },
    { 220 , 230 , 220, 200,220 , 180 ,210 , 220},
    { 120 , 120 , 220 , 120, 120 ,120 ,120,120 },
    { 120 , 20 , 120 , 120 , 20 , 120 , 120 , 120},
    {0,0,0,0}
  },

  // 3a : ERROR SYSTEM !   GREEN
  {
    { 10 , 20, 10 ,10 , 20, 10, 80 , 60 },
    { 220 , 120 , 220 , 210, 120 , 220 , 120 , 220 },
    { 0 , 10 , 20 , 66 , 20, 66 , 40 , 0},
    { 10 , 10 , 66 , 10, 100, 100, 100 ,100 },
    { 120 , 66 , 120, 120, 66, 120 , 120 , 220 },
    { 220 , 230 , 66, 200,220 , 66 ,210 , 220},
    { 120 , 120 , 66 , 66, 66 ,66 ,120,120 },
    { 120 , 20 , 66 , 66 , 66 , 120 , 120 , 120},
    { 7,0,0,2 }
  },

  // 3b : CONDUCTOR     GREEN
  {
    { 10 , 10, 10 ,10 , 20, 10, 10 , 60 },
    { 66 , 66 , 66 , 66, 66, 66 , 66 , 66 },
    { 66, 66 , 66 , 66 , 66, 66 , 66, 66},
    { 10 , 10 , 66 , 10, 100, 100, 100 ,100 },
    { 120 , 66 , 120, 120, 66, 120 , 120 , 220 },
    { 220 , 230 , 66, 200,220 , 66 ,210 , 220},
    { 120 , 120 , 66 , 66, 66 ,66 ,120,120 },
    { 120 , 20 , 66 , 66 , 66 , 120 , 120 , 120},
    { 4,4,3,2}
  },

  // 3c : EXPLAINER      GREEN
  {
    { 1 , 2, 3 ,4 , 5, 10, 10 , 60 },
    { 1 , 2 , 10 , 20, 66, 66 , 66 , 66 },
    { 20, 66 , 66 , 20 , 66, 66 , 66, 66},
    { 10 , 10 , 10 , 10, 10, 10, 10 ,100 },
    { 120 , 66 , 66, 66, 66, 66 , 66 , 66 },
    { 220 , 230 , 220, 200,220 , 200 ,210 , 220},
    { 120 , 120 , 120 , 120, 120 ,120 ,120,120 },
    { 120 , 120, 120 , 120 , 120 , 120 , 120 , 120},
    { 5,3,2,4 }
  },

  // 4a HEAD ONLY    RED
  {
    { 100 , 100, 100,100 , 100, 100, 100 , 100 },
    { 100 , 100 , 100 , 100, 100, 100 , 100 , 100 },
    { 100 , 100 , 100 , 100, 100, 100 , 100 , 100 },
    { 100 , 100 , 100 , 100, 100, 100 , 100 , 100 },
    { 100 , 100 , 100 , 100, 100, 100 , 100 , 100 },
    { 100 , 100 , 100 , 100, 100, 100 , 100 , 100 },
    { 50 , 120 , 120 , 220, 120 ,120 ,120,120 },
    { 120 , 120, 120 , 150 , 150 , 120 , 120 , 120},
    { 6,3,4,5 }
  },

  // 4b WARM UP     RED
  {
    { 100 , 100, 100,100 , 100, 10, 100 , 100 },
    { 100 , 100 , 10 , 100, 100, 100 , 100 , 100 },
    { 100, 100 , 100 , 100 , 100, 100 , 100, 100},
    { 100 , 100 , 100 , 10, 100, 100, 100 ,100 },
    { 100 , 10, 100, 100, 100, 100 , 100 , 100 },
    { 100 , 100 , 100, 100,100 , 100 ,100 , 100},
    { 50 , 50 , 50 , 50, 50 ,50 ,50,50 },
    { 50 , 50, 50 , 50 , 50 , 50 , 50 , 50 },
    { 6,1,1,1 }
  },

  // 4c REQUIRE     RED
  {
    { 200 , 200, 200,200 , 200, 200, 200 , 200 },
    { 100 , 210 , 110 , 100, 100, 100 , 100 , 100 },
    { 200, 200 , 200 , 230 , 230, 230 ,200, 200},
    { 0 , 0 , 0 , 0, 0, 0, 0 ,0 },
    { 220 , 220, 220, 222, 220, 220 , 230 , 200 },
    { 230 , 200 , 230, 160,200 , 200 ,200 , 200},
    { 150 , 130 , 150 , 150, 130 ,150 ,150,150 },
    { 150 , 150, 130 , 150 , 150 , 130 , 150 , 150 },
    { 7,7,7,7 }
  }
};



int cycleValueMax = 6; 
int cycleValue = 0; 


#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

      

void setup() {

  // Serial is getting data from bt- HM10
  Serial.begin(9600);
  mySerial.begin(9600);

  
  // Relay pin to OUTPUT
  pinMode(relayPinA, OUTPUT);
  pinMode(relayPinB, OUTPUT);
  pinMode(relayPinC, OUTPUT);
  pinMode(relayPinD, OUTPUT);

  // motor Speed
  pinMode(mosfetPinA,OUTPUT);
  pinMode(mosfetPinB,OUTPUT);
  pinMode(mosfetPinC,OUTPUT);
  pinMode(mosfetPinD,OUTPUT);
  
  

  //LEDS to OUTPUT
  pinMode(redLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(yellowLed,OUTPUT);

  pinMode(led,OUTPUT);

  //DEFAULT POSITION
  relayIdle();

}



void loop() {


    
  

  checkReceiveSerial++;
  if(checkReceiveSerial > checkReceiveSerialLimit){
    digitalWrite(redLed,LOW); // HM 10 IS NOT ANYMORE RECEIVING
  }

  //digitalWrite(led,HIGH);

//analogWrite(redLed,255);

  while (Serial.available()) {

      checkReceiveSerial = 0;

      analogWrite(redLed,255);

      //digitalWrite(redLed,HIGH); // RED LIGHT IS TELLING US THAT WE ARE RECEIVING SERIAL

      byte b  = Serial.read();
      char c = (char)b;
      int i = (int)b;

      

      //Serial.println(c);

      // READ BT MESSAGE PROCESS
      if(!messageIsReady && b == startPatternByte){
        messageIsReady = true;
        return;  
      }

      if(messageIsReady){

          if(messageByteIndex < bytesExpected){
          
            readBTInstructions(messageByteIndex,c,i); // READ BT DATA INSTRUCTIONS
            messageByteIndex++;
            
          }else{
            
            if(b == endPatternByte){
              messageIsReady = false;
              messageByteIndex = 0;
            }
            
          }
        
      }
      
  }


}

void readBTInstructions(int messageIndex,char c,int i){

  if(messageIndex == 0){
    // READ DIRECTION VALUE
    //digitalWrite(greenLed,LOW); 
    saveDirection = c;
    
    if( c == 'l' )  {
      relayTurnCounterClockwise();
      //relayTurnClockwise();
      //relayMoveForward();
      playAnimation = false;
      resetServo();
    }
    if( c == 'r' ){
      relayTurnClockwise();
      //relayTurnCounterClockwise();
      //relayMoveBackward();
      playAnimation = false;
      resetServo();
    }
    if( c == 'd' ){
      relayMoveForward();
      //relayMoveBackward();
      //relayTurnClockwise();
      playAnimation = false;
      resetServo();
    }
    if( c == 'u' ){
      relayMoveBackward();
      //relayMoveForward();
      //relayTurnCounterClockwise();
      playAnimation = false;
      resetServo();
    }
    if( c == 'x' ){
      relayIdle();
      digitalWrite(yellowLed,HIGH);
    }

  }

  if(messageIndex == 1){
    // READ BUTTONS VALUE
   
     

    cycleValue = 0;

    if( c == 'x' ){
  
    }

    if( c == '1' || c == '2' || c == '3' ){
      playAnimation = true;
      //animateMecca(sliderState,c);

      //buttonState = c;

    }

    if( c == '1') buttonState = 0;
    if( c == '2') buttonState = 1;
    if( c == '3') buttonState = 2;
    if( c == '4') {
      resetServo();
      playAnimation = false;
      relayIdle();
    }
      
  }

  if(messageIndex == 2){
    // SLIDER STATE, USED FOR SERVOS, NOT HERE.
 
  }

  if(messageIndex == 3){

    analogWrite(greenLed,120);
    
    // SET THE SPEED
/*
    if(saveDirection == 'u' || saveDirection == 'r'){
      analogWrite(mosfetPinA, i);
      analogWrite(mosfetPinB, 0);
    }

    if(saveDirection == 'd' || saveDirection == 'l'){
      analogWrite(mosfetPinA, 0);
      analogWrite(mosfetPinB, i);
          
    }

    */
      analogWrite(mosfetPinA, i);
      analogWrite(mosfetPinB, i);
    analogWrite(mosfetPinC, i);
    analogWrite(mosfetPinD, i);
  }

  
}

void resetServo(){

   head.setServotoLIM(0);
   head.setServotoLIM(1);
   
   leftArm.setServotoLIM(0);
   leftArm.setServotoLIM(1);
   leftArm.setServotoLIM(2);

   rightArm.setServotoLIM(0);
   rightArm.setServotoLIM(1);
   rightArm.setServotoLIM(2);

}


//////////////////
// RELAY
//////////////////

void relayIdle(){

   // IDLE
   digitalWrite(relayPinA, LOW);
   digitalWrite(relayPinB, LOW);
   digitalWrite(relayPinC, LOW);
   digitalWrite(relayPinD, LOW);

}

void relayTurnClockwise(){

    // LEFT
   digitalWrite(relayPinA,HIGH); 
   digitalWrite(relayPinB,LOW);
   digitalWrite(relayPinC,LOW);
   digitalWrite(relayPinD,HIGH);
   
}


void relayTurnCounterClockwise(){

   // RIGHT
   digitalWrite(relayPinA,LOW);
   digitalWrite(relayPinB,HIGH);
    digitalWrite(relayPinC,HIGH);
   digitalWrite(relayPinD,LOW); 
    
}


void relayMoveForward(){

   // FORWARD
   digitalWrite(relayPinA,LOW);
   digitalWrite(relayPinB,HIGH);
   digitalWrite(relayPinC,LOW);
   digitalWrite(relayPinD,HIGH);

}


void relayMoveBackward(){
      
   // BACKWARD
   digitalWrite(relayPinA,HIGH);
   digitalWrite(relayPinB,LOW);
   digitalWrite(relayPinC,HIGH);
   digitalWrite(relayPinD,LOW);

}


void updateServoPosition(MeccaBrain *s,int a, int b, int c){

    s->communicate();
    s->setServoPosition(0,a);
    s->setServoPosition(1,b);
    s->setServoPosition(2,c);

 }

void updateHeadServoPosition(MeccaBrain *h,int a, int b, byte r,byte g ,byte bb, byte d){

    h->communicate();
    h->setServoPosition(0,a);
    h->setServoPosition(1,b);
    h->setLEDColor(r,g,bb,d);

}



