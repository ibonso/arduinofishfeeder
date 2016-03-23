#include <MsTimer2.h>
#include <Servo.h>

Servo myservo;  
volatile int tapaTimer=0;
volatile int tapaBoton=0;




int PIN_SERVO=3;
int PIN_BOTON_INT=2;

int cnt=0;


volatile long mediosegundos=0;
volatile long minutos=0;


void setup() 
{
  //Serial.begin(9600);
  MsTimer2::set(500, intTimerTapa); // 500ms period
  MsTimer2::start();
    
  //pinMode(PIN_BOTON_INT, INPUT_PULLUP);
  attachInterrupt(0, buttonPushed, FALLING);
  //sei();//allow interrupts
  myservo.attach(PIN_SERVO);  // attaches the servo on pin 
  myservo.write(0);   
}


void loop() {

  noInterrupts(); 
  int iTapaTimer=tapaTimer;  
  int iTapaBoton=tapaBoton;
  interrupts();
  
  if( iTapaTimer==1 ) {
    activarServoTapa();

    noInterrupts(); 
    tapaTimer=0;
    minutos=0; //Set to 0 to estart counting for another 24H
    cnt++;  
    interrupts(); 
  }
  if( iTapaBoton==1 ) {
    activarServoTapa();
    noInterrupts(); 
    tapaBoton=0;
    interrupts(); 
  }

  //delay(10000);
  if(cnt==15) //After 15 days I stop feedeing... 
    exit(0); 
  
}



static void activarServoTapa()
{
  int val=50;
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(150);                           // waits for the servo to get there
  val=0;
  myservo.write(val);   
  delay(1000);                           
 
  
}


/** Interrupt handler button pin 2 (INT 0)**/
void buttonPushed()
{
  tapaBoton=1;
}

/**
 *  is exec every 500 ms
 */
void intTimerTapa()
{
  mediosegundos++;
  if( (mediosegundos % 120) == 0 ) {  // 1 min (60 sg)
      minutos++;
      mediosegundos=0;
      /*** 
      For testing purpose...
      ---------
      tapaTimer=1; 
      Serial.println("Minutos (repeticiones): "); 
      char buff[20]; // "-2147483648\0"
      itoa(minutos,buff,10);
      Serial.println(buff);
      ***/
  }

  if( minutos >= 24*60) {
    tapaTimer=1;
  }

}






