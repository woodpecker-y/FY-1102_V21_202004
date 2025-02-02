/*
***********************************************************************************
*			
*功能：
*
*
*
*说明：1.
*      2.
*      3.
*
*
*By			:许伟
*Contact	:511328030@qq.com
*History	:2016-1-28 14:37:47
***********************************************************************************
*/
#ifndef _MALLOC_H_
#define _MALLOC_H_

#include "TypeDef.h"
#include "stm32f10x.h"
#ifdef _MALLOC_C_
#define MALLOC_EXT
#else 
#define MALLOC_EXT extern
#endif 


 
#ifndef NULL
#define NULL 0
#endif

//定义三个内存池
#define SRAMIN		0		//内部内存池
#define SRAMEX		1		//外部内存池
#define SRAMCCM	2		//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)


#define SRAMBANK	1		//定义支持的SRAM块数.	


//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE			32  	  					//内存块大小为32字节
#define MEM1_MAX_SIZE				32*128  						//最大管理内存 100K
#define MEM1_ALLOC_TABLE_SIZE		MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小

//mem2内存参数设定.mem2的内存池处于外部SRAM里面
//#define MEM2_BLOCK_SIZE			32  	  						//内存块大小为32字节
//#define MEM2_MAX_SIZE			1 *32  						//最大管理内存960K
//#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小
		 
//mem3内存参数设定.mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
//#define MEM3_BLOCK_SIZE			32  	  						//内存块大小为32字节
//#define MEM3_MAX_SIZE			1 *32  						//最大管理内存60K
//#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//内存表大小
		 

//内存管理控制器
typedef struct 
{
	void (*init)(u8);					//初始化
	u8 (*perused)(u8);				//内存使用率
	u8 	*membase[SRAMBANK];		//内存池 管理SRAMBANK个区域的内存
	u16 *memmap[SRAMBANK];			//内存管理状态表
	u8  memrdy[SRAMBANK];			//内存管理是否就绪
}_m_mallco_dev;

//MALLOC_EXT	_m_mallco_dev mallco_dev;	 //在mallco.c里面定义

#ifdef _MALLOC_C_
void mymemset(void *s,u8 c,u32 count);			//设置内存
void mymemcpy(void *des,void *src,u32 n);		//复制内存     

u32 my_mem_malloc(u8 memx,u32 size);			//内存分配(内部调用)
u8 my_mem_free(u8 memx,u32 offset);			//内存释放(内部调用)
#endif 




MALLOC_EXT	void mymemcpy(void *des,void *src,u32 n);
////////////////////////////////////////////////////////////////////////////////
MALLOC_EXT	void my_mem_init(u8 memx);					//内存管理初始化函数(外/内部调用)

MALLOC_EXT	u8 my_mem_perused(u8 memx);				//获得内存使用率(外/内部调用)
MALLOC_EXT	void myfree(u8 memx,void *ptr);  			//内存释放(外部调用)
MALLOC_EXT	void *mymalloc(u8 memx,u32 size);			//内存分配(外部调用)
MALLOC_EXT	void *myrealloc(u8 memx,void *ptr,u32 size);	//重新分配内存(外部调用)
#endif
