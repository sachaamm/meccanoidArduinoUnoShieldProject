#include <SoftwareSerial.h> 
#include <Esplora.h> 

int hm10Tx = 8;  // TX-O pin of hm10 mate, Arduino D8
int hm10Rx = 7;  // RX-I pin of hm10 mate, Arduino D7

SoftwareSerial hm10(hm10Tx, hm10Rx);

int thresholdJoystick = 500;



// 
byte startPatternByte = 'a';
byte endPatternByte = 'z';

char directionJoystick = 'x'; // default = idle // l : LEFT / r : RIGHT / u : UP / d : DOWN
char buttonValue = 'x'; // default = none // 1 : UP / 2 : LEFT / 3 : RIGHT / 4 : DOWN

int sliderState = 0;


void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  hm10.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  hm10.print("$");  // Print three times individually
  hm10.print("$");
  hm10.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  hm10.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  hm10.begin(9600);  // Start hm10 serial at 9600

  buttonValue = 'x'; // default : none
  
}

void loop()
{
  
  int buttonDown = Esplora.readButton(SWITCH_DOWN);
  int buttonUp = Esplora.readButton(SWITCH_UP);
  int buttonLeft = Esplora.readButton(SWITCH_LEFT);
  int buttonRight = Esplora.readButton(SWITCH_RIGHT);

  directionJoystick = 'x'; // default = idle 
  buttonValue = 'x'; // default : none
  

  ///////////////////////////////////
  // READ SENSORS VALUES 
  ///////////////////////////////////
  
  // JOYSTICK
  int xValue = Esplora.readJoystickX();        // read the joystick's X position
  int yValue = Esplora.readJoystickY();        // read the joystick's Y position

   // BUTTONS
  readButtonState(buttonUp,SWITCH_UP,'1');
  readButtonState(buttonLeft,SWITCH_LEFT,'2');
  readButtonState(buttonRight,SWITCH_RIGHT,'3');
  readButtonState(buttonDown,SWITCH_DOWN,'4');


  if(xValue > thresholdJoystick && xValue > yValue) directionJoystick = 'l'; // MOVE TO LEFT
  if(xValue < -thresholdJoystick && xValue < yValue)  directionJoystick = 'r'; // MOVE TO RIGHT
  if(yValue > thresholdJoystick && yValue > xValue) directionJoystick = 'd'; // MOVE BACKWARD
  if(yValue < -thresholdJoystick && yValue < xValue)  directionJoystick = 'u'; // MOVE FORWARD

  if(directionJoystick != 'x'){
    
    //buttonValue = 'x'; // ON REMET A ZERO buttonValue uniquement quand on recoit une direction de deplacement, ce qui permet de sauvegarder buttonValue quand le robot ne bouge pas

    
  }

  sliderState = Esplora.readSlider() / 256; // 4 ANIMATION STATES 

  switch(sliderState){

    case 3:
    Esplora.writeRGB(255, 0, 0);  // make the LED red   
    break;

    case 2:
    Esplora.writeRGB(0, 255, 0);  // make the LED green  
    break;

    case 1:
    Esplora.writeRGB(0, 0, 255);  // make the LED blue  
    break;

    case 0:
    Esplora.writeRGB(255, 255, 0); // make the LED yellow
    break;

    default:
    Esplora.writeRGB(255, 0, 0);  // make the LED red
    break;  
    
  }

  /////////////////////////////
  // DEFINE MESSAGE
  /////////////////////////////
  sendBTMessage();

  

  
  
 //delay(20);
  

  
  

  
}

////////////////////////////
// COMMUNICATION
////////////////////////////

void sendBTMessage(){

  //sendStartPattern();
  hm10.write(startPatternByte);
  hm10.write(directionJoystick);
  hm10.write(buttonValue);
  hm10.write(sliderState);
  hm10.write(endPatternByte);
  //sendEndPattern();

}

/*
void sendStartPattern(){

  for(int i = 0 ; i < sizeof(startPattern) ; i++){
    hm10.write(startPattern[i]);   
  }

}

void sendEndPattern(){

  for(int i = 0 ; i < sizeof(endPattern) ; i++){
    hm10.write(endPattern[i]);    
  }

}
*/


void readButtonState(int button, int argument,char message){

  if(button == LOW)  // If button is pressed
  {
    // Send message to hm10
    //hm10.print(message);
    buttonValue = message;
    
    
    while(button == LOW){ 
      button = Esplora.readButton(argument);
      
    } // Assures message is only sent once.
    
  }

}





