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


int ponto_provisorio = 0;
int marcador_run = 0;
int i = 0;

//posicao do inimigo 1
int n = 15;

//posicao do inimigo 2
int n2 = 15;

//pontuacao
int pontos = 0;

//flag para ver se o dinossauro esta no ar ou no chao
int flagDino = 1;

//int para mudar as pernas do dinossauro e as asas do passaro
int andar = 1;

//guarda o tempo em que o dinossauro passou no ar
int tempopulo = 0;

//troca o inicio dos numeros da pontuacao para mais digitos
int atualiza = 3;

//guarda a velocidade em que o jogo vai roda
int velocidade = 0;

//conta o intervalo entre dois inimigos
int pascac = 0;

//conta se o inimigo 1 sera algum dos cactos ou um passaro 
int enemi = 0;

//conta se o inimigo 2 sera algum dos cactos ou um passaro
int enemi2 = 0;

void setup() {
  pinMode(9,INPUT);//Botao
  pinMode(6,OUTPUT);//Buzzer
  lcd.createChar(5, cacto2);
  lcd.createChar(6, cacto);
  lcd.createChar(7, newChar8);
  lcd.createChar(8, newChar9);
  lcd.createChar(9, newCharl);
  lcd.createChar(10, passaroA);
  lcd.createChar(11, passaroB);
  lcd.begin(16, 2);
  
  //Menu
  lcd.setCursor(3,0);
  lcd.print("DINO RUNNER");
  lcd.setCursor(0,1);
  
  //literalmente qualquer botao funciona 
  lcd.print("Press any button");
  
  //aguarda o individuo apertar o botao
 while(!digitalRead(9)){
 	
 }
  //preparacao para o inicio do jogo
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
    //o valor da variável vai de 8 a zero pois como o display é 16x2, 8 é a metade da tela, dessa forma os cactos e passaros vao espaçados na mesma distancia 
    pascac = 8; 
  }
  //indo de 8 a zero novamente
  pascac-=1;
  
  if(enemi > 1){ //maior do que 1 é cacto
    n = printCacto(n,enemi);
    if(n == 15){
      enemi = 0;
    }
  }else if(enemi == 1){ //igual a 1 é passaro
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
	lcd.setCursor(12+atualiza,0);
    lcd.print(pontos);
  
    flagDino = 1;
  	pontos+=1; //cada passo do dinossauro é um novo ponto
 
  if(velocidade <200){ //limite pela segurança do funcionamento do jogo
  	velocidade +=1; //aumento da velocidade
  }
  //aumento da casa de valores da pontuação do jogador 
  if(pontos == 10){
    atualiza -= 1;
  }
   if(pontos == 100){
    atualiza -= 1;
     tone(6,294,300); //RE
    delay(50);
     tone(6,392,200);
  }
  if(pontos == 1000){
    atualiza -= 1;
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
       
  if(flagDino == 1 && b == 1){ //game over: cacto e dinossauro na mesma casa
    funcaoGameOver ();
    return 15; // para setar enemi = 0 no inicio do código 
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
       
  if(flagDino == 0 && b == 1){ //dino no ar e passaro na mesma casa que ele
    funcaoGameOver ();
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

//Funçao de Game Over
int funcaoGameOver (){
  	lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Game Over");
    lcd.setCursor(0,1);
    lcd.print("Pontuacao:");
     
    if(marcador_run > 0){  
     ponto_provisorio = readIntFromEEPROM(0); 
    }
    
    if(ponto_provisorio < pontos){ //checa se a pontuação atual é maior do que a nova
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
    atualiza = 3;
  	//reseta as variáveis para um novo jogo
    pontos = 0;
    velocidade = 0; 
    n = 15;
    n2 = 15;
    enemi = 0; 
    enemi2 = 0;
    pascac = 0;
    lcd.clear();
  
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



