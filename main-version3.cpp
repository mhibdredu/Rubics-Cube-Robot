#include <Arduino.h>
#include <A4988.h>
#include <string.h>

#define rmdirection 0x016
#define lmdirection 0x018
#define bmdireciton 0x01A
#define lrmdirection 0x01C
#define rmstep 0x017
#define lmstep 0x019
#define bmstep 0x01B
#define lrmstep 0x01D
#define sdcright 0x022
#define sdcleft 0x023
#define bdcup 0x024
#define bdcdown 0x025
#define lmr 0x01E
#define lml 0x01F
#define lmu 0x020
#define lmd 0x021
#define stepcounts 200

// Stepper Motor Dec.
A4988 rightmotor (stepcounts , rmdirection , rmstep);
A4988 leftmotor  (stepcounts , lmdirection , lmstep);
A4988 downmotor  (stepcounts , bmdireciton , bmstep);
A4988 doplmotor  (stepcounts , lrmdirection , lrmstep);

// Parameters

int lmrs , lmls , lmus , lmds , x = 0 , swapface , index = 3  ,dface = 1, faceshashset[4]= { 1 ,2, 3 , 4 }; // Frotn , UP, Back , Down
String serialinput , soultionmovment , soultion;
void scancube();
void soultiuonimplimantiaon(char movment[5]);

void setup() {
  
  pinMode(rmdirection,OUTPUT);
  pinMode(lmdirection,OUTPUT);
  pinMode(bmdireciton,OUTPUT);
  pinMode(lrmdirection , OUTPUT);
 
  pinMode(rmstep,OUTPUT);
  pinMode(lmstep,OUTPUT);
  pinMode(bmstep,OUTPUT);
  pinMode(lrmstep , OUTPUT);
 
  pinMode(sdcright,OUTPUT);
  pinMode(sdcleft,OUTPUT);
 
  pinMode(bdcup,OUTPUT);
  pinMode(bdcdown,OUTPUT);
 
  pinMode(lmr,INPUT);
  pinMode(lml,INPUT);
  pinMode(lmd,INPUT);
  pinMode(lmu,INPUT);

  digitalWrite(sdcright,LOW);
  digitalWrite(sdcleft,LOW);
  
  digitalWrite(bdcup,LOW);
  digitalWrite(bdcdown,LOW);
  
  lmrs = digitalRead(lmr);
  lmls = digitalRead(lml);
  lmus = digitalRead(lmu);
  lmds = digitalRead(lmd);
  
  if(lmus == LOW){
    while (lmus == 0){
      digitalWrite(bdcup , 1);
      lmus = digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  if(lmrs == 0){
    while (lmrs == 0){
      digitalWrite(sdcright , 1);
      lmrs == digitalRead(lmr);
      if(lmrs == 1){
        digitalWrite(sdcright , 0);
      }
    }
  }
  Serial.begin(9600);
}

void loop() {
  
  serialinput = Serial.readString();
  if(serialinput == "Start"){
    scancube();
  } else {
    char arr[serialinput.length() + 1]; 
    strcpy(arr, serialinput.c_str()); 
    char *token = strtok(arr, " ");
    while (token != NULL)
    {
        soultiuonimplimantiaon(token);
        token = strtok(NULL, " ");
    }

  }
  
}

void scancube(){
	Serial.println("Kalibrierung starten");
  lmrs = digitalRead(lmr);
  lmls = digitalRead(lml);
  lmus = digitalRead(lmu);
  lmds = digitalRead(lmd);
  // Let side M leave the cube
  if(lmls == 0){
    while(lmls == 0){
      digitalWrite(lml , 1);
      if(lmls == 1){
        digitalWrite(lml , 0);
      }
    }
  }
  
  // Scan Front Left Back Right
  for (int i = 0 ; i < 4 ; i++){
    downmotor.rotate(90);
    if(i == 3){} else { delay(5000); }
  }

  if(lmrs == 0 ){
    while (lmrs == 0 ){
      digitalWrite(sdcright , 1);
      lmrs = digitalRead(lmr);
      if (lmrs == 1){
        digitalWrite(sdcright , 0);
      }
    }
  }
  if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  doplmotor.rotate(90);
  delay(5000);
  doplmotor.rotate(180);
  delay(3500);
  doplmotor.rotate(90);

  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  Serial.println("Kalibrierung enden");
}

void soultiuonimplimantiaon(char movment[5]){
  if(movment == "R"){
  	Serial.println("Drehen den rechten Motor um 90 Grad");
    rightmotor.rotate(90);
  } else if(movment == "R2"){
    Serial.println("Drehen den rechten Motor um 180 Grad");
    rightmotor.rotate(180);
  } else if(movment == "R'"){
  	Serial.println("Drehen den rechten Motor um -90 Grad");
    rightmotor.rotate(-90);
  } else if(movment == "L"){
 		Serial.println("Drehen den linken Motor um 90 Grad");
    leftmotor.rotate(90);
  } else if(movment == "L2"){
  	Serial.println("Drehen den linken Motor um 180 Grad");
    leftmotor.rotate(180);
  } else if(movment == "L'"){
  	Serial.println("Drehen den linken Motor um -90 Grad");
    leftmotor.rotate(-90);
  } else if(movment == "F"){
    swapface = 0;
    while(dface != 1){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else if (dface != 1) {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
 	Serial.println("Drehen den unten Motor um 90 Grad");
  downmotor.rotate(90);
  } else if(movment == "F2"){
    swapface = 0;
    while(dface != 1){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else if (dface != 1) {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um 180 Grad");
  downmotor.rotate(180);

  } else if(movment == "F'"){
    swapface = 0;
    while(dface != 1){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else if (dface != 1) {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um -90 Grad");
  downmotor.rotate(-90);
  } else if(movment == "B"){
    swapface = 0;
  while(dface != 3){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um 90 Grad");
  downmotor.rotate(90);
  } else if(movment == "B2"){
    swapface = 0;
    while(dface != 1){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else if (dface != 1) {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um 180 Grad");
  downmotor.rotate(180);
  } else if(movment == "B'"){
    swapface = 0;
  while(dface != 3){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um -90 Grad");
  downmotor.rotate(-90);
  } else if(movment == "U"){
    swapface = 0;
  while(dface != 2){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um 90 Grad");
  downmotor.rotate(90);
  } else if(movment == "U2"){
        swapface = 0;
  while(dface != 2){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um 180 Grad");
  downmotor.rotate(180);

  } else if(movment == "U'"){
        swapface = 0;
  while(dface != 2){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
  if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um -90 Grad");
  downmotor.rotate(-90);

  } else if(movment == "D"){
    swapface = 0;
  while(dface != 4){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
    if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um 90 Grad");
  downmotor.rotate(90);

  } else if(movment == "D2"){
    swapface = 0;
  while(dface != 4){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
      if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um 180 Grad");
  downmotor.rotate(180);

  } else if(movment == "D'"){
    swapface = 0;
  while(dface != 4){
    if (index == 3){
      index = 0;
      dface = faceshashset[index];
      swapface++;
    } else {
      index++;
      dface = faceshashset[index];
      swapface++;
    }
  }
      if(swapface > 0){
    if(lmds == 0 ){
    while (lmds == 0){
      digitalWrite(bdcdown, 1);
      lmds = digitalRead(lmd);
      if(lmds == 1){
        digitalWrite(bdcdown , 0);
      }
    }
  }
  delay(100);
  doplmotor.rotate(90*swapface);
  delay(100);
  if(lmus == 0){
    while (lmus == 0){
      digitalWrite(bdcup, 1);
      lmus == digitalRead(lmu);
      if(lmus == 1){
        digitalWrite(bdcup , 0);
      }
    }
  }
  }
  Serial.println("Drehen den unten Motor um -90 Grad");
  downmotor.rotate(-90);

  }
}
