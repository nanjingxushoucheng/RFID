/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <string.h>
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./exti/bsp_exti.h"



/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
OS_MEM  mem;                    //声明内存管理对象
uint8_t ucArray [ 70 ] [ 5 ];   //声明内存分区大小


/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

OS_TCB   AppTaskAtParserTCB;
OS_TCB   AppTaskAtProcessTCB;
OS_TCB   AppTaskLogTCB;



/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskAtParserStk [ APP_TASK_AT_PARSER_STK_SIZE ];
static  CPU_STK  AppTaskAtProcessStk [ APP_TASK_AT_PROCESS_STK_SIZE ];
static  CPU_STK  AppTaskLogStk [ APP_TASK_LOG_SIZE ];



/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskAtParser  ( void * p_arg );
static  void  AppTaskAtProcess  ( void * p_arg );
static  void  AppTaskLog  ( void * p_arg );



/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */


}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    CPU_IntDisMeasMaxCurReset();

    /* 配置时间片轮转调度 */
    OSSchedRoundRobinCfg((CPU_BOOLEAN   )DEF_ENABLED,          //使能时间片轮转调度
		                     (OS_TICK       )0,                    //把 OSCfg_TickRate_Hz / 10 设为默认时间片值
												 (OS_ERR       *)&err );               //返回错误类型


		/* 创建内存管理对象 mem */
		OSMemCreate ((OS_MEM      *)&mem,             //指向内存管理对象
								 (CPU_CHAR    *)"Mem For Test",   //命名内存管理对象
								 (void        *)ucArray,          //内存分区的首地址
								 (OS_MEM_QTY   )70,               //内存分区中内存块数目
								 (OS_MEM_SIZE  )5,                //内存块的字节数目
								 (OS_ERR      *)&err);            //返回错误类型


    OSTaskCreate((OS_TCB     *)&AppTaskAtParserTCB,                /* Create the At Parser task                                */
                 (CPU_CHAR   *)"App Task At Parser",
                 (OS_TASK_PTR ) AppTaskAtParser,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_AT_PARSER_PRIO,
                 (CPU_STK    *)&AppTaskAtParserStk[0],
                 (CPU_STK_SIZE) APP_TASK_AT_PARSER_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_AT_PARSER_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTaskAtProcessTCB,                /* Create the At Process task                                */
                 (CPU_CHAR   *)"App Task At Process",
                 (OS_TASK_PTR ) AppTaskAtProcess,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_AT_PROCESS_PRIO,
                 (CPU_STK    *)&AppTaskAtProcessStk[0],
                 (CPU_STK_SIZE) APP_TASK_AT_PROCESS_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_AT_PROCESS_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);


    OSTaskCreate((OS_TCB    *)&AppTaskLogTCB,				 /* Create the Log task								  */
                (CPU_CHAR   *)"App Log Process",
                (OS_TASK_PTR ) AppTaskLog,
                (void       *) 0,
                (OS_PRIO     ) APP_TASK_LOG_PRIO,
                (CPU_STK    *)&AppTaskLogStk[0],
                (CPU_STK_SIZE) APP_TASK_LOG_SIZE / 10,
                (CPU_STK_SIZE) APP_TASK_LOG_SIZE,
                (OS_MSG_QTY  ) 5u,
                (OS_TICK     ) 0u,
                (void       *) 0,
                (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR     *)&err);

	  OSTaskDel ( & AppTaskStartTCB, & err );


}


/*
*********************************************************************************************************
*                                          AT PARSER TASK
*********************************************************************************************************
*/

static  void  AppTaskAtParser ( void * p_arg )
{
	OS_ERR      err;
	OS_MSG_SIZE    msg_size;
	CPU_SR_ALLOC();
	
	char * pMsg;
	(void)p_arg;
	
	while (DEF_TRUE) {
															//任务体
		/* 阻塞任务，等待任务消息 */
		pMsg = OSTaskQPend ((OS_TICK        )0,                    //无期限等待
												(OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
												(OS_MSG_SIZE   *)&msg_size,            //返回消息长度
												(CPU_TS        *)0,                    //返回消息被发布的时间戳
												(OS_ERR        *)&err);                //返回错误类型

//		OS_CRITICAL_ENTER();                                       //进入临界段，避免串口打印被打断
		printf ( "%c", * pMsg );                                   //打印消息内容

//		OS_CRITICAL_EXIT();                                        //退出临界段

		/* 退还内存块 */
		OSMemPut ((OS_MEM  *)&mem,                                 //指向内存管理对象
							(void    *)pMsg,                                 //内存块的首地址
							(OS_ERR  *)&err);		                             //返回错误类型
	}


}


/*
*********************************************************************************************************
*                                          AT PROCESS TASK
*********************************************************************************************************
*/

static  void  AppTaskAtProcess ( void * p_arg )
{
		OS_ERR      err;
    if(0){


   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );
    }
        }
    while(1)
    {
        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
    }
}


/*
*********************************************************************************************************
*                                          LOG TASK
*********************************************************************************************************
*/
static  void  AppTaskLog  ( void * p_arg )
{
		(void)p_arg;
		LED_GPIO_Config();

		while(DEF_TRUE){
			LED_Toggle(GPIOC,GPIO_Pin_2);
			LED_Toggle(GPIOC,GPIO_Pin_3);
		}
}
