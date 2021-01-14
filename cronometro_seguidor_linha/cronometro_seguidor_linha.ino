/*
 * ATENCAO: START_SENSOR E STOP_SENSOR FORAM SIMULADOS COM BOTÃO,
 * PARA USO COM SENSOR IR DEVE SE ALTERAR AS LINHAS (42, 43 -> INPUT) E (156, 178 -> RETIRAR O !)
 */


#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define RESET 10            //botao reset
#define START_SENSOR 11     //sensor start
#define STOP_SENSOR 12      //sensor stop
#define VOLTAS 2            //variavel de definicao de numeros de volta
#define BUZZER 8            //pino buzzer

//0x3F (modulo Luis)
//0x27 (default)
LiquidCrystal_I2C lcd(0x3F, 16, 2); // modulo para lcd 

int seg = 0;
int mili = 0;
int seg1 = 0;
int buzzer_count = 0;
int buzzer_duration = 0;
unsigned long previous_time = 0;
unsigned long current_time = millis();
unsigned long timer_seg = (millis() / 100);
unsigned long timer_mil = millis();
unsigned long timer_mil2 = millis();
bool stop_count = false;
bool start_count = false;
bool fim = false;
int conta_volta = 0;

/*
 * SETUP
 */
void setup() {

  pinMode (RESET, INPUT_PULLUP); 
  pinMode (START_SENSOR, INPUT_PULLUP); 
  pinMode (STOP_SENSOR, INPUT_PULLUP); 
  pinMode (BUZZER, OUTPUT); 
  
  //lcd.begin(16, 2); // lcd default
  lcd.init(); //modulo Luis
  lcd.setBacklight(HIGH);

  lcd.setCursor(0, 0);
  lcd.print("   Cronometro   " );
  lcd.setCursor(0, 1);
  lcd.print(" Seguidor Linha " );  

  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Espaco CMaker" );
  lcd.setCursor(0, 1);
  lcd.print("   Versao 1.0   " );  
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("#  Aguardando  #" );
  lcd.setCursor(0, 1);
  lcd.print("#  Competidor  #" );
}
/*
 * LOOP
 */
void loop() {
  //atualiza valor para controle do buzzer
  current_time = millis();
  //le os sensores e botao reset
  le_sensores();
  //verifica bip
  bip(buzzer_count,buzzer_duration);
  //flag start = true e stop_count = false inicia contagem 
  if(start_count && !stop_count){
    conta_tempo();
    atualiza_display();
  }else{    
     //verifica se o tempo esta contando para atualizar o display
     if(conta_volta>0){
       atualiza_display();
     }
  }
}
/*
 * CONTA TEMPO
 */
void conta_tempo(){
  //atualiza os segundos
  if ((millis() / 100) - timer_seg > 9){
    timer_seg = (millis() / 100);
    seg++;
    atualiza_display();
  }
  //atualiza os milisegundos
  timer_mil = (millis());
}
/*
 * ATUALIZA DISPLAY
 */
void atualiza_display(){
    //Atualiza a primeira linha com o valor da volta atual
    lcd.setCursor(0,0);
    lcd.print("ATUAL:");
    lcd.setCursor(7,0);
    lcd.print((seg/60)/10);
    lcd.setCursor(8,0);
    lcd.print((seg/60)%10);
    lcd.setCursor(9,0);
    lcd.print(":");
    lcd.setCursor(10,0);
    lcd.print((seg%60)/10);
    lcd.setCursor(11,0);
    lcd.print((seg%60)%10); 
    lcd.setCursor(12,0);
    lcd.print(":");    
    lcd.setCursor(13,0);
    lcd.print((timer_mil/100)%10);
    lcd.setCursor(14,0);
    lcd.print((timer_mil/10)%10);    
    lcd.setCursor(15,0);
    lcd.print(timer_mil%10); 
    //atualiza a segunda linha com o valor da primeira volta caso esteja na segunda volta
    if(conta_volta>1){
      lcd.setCursor(0,1);
      lcd.print("LAST:");
      lcd.setCursor(7,1);
      lcd.print((seg1/60)/10);
      lcd.setCursor(8,1);
      lcd.print((seg1/60)%10);
      lcd.setCursor(9,1);
      lcd.print(":");
      lcd.setCursor(10,1);
      lcd.print((seg1%60)/10);
      lcd.setCursor(11,1);
      lcd.print((seg1%60)%10); 
      lcd.setCursor(12,1);
      lcd.print(":");    
      lcd.setCursor(13,1);
      lcd.print((timer_mil2/100)%10);
      lcd.setCursor(14,1);
      lcd.print((timer_mil2/10)%10);    
      lcd.setCursor(15,1);
      lcd.print(timer_mil2%10);              
    }
}
/*
 * LE SENSORES
 */
void le_sensores(){
  //verifica sensor start
  if(!digitalRead(START_SENSOR)){
    //start primeira volta
    if(!start_count && !fim && !stop_count){
      lcd.clear();
      start_count = true;  
      stop_count = false;
      timer_seg = (millis() / 100);
      conta_volta++; //conta_volta recebe +1
      buzzer_count = 2; //2 pulsos, on e off
      buzzer_duration = 200; //tempo em milisegundos
      bip(buzzer_count,buzzer_duration);
    }
    //start segunda volta
    if(conta_volta == 1 && start_count && !digitalRead(STOP_SENSOR)){
      conta_volta++; //conta numero de voltas
      seg1 = seg;
      timer_mil2 = timer_mil;
      seg = 0;      
      start_count = false;
    }
  }
  //para contagem, sensor stop
  if(!digitalRead(STOP_SENSOR)){
    if(conta_volta == 1 && start_count && !digitalRead(STOP_SENSOR)){
      conta_volta++; //conta numero de voltas
      seg1 = seg;
      timer_mil2 = timer_mil;
      seg = 0;      
      timer_mil = 0;
      start_count = false;
      buzzer_count = 2;
      buzzer_duration = 200;
      bip(buzzer_count,buzzer_duration);
    }
    //fim de segunda volta e prova
    if(conta_volta >= VOLTAS && ((seg%60)%10)>0 && !fim){
      stop_count = true;
      start_count = false;
      fim = true;
      buzzer_count = 6;
      buzzer_duration = 150;
      bip(buzzer_count,buzzer_duration);
    }
  }
  //botao reset, zera o tempo e variaveis auxiliares
  if (!digitalRead(RESET)==1 && fim && conta_volta > 0){
    stop_count = false;
    start_count = false;
    fim = false;
    conta_volta = 0;
    seg = 0;
    seg1 = 0;
    timer_mil = 0;
    timer_mil2 = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("#  Aguardando  #" );
    lcd.setCursor(0, 1);
    lcd.print("#  Competidor  #" );
  }
}
/*
 * BUZZER
 */
int bip(int quantidade, int duracao){
  buzzer_count = quantidade;//on off
  if (current_time - previous_time >= duracao && buzzer_count>0){
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    previous_time = current_time;
    buzzer_count--;
  }
}
