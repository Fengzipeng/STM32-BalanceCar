#ifndef _control_H
#define _control_H
#include "stm32f10x.h"
#include "public.h"
#define IO_10 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define IO_11 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
u8 StartWork_Test(float angle,int EL,int ER,float g);
void Emergency_Check(float angle);
int EXTI15_10_IRQHandler(void);
void Get_Current_Angle(void);
int turn(int encoder_left,int encoder_right,float gyro);
int velocity(int encoder_left,int encoder_right);
int Balance(float angle,float Gyro);
void Set_Pwm(int moto1,int moto2);
int myabs(int a);
void Limit_PWM(void);
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right);
int Put_Down(float Angle,int encoder_left,int encoder_right);
u8 Turn_Off(float angle,float voltage);
extern  u8 Flag_Forward,Flag_Backward,Flag_Left,Flag_Right,Flag_Avoidance,Flag_Stop,Work_Flag;
extern	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
extern float Angle_Balance,Gyro_Balance,Gyro_Turn,Acceleration_Z;
extern u8 flag,delay_countu,emergency_flag;
extern float Balance_kd,Velocity_ki;
extern int Balance_kp,Velocity_kp;
extern int Speed_Intput,Speed_Divide;
extern int Motor1_PWM,Motor2_PWM;
extern int Encoder_Left,Encoder_Right;
#endif
