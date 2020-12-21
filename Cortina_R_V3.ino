#include <EEPROM.h>

const int BOTAOEXECUTAR = 2;
const int LED1 = 4;
const int SWFOTO = 5;
const int PWM1 = 6;
const int SWPWM = 7;
const int SWGRAVAR = 8;
const int MOTOR2 = 9;
const int MOTOR1 = 10;
const int SENSORHALL = 11;
const int POWER24 = 12;
const int SENSORSTOP = A1;
const int SENSORFOTO = A2;
const int RX2 = A3;
const int RX1 = A4;
const int SENSORTENSAO = A5;

int TEMPOBOTAO = 100;
int TEMPOMOTOR = 100;

int CG;
int IG;
int PG;
int MG; // Grava no 2 PASSO de GRAVAR2 ao ABRIR
int G;

int R = 0;
int STOP = 0;
int FOTO = 0;
int F = 0;
int SWF = 0;

int ABR = 8; // 10 x 20 = 200
int STPWM = 0;
int PWM = 255;
int PWMMAX = 5;
int PWMMIN = 130;
int PWMC = 25;
int PWMSTOP = 255;

int RECEPTOR = 10;
int TENSAO = 950;

int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);



// VOID SETUP

void setup() {
  Serial.begin(115200);

  pinMode(SWGRAVAR, INPUT);
  pinMode(BOTAOEXECUTAR, INPUT);
  pinMode(SENSORHALL, INPUT);
  pinMode(SWPWM, INPUT);
  pinMode(SENSORTENSAO, INPUT);
  pinMode(RX1, INPUT);
  pinMode(RX2, INPUT);
  pinMode(SENSORSTOP, INPUT);
  pinMode(SENSORFOTO, INPUT);
  pinMode(SWFOTO, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(POWER24, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1, HIGH);
    delay(50);
    digitalWrite(LED1, LOW);
    delay(50);
  }

  digitalWrite(POWER24, LOW);
  
int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);

  PWM = PWMSTOP;
  G = 0;
  R = 0;
  STOP = 0;
  FOTO = 0;
  F = 0;
  SWF = 0;

  SERIALSHOW ();
  Serial.println("LIGADO!!!");
  Serial.println("");
}

// VOID

void DIPS(){
    if (digitalRead(SWFOTO) == HIGH) {
    SWF = 1;
  } else {
    SWF = 0;
  }

  if (digitalRead(SWPWM) == HIGH) {
    STPWM = 1;
  } else {
    STPWM = 0;
  }
}

// VOID

 void SERIALSHOW(){
int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);
    
  Serial.print("C=");
  Serial.println(C);
  Serial.print("I=");
  Serial.println(I);
  Serial.print("P=");
  Serial.println(P);
  Serial.print("M=");
  Serial.println(M);
  Serial.println("");
 }

// VOID

void FOTOCELULA(){
    if (analogRead(SENSORFOTO) >= TENSAO) {
    Serial.println("---SISTEMA Interrumpido por SENSOR FOTOCELULA - Passo seguinte automatico inverter");
    Serial.println("");
    FOTO = 1;
  } else {
    FOTO = 0;
  }
}

// VOID

void STOPEMERGENCIA(){
    if (analogRead(SENSORSTOP) >= TENSAO) {
    Serial.println("---SISTEMA Interrumpido por SENSOR STOP - PARADO a espera de ordem para inverter!");
    Serial.println("");
    STOP = 1;
  } else {
    STOP = 0;
  }
}

// VOID

void EPWM() {

  if (STPWM == 1) {
int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);
    
    // FECHAR PWM

    if (P == 1) {
      if (CG > ABR && CG < M) {
        if (PWM > PWMMAX) {
          PWM -= PWMC;
        } else {
          PWM = PWMMAX;
        }
      }

      if ( CG <= ABR) {
        if (PWM < PWMMIN) {
          PWM += PWMC;
        } else {
          PWM = PWMMIN;
        }
      }

      if ( CG <= 0) {
        PWM = PWMSTOP;
      }

      analogWrite(PWM1, PWM);
      
      Serial.print("PWM=");
      Serial.println(PWM);
      Serial.println("");
          
    }

  // ABRIR PWM

    if (P == 2) {
      if (CG >= 0 && CG < M - ABR) {
        if (PWM > PWMMAX) {
          PWM -= PWMC;
        } else {
          PWM = PWMMAX;
        }
      }

      if ( CG >= M - ABR) {
        if (PWM < PWMMIN) {
          PWM += PWMC;
        } else {
          PWM = PWMMIN;
        }
      }

      if ( CG >= M) {
        PWM = PWMSTOP;
      }

      analogWrite(PWM1, PWM);
      
      Serial.print("PWM=");
      Serial.println(PWM);
      Serial.println("");
          
    } 
  
  // GRAVAR PWM

  if (G == 1) {
    PWM = PWMMIN;
    analogWrite(PWM1, PWM);
    Serial.print("PWM=");
    Serial.println(PWM);
    Serial.println("");
  }

  if (G == 2) {
    if (CG >= 0) {
      if (PWM > PWMMAX) {
        PWM -= PWMC;
      } else {
        PWM = PWMMAX;
      }

          if (CG >= 255 - ABR) {
      if (PWM < PWMMIN) {
        PWM += PWMC;
      } else {
        PWM = PWMMIN;
      }
    }
    
        analogWrite(PWM1, PWM);
        
        Serial.print("PWM=");
        Serial.println(PWM);
        Serial.println("");
  }
  }
}

  if (STPWM == 0) {
    PWM = PWMMIN;
    analogWrite(PWM1, PWM);
    Serial.println("PWM SW desligado - Potencia 70 em 255 !!!");
  }
}

// VOID

void HALL() {
int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);

  if (digitalRead(SENSORHALL) == LOW) { // em LOW se estiver desligado conta igual

    if(P == 1) {
      CG --;
      Serial.println("HALL DETECTADO: CG-- / P == 1");
      Serial.print("CG=");
      Serial.println(CG);
      Serial.println("");
      if (CG < 0) {
        CG = 0;
      }
    }

    if (P == 2) {
      CG ++;
      Serial.println("HALL DETECTADO: CG++ / P == 2");
      Serial.print("CG=");
      Serial.println(CG);
      Serial.println("");
    }

    if (G == 1) {
      CG = 0;
      PG = 0;
      Serial.println("HALL DETECTADO: CG = 0 / G == 1");
      Serial.print("CG=");
      Serial.println(CG);
      Serial.println("");
    }

    if (G == 2) {
      CG ++;
      PG = 0;
      
      Serial.println("HALL DETECTADO: CG++ / G == 2");
      Serial.print("CG=");
      Serial.println(CG);
      Serial.println("");
    }
  }
}

// VOID

void COMANDO(){
     
   if (analogRead(RX1) < RECEPTOR) {
    Serial.println("---Comando Precionado!!");
    Serial.println("");
       R=1;
       Serial.println("R =");
       Serial.println(R);
       Serial.println("");
  }
}

// VOID

void VOLTAGEM(){
    if (analogRead(SENSORTENSAO) < TENSAO) {
    Serial.println("---SISTEMA Interrumpido por falta de corrente - A gravar CONTAGEM");
    Serial.println(5 / 1023 * analogRead(SENSORTENSAO) ); // 5V MAX
    Serial.println("");
    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, LOW);
    digitalWrite(POWER24, LOW);
    PWM=PWMSTOP;
    EEPROM.update(0, CG);
    loop();
  }
}

// VOID LOOP

void loop() {
int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);

COMANDO();
VOLTAGEM();
STOPEMERGENCIA();
FOTOCELULA();
DIPS();

// Automaticamente

while (F == 1 && STOP == 0){
  int P = EEPROM.read(2);
      if (P == 1) { // interrunpido inverte sentido ordem da FOTOCELULA
        Serial.println("CASE 1");
        Serial.println("Ordem da FOTOCELULA!!!");
        CG = C;
        PG = 2;
        EEPROM.update(2, PG); //P = 2
        ABRIR();
        break;
      }
  }

  while (F == 2 && STOP == 0){
  int P = EEPROM.read(2);
      if (P == 2) { // interrunpido inverte sentido ordem da FOTOCELULA
        Serial.println("CASE 1");
        Serial.println("Ordem da FOTOCELULA!!!");
        CG = C;
        PG = 1;
        EEPROM.update(2, PG); //P = 1
        FECHAR();
        break;
      }
  }

 // Manualmente

  if (digitalRead(SWGRAVAR) == HIGH && digitalRead(BOTAOEXECUTAR) == HIGH) {
    if (G == 0) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED1, HIGH);
        delay(TEMPOBOTAO);
        digitalWrite(LED1, LOW);
        delay(TEMPOBOTAO);
      }
      EEPROM.write(0, 0);
      EEPROM.write(1, 0);
      EEPROM.write(2, 0);
      EEPROM.write(3, 0);

      G = 1;
      PWM = PWMMIN;
      GRAVAR1();
    }

    if (G == 2) {
      for (int i = 0; i < 6; i++) {
        digitalWrite(LED1, HIGH);
        delay(TEMPOBOTAO);
        digitalWrite(LED1, LOW);
        delay(TEMPOBOTAO);
      }
      PWM = PWMMIN;
      GRAVAR2();
    } else {
      for (int i = 0; i < 8; i++) {
        digitalWrite(LED1, HIGH);
        delay(50);
        digitalWrite(LED1, LOW);
        delay(50);
      }
    }
  }

  if (digitalRead(BOTAOEXECUTAR) == HIGH && digitalRead(SWGRAVAR) == LOW && FOTO == 0 && STOP == 0 || R == 1 && FOTO == 0 && STOP == 0) {

    SERIALSHOW ();
    Serial.println("---BOTAO EXECUTAR");
    Serial.println("");

    if(R == 1){
      R = 0;
      delay(1000);
    }

    for (int i = 0; i < 2; i++) {
      digitalWrite(LED1, HIGH);
      delay(TEMPOBOTAO);
      digitalWrite(LED1, LOW);
      delay(TEMPOBOTAO);
    }
    
    if (M < 255) {
      if (P == 0) {
        Serial.println("CASE 0");
        if (C == M) {
          CG = C;
          PG = 1;
          EEPROM.update(2, PG); //P = 1
          FECHAR();
        } else {
          CG = C;
          PG = 2;
          EEPROM.update(2, PG); //P = 2
          ABRIR();
        }
      }

      if (P == 1) { // interrunpido inverte sentido
        Serial.println("CASE 1");
        CG = C;
        PG = 2;
        EEPROM.update(2, PG); //P = 2
        ABRIR();
      }

      if (P == 2) { // interrunpido inverte sentido
        Serial.println("CASE 2");
        CG = C;
        PG = 1;
        EEPROM.update(2, PG); //P = 1
        FECHAR();
      }
    } else {
      Serial.println("!!!Falta gravacao!!!");
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED1, HIGH);
        delay(50);
        digitalWrite(LED1, LOW);
        delay(50);
      }
    }
  }
}

// VOID

void FECHAR() {
    Serial.println("VOID FECHAR");
    Serial.println("");
    PWM = PWMMIN;
    int P = EEPROM.read(2);
    
  while (P == 1) {
int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);

    HALL();
    EPWM();
    COMANDO();
    VOLTAGEM();
    STOPEMERGENCIA();
    FOTOCELULA();
    DIPS();

    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, HIGH);
    digitalWrite(POWER24, HIGH);

    SERIALSHOW();

    if (CG == 0) {
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(POWER24, LOW);

      PWM = PWMSTOP;
      PG = 0;
      IG = 0;
      F = 0;

      EEPROM.update(0, CG);
      EEPROM.update(1, IG);
      EEPROM.update(2, PG);

      Serial.println("Paragem Automatica");
      Serial.println("--- Fechado!!!");
      Serial.println("");
      break;
    }


    if (digitalRead(BOTAOEXECUTAR) == HIGH  || R == 1 || FOTO == 1 && F == 0 || STOP == 1) {
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(POWER24, LOW);

      PWM = PWMSTOP;
      STOP = 0;

    if(R == 1){
      R = 0;
      delay(1000);
    }
      
      if (FOTO == 1){
        if (SWF == 0){
        F = 1;
        } else {
        F = 1;
        }
      } else {
        F = 0;
      }

      EEPROM.update(0, CG);
      EEPROM.update(1, IG);
      EEPROM.update(2, PG);

      digitalWrite(LED1, HIGH);
      delay(TEMPOBOTAO);
      digitalWrite(LED1, LOW);
      delay(TEMPOBOTAO);

      Serial.println("Botao Pressionado!!!");
      Serial.println("--- FECHO Interrunpido!!!");
      Serial.println("");
      break;
    }
    delay(TEMPOMOTOR);
  }
}

// VOID

void ABRIR() {
    Serial.println("VOID ABRIR");
    Serial.println("");
    PWM = PWMMIN;
    int P = EEPROM.read(2);

  while (P == 2) {
int C = EEPROM.read(0);
int I = EEPROM.read(1);
int P = EEPROM.read(2);
int M = EEPROM.read(3);

    HALL();
    EPWM();
    COMANDO();
    VOLTAGEM();
    STOPEMERGENCIA();
    FOTOCELULA();
    DIPS();

    digitalWrite(MOTOR1, HIGH);
    digitalWrite(MOTOR2, LOW);
    digitalWrite(POWER24, HIGH);

    SERIALSHOW();

    if (CG == M) {
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(POWER24, LOW);

      PWM = PWMSTOP;
      PG = 0;
      IG = 0;
      F = 0;

      EEPROM.update(0, CG);
      EEPROM.update(1, IG);
      EEPROM.update(2, PG);

      Serial.println("Paragem Automatica");
      Serial.println("--- ABERTO!!!");
      Serial.println("");
      break;
    }


    if (digitalRead(BOTAOEXECUTAR) == HIGH || R == 1 || FOTO == 1 && F == 0 && SWF == 1 || STOP == 1) {
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(POWER24, LOW);

      PWM = PWMSTOP;
      STOP = 0;
      if(R == 1){
        R = 0;
        delay(1000);
      }

      if (FOTO == 1){
        if (SWF == 0){
        F = 1;
        } else {
        F = 2;
        }
      } else {
        F = 0;
      }

        EEPROM.update(0, CG);
        EEPROM.update(1, IG);
        EEPROM.update(2, PG);

      digitalWrite(LED1, HIGH);
      delay(TEMPOBOTAO);
      digitalWrite(LED1, LOW);
      delay(TEMPOBOTAO);

      Serial.println("Botao Pressionado!!!");
      Serial.println("--- ABERTURA Interrunpida!!!");
      Serial.println("");
      break;
    }
    delay(TEMPOMOTOR);
  }
}

// VOID 1/2 GRAVAR FECHA

void GRAVAR1() {
  // vai fechar o 1 PASSO de gravar -  picar o FECHADO para depois gravar o tempo de abertura/fecho
PWM = PWMMIN;

  while (G == 1) {
    Serial.println("VOID GRAVAR1");
    Serial.println("");

    HALL();
    EPWM();
    COMANDO();
    STOPEMERGENCIA();
    FOTOCELULA();
    DIPS();

    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, HIGH);
    digitalWrite(POWER24, HIGH);

    SERIALSHOW();

    if (digitalRead(BOTAOEXECUTAR) == HIGH || R == 1 || FOTO == 1 && F == 0 || STOP == 1) {
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(POWER24, LOW);

      PWM = PWMSTOP;
      G = 2;
      F = 0;
      STOP = 0;
      
      if(R == 1){
        R = 0;
        delay(1000);
      }

      digitalWrite(LED1, HIGH);
      delay(TEMPOBOTAO);
      digitalWrite(LED1, LOW);
      delay(TEMPOBOTAO);

      Serial.println("Botao Pressionado!!!");
      Serial.println("Fecho localizado!!! - Espere, vai inverter auto!");
      Serial.println("");
      break;
    }
    delay(TEMPOMOTOR);
  }
}

// VOID 2/2 GRAVAR ABRE

void GRAVAR2() {
  // vai Abrir e gravar no 2 PASSO
PWM = PWMMIN;

  while (G == 2) {
    Serial.println("VOID GRAVAR2");
    Serial.println("");
    
    HALL();
    EPWM();
    COMANDO();
    STOPEMERGENCIA();
    FOTOCELULA();
    DIPS();

    digitalWrite(MOTOR1, HIGH);
    digitalWrite(MOTOR2, LOW);
    digitalWrite(POWER24, HIGH);

    SERIALSHOW();

    if (digitalRead(BOTAOEXECUTAR) == HIGH || R == 1 || FOTO == 1 && F == 0 && SWF == 1|| STOP == 1) {
      digitalWrite(MOTOR1, LOW);
      digitalWrite(MOTOR2, LOW);
      digitalWrite(POWER24, LOW);

      PWM = PWMSTOP;
      G = 3;
      MG = CG;
      IG = 0;
      STOP = 0;

      if(R == 1){
        R = 0;
        delay(1000);
      }
      
      if (FOTO == 1){
        if (SWF == 0){
        F = 1;
        } else {
        F = 2;
        }
      } else {
        F = 0;
      }

        EEPROM.update(0, CG);
        EEPROM.update(1, IG);
        EEPROM.update(2, PG);
        EEPROM.update(3, MG);

      digitalWrite(LED1, HIGH);
      delay(TEMPOBOTAO);
      digitalWrite(LED1, LOW);
      delay(TEMPOBOTAO);

      Serial.println("Botao Pressionado!!!");
      Serial.println("Gravacao Concluida!!!");
      Serial.println("");
      break;
    }
    delay(TEMPOMOTOR);
  }
}
