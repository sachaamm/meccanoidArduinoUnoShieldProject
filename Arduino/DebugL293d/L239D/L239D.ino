int pin1Moteur1=12; //pin de commande moteur 1
int pin2Moteur1=8; // pin de commande moteur 1
int pinPMoteur1=11;// pin PWM moteur 1
int pin1Moteur2=2; // pin de commande moteur 2
int pin2Moteur2=4; // pin de commande moteur 2
int pinPMoteur2=5; // pin PWM moteur 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //initialise la communication série
  pinMode(pin1Moteur1,OUTPUT);
  pinMode(pin2Moteur1,OUTPUT);
  pinMode(pinPMoteur1,OUTPUT);
  pinMode(pin1Moteur2,OUTPUT);
  pinMode(pin2Moteur2,OUTPUT);
  pinMode(pinPMoteur2,OUTPUT);

  pinMode(13,OUTPUT);
}

void loop() {
  actionMoteur(1,1,100); //moteur 1 100% puissance sens 1
  actionMoteur(2,1,100); //moteur 2 100% puissance sens 1
  digitalWrite(13,HIGH);
  delay(5000); //attente de 5 secondes
  actionMoteur(1,-1,100); //moteur 1 100% puissance sens 1
  actionMoteur(2,-1,100); //moteur 2 100% puissance sens 1
  digitalWrite(13,LOW);
  delay(5000); //attente de 5 secondes
  /*
  actionMoteur(1,1,50); //moteur 1 50% puissance sens 1
  actionMoteur(2,-1,50); //moteur 2 50% puissance sens -1
  delay(2000);
  actionMoteur(1,0,0); //arrêt moteur 1
  actionMoteur(2,0,0); //arrêt moteur 2
  delay(1000);

  */
}

void actionMoteur(int moteur,int sens,int pourcentage){
  int pin1,etat1,pin2,etat2,pinP,puissance; //variable de la fonction
  //test numéro du moteur
  if (moteur==1){
    pin1=pin1Moteur1;
    pin2=pin2Moteur1;
    pinP=pinPMoteur1;
  }
  else {
    pin1=pin1Moteur2;
    pin2=pin2Moteur2;
    pinP=pinPMoteur2;
  }
  //test sens du moteur 1,-1 (sens contrainre) ou tout autre valeur (stoppe le moteur)
  if (sens==1){
    etat1=1;
    etat2=0;
  }
  else if (sens==-1){
    etat1=0;
    etat2=1;
  }
  else {
    etat1=0;
    etat2=0;
  }
  puissance=map(pourcentage,0,100,0,255);
  analogWrite(pinP,puissance);
  digitalWrite(pin1,etat1);
  digitalWrite(pin2,etat2);
  //affichage sur le moniteur série (facultatif)
  Serial.print("Moteur : ");
  Serial.print(moteur);
  if (sens==-1 || sens==1){
    Serial.print(" sens : ");
    Serial.print(sens);
  }
  else {
    Serial.print(" ! stop ! ");
  }
  Serial.print(" puissance : ");
  Serial.println(pourcentage);
}

