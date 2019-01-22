// The code to upload on Arduino A - the arm/head Servomotors controller 
#include "MeccaBrain.h"

MeccaBrain head(11); // VIOLET
MeccaBrain leftArm(12); // GRIS
MeccaBrain rightArm(13); // BLEU


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
int bytesExpected = 3; // direction / animation / slider_state
bool debugMessageProcess = false;

//RELAY
int relayPinA = 2;
int relayPinB = 3;
int relayPinC = 4;
int relayPinD = 5;
bool debugRelay = false;


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
int redLed = 7;
int greenLed = 8;
int yellowLed = 9;


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
    { 7,0,0,2}
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
    { 5,3,2,4}
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
    { 6,3,4,5  }
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
    { 7,7,7,7}
  }
};



int cycleValueMax = 6; 
int cycleValue = 0; 


      

void setup() {

  // Serial is getting data from bt- HM10
  Serial.begin(9600);

  // Relay pin to OUTPUT
  pinMode(relayPinA, OUTPUT);
  pinMode(relayPinB, OUTPUT);
  pinMode(relayPinC, OUTPUT);
  pinMode(relayPinD, OUTPUT);

  //LEDS to OUTPUT
  pinMode(redLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(yellowLed,OUTPUT);

  pinMode(led,OUTPUT);

  //DEFAULT POSITION
  relayIdle();

}



void loop() {


    
    // use positions variables to set servp positions
   if(playAnimation){

      if( millis() % animationCycle == 0 && cycleValue < cycleValueMax){ 
        
        int animationIndex = (sliderState * 3 + buttonState); // 0 0 : 0 / 1 / 2 

        MeccaMove mm = animations[animationIndex];
        
        int lA = mm.leftArmAPosition[frameIndex % (sizeof(mm.leftArmAPosition)/2) ];
        int lB = mm.leftArmBPosition[frameIndex % (sizeof(mm.leftArmBPosition)/2) ];
        int lC = mm.leftArmCPosition[frameIndex % (sizeof(mm.leftArmCPosition)/2) ];
        int rA = mm.rightArmAPosition[frameIndex % (sizeof(mm.rightArmAPosition)/2) ];
        int rB = mm.rightArmBPosition[frameIndex % (sizeof(mm.rightArmBPosition)/2) ];
        int rC = mm.rightArmCPosition[frameIndex % (sizeof(mm.rightArmCPosition)/2) ];
    
        int hA = mm.headAPosition[frameIndex % (sizeof(mm.headAPosition)/2)];
        int hB = mm.headBPosition[frameIndex % (sizeof(mm.headBPosition)/2)];


        byte r = mm.headColor[0];
        byte g = mm.headColor[1];
        byte b = mm.headColor[2];
        byte d = mm.headColor[3];
        
        
        updateServoPosition(&leftArm,lA,lB,lC);
        updateServoPosition(&rightArm,rA,rB,rC);
        updateHeadServoPosition(&head,hA,hB,r,g,b,d);

      
        frameIndex++;
        cycleValue++;
        
      }
      //digitalWrite(yellowLed,HIGH);
      
    }else{
    
      resetServo();
      //digitalWrite(yellowLed,LOW);
  
  }

  checkReceiveSerial++;
  if(checkReceiveSerial > checkReceiveSerialLimit){
    digitalWrite(redLed,LOW); // HM 10 IS NOT ANYMORE RECEIVING
  }

  while (Serial.available()) {

      checkReceiveSerial = 0;

      digitalWrite(led,HIGH); // RED LIGHT IS TELLING US THAT WE ARE RECEIVING SERIAL

      digitalWrite(redLed,HIGH); // RED LIGHT IS TELLING US THAT WE ARE RECEIVING SERIAL

      byte b  = Serial.read();
      char c = (char)b;

      // READ BT MESSAGE PROCESS
      if(!messageIsReady && b == startPatternByte){
        messageIsReady = true;
        return;  
      }

      if(messageIsReady){

          if(messageByteIndex < bytesExpected){
          
            readBTInstructions(messageByteIndex,c); // READ BT DATA INSTRUCTIONS
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

void readBTInstructions(int messageIndex,char c){

  if(messageIndex == 0){
    /*
    // READ DIRECTION VALUE
    digitalWrite(greenLed,LOW); 
    
    if( c == 'l' )  {
      relayTurnClockwise();
      //relayMoveForward();
      playAnimation = false;
      resetServo();
    }
    if( c == 'r' ){
      relayTurnCounterClockwise();
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
    */

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

    sliderState = c;

    if(buttonState == '1' || buttonState == '2' || buttonState == '3' ){
        playAnimation = true;
        //animateMecca(c,buttonState);
    }

    if(buttonState == '4'){
      //relayIdle();
    }

    if(c == 3){
      digitalWrite(greenLed,HIGH);
    }
    //digitalWrite(greenLed,HIGH);
 
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



