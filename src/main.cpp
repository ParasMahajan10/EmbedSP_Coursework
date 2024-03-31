#include "mbed.h"
#include "ESPRobot.h"

/*
DIRECTION CONTROL PIN CONFIG
FORWARD : IN_1 = IN_3 = 0; IN_2 = IN_4 = 1;
REVERSE : IN_1 = IN_3 = 1; IN_2 = IN_4 = 0;
*/
DigitalOut LED(D13);
///////snes//////////////
AnalogIn IrSensor(PA_0);  //a0  right
AnalogIn IrSensor2(PC_1);  //a4  Mid
AnalogIn IrSensor3(PA_4); //a2  left
///////////////////int/////
InterruptIn blueButton(PA_10);

void callbackFunction();


/* VARIABLES */
int Delay = 2000000; // 3 sec

float thr = 30.0+1000;
float DC = 0.90;
float DC1 = 0.20;

float t_period = 0.1;
float dc_rw, dc_lw;
/* --------- */
char recv ;
volatile bool mode = false ;

volatile bool updateMovement1 = false;
volatile bool updateMovement2 = false;
volatile bool updateMovement3 = false;
volatile bool updateMovement4 = false;
volatile bool updateMovement5 = false;
volatile bool updateMovement6 = false;
volatile bool updateMovement7 = false;
volatile bool updateMovement8 = false;
//////////////////////////////////
volatile bool flag1 = false;
volatile bool flag2 = false;
volatile bool flag3 = false;
volatile bool flag4 = false;
/* FUNCTIONS AND ISR */
void Forward(float dc_rw, float dc_lw);
void Take_turn(float dc_rw, float dc_lw);
void On_spot_turn_R(float dc_rw, float dc_lw);
void On_spot_turn_L(float dc_rw, float dc_lw);
void Curve_move_L(float dc_rw, float dc_lw);
void Curve_move_R(float dc_rw, float dc_lw);
/* --------- */
void updateMovementISR();
void Do_MovementISR();

//////
Serial bluetooth(PA_9, PA_10); // TX, RX (assuming UART pins)
Serial pc(USBTX, USBRX); // TX, RX
/* TIMER FOR INTERRUPT */
Ticker timer;

int main(){
//Take_turn(0.00, 0.00);

ESPRobot robot(PB_10, PB_4, PB_3, PB_5, PA_8, PC_7);

float IrVoltage = 0.0; //// sens init!
 float IrVoltage2 = 0.0;
 float IrVoltage3 = 0.0;
 

timer.attach(&Do_MovementISR, 0.0001);
//bluetooth.attach(&callbackFunction);   // serial interupt
//blueButton.read(&callbackFunction);

 //if (bluetooth.readable())

while(1){
 //recv = bluetooth.getc();   // bluetooth reading

IrVoltage = IrSensor.read();
    IrVoltage = IrVoltage * 100 + 1000;
    IrVoltage2 = IrSensor2.read();
    IrVoltage2 = IrVoltage2 * 100 + 1000;
    IrVoltage3 = IrSensor3.read();
    IrVoltage3 = IrVoltage3 * 100 + 1000;
 
     if (IrVoltage < thr && IrVoltage2 < thr && IrVoltage3 > thr ) {
  updateMovement1 = true;
 
  }
     else if (IrVoltage < thr && IrVoltage2 > thr && IrVoltage3 < thr ) {
   updateMovement2 = true;
  
 
  }
     else if (IrVoltage < thr && IrVoltage2 > thr && IrVoltage3 > thr ) {
    updateMovement3 = true;

  }
    else if (IrVoltage > thr && IrVoltage2 < thr && IrVoltage3 < thr ) {
    updateMovement4 = true;

  }
    else if (IrVoltage > thr && IrVoltage2 < thr && IrVoltage3 > thr ) {
    updateMovement5 = true;

  }
   else if (IrVoltage > thr && IrVoltage2 > thr && IrVoltage3 > thr ) {
   updateMovement7 = true;

  }
    else if (IrVoltage > thr && IrVoltage2 > thr && IrVoltage3 < thr ) {
   updateMovement6 = true;

  }
   
   else if (IrVoltage < thr && IrVoltage2 < thr && IrVoltage3 < thr ) {
   updateMovement8 = true;

  
  
   }
   else{
       updateMovement8 = true;

   } 

if (bluetooth.readable()) {
    LED = !LED;
    mode = true ;
}

///////////////
while(mode){
recv = bluetooth.getc();   // bluetooth reading

bluetooth.printf("Received character from Bluetooth: %c\n", recv);
 switch (recv){
    
case 'a' :
 //Take_turn(0.78, 0.85);
    updateMovement8 = true;
break;
case 's' :
Curve_move_L(DC1, DC1);
//updateMovement4 = true;
break;
case 'd' :
Curve_move_R(DC1, DC1);
//updateMovement2 = true;
break;
case 'f' :
  // Take_turn(0.00, 0.00);
   updateMovement7 = true;
break;
case 'w' :
mode = false ;
 }


}
 
}
/////////


}



/* TAKE TURNS */
/*void Take_turn(float dc_rw, float dc_lw ){

    IN_1 = 0;
    IN_3 = 0;

    IN_2 = 1;
    IN_4 = 1;

    Rwheel.period(t_period);     
    Rwheel.write(dc_rw);      
Lwheel.period(t_period);      
    Lwheel.write(dc_lw);
    

}


/* TURN ON THE SPOT */
/*void On_spot_turn_R(float dc_rw, float dc_lw){

    IN_1 = 1;
    IN_3 = 0;

    IN_2 = 0;
    IN_4 = 1;  

    Rwheel.period(t_period);     
    Rwheel.write(dc_rw);      

    Lwheel.period(t_period);      
    Lwheel.write(dc_lw);
}

void On_spot_turn_L(float dc_rw, float dc_lw){

    IN_1 = 0;
    IN_3 = 1;

    IN_2 = 1;
    IN_4 = 0;  

    Rwheel.period(t_period);     
    Rwheel.write(dc_rw);      

    Lwheel.period(t_period);      
    Lwheel.write(dc_lw);
}

void Curve_move_R(float dc_rw, float dc_lw){

    IN_1 = 1;
    IN_3 = 0;

    IN_2 = 0;
    IN_4 = 1;  

    Rwheel.period(t_period);     
    Rwheel.write(dc_rw + 0.40f);      

    Lwheel.period(t_period);      
    Lwheel.write(dc_lw - 0.40f);
}

void Curve_move_L(float dc_rw, float dc_lw){

    IN_1 = 0;
    IN_3 = 1;

    IN_2 = 1;
    IN_4 = 0;  

    Rwheel.period(t_period);     
    Rwheel.write(dc_rw - 0.40f);      

    Lwheel.period(t_period);      
    Lwheel.write(dc_lw + 0.40f);
}

////////////////////////////////////////*/

void Do_MovementISR(){
    
if (updateMovement1) {
 //Take_turn(0.50, 0.00);
 robot.takeTurn(0.90, 0.50); // Right
 
 updateMovement1 = false; // Reset the flag
  }


     if (updateMovement2) {
  //Take_turn(0.00, 0.50);
   robot.takeTurn(0.90, 0.50); // Right
  updateMovement2 = false; // Reset the flag
 
  }
     if (updateMovement3) {
      
   //Take_turn(0.50, 0.00);
    robot.takeTurn(0.90, 0.50); // Right
updateMovement3 = false; // Reset the flag
  }
    if (updateMovement4) {
   //Take_turn(0.00, 0.50);
     robot.takeTurn(0.50, 0.90); // Left
updateMovement4 = false; // Reset the flag
  }
    if (updateMovement5) {
   robot.forward(0.90, 0.90); //forward
   
updateMovement5 = false; // Reset the flag
  }
    if (updateMovement6) {
  //Take_turn(0.00, 0.50);
    robot.takeTurn(0.50, 0.90); // Left
updateMovement6 = false; // Reset the flag
  }
   if (updateMovement7) {
  robot.forward(0.90, 0.90); //forward

  
updateMovement7 = false; // Reset the flag

  }
   
    if(updateMovement8){

        Take_turn(0.70, 0.65);
        

updateMovement8 = false; // Reset the flag

    }

}

/*
void callbackFunction() {
     LED = !LED;
     recv = bluetooth.getc();   // bluetooth reading
    bluetooth.printf("Received character from Bluetooth: %c\n", recv);

 
 switch (recv){
    
case 'a' :
 //Take_turn(0.78, 0.85);
   flag1= true;
break;
case 's' :
//Curve_move_L(DC1, DC1);
flag2= true;
break;
case 'd' :
//Curve_move_R(DC1, DC1);
flag3= true;
break;
case 'f' :
  // Take_turn(0.00, 0.00);
   flag4= true;
break;
case 'w' :
mode = true;
 }
   
    //wait(1);
    //Take_turn(0.00, 0.00);
  
}
*/