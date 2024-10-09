#include "proj_config.h"
#include "osal/osal.h"
#include "wifi.h"
#include "wifi_port.h"
#include "netif/ethernetif.h"
#include "usr_app.h"
#include "utils/system_parameter.h"
#include "utils/debug/CmBacktrace/cm_backtrace.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/runtime/runtime.h"
#include "hal/hal_interrupt.h"
#include "ln_utils.h"
#include "ln_misc.h"
#include "ln_nvds.h"
#include "ln_kv_api.h"
#include "ln_at.h"
#include "flash_partition_table.h"

#include "ln_ble_app_kv.h"
#include "usr_app.h"
#include "ln_ble_app_default_cfg.h"
#include"dimmingLib.h"

#include "hal/hal_common.h"
#include "hal/hal_gpio.h"
#include "ln_test_common.h"
#include "ln_show_reg.h"
#include "utils/debug/log.h"
#include "hal/hal_timer.h"
#include "hal/hal_clock.h"

#include "ln_drv_pwm.h"
static void pwm_test ()
{
   uint32_t pwm_duty = 0;
  /****************** 1. 系统初始化 ***********************/
SetSysClock();
log_init();   
LOG(LOG_LVL_INFO,"ln882H init! \n");
ln_show_reg_init();

/****************** 2. 外设配置 ***********************/
pwm_init(10000,20,PWM_CHA_0,GPIO_B,GPIO_PIN_5);     //初始化PWM,设置频率为10K,占空比为20%
pwm_init(10000,20,PWM_CHA_1,GPIO_B,GPIO_PIN_6);     //初始化PWM
pwm_init(10000,20,PWM_CHA_2,GPIO_B,GPIO_PIN_7);     //初始化PWM
pwm_start(PWM_CHA_0);  
pwm_start(PWM_CHA_1);
pwm_start(PWM_CHA_2);                             //开始产生PWM
while(1)
{
    pwm_duty ++;
      LOG(LOG_LVL_INFO,"Duty = %d\n",pwm_duty);
    if(pwm_duty > 100) pwm_duty = 0;
    
    pwm_set_duty(pwm_duty,PWM_CHA_0);               //配置占空比
    pwm_set_duty(pwm_duty,PWM_CHA_1); 
    pwm_set_duty(pwm_duty,PWM_CHA_2); 
    
    ln_delay_ms(100);
}
}
int main (int argc, char* argv[])
{
    LN_UNUSED(argc);
    LN_UNUSED(argv);
    //0. check reboot cause
    ln_chip_get_reboot_cause();

    //1.sys clock,interrupt
    SetSysClock();
    set_interrupt_priority();
    switch_global_interrupt(HAL_ENABLE);
    ln_runtime_measure_init();

    //2. register os heap mem
    OS_DefineHeapRegions();

    //3.init log&AT
    log_init();
// extern int ate_init(void);
  //  ate_init();

    //4.cm backtrace
    cm_backtrace_init("combo_mcu_basic_example", "hw", "sw");
    LOG(LOG_LVL_INFO, "------  combo_mcu_basic_example  ------\r\n");

   // 5. init NVDS&KV
    if (NVDS_ERR_OK != ln_nvds_init(NVDS_SPACE_OFFSET)) {
        LOG(LOG_LVL_ERROR, "NVDS init filed!\r\n");
    }

    if (KV_ERR_NONE != ln_kv_port_init(KV_SPACE_OFFSET, (KV_SPACE_OFFSET + KV_SPACE_SIZE))) {
        LOG(LOG_LVL_ERROR, "KV init filed!\r\n");
    }
     pwm_init(10000,20,PWM_CHA_0,GPIO_B,GPIO_PIN_5);  
    //6.init system parameter
    sysparam_integrity_check_all();

    //7.rf preprocess,img cal
    wifi_rf_calibration();

    //8.Init wifi stack.
    wifi_init();
//    wlib_pvtcmd_output_cb_set(ln_at_vprintf);

    //9.Init lwip stack.
    lwip_tcpip_init();
    if ( 0 != ota_port_init()) {
        LOG(LOG_LVL_ERROR, "ota port failed!\r\n");
    }
    myLampLoop();
    //10.Creat usr app task.
    creat_usr_app_task();

    OS_ThreadStartScheduler();

    return 0;
}

// ----------------------------------------------------------------------------
