#include "lib/include.h" //librerias

#define SENSOR  (*((volatile uint32_t *)0x4002400C)) 
#define LIGHT   (*((volatile uint32_t *)0x400050FC)) 
// Linked data structure

struct State 
{
  uint32_t Out;
  uint32_t Time; 
  uint32_t Next[4];
};

typedef const struct State State_t; 
#define goN   0 
#define waitN 1
#define goE   2
#define waitE 3

State_t FSM[4]= 
{
 {0x21,300,{goN,waitN,goN,waitN}},
 {0x22, 50,{goE,goE,goE,goE}},
 {0x0C,300,{goE,goE,waitE,waitE}},
 {0x14, 50,{goN,goN,goN,goN}}};

uint32_t S;  // index to the current state
uint32_t Input;

int main(void)
{
  Configurar_PLL(_80MHZ); // 
  SysTick_Init();   // Program 10.2
  Configurar_GPIO(); 
  
  S = goN; //0
  
  while(1)
  {
    LIGHT = FSM[S].Out;  
    SysTick_Wait10ms(FSM[S].Time);
    Input = SENSOR;    
    S = FSM[S].Next[Input]; 
  }
}
