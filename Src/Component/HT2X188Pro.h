/*  
***************************************************************
*****************Tools Version 20161020************************
* @copy  COPYRIGHT 2016 Foryon  
*
* @file :
* @version :
* @function :
* 
* 
* 
* 
* 
* 
* @brief :
* 
* 
* 
*
* @author :
* @date :2018/8/12 8:30
***************************************************************
*/ 
#ifndef _HT2X188PRO_H_
#define _HT2X188PRO_H_
 

#include "stm32f4xx_hal.h"
#include "Typedef.h "
#include "UART_RBC.h"
#include "COM_Server.h"

#ifdef  Meter_HT2X_ENABLE
 
#ifdef _HT2X188PRO_C_
#define HT2X188PRO_EXT
#else
#define HT2X188PRO_EXT extern
#endif
 


#define HT2X188Pro_PackSize			UART_TO_HT2X188PackSize_S
#define HT2X188Pro_HeadSize			11				    //数据包头大小
#define HT2X188Pro_StartCode		0X68				//起始符
#define HT2X188Pro_ProtocolCode		0X20				//版本号
#define HT2X188Pro_EndCode			0X16				//结束符


 
#pragma pack(1)		//字节对齐
typedef struct
{

	INT8U	Start;		//起始字符		1字节
	INT8U	Type;		//协议类型		1字节
	INT32U	SN;			//产品编号		4字节
	INT8U	FAdd1;		//保留字节		1字节
	INT16U	FAdd2;		//保留字节		1字节
	INT8U	ConType;	//控制类型		1字节
	INT8U	Lenth;		//数据长度		2字节
}HT2X188Pro_Head_Stru;
#pragma pack()			//字节对齐



//HT2X-188 热量表通信协议包
#pragma pack(1)			//字节对齐

typedef struct
{
	INT8U	Lead1;		//引导字符1 1字节
	INT8U	Lead2;		//引导字符2 1字节
	INT8U	Lead3;		//引导字符3 1字节
	INT8U	Lead4;		//引导字符4 1字节
	
	HT2X188Pro_Head_Stru Head;
	INT16U	Flag;					//数据标志
	INT8U	SER;					//数据2
	INT8U	Check;				//校验和
	INT8U	End;					//结束标志
}HT2X188_Pack01S_Stru;					//召测实时数据


typedef struct
{
	HT2X188Pro_Head_Stru Head;
	INT16U	Flag;					//数据标识
	INT8U	SER;					//序列号
	
	INT32U	CODE_Energy; 			//当前冷量4字节		//2位小数
	INT8U	CODE_Energy_Unit;		//当前冷量单位1字节
	
	INT32U	Total_Energy;			//当前热量4字节		//2位小数
	INT8U	Total_Energy_Unit;		//当前热量单位1字节

	INT32U	Instant_Energy; 		//热功率4字节			//2位小数
	INT8U	Instant_Energy_Unit;	//热功率单位1字节

	INT32U	Instant_Current;        //流速4字节			//4位小数
	INT8U	Instant_Current_Unit;   //流速单位1字节
	
	INT32U	Total_Current;			//累计流量4字节		//2位小数
	INT8U	Total_Current_Unit;		//累计流量单位1字节
	
	INT16U	Input_TempL;			//入口温度 3字节		//2位小数
	INT8U	Input_TempH;

	INT16U	Output_TempL;			//出口温度 3字节		//2位小数
	INT8U	Output_TempH;
	
	INT16U	Work_Time_L;			//工作时间 3字节		//没有小数位
	INT8U	Work_Time_H;

	INT8U	Second; 				//实时时间
	INT8U	Minute; 				//实时时间
	INT8U	Hour;					//实时时间
	INT8U	Day;					//实时时间
	INT8U	Month;				    //实时时间
	INT16U	Year;					//实时时间

	INT8U	STATE1; 				//状态
	INT8U	STATE2; 				//状态
	
	INT8U	Check;				//校验和
	INT8U	End;					//结束标志
}HT2X188_Pack129S_Stru;//返回实时数据





typedef union
{
	INT8U Data[HT2X188Pro_PackSize];
	struct
	{
		
		INT8U	Start;			//起始字符	1字节
		INT8U	Type;			//协议类型	1字节
		INT32U	SN;			//产品序列号	4字节
		INT8U	FAdd1;		//保留字节	1字节
		INT16U	FAdd2;		//保留字节	1字节

		INT8U	ConType;		//控制类型	1字节
		
		INT8U	Lenth;		//数据长度	1字节
		INT8U	Available[HT2X188Pro_PackSize-HT2X188Pro_HeadSize];//有效数据
	}DefaultPack;


	struct
	{
		
		INT8U	Start;		//起始字符	1字节
		INT8U	Type;		//协议类型	1字节
		INT32U	SN;			//产品序列号	4字节
		INT8U	FAdd1;		//保留字节	1字节
		INT16U	FAdd2;		//保留字节	1字节

		INT8U	ConType;	//控制类型	1字节
		INT8U	Lenth;		//数据长度	1字节
		INT16U	Flag;		//数据标志
		INT8U	Available[HT2X188Pro_PackSize-HT2X188Pro_HeadSize-2];//有效数据
	}CheckPack;	


	HT2X188_Pack01S_Stru Pack0X01S;
	HT2X188_Pack129S_Stru Pack0X129S;
}HT2X188_Pack_Uni;



typedef union
{
	HT2X188_Pack01S_Stru Pack0X01S;

}HT2X188_Send_Uni;

#pragma pack()		//字节对齐


 
#ifdef _HT2X188PRO_C_

INT8U HT2X188_Pack_RxAnalyze_S(UART_RBC_Stru* Ctrl_Point,INT8U DataSize);
INT8U HT2X188_Pack_Rx_S(UART_RBC_Stru* Ctrl_Point,INT8U Protocol);

INT8U HT2X188_Pack_0X129_S(UART_RBC_Stru* PORT);
void HT2X188_Pack_0X01_S(UART_RBC_Stru* PORT);



#endif
 




 
HT2X188PRO_EXT void HT2X188_Pack_RxServer_S( UART_RBC_Stru* Ctrl_Point );
HT2X188PRO_EXT INT8U HT2X188_Pack_TxServer_S(UART_RBC_Stru* Ctrl_Point);



#endif
 
 
 
 
 
 
#endif
/* END */
/*  
 @copy COPYRIGHT 2016 Foryon     
*/  
 




 
