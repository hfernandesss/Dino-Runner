#include  <LiquidCrystal.h>
#include  <time.h>
#include <EEPROM.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);



// cacto
byte cacto[8] = {
  B00100,
  B00100,
  B10101,
  B10101,
  B11111,
  B00100,
  B00100,
  B00100
    
};

//cacto2
byte cacto2[8] = {
  B00100,
  B00100,
  B10100,
  B10101,
  B11101,
  B00111,
  B00100,
  B00100
    
};

// dinossauro perna fechada
byte newChar8[8] = {
	B00111,
	B00101,
	B00110,
	B01100,
	B01110,
	B11100,
	B01110,
	B01010
};

// dinossauro perna aberta
byte newChar9[8] = {
	B00111,
	B00101,
	B00110,
	B01100,
	B01110,
	B11100,
	B01110,
	B10001
};

// vazio
byte newCharl[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

//passaro asa aberta
byte passaroA[8] = {
  B00100,
  B01000,
  B11111,
  B00111,
  B00110,
  B00110,
  B00100,
  B00000
};

//passaro asa fechada
byte passaroB[8] = {
  B00100,
  B00110,
  B00110,
  B00111,
  B01011,
  B11111,
  B01111,
  B00000
};

int oi;
int ponto_provisorio = 0;
int marcador_run = 0;
int i = 0;

//posicao do enimigo 1
int n = 15;

//posicao do enimigo 2
int n2 = 15;

//pontuacao
int pontos = 0;

//flag para ver se o dinossauro esta no ar ou no chao
int flagDino = 1;

//int para mudar as pernas do dinossauro e as asas do 
int andar = 1;
int tempopulo = 0;
int sla = 3;
int velocidade = 0;
int pascac = 0;
int enemi = 0;
int enemi2 = 0;

void setup() {
  pinMode(9,INPUT);
  pinMode(6,OUTPUT);//Buzzer
  lcd.createChar(5, cacto2);
  lcd.createChar(6, cacto);
  lcd.createChar(7, newChar8);
  lcd.createChar(8, newChar9);
  lcd.createChar(9, newCharl);
  lcd.createChar(10, passaroA);
  lcd.createChar(11, passaroB);
  lcd.begin(16, 2);
  
  lcd.setCursor(3,0);
  lcd.print("DINO RUNNER");
  lcd.setCursor(0,1);
  lcd.print("Press any button");
  
 while(!digitalRead(9)){
 	
 }
  
    tone(6,294,300);
    delay(200);
    tone(6,392,200);
  delay(500);
  lcd.clear();
  
 
}

void loop() {
	
/*O numero abaixo define o tempo de salto do dinossauro
  Desta forma, para altera-lo basta variar:
  "n" tal que (tempopulo < n)*/
  
  	  if(digitalRead(9) && tempopulo<3){
        flagDino = 0;
        tempopulo +=1;
        if(tempopulo == 1){
          tone(6,262,200);
        }
      }else{
        tempopulo = 0;
      }
  printaDinossauroMovimento();
  
  if(pascac == 0){
    if(enemi== 0){
      enemi = random(1,4);
    }else{
      enemi2 = random(1,4);
    }
    pascac = 8;
  }
   	pascac-=1;
  if(enemi > 1){
    n = printCacto(n,enemi);
    if(n == 15){
      enemi = 0;
    }
  }else if(enemi == 1){
    n = printPassaro(n);
    if(n == 15){
      enemi = 0;
    }
  }
  if(enemi2 > 1){
    n2 = printCacto(n2,enemi2);
    if(n2 == 15){
      enemi2 = 0;
    }
  }else if(enemi2 == 1){
    n2 = printPassaro(n2);
    if(n2 == 15){
      enemi2 = 0;
    }
  }
	lcd.setCursor(12+sla,0);
    lcd.print(pontos);
  
    flagDino = 1;
  	pontos+=1;
 
  if(velocidade <200){
  	velocidade +=1;
  }
  if(pontos == 10){
    sla -= 1;
  }
   if(pontos == 100){
    sla -= 1;
     tone(6,294,300); //RE
    delay(50);
     tone(6,392,200);
  }
  if(pontos == 1000){
    sla -= 1;
    tone(6,294,300); //RE
    delay(50);
    tone(6,392,200);
  }
  delay(300-velocidade);
  lcd.clear();
}

int printCacto(int b, int m){      
       lcd.setCursor(b-=1,1);
       lcd.write(3+m);
       
  if(flagDino == 1 && b == 1){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Game Over");
    lcd.setCursor(0,1);
    lcd.print("Pontuacao:");
     if(marcador_run > 0){
     ponto_provisorio = readIntFromEEPROM(0);
     
    }
    if(ponto_provisorio < pontos){
      writeIntIntoEEPROM(0, pontos);
    }
    tone(6,392,200);
    lcd.setCursor(10,1);
    lcd.print(pontos);
    delay(500);
    marcador_run++;
    while(!digitalRead(9)){
    }
    lcd.clear();
    lcd.setCursor(0,0);
    ponto_provisorio = readIntFromEEPROM(0);
    lcd.print("Maior pontuacao");
    lcd.setCursor(5,1);
    lcd.print(ponto_provisorio);
    delay(1000);
    tone(6,294,300);
    delay(200);
    tone(6,392,200);
    sla = 3;
  	pontos = 0;
    velocidade = 0;
    n = 15;
    n2 = 15;
    enemi = 0; 
    enemi2 = 0;
    pascac = 0;
    lcd.clear();
    return 15;
  }
  
  if(b==0){
    b = 15;
  }
  return b;
}

//Funcao para printar o passaro em movimento
int printPassaro(int b){      
       lcd.setCursor(b-=1,0);
       lcd.write(10+andar);
       
  if(flagDino == 0 && b == 1){
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Game Over");
    lcd.setCursor(0,1);
    lcd.print("Pontuacao:");
    if(marcador_run > 0){
     ponto_provisorio = readIntFromEEPROM(0);
    }
    if(ponto_provisorio < pontos){
      writeIntIntoEEPROM(0, pontos);
    }
    tone(6,392,200);
    lcd.setCursor(10,1);
    lcd.print(pontos);
    delay(500);
    marcador_run++;
    while(!digitalRead(9)){
    }
    lcd.clear();
    lcd.setCursor(0,0);
    ponto_provisorio = readIntFromEEPROM(0);
    lcd.print("Maior pontuacao");
    lcd.setCursor(5,1);
    lcd.print(ponto_provisorio);
    delay(1000);
    tone(6,294,300);
    delay(200);
    tone(6,392,200);
    sla = 3;
  	pontos = 0;
    velocidade = 0;
    n = 15;
    n2 = 15;
    enemi = 0; 
    enemi2 = 0;
    pascac = 0;
    lcd.clear();
    return 15;
  }
  
  if(b==0){
    b = 15;
  }
  return b;
}

//Funcao para printar o dinossauro 
void printaDinossauroMovimento(){
  
	lcd.setCursor(1,flagDino);
	lcd.write(7+andar);
  	if(andar == 1){
    	andar = 0;
    }else{
     	andar = 1;
    } 
}

//Funcao para guardar um inteiro na EEPROM
void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

//Funcao para ler um inteiro da EEPROM
int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}



