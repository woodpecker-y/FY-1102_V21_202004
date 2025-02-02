/*  
***************************************************************
*****************Tools Version 20161020************************
* @copy  COPYRIGHT 2016 Foryon*******************************  
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
* @brief :河北丰源热量表通信协议 FY188 处理框架
* 
* 
* 
*
* @author :许伟
* @date :2017/5/24 9:37
***************************************************************
*/ 
#define _FY188PRO_C_

#include "FY188Pro.h"
#include "main.h"



UART_TO_FY188_QueueSend_Stru FY188_Q_RX_Buffer;		//通信发送队列 接收缓冲器
UART_TO_FY188_QueueSend_Stru* FY188_Q_RX_BuffeP;	//通信发送队列 接收缓冲器P







/*
函数名称:ProtocolCon_Rx_JK_HotMeter_CJ_T188( INT8U PortNum )
函数功能:协议预处理包，描述  将接收队列缓冲器的数据进行解析并进入解析包

*/
void FY188_Pack_RxServer_S( UART_RBC_Stru* Ctrl_Point )
{
	INT8U Bufer;
	INT32U DataBdry =0;	//有效数据区边界
	FY188_Pack_Uni* FY188_RXPack =(FY188_Pack_Uni*)(Ctrl_Point->InputPack);
	while (Ctrl_Point->Rx_Rear !=Ctrl_Point->Rx_Front) 
	{
	
		Bufer=Ctrl_Point->InputBuffer[Ctrl_Point->Rx_Rear];	//从接收缓冲区读取一个字节的数据
		
		if (*(Ctrl_Point->Ticks)- Ctrl_Point->RecPackTimeOut > Ctrl_Point->TimeOut)//帧间隔没有收到新的数据，即将接收下一帧完整数据
		{
			Ctrl_Point->RecPackState=0;	//清除正在处理接收包数据标志
			Ctrl_Point->RecPackPoint=0;	//包指针
		}		
		switch(Ctrl_Point->RecPackState)
		{
			case 0:	//接收起始码
			{
				if (Bufer == FY188Pro_StartCode) 
				{ 
					Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
					Ctrl_Point->RecPackPoint +=1;
					Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);		//收到数据间隔时间清零
					Ctrl_Point->RecPackState=1;						//开始接收数据包
					DataBdry =0;
					
				}
			}break;
			
			case 1:	//接收版本号
			{		
				if ( Bufer == FY188Pro_ProtocolCode)
				{

					Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
					Ctrl_Point->RecPackPoint +=1;
					Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
					Ctrl_Point->RecPackState=2;//开始接收数据包	
				}
			}break;
			
			case 2://接收SN 第1字节
			{
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=3;//开始接收数据包	
						
			}break;
			
			case 3://接收SN 第2字节
			{
					
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=4;//开始接收数据包	


			}break;
	
			case 4:	//接收SN 第3字节
			{
					
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=5;//开始接收数据包	

			}break;
			case 5:	//接收SN 第4字节
			{
					
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=6;//开始接收数据包 

			}break;			

			case 6:	//接收FAdd1
			{
					
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=7;//开始接收数据包 

					
			}break;	
			
			case 7://接收FAdd2
			{
					
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=8;//开始接收数据包 			
			}break;
			
			case 8://接收接收FAdd3
			{
					
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=9;//开始接收数据包 	


			}break;

			case 9://接收ConType
			{
					
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);	 //收到数据间隔时间清零
				Ctrl_Point->RecPackState=10;//开始接收数据包 	
				
			}break;

			case 10:	//数据长度
			{

				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);		//收到数据间隔时间清零

				if(FY188_RXPack->DefaultPack.Lenth )	//判断数据内容是否存在
				{
					Ctrl_Point->RecPackState=11;			//开始接收数据包
				}
				else
				{
					Ctrl_Point->RecPackState=12;			//开始接收数据包
				}
				DataBdry =FY188_RXPack->DefaultPack.Lenth;
			}break;
		
			case 11:	//有效数据区
			{		


				Ctrl_Point->RecPackState=11;					//开始接收数据包
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint] =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);		//收到数据间隔时间清零

				DataBdry -=1;
				if(DataBdry ==0)
//				if ( (FY188_RXPack->DefaultPack.Lenth + FY188Pro_HeadSize)==Ctrl_Point->RecPackPoint)
				{
					Ctrl_Point->RecPackState=12;
				}
				
			}break;

			case 12:	//校验和
			{
				Ctrl_Point->RecPackState=13;					//开始接收数据包
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint] =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);		//收到数据间隔时间清零
				
			}break;
			
			case 13:	//结束标志
			{
				Ctrl_Point->InputPack[Ctrl_Point->RecPackPoint]  =Bufer;
				Ctrl_Point->RecPackPoint +=1;
				Ctrl_Point->RecPackState=0; 
				
				if (Bufer ==FY188Pro_EndCode)
				{
					FY188_Pack_RxAnalyze_S(Ctrl_Point,FY188_RXPack->DefaultPack.Lenth+FY188Pro_HeadSize);
				}
				Ctrl_Point->RecPackTimeOut=*(Ctrl_Point->Ticks);
			}break;	
			
			default:	//错误状态,恢复状态为0
			{
				Ctrl_Point->RecPackState=0;

			}break;
		}
		
		if (++Ctrl_Point->Rx_Rear >= UART_TO_FY188BufferSize_S)
		{
			Ctrl_Point->Rx_Rear=0;
		}	

	}
/* 无操作系统环境下使用   ，操作系统环境下建议 创建单独任务         */
}









/*
函数名称:void  RXPack_AnalyzeForyonV10(INT8U PortNum,INT16U DataNum)
函数功能:通信解析并处理子函数
*/
INT8U FY188_Pack_RxAnalyze_S(UART_RBC_Stru* Ctrl_Point,INT8U DataSize)
{
	INT8U CheckFlg =0;	//错误标志
	INT16U ProtoNO =0;	//协议编号
	FY188_Pack_Uni* Packin =(FY188_Pack_Uni*)Ctrl_Point->InputPack;	//指针变换
	
	CheckFlg =SUMCheck_Check(((INT8U*)Ctrl_Point->InputPack),DataSize);
	if(CheckFlg !=0)//校验错误，退出函数
	{
		return	CheckFlg;
	}
	else
	{

		ProtoNO =Packin->DefaultPack.ConType;//传递协议编码
		if (ProtoNO ==0X01)
		{
			ProtoNO=0X81;
		}
		CheckFlg =FY188_Pack_Rx_S(Ctrl_Point,ProtoNO);
	}
	return CheckFlg;
}




/*

函数名称: INT8U FY188_Pack_RXServer(UART_RBC_Stru* Ctrl_Point,INT8U Protocol)
函数功能:丰源智控协议接收服务函数
Ctrl_Point:接收控制器
Protocol: 协议编号
返回值: 0X00 协议正常 0X01 协议处理出现问题 0XFF 协议不存在
*/
INT8U FY188_Pack_Rx_S(UART_RBC_Stru* Ctrl_Point,INT8U Protocol)
{

	INT8U ErrorFlg =0;
	FY188_Pack_Uni* PackData =(FY188_Pack_Uni*)(Ctrl_Point->InputPack);		   	//输入指针交接
	INT8U Proto_RX =Protocol;
	INT16U DevNum =0;
	INT32U MeterID=0;

	INT8U DataBuffer=0;		//数据缓冲区
	INT16U DataBuffer2=0;	//数据缓冲区
	INT32U DataBuffer4=0;	//数据缓冲区

	INT16U TempL =0;
	INT8U TempH =0;

	INT16U WorkTime_L =0;
	INT8U WorkTime_H =0;
	
	switch (Proto_RX)
	{
		case 129:
		{
			if(FY188_Pack_0X129_S(Ctrl_Point) ==0x00)			//接收协议处理
			{
				if(ClientCH1Ctrler.Device.Type ==Meter_B)		//大口径楼栋热量表
				{
					MeterID =PackData->Pack0X129S.Head.SN;	//获取设备的ID
					DevNum =ClientCH1Ctrler.Device.Num;		//获取设备下标
					
					
					//if( GetMeters_Num(MeterID,Meter_B,&DevNum) ==HAL_OK)	//设备查找成功
					if( SysDeviceList.Device[DevNum].ID ==MeterID)	//设备查找成功
					{
						//累计冷量采集   
						DataBuffer4 =PackData->Pack0X129S.Total_Code;
						SysDevData[DevNum].Device1.Total_Code =BcdToHex4(&DataBuffer4);//累计冷量
						SysDevData[DevNum].Device1.Total_Code_Unit =PackData->Pack0X129S.Total_Code_Unit;//累计冷量单位

						//当前热量采集
						DataBuffer4 =PackData->Pack0X129S.Total_Energy;
						SysDevData[DevNum].Device1.Total_Energy =BcdToHex4(&DataBuffer4);//热量值
						SysDevData[DevNum].Device1.Total_Energy_Unit =PackData->Pack0X129S.Total_Energy_Unit;//热量单位

						//热功率采集
						DataBuffer4 =PackData->Pack0X129S.Instant_Energy;
						SysDevData[DevNum].Device1.Instant_Energy =BcdToHex4(&DataBuffer4);//热功率
						SysDevData[DevNum].Device1.Instant_Energy_Unit =PackData->Pack0X129S.Instant_Energy_Unit;//热功率单位

						//瞬时流量采集
						DataBuffer4 =PackData->Pack0X129S.Instant_Current;
						SysDevData[DevNum].Device1.Instant_Current =BcdToHex4(&DataBuffer4);//流速
						SysDevData[DevNum].Device1.Instant_Current_Unit =PackData->Pack0X129S.Instant_Current_Unit;//流速单位

						//累计流量采集
						DataBuffer4 =PackData->Pack0X129S.Total_Current;						
						SysDevData[DevNum].Device1.Total_Current =BcdToHex4(&DataBuffer4);//累计流量
						SysDevData[DevNum].Device1.Total_Current_Unit =PackData->Pack0X129S.Total_Current_Unit;//累计流量

						
						//进水温度采集
						DataBuffer2 =PackData->Pack0X129S.Input_TempL;
						TempL =BcdToHex2(&DataBuffer2);
						
						DataBuffer =PackData->Pack0X129S.Input_TempH;
						TempH =BcdToHex(DataBuffer);
						SysDevData[DevNum].Device1.Input_Temp =TempH*10000+TempL;

						//出水温度采集
						DataBuffer2 =PackData->Pack0X129S.Output_TempL;
						TempL =BcdToHex2(&DataBuffer2);
						
						DataBuffer =PackData->Pack0X129S.Output_TempH;
						TempH =BcdToHex(DataBuffer);
						SysDevData[DevNum].Device1.Output_Temp =TempH*10000+TempL;


						//运行时间
						DataBuffer2 =PackData->Pack0X129S.Work_Time_L;
						WorkTime_L =BcdToHex2(&DataBuffer2);
						DataBuffer =PackData->Pack0X129S.Work_Time_H;
						WorkTime_H =BcdToHex(DataBuffer);
						SysDevData[DevNum].Device1.Work_Time =WorkTime_H*10000UL+WorkTime_L;


						//仪表运行状态监测
						//仪表状态第一个字节
						SysDevData[DevNum].Device1.STATE1 =PackData->Pack0X129S.STATE1;//仪表状态
	 
						//仪表状态第二个字节
						SysDevData[DevNum].Device1.STATE2 =PackData->Pack0X129S.STATE2;//仪表状态 

						SysDevData[DevNum].Device1.Apportion_Energy =SysDevData[DevNum].Device1.Total_Energy -SysDevData[DevNum].Device1.CycBot_Energy;  //周期热量计算
						SysDevStatus[DevNum].Device1.ComSucNum +=1;
						SysDevStatus[DevNum].Device1.ComFauNum =0;

						if( SysDevData_Save(DevNum) ==HAL_OK)
						{
							dbg_printf(DEBUG_DEBUG,"设备数据保存  编号: %d \r\n ",DevNum);
						}
							/* 后台实时数据转发   自动抄收*/						
							if(( SysPara.SendMode ==DevSendMode_Auto)&&( ClientCH1Ctrler.UaComFlg ==0))//数据上报类型为自动跟踪 并且外部触发无效
							{
								UART_TO_FY1000_QueueSend_Stru FY1000_Q_TX_Buffer;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.CtrlFlag =0XAA;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Dev_Type =Meter_B;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Dev_ID =MeterID;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.BackFlag=COMBack_OK;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Input_Temp =SysDevData[DevNum].Device1.Input_Temp;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Output_Temp =SysDevData[DevNum].Device1.Output_Temp;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Instant_Current =SysDevData[DevNum].Device1.Instant_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Instant_Current_Unit =SysDevData[DevNum].Device1.Instant_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Instant_Energy =SysDevData[DevNum].Device1.Instant_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Instant_Energy_Unit =SysDevData[DevNum].Device1.Instant_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Total_Current =SysDevData[DevNum].Device1.Total_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Total_Current_Unit =SysDevData[DevNum].Device1.Total_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Total_Code =SysDevData[DevNum].Device1.Total_Code;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Total_Code_Unit =SysDevData[DevNum].Device1.Total_Code_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Total_Energy =SysDevData[DevNum].Device1.Total_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Total_Energy_Unit =SysDevData[DevNum].Device1.Total_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Work_Time =SysDevData[DevNum].Device1.Work_Time;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.STATE1 =SysDevData[DevNum].Device1.STATE1;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.STATE2 =SysDevData[DevNum].Device1.STATE2;


 
								if(FY_1000Send_Code_QInput(&FY1000_Q_TX_Buffer,0XB0)==pdTRUE)
								{
									dbg_printf(DEBUG_DEBUG,"设备自动抄收数据转发  编号: %d SN:%08lX\r\n ",DevNum,FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D1.Dev_ID);
								}
							}
							/* 后台实时数据转发 自动抄收END*/

							/* 后台实时数据转发   远程抄收*/
							if( ClientCH1Ctrler.UaComFlg !=0 )//数据上报类型为自动跟踪	或者外部触发数据超收 
							{
								UART_TO_FY1000_QueueSend_Stru FY1000_Q_TX_Buffer;

								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.CtrlFlag =0XAA;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Dev_Type =Meter_B;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Dev_ID =MeterID;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.BackFlag=COMBack_OK;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Input_Temp =SysDevData[DevNum].Device1.Input_Temp;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Output_Temp =SysDevData[DevNum].Device1.Output_Temp;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Instant_Current =SysDevData[DevNum].Device1.Instant_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Instant_Current_Unit =SysDevData[DevNum].Device1.Instant_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Instant_Energy =SysDevData[DevNum].Device1.Instant_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Instant_Energy_Unit =SysDevData[DevNum].Device1.Instant_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Total_Current =SysDevData[DevNum].Device1.Total_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Total_Current_Unit =SysDevData[DevNum].Device1.Total_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Total_Code =SysDevData[DevNum].Device1.Total_Code;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Total_Code_Unit =SysDevData[DevNum].Device1.Total_Code_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Total_Energy =SysDevData[DevNum].Device1.Total_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Total_Energy_Unit =SysDevData[DevNum].Device1.Total_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Work_Time =SysDevData[DevNum].Device1.Work_Time;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.STATE1 =SysDevData[DevNum].Device1.STATE1;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.STATE2 =SysDevData[DevNum].Device1.STATE2;
								
								if(FY_1000Send_Code_QInput(&FY1000_Q_TX_Buffer,0X02)==pdTRUE)
								{
									dbg_printf(DEBUG_DEBUG,"设备远程抄收数据转发  编号: %d SN:%08lX\r\n ",DevNum,FY1000_Q_TX_Buffer.SendData.Pack_0X02_D1.Dev_ID);
								}
							}
							/* 后台实时数据转发 远程抄收END*/	

							
						ClientCH1Ctrler.SignleCom =RESET;

					}
				}

				
				else if(ClientCH1Ctrler.Device.Type ==Meter_U)	//户用超声波热量表
				{
				
					MeterID =PackData->Pack0X129S.Head.SN;	//获取设备的ID
					DevNum =ClientCH1Ctrler.Device.Num;		//获取设备下标
					
					//if( GetMeters_Num(MeterID,Meter_U,&DevNum) ==HAL_OK)	//设备查找成功
					if( SysDeviceList.Device[DevNum].ID ==MeterID)	//设备查找成功
					{
						//累计冷量采集	 
						DataBuffer4 =PackData->Pack0X129S.Total_Code;
						SysDevData[DevNum].Device2.Total_Code =BcdToHex4(&DataBuffer4);//累计冷量
						SysDevData[DevNum].Device2.Total_Code_Unit =PackData->Pack0X129S.Total_Code_Unit;//累计冷量单位

						//当前热量采集
						DataBuffer4 =PackData->Pack0X129S.Total_Energy;
						SysDevData[DevNum].Device2.Total_Energy =BcdToHex4(&DataBuffer4);//热量值
						SysDevData[DevNum].Device2.Total_Energy_Unit =PackData->Pack0X129S.Total_Energy_Unit;//热量单位

						//热功率采集
						DataBuffer4 =PackData->Pack0X129S.Instant_Energy;
						SysDevData[DevNum].Device2.Instant_Energy =BcdToHex4(&DataBuffer4);//热功率
						SysDevData[DevNum].Device2.Instant_Energy_Unit =PackData->Pack0X129S.Instant_Energy_Unit;//热功率单位

						//瞬时流量采集
						DataBuffer4 =PackData->Pack0X129S.Instant_Current;
						SysDevData[DevNum].Device2.Instant_Current =BcdToHex4(&DataBuffer4);//流速
						SysDevData[DevNum].Device2.Instant_Current_Unit =PackData->Pack0X129S.Instant_Current_Unit;//流速单位

						//累计流量采集
						DataBuffer4 =PackData->Pack0X129S.Total_Current;						
						SysDevData[DevNum].Device2.Total_Current =BcdToHex4(&DataBuffer4);//累计流量
						SysDevData[DevNum].Device2.Total_Current_Unit =PackData->Pack0X129S.Total_Current_Unit;//累计流量

						
						//进水温度采集
						DataBuffer2 =PackData->Pack0X129S.Input_TempL;
						TempL =BcdToHex2(&DataBuffer2);
						
						DataBuffer =PackData->Pack0X129S.Input_TempH;
						TempH =BcdToHex(DataBuffer);
						SysDevData[DevNum].Device2.Input_Temp =TempH*10000+TempL;

						//出水温度采集
						DataBuffer2 =PackData->Pack0X129S.Output_TempL;
						TempL =BcdToHex2(&DataBuffer2);
						
						DataBuffer =PackData->Pack0X129S.Output_TempH;
						TempH =BcdToHex(DataBuffer);
						SysDevData[DevNum].Device2.Output_Temp =TempH*10000+TempL;


						//运行时间
						DataBuffer2 =PackData->Pack0X129S.Work_Time_L;
						WorkTime_L =BcdToHex2(&DataBuffer2);
						DataBuffer =PackData->Pack0X129S.Work_Time_H;
						WorkTime_H =BcdToHex(DataBuffer);
						SysDevData[DevNum].Device2.Work_Time =WorkTime_H*10000UL+WorkTime_L;
						
						if( SysDevData_Save(DevNum) ==HAL_OK)
						{
							dbg_printf(DEBUG_DEBUG,"设备数据保存  编号: %d \r\n ",DevNum);
						}


						//仪表运行状态监测
						//仪表状态第一个字节
						SysDevData[DevNum].Device2.STATE1 =PackData->Pack0X129S.STATE1;//仪表状态

						//仪表状态第二个字节
						SysDevData[DevNum].Device2.STATE2 =PackData->Pack0X129S.STATE2;//仪表状态
						
						SysDevStatus[DevNum].Device2.ComSucNum +=1;
						SysDevStatus[DevNum].Device2.ComFauNum =0;


						if( SysPara.DeviceType !=Type_Valve)	//非通断时间面积法热计量系统
						{
							INT16U UserNum =0;
							INT32U UserID =ClientCH1Ctrler.Device.UserKEY;
							
							if(GetUser_Num(UserID,&UserNum ) ==HAL_OK) //用户查询成功
							{
								UserData[UserNum].Apportion_Energy =SysDevData[DevNum].Device2.Total_Energy; //将阀门开阀数据传递给用户
								 if(UserData_Save(UserNum) ==HAL_OK)
								 {
									dbg_printf(DEBUG_DEBUG,"用户数据保存  编号: %d \r\n ",UserNum);
								 }
							}						
						}


						/* 后台实时数据转发  自动抄收 */
						//	if( SysPara.SendMode ==DevSendMode_Auto )//数据上报类型为自动跟踪  或者外部触发数据超收 							
							if(( SysPara.SendMode ==DevSendMode_Auto)&&( ClientCH1Ctrler.UaComFlg ==0))//数据上报类型为自动跟踪 并且外部触发无效
							{
								UART_TO_FY1000_QueueSend_Stru FY1000_Q_TX_Buffer;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.CtrlFlag =0XAA;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Dev_Type =Meter_U;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Dev_ID =MeterID;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.BackFlag =COMBack_OK;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Input_Temp =SysDevData[DevNum].Device2.Input_Temp;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Output_Temp =SysDevData[DevNum].Device2.Output_Temp;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Instant_Current =SysDevData[DevNum].Device2.Instant_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Instant_Current_Unit =SysDevData[DevNum].Device2.Instant_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Instant_Energy =SysDevData[DevNum].Device2.Instant_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Instant_Energy_Unit =SysDevData[DevNum].Device2.Instant_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Total_Current =SysDevData[DevNum].Device2.Total_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Total_Current_Unit =SysDevData[DevNum].Device2.Total_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Total_Code =SysDevData[DevNum].Device2.Total_Code;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Total_Code_Unit =SysDevData[DevNum].Device2.Total_Code_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Total_Energy =SysDevData[DevNum].Device2.Total_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Total_Energy_Unit =SysDevData[DevNum].Device2.Total_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Work_Time =SysDevData[DevNum].Device2.Work_Time;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.STATE1 =SysDevData[DevNum].Device2.STATE1;
								FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.STATE2 =SysDevData[DevNum].Device2.STATE2;
			
								if(FY_1000Send_Code_QInput(&FY1000_Q_TX_Buffer,0XB0)==pdTRUE)
								{
									dbg_printf(DEBUG_DEBUG,"设备自动超收数据转发  编号: %d SN:%08lX\r\n ",DevNum,FY1000_Q_TX_Buffer.SendData.Pack_0XB0_D2.Dev_ID);
								}
							}
						/* 后台实时数据转发 自动抄收END*/

						/* 后台实时数据转发   远程抄收*/
							if( ClientCH1Ctrler.UaComFlg !=0 )//外部触发数据超收 
							{
								UART_TO_FY1000_QueueSend_Stru FY1000_Q_TX_Buffer;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.CtrlFlag =0XAA;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Dev_Type =Meter_U;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Dev_ID =MeterID;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.BackFlag =COMBack_OK;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Input_Temp =SysDevData[DevNum].Device2.Input_Temp;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Output_Temp =SysDevData[DevNum].Device2.Output_Temp;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Instant_Current =SysDevData[DevNum].Device2.Instant_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Instant_Current_Unit =SysDevData[DevNum].Device2.Instant_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Instant_Energy =SysDevData[DevNum].Device2.Instant_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Instant_Energy_Unit =SysDevData[DevNum].Device2.Instant_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Total_Current =SysDevData[DevNum].Device2.Total_Current;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Total_Current_Unit =SysDevData[DevNum].Device2.Total_Current_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Total_Code =SysDevData[DevNum].Device2.Total_Code;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Total_Code_Unit =SysDevData[DevNum].Device2.Total_Code_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Total_Energy =SysDevData[DevNum].Device2.Total_Energy;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Total_Energy_Unit =SysDevData[DevNum].Device2.Total_Energy_Unit;
								
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Work_Time =SysDevData[DevNum].Device2.Work_Time;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.STATE1 =SysDevData[DevNum].Device2.STATE1;
								FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.STATE2 =SysDevData[DevNum].Device2.STATE2;
								

								if(FY_1000Send_Code_QInput(&FY1000_Q_TX_Buffer,0X02)==pdTRUE)
								{
									dbg_printf(DEBUG_DEBUG,"设备自动超收数据转发  编号: %d SN:%08lX\r\n ",DevNum,FY1000_Q_TX_Buffer.SendData.Pack_0X02_D2.Dev_ID);
								}
							}
						/* 后台实时数据转发 远程抄收END*/

						
						ClientCH1Ctrler.SignleCom =RESET;
					}
				}
			}
		}
		break;
		
		default :
		{
		   ErrorFlg =0XFF; 
		}break;

	}
	return ErrorFlg;
}


/*
FY188_打包服务函数

*/
INT8U FY188_Pack_TxServer_S(UART_RBC_Stru* Ctrl_Point)
{

	BaseType_t Err = HAL_ERROR;

	/* 总线空闲状态检查 */
//	if(*(Ctrl_Point->SendBusy) != HAL_OK)
//	{
//		return HAL_ERROR;
//	}

	/* 接收队列数据 */
	Err =xQueueReceive(UART_TO_FY188_Msg,&FY188_Q_RX_BuffeP,5);
	if( Err != pdTRUE)
	{
		return HAL_ERROR;
	}

	/* 复制待发送数据并打包 */
	FY188_Q_RX_Buffer =*FY188_Q_RX_BuffeP;
	Ctrl_Point->PackCreatFlag =FY188_Q_RX_Buffer.PackCreatFlag;
	Ctrl_Point->PackINPort =FY188_Q_RX_Buffer.PackINPort;
	
	/* 创建启动标志检查 总线空闲状态检查 */
	if(Ctrl_Point->PackCreatFlag != ENABLE)
	{
		return HAL_ERROR;
	}
	Ctrl_Point->PackCreatFlag =DISABLE;

	/* 打包协议编号识别 */
	switch(Ctrl_Point->PackINPort)
	{
		case 0X01:
		{
			FY188_Pack_0X01_S(Ctrl_Point);

		}break;
		case 0X04:
		{
			FY188_Pack_0X04_S(Ctrl_Point);
		}break;

		default:
		{
			Ctrl_Point->PackINPort =0;
		}
		break;
	}
	return HAL_OK;
}





 


/*
	主机/服务端接收仪表返回实时数据
*/
INT8U FY188_Pack_0X129_S(UART_RBC_Stru* PORT)
{

	FY188_Pack_Uni* PackData =(FY188_Pack_Uni*)(PORT->InputPack);	//指针变换
	INT8U BackVal =0X00;
	
	if ((PackData->Pack0X129S.Head.Start==0X68)&&(PackData->Pack0X129S.Head.Type ==0X25))	//在此判断帧头的合法性
	{
		if ((PackData->Pack0X129S.Flag==0X901F)&&( PackData->Pack0X129S.SER==0X03))
		{
			BackVal =0X00;
		}
	}
	else
	{
		BackVal =0X01;
	}
	return BackVal;
}


/*
	主机/服务端召测实时数据命令
*/
void FY188_Pack_0X01_S(UART_RBC_Stru* PORT)
{

	FY188_Pack_Uni* PackData =(FY188_Pack_Uni*)(PORT->OutputPack);	//指针变换
	INT8U PackSize =0;
	

	PackData->Pack0X01S.Lead1=0X73;			//同步码	
	PackData->Pack0X01S.Lead2=0X73;			//同步码	
	PackData->Pack0X01S.Lead3=0XFE;			//同步码	
	PackData->Pack0X01S.Lead4=0XFE;			//同步码	
		
	PackData->Pack0X01S.Head.Start =0X68;
	PackData->Pack0X01S.Head.Type =0X20;

	PackData->Pack0X01S.Head.SN =FY188_Q_RX_Buffer.ID;
	PackData->Pack0X01S.Head.FAdd1=0X00;
	PackData->Pack0X01S.Head.FAdd2=0X1111;

	PackData->Pack0X01S.Head.ConType =0X01;

	PackData->Pack0X01S.Head.Lenth =sizeof(FY188_Pack01S_Stru)-FY188Pro_HeadSize-2-4;//实际有效数据区长度
	PackSize =sizeof(FY188_Pack01S_Stru)-2;	//校验数据长度计算
	


	PackData->Pack0X01S.Flag =0X901F;
	PackData->Pack0X01S.SER =0X03;
	
	PackData->Pack0X01S.Check =SUMCheck_Input((INT8U*)PackData+4,PackSize-4);
	PackData->Pack0X01S.End =0X16;
	
	PackSize+=2;
	PORT->OutputPackSize =PackSize;
	PORT->PackSendFlag =ENABLE;
}







/*

	FY188_Pack_0X04_S
	主机/服务端设置实时时钟命令
*/
void FY188_Pack_0X04_S(UART_RBC_Stru* PORT)
{

	FY188_Pack_Uni* PackData =(FY188_Pack_Uni*)(PORT->OutputPack);	//指针变换
	INT8U PackSize =0;
	INT8U TimeBufer =0;
	INT16U TimeBufer2 =0;

	PackData->Pack0X04S.Lead1=0X73;			//同步码	
	PackData->Pack0X04S.Lead2=0X73;			//同步码	
	PackData->Pack0X04S.Lead3=0XFE;			//同步码	
	PackData->Pack0X04S.Lead4=0XFE;			//同步码	
	
	PackData->Pack0X04S.Head.Start =0X68;
	PackData->Pack0X04S.Head.Type =0X20;

	PackData->Pack0X04S.Head.SN =0XAAAAAAAA;
	PackData->Pack0X04S.Head.FAdd1=0XAA;
	PackData->Pack0X04S.Head.FAdd2=0XAAAA;

	PackData->Pack0X04S.Head.ConType =0X04;

	
	PackData->Pack0X04S.Head.Lenth =sizeof(FY188_Pack04S_Stru)-FY188Pro_HeadSize-2-4;//实际有效数据区长度
	PackSize =sizeof(FY188_Pack04S_Stru)-2;	//校验数据长度计算


	PackData->Pack0X04S.Flag =0XA015;
	PackData->Pack0X04S.SER =0XAA;

	PCF8563_Read(&RTC_Time);
	
	TimeBufer =RTC_Time.Second;		//秒
	PackData->Pack0X04S.Second =HexToBcd(TimeBufer);

	TimeBufer =RTC_Time.Minute;		//分
	PackData->Pack0X04S.Minute =HexToBcd(TimeBufer);

	TimeBufer =RTC_Time.Hour;			//时
	PackData->Pack0X04S.Hour =HexToBcd(TimeBufer);

	TimeBufer =RTC_Time.Day;			//日
	PackData->Pack0X04S.Day =HexToBcd(TimeBufer);

	TimeBufer =RTC_Time.Month;		//月
	PackData->Pack0X04S.Month =HexToBcd(TimeBufer);
	
	TimeBufer2 =RTC_Time.Year;		//年
	
	PackData->Pack0X04S.Year =HexToBcd2(&TimeBufer2);		
	
	PackData->Pack0X04S.Check =SUMCheck_Input((INT8U*)PackData+4,PackSize-4);
	PackData->Pack0X04S.End =0X16;

	
	PackSize+=2;
	PORT->OutputPackSize =PackSize;
	PORT->PackSendFlag =ENABLE;


}







/* END */
/*
 @copy COPYRIGHT 2016 Foryon     
*/
 




 
