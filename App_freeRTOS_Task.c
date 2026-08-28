#include "App_freeRTOS_Task.h"

//STM32F103C8T6 => SRAM 20KB  分配12k给操作系统


//摇杆数据结构体
//Joystick_Struct joystick={0,0,0,0};


//电源管理任务
void power_task(void *parameter);
//最小推荐堆栈大小为128字节
#define POWER_TASK_STACK_SIZE 128
#define POWER_TASK_PRIORITY 4  //电源管理任务，这个需要最高级
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000


//通讯任务
void com_task(void *args);
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PRIORITY 3  //通讯任务，这个需要第二级
TaskHandle_t com_task_handle;
//任务周期
#define COM_TASK_PERIOD 6

//按键任务
void key_task(void *args);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2  
TaskHandle_t key_task_handle;
//任务周期
#define KEY_TASK_PERIOD 20

//摇杆任务
void joystick_task(void *args);
#define JOYSTICK_TASK_STACK_SIZE 128
#define JOYSTICK_TASK_PRIORITY 2
TaskHandle_t joystick_task_handle;
//任务周期
#define JOYSTICK_TASK_PERIOD 20


void App_FreeRTOS_start(void)
{

  //1.创建任务
  xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
  
  //2.通讯任务
  xTaskCreate(com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);
  
  //3.按键任务
  xTaskCreate(key_task, "key_task", KEY_TASK_STACK_SIZE, NULL, KEY_TASK_PRIORITY, &key_task_handle);
  
  //4.摇杆任务
  xTaskCreate(joystick_task, "joystick_task", JOYSTICK_TASK_STACK_SIZE, NULL, JOYSTICK_TASK_PRIORITY, &joystick_task_handle);
  //启动调度器
  vTaskStartScheduler();

  
}



void power_task(void *parameter)
{
    //获取当前的基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
      
      //每10秒执行一次
      vTaskDelayUntil(&xLastWakeTime, POWER_TASK_PERIOD);
      //启动电源
      Int_IP5305T_start();
    }
}

void joystick_task(void *args){
  //获取当前的基准时间
  TickType_t xLastWakeTime = xTaskGetTickCount();
  Int_Joystick_Init();
  while(1){
    //统一的处理方式
    App_process_key_data();
    vTaskDelayUntil(&xLastWakeTime, JOYSTICK_TASK_PERIOD);
  }
}


void key_task(void *args){
  //获取当前的基准时间
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1){
    //统一的处理方式
    App_process_key_data();
    vTaskDelayUntil(&xLastWakeTime, KEY_TASK_PERIOD);
  }

}

uint8_t com_buff[TX_PLOAD_WIDTH]={0};

void com_task(void *args){
  //获取当前的基准时间
  TickType_t xLastWakeTime = xTaskGetTickCount();
  while(1){
    App_transmit_data();
    //6ms执行一次
    vTaskDelayUntil(&xLastWakeTime, COM_TASK_PERIOD);
  }

}


