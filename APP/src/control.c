#include "control.h"
#include "math.h"
#define mpu6050_Normal
float Angle_Balance,Gyro_Balance,Gyro_Turn,Acceleration_Z;
u8  Work_Flag=0,Flag_Stop=1,Flag_Avoidance=1;//����ģʽĬ�ϴ�
int Encoder_Left,Encoder_Right;
int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
int Motor1_PWM,Motor2_PWM;
float Voltage;
u8 Flag_Forward,Flag_Backward,Flag_Left,Flag_Right;
int Balance_kp=340,Velocity_kp=80;  //������ֵΪģ�ͳ��ϵ���ʱ����ֵ
float Balance_kd=16,Velocity_ki=0.4; //�궨�巽�����ʱ�޸�
int Speed_Intput,Speed_Divide=1;   //���Կ����źŵ��ٶ�ֵ �ٶȷָ�������õ�λ
float Speed_kp=1; //�ٶ�����������ϵ��
u8 break_flag=1,LED_Delay_Count,flag,delay_count,RX_Status,NRF_LED_Count;
int EXTI9_5_IRQHandler() 
{
	if( EXTI_GetITStatus(EXTI_Line5)!=RESET) //�ж��жϷ���
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		LED_Delay_Count++;
		flag++;
		if(LED_Delay_Count==20)
		{
			LED_Delay_Count=0;
			if( LED1_Status == 1) LED1_ON;
			else LED1_OFF;
		}
		if(NRF_LED_Count>=10)
		{
			NRF_LED_Count=0;
			if( LED2_Status == 1) LED2_ON;
			else LED2_OFF;
		}
		Get_Current_Angle();
		if(IO_10==1&&IO_11==0)
		{
			Flag_Backward=1;
		}
		else Flag_Backward=0;
		if(break_flag==1)
	  {
			//Work_Flag=1;
			//Flag_Stop=0;
			if(Work_Flag!=1) StartWork_Test(Angle_Balance,Encoder_Left,Encoder_Right,Gyro_Balance);
			Emergency_Check(Angle_Balance);
	    break_flag=0;
			RX_Status=NRF24L01_ReceiveData(RX_Buff);
			if(RX_Status==RX_OK)
			{			
				NRF_LED_Count+=2;
				RX_Status=0;
			}
   	  return 0; 
	  }
		else break_flag=1;
		Encoder_Left=Read_Encoder(2); //�ȶ�ȡ��ȡ��������ֵ
		Encoder_Right=Read_Encoder(4); 
		//Ratio_Sure();
	  Balance_Pwm = Balance(Angle_Balance,Gyro_Balance); 
	  Velocity_Pwm = velocity(Encoder_Left,Encoder_Right);
		Turn_Pwm   = turn(Encoder_Left,Encoder_Right,Gyro_Turn);
	  Motor1_PWM=Balance_Pwm+Velocity_Pwm-Turn_Pwm;
	  Motor2_PWM=Balance_Pwm+Velocity_Pwm+Turn_Pwm;
		//Read_Distance();   //��ȡ����
		Limit_PWM();
		if(Work_Flag==1&&Flag_Stop==0) Set_Pwm(Motor1_PWM,Motor2_PWM);
		   else Set_Pwm(0,0);
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
   return 0;
}
void Get_Current_Angle() //��ȡ��ǰ�Ƕ�ֵ
{
	#ifdef mpu6050_dmp
	Read_DMP(&Pitch,&Roll,&Yaw);
	Angle_Balance=Roll-0;
	Gyro_Balance=gyro[1];	
	Gyro_Turn=gyro[2];
	Acceleration_Z=accel[2];
	#endif
	#ifdef mpu6050_Normal
	float GX,GY,GZ,AY,AZ,AX;
	float Accel_Angle;
	GX=MPU6050_GetData(GYRO_XOUT_H);  //+-2000����
	GY=MPU6050_GetData(GYRO_YOUT_H);
	GZ=MPU6050_GetData(GYRO_ZOUT_H);
	AX=MPU6050_GetData(ACCEL_XOUT_H);
	AY=MPU6050_GetData(ACCEL_YOUT_H);
	AZ=MPU6050_GetData(ACCEL_ZOUT_H);
	if(GX>32768)  GX-=65536;
	if(GY>32768)  GY-=65536;
	if(AX>32768)  AX-=65536;
	if(AY>32768)  AY-=65536;
	if(AZ>32768)  AZ-=65536;
	Gyro_Balance=GX/32.8;	
	Gyro_Turn=GZ;
	Acceleration_Z=AZ;
	Accel_Angle=atan2(AY,AZ)*180/3.14;
	Angle_Balance=Kalman_Filter(Accel_Angle,Gyro_Balance);//�������˲�
	#endif
}
int Balance(float angle,float Gyro)
{  
   float Bias;
	 int pwm;
	 Bias=angle-0;       //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 pwm=Balance_kp*Bias+Gyro*Balance_kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return pwm;
}
int velocity(int encoder_left,int encoder_right)
{
	static int Encoder_Least,Encoder;
	static int Encoder_Integral;
	 u8 Speed_Intput=20;
	int Movement;
	int Velocity_pwm;
	//if(Flag_Avoidance==1) Speed_Divide=2;
	if( (RX_Buff[1]&0x80)!=0 ) 
	{
		if( (RX_Buff[1]&0x20) !=0)
		{
			Flag_Forward=1;
			Flag_Backward=0;
		}
		if( (RX_Buff[1]&0x10) !=0)
		{
			Flag_Forward=0;
			Flag_Backward=1;
		}
		//Speed_Intput=RX_Buff[1];
	}
	else 
	 {
		  Flag_Forward=0;
		  Flag_Backward=0;
		  Speed_Intput=0;
	 }
	if   ( (Flag_Forward==1)&&(Flag_Backward==0))   Movement=-Speed_Intput;	        
	if ((Flag_Backward==1)&&(Flag_Forward==0))	Movement=Speed_Intput;  
	if ( (Flag_Backward==0)&&(Flag_Forward==0) ) Movement=0;
	Encoder_Least =(encoder_left+encoder_right); //�����ٶ�ƫ��
  Encoder=Encoder*0.7;	
	Encoder += Encoder_Least*0.3;	     //===һ�׵�ͨ�˲���  
	Encoder_Integral+=Encoder;                    //===���ֳ�λ�� ����ʱ�䣺10ms   
  Encoder_Integral=Encoder_Integral-Movement;  	
	if(Encoder_Integral>6000)  	Encoder_Integral=6000;    //===�����޷�
	if(Encoder_Integral<-6000)	Encoder_Integral=-6000;     //===�����޷�	
	Velocity_pwm = Encoder*Velocity_kp+Velocity_ki*Encoder_Integral;                 //===�ٶȿ���
	if( Flag_Stop==1)Encoder_Integral=0;                                               
	return Velocity_pwm;
}
int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
	  static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
	  float Turn_Amplitude=30,Kp=18,Kd=0; 
    if( (RX_Buff[1]&0x08)!=0 )	
	  {
		 if( (RX_Buff[1]&0x02) !=0)
		 {
			Flag_Left=1;
			Flag_Right=0;
		 }
		 else 
		 {
			Flag_Left=0;
			Flag_Right=1;
		  }
		 //Turn_Amplitude=RX_Buff[2];
	  }
		else 
		{
			Flag_Left=0;
			Flag_Right=0;
			Turn_Amplitude=0;
		}
	  //=============ң��������ת����=======================//
  	if(Flag_Left==1||Flag_Right==1)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
		{
			if(++Turn_Count==1) Encoder_temp=myabs(encoder_left+encoder_right);
			Turn_Convert=50/Encoder_temp;
			if(Turn_Convert<0.6)Turn_Convert=0.6;
			if(Turn_Convert>3)Turn_Convert=3;
		}	
	  else
		{
			Turn_Convert=0.9;
			Turn_Count=0;
			Encoder_temp=0;
		}
		if(Flag_Left==1&&Flag_Right==0)	Turn_Target-=Turn_Convert;
		if(Flag_Right==1&&Flag_Left==0)	Turn_Target+=Turn_Convert; 
		if(Flag_Right==0&&Flag_Left==0) Turn_Target=0;
    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת���ٶ��޷�
	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
		if(Flag_Forward==1||Flag_Backward==1)  Kd=-0.01f;        
		else Kd=0;  //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
  	//=============ת��PD������=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}

int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
void Set_Pwm(int moto1,int moto2)
{
    	if(moto1>0)	{Direction_Control(1,0);PWMA=moto1;LED3_ON;}
			else 	      {Direction_Control(1,1);PWMA=myabs(moto1);LED3_OFF;}
			
		  if(moto2>0)	{Direction_Control(2,0);PWMB=moto2;LED4_ON;}
			else        {Direction_Control(2,1);PWMB=myabs(moto2);LED4_OFF;}	
}
void Limit_PWM()
{	
	  int Amplitude=7000;    //===PWM������7200 ������7000
    if(Motor1_PWM<-Amplitude) Motor1_PWM=-Amplitude;	
		if(Motor1_PWM>Amplitude)  Motor1_PWM=Amplitude;	
	  if(Motor2_PWM<-Amplitude) Motor2_PWM=-Amplitude;	
		if(Motor2_PWM>Amplitude)  Motor2_PWM=Amplitude;		
	
}
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count0,count1,count2;
	if(flag==0)                                     //��һ��
	 {
	      if(myabs(encoder_left)+myabs(encoder_right)<80)    //����1��С���ӽ���ֹ
				count0++;
        else 
        count0=0;		
        if(count0>5)				
		    flag=1,count0=0; 
	 } 
	 if(flag==1)                                     //����ڶ���
	 {
		    if(++count1>100)       count1=0,flag=0;              //��ʱ���ٵȴ�2000ms
	      if(Acceleration>2600&&(Angle>(-20))&&(Angle<(20)))   //����2��С������0�ȸ���������
		    flag=2; 
	 } 
	 if(flag==2)                                                                  //������
	 {
		  if(++count2>100)       count2=0,flag=0;                                   //��ʱ���ٵȴ�1000ms
	    if(myabs(encoder_left+encoder_right)>135)                                 //����3��С������̥��Ϊ�������ﵽ����ת��   
      {
				flag=0;                                                                                     
				return 1;                                                               //��⵽С��������
			}
	 }
	return 0;
}
int Put_Down(float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count;	 
	 if(Flag_Stop==0)                           //��ֹ���      
   return 0;	                 
	 if(flag==0)                                               
	 {
	      if(Angle>(-10+1)&&Angle<(10+1)&&encoder_left==0&&encoder_right==0)         //����1��С������0�ȸ�����
		    flag=1; 
	 } 
	 if(flag==1)                                               
	 {
		  if(++count>30)                                          //��ʱ���ٵȴ� 500ms
		  {
				count=0;flag=0;
		  }
	    if(encoder_left<-3&&encoder_right<-3&&encoder_left>-60&&encoder_right>-60)                //����2��С������̥��δ�ϵ��ʱ����Ϊת��  
      {
				flag=0;
				return 1;                                             //��ⱻ����
			}
	 }
	return 0;
}
u8 Turn_Off(float angle,float voltage)
{
	    u8 status;
			if( fabs(angle)>30||voltage<11.1)//��ص�ѹ����11.1V�رյ��
			{	                                                 //===��Ǵ���40�ȹرյ��
			 status=1;
       //Direction_Control(1 ,0); //�رյ��
			// Direction_Control(2 ,0);
      }
			else
      status=0;
      return status;//�����Ƿ�رյľ���
}
u8 StartWork_Test(float angle,int EL,int ER,float g)
{
	static u8 times;
	if( (fabs(angle)<10) && ( (EL+ER)<10) &&(g!=0) )times++;
	else times=0;    //���¼��
	if(times>=50) { Work_Flag=1,Flag_Stop=0,Beep_Open=0;return 1;}
	else 
		return 0;
}
void Emergency_Check(float angle)
{
	if(fabs(angle)>=30) 
	{ 
		Flag_Stop=1,Work_Flag=0;
		Beep_Open=1;
	}
}
