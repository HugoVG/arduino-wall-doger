//// speed settings
const int turn_speed_fwd =200;
const int turn_speed_bwd =200;
const int roam_speed =150;

enum hardwareLinks{
  L_fwd =10,
  L_bwd =11,
  R_fwd =8,
  R_bwd =9,
  LED =13,
  afstand_echo =2,
  afstand_trig =3
};

// other settings
const int serial_speed =9600;
const int detection_distance =10;
const int turn_time =1000;

// varable delcaration
float distance;
bool turning;
bool forward;

void writePlusShow(int timer, int color){
    Serial.print(timer);
    Serial.println("...");
    analogWrite(LED, color);
    delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(serial_speed); // initaliseer de communicatie met de computer

  pinMode(L_fwd, OUTPUT); // initaliseer de in- en uitgangen
  pinMode(L_bwd, OUTPUT);
  pinMode(R_fwd, OUTPUT);
  pinMode(R_bwd, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(afstand_trig, OUTPUT);
  pinMode(afstand_echo, INPUT);

  Serial.println();
  for(int i = 5; i > 0 ;i--){
    if (i % 2 == 1){
      writePlusShow(i, 255);
    }
    else{
      writePlusShow(i, 0);
    }
  }
}

float detect_distance() {
  float dur; // variable voor het opslaan van de tijd voor de echo
  float dis; // variable voor het opslaan van de avstand in deze functie
  digitalWrite(afstand_trig, LOW);       // zet de afstand_trig op LOW
  delayMicroseconds(2);             // wacht 2 microseconden zodat de afstand_trig ook daadwerkelijk LOW is

  digitalWrite(afstand_trig, HIGH);      // zet de afstand_trig op HIGH om ultrasoon geluid te zenden
  delayMicroseconds(10);            // wacht 10 microseconden
  digitalWrite(afstand_trig, LOW);       // set de afstand_trig op LOW om het zenden te stoppen

  dur = pulseIn(afstand_echo, HIGH);     // vraag de tijdsduur op van hoe lang het duurde tot de afstand_echo HIGH was. 
                                          // Let op dat pulseIn het programma voor pauseert totdat er een signaal terug is ontvangen
                                          // standaard is de timeout 1 seconden. Dus het programma staat maximaal 1 seconde stil.
  dis = (dur * 0.0343) / 2;    // bereken de afstand aan de hand van de snelheid van het geluid
                                          // dit getal moet door 2 worden gedeeld omdat het geluid heen en terug is gegaan.
  return dis;
}

void loop() {
  distance = detect_distance(); // meet de afstand tot de muur
  Serial.print("Afstand: ");             // Toon het resultaat op de seri??le monitor
  Serial.println(distance);
  
  if(distance < detection_distance) {
    forward = false;
    analogWrite(L_fwd, 0);
    analogWrite(R_fwd, turn_speed_fwd);
    analogWrite(L_bwd, turn_speed_bwd);
    analogWrite(R_bwd, 0);
    analogWrite(LED, 255);
    delay(turn_time); // zodat de robot gegarandeerd een grotere hoek heeft gedraaid
  }
  else {
    
    analogWrite(L_fwd, roam_speed);
    analogWrite(R_fwd, roam_speed);
    analogWrite(L_bwd, 0);
    analogWrite(R_bwd, 0);
    analogWrite(LED, 0);
  }
}
