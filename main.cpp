#include <Arduino.h>
#include <A4988.h>
#include <string.h>
//#include <bits/stdc++.h>

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

int lmrs , lmls , lmus , lmds , x , swapface , index = 3  ,dface = 1, faceshashset[4]= { 1 ,2, 3 , 4 }; // Frotn , UP, Back , Down
String serialinput , soultionmovment , soultion;

void setup() {
  
  pinMode(rmdirection,OUTPUT);
  pinMode(lmdirection,OUTPUT);
  pinMode(bmdireciton,OUTPUT);
 
  pinMode(rmstep,OUTPUT);
  pinMode(lmstep,OUTPUT);
  pinMode(bmstep,OUTPUT);
 
  pinMode(sdcright,OUTPUT);
  pinMode(sdcleft,OUTPUT);
 
  pinMode(bdcup,OUTPUT);
  pinMode(bdcdown,OUTPUT);
 
  pinMode(lmr,INPUT);
  pinMode(lml,OUTPUT);
  pinMode(lmd,OUTPUT);
  pinMode(lmu,OUTPUT);

  digitalWrite(sdcright,LOW);
  digitalWrite(sdcleft,LOW);
  
  digitalWrite(bdcup,LOW);
  digitalWrite(bdcdown,LOW);
  
  lmrs = digitalRead(lmr);
  lmls = digitalRead(lml);
  lmus = digitalRead(lmu);
  lmds = digitalRead(lmd);

  if (lmrs == HIGH || lmls == LOW || lmus == LOW || lmds == HIGH){
    
    while(lmrs == HIGH || lmls == LOW){
      
      digitalWrite(sdcleft , HIGH);
      
      lmrs = digitalRead(lmr);
      lmls = digitalRead(lml);

      if(lmls == HIGH && lmrs == LOW){
        digitalWrite(sdcleft , LOW);
      }

    }

    while(lmus == LOW || lmds == HIGH){

      digitalWrite(bdcup , HIGH);
      
      lmus = digitalRead(lmu);
      lmds = digitalRead(lmd);

      if(lmus == HIGH && lmds == LOW){
        digitalWrite(bdcdown, LOW);
      }
      
    }

  }
  
  Serial.begin(9600);
}

void loop() {
  serialinput = Serial.readString();
  
  if(serialinput == "start") scancube(); 
        else  soultion = serialinput ; splitsoultionintoarray(soultion);
  
}

void scancube(){
  
  if(lmrs == LOW || lmls == HIGH){
    while (lmrs == LOW || lmls == HIGH){
      digitalWrite(sdcright , HIGH);
      lmrs = digitalRead(lmr);
      lmls = digitalRead(lml);
      if(lmrs == HIGH && lmls == LOW){
        digitalWrite(sdcright , LOW);
      }
    }
  }

  if(lmrs == HIGH && (lmds == LOW || lmus == HIGH )){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  
  for (int i = 0 ; i < 4 ; i++){
    doplmotor.move(90);
    if (i == 3){
      
    } else {
      delay(5000);
    }

  }
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  if(lmus == HIGH && (lmrs == HIGH || lmls == LOW)){
    while (lmrs == HIGH || lmls == LOW){
      digitalWrite(sdcleft , HIGH);
      lmrs = digitalRead(lmr);
      lmls = digitalRead(lml);
      if(lmrs == LOW && lmls == HIGH){
        digitalWrite(sdcleft , LOW);
      }
    }
  }
  
  downmotor.move(90);
  delay(5000);
  downmotor.move(180);
  delay(5000);
  downmotor.move(-270);

  if(lmrs == LOW || lmls == HIGH){
    while (lmrs == LOW || lmls == HIGH){
      digitalWrite(sdcright , HIGH);
      lmrs = digitalRead(lmr);
      lmls = digitalRead(lml);
      if(lmrs == HIGH && lmls == LOW){
        digitalWrite(sdcright , LOW);
      }
    }
  }
}

void soultionimp(char movment[5]){
  int moves;
  while (movment[x] != '\0')
  {
    moves+= (int) movment[x];
  }
  
  switch (moves)
  {
  
  case 82: // R
    rightmotor.move(90);
    break;

  case 132: // R2
    rightmotor.move(180);
    break;

  case 121: // R'
    rightmotor.move(-90);
    break;

  case 76: // L
    leftmotor.move(90);
    break;

  case 126: // L2
    leftmotor.move(180);
    break;
  
  case 115: // L'
    leftmotor.move(-90);
    break;
  
  case 70: // F
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 1) downmotor.move(90);
    break;

  case 120: // F2
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 1) downmotor.move(180);
    break;

  case 109: // F'
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 1) downmotor.move(-90);
    break;

  case 66: // B
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 3) downmotor.move(90);
    break;

  case 116: // B2
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 3) downmotor.move(180);
    break;

  case 105: // B'
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 3) downmotor.move(-90);
    break;

  case 85: // U
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 2) downmotor.move(90);
    break;

  case 135: // U2
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 2) downmotor.move(180);
    break;

  case 124: // U'
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 2) downmotor.move(-90);
    break;

  case 68: // D
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 4) downmotor.move(90);
    break;

  case 118: // D2
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 4) downmotor.move(180);
    break;

  case 107: // D'
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

  if(lmds == LOW || lmus == HIGH ){
    while (lmds == LOW || lmus == HIGH)
    {
      digitalWrite(bdcdown , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == HIGH && lmus == LOW){
        digitalWrite(bdcdown , LOW);
      }
    }
  }
  doplmotor.move(90*swapface);
  if(lmds == HIGH || lmus == LOW ){
    while (lmds == HIGH || lmus == LOW)
    {
      digitalWrite(bdcup , HIGH);
      lmds = digitalRead(lmd);
      lmus = digitalRead (lmu);
      if(lmds == LOW && lmus == HIGH){
        digitalWrite(bdcup , LOW);
      }
    }
  }
  }
  if(dface == 4) downmotor.move(-90);
    break;

  default:
    break;
  }

}


void splitsoultionintoarray(String str){

    String str1 = str;
    char arr[str1.length() + 1]; 
    strcpy(arr, str1.c_str()); 
    char *token = strtok(arr, " ");
    while (token != NULL)
    {
        soultionimp(token);
        token = strtok(NULL, " ");
    }

  }

  /*
 #include <bits/stdc++.h> 
#include <iostream>
using namespace std; 
 
int main() 
{ 
      
    string str = "";
    getline (cin ,str);
    char arr[str.length() + 1]; 
    strcpy(arr, str.c_str()); 
    char *token = strtok(arr, " ");
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }
 
    return 0; 
} 
*/
