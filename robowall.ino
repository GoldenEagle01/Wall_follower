#define SERIALDUMP

#define SSIDE     8
#define SFRONT    7
#define READDELAY 10
#define SPEEDDIF  35
#define SPEEDL    120
#define SPEEDR    120

#define MLA       3   
#define MLB       5
#define MRA       6
#define MRB       9

void setup() {
  #if defined(SERIALDUMP)
    Serial.begin(9600);
  #endif
  pinMode(MLA, OUTPUT);
  pinMode(MLB, OUTPUT);
  pinMode(MRA, OUTPUT);
  pinMode(MRB, OUTPUT);
  setMotors(0,0);
  pinMode(SSIDE, INPUT);
  pinMode(SFRONT, INPUT);
  delay(5000);  // ca sa am timp sa asez robotul pe traseu
}

void loop() {
  while(digitalRead(SFRONT)){   // daca nu este nimic in fata merge paralel
    switch(onEdge()){
       case  0: setMotors(SPEEDL, SPEEDR); break;                         // merge drept
       case -1: setMotors(SPEEDL+SPEEDDIF, SPEEDL-1.6*SPEEDDIF); break;   // vireaza usor la stanga
       case  1: setMotors(SPEEDL-1.6*SPEEDDIF, SPEEDR+SPEEDDIF); break;   // vireaza usor la dreapta
    }
  }
  setMotors(SPEEDL, -SPEEDR);  // intoarcere 90' spre dreapta daca areobstacol in fata
}


int onEdge(){ // citeste 5 valori
  boolean var0 = digitalRead(SSIDE);
  delay(READDELAY);
  boolean var1 = digitalRead(SSIDE);
  delay(READDELAY);
  boolean var2 = digitalRead(SSIDE);
  delay(READDELAY);
  boolean var3 = digitalRead(SSIDE);
  delay(READDELAY);
  boolean var4 = digitalRead(SSIDE);
  
  if(var0&&var1&&var2&&var3&&var4) return 1;      // prea aproape daca toate citirile sunt 1
  if(!var0&&!var1&&!var2&&!var3&&!var4) return -1;// prea departe daca toate citirile sunt 0
  else return 0;                                  // perfect !!!  daca exista citiri diferite
}

void setMotors(int lspeed, int rspeed){
  #if defined(SERIALDUMP)
    Serial.print(lspeed);
    Serial.print(" ");
    Serial.println(rspeed);
  #endif

  if(lspeed>0){
    if(lspeed>255) lspeed=255; // daca viteza dep 255 se rectifica variabila
    analogWrite(MLA, lspeed);
    analogWrite(MLB, 0); 
  }else{
    if(lspeed<-255) lspeed=255; // daca viteza dep -255 se rectifica variabila si se aduce la valoarea absoluta
    analogWrite(MLA, 0);
    analogWrite(MLB, lspeed); 
  }
  if(rspeed>0){
    if(rspeed>255) rspeed=255; // same shit 
    analogWrite(MRA, rspeed);
    analogWrite(MRB, 0); 
  }else{
    if(rspeed<-255) rspeed=255; // same shit
    analogWrite(MRA, 0);
    analogWrite(MRB, rspeed); 
  }
  
}

