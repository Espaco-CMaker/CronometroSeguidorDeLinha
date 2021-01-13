#include <LiquidCrystal_I2C.h>// biblioteca modulo i2c
#include <Wire.h> // para modulo lcd i2c

#define btn1 10            //entrada sensor
#define voltas 2

//0x3F (modulo Luis)
//0x27 (default)
LiquidCrystal_I2C lcd(0x3F, 16, 2); // modulo para lcd 

int seg = 0;
int mili = 0;
int seg1 = 0;
unsigned long timer_seg = (millis() / 100);
unsigned long timer_mil = millis();
unsigned long timer_mil2 = millis();
bool stop_count = false;
bool start_count = false;
bool curr_lap = false;
bool last_lap = false;
bool fim = false;
int conta_volta = 0;

void setup() {

  pinMode (btn1, INPUT_PULLUP);
  
  //lcd.begin(16, 2); //default
  lcd.init(); //modulo Luis
  lcd.setBacklight(HIGH);

  lcd.setCursor(0, 0);
  lcd.print("   Cronometro   " );
  lcd.setCursor(0, 1);
  lcd.print(" Seguidor Linha " );  

  delay(2000);
  lcd.clear(); //limpa dados lcd
  lcd.setCursor(0, 0);
  lcd.print(" Espaco CMaker" );
  delay(2000);
  lcd.clear(); //limpa dados lcd
  lcd.setCursor(0, 0);
  lcd.print("#  Aguardando  #" );
  lcd.setCursor(0, 1);
  lcd.print("#  Competidor  #" );
}

void loop() {
  verifica_botao();
  if(start_count){
    conta_tempo();
    atualiza_display();
  }else{    
     if(conta_volta>0){
       atualiza_display();
     }
  }
}



void conta_tempo(){
  if ((millis() / 100) - timer_seg > 9){
    timer_seg = (millis() / 100);
    seg++;
    atualiza_display();
    //Serial.println(seg);
  }
  //if ((millis()) - timer_mil > 9){
    timer_mil = (millis());
  //  mili = millis();
    //Serial.println(seg);
 // }
  
}

void atualiza_display(){
    // 1
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

void verifica_botao(){
  if (!digitalRead(btn1)==1){    
    
    if(!start_count && !fim){
      lcd.clear();
      start_count = true;  
      timer_seg = (millis() / 100);
      conta_volta++; //conta numero de voltas
    }
    if(!curr_lap){
      curr_lap = true;
    }
    if(conta_volta == 1 && start_count && ((seg%60)%10)>0){
      conta_volta++; //conta numero de voltas
      seg1 = seg;
      timer_mil2 = timer_mil;
      seg = 0;      
    }
    if(conta_volta >= voltas && ((seg%60)%10)>0){
      start_count = false;  
      fim = true;
    }
  }
}
