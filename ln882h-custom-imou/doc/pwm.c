#include "ln_adv_timer_test.h"
#include "ln_test_common.h"
#include "hal/hal_adv_timer.h"
#include "hal/hal_gpio.h"
#include "log.h"
#include "reg_adv_timer.h"
void ln_adv_timer_test(void)
{
__NVIC_SetPriorityGrouping(4);
__enable_irq();
/* pwm 引脚初始化 */
hal_gpio_afio_select(GPIOB_BASE,GPIO_PIN_5,ADV_TIMER_PWM0);
hal_gpio_afio_select(GPIOB_BASE,GPIO_PIN_6,ADV_TIMER_PWM1);
hal_gpio_afio_select(GPIOB_BASE,GPIO_PIN_7,ADV_TIMER_PWM2);
hal_gpio_afio_en(GPIOB_BASE,GPIO_PIN_5,HAL_ENABLE);
hal_gpio_afio_en(GPIOB_BASE,GPIO_PIN_6,HAL_ENABLE);
hal_gpio_afio_en(GPIOB_BASE,GPIO_PIN_7,HAL_ENABLE);
gpio_init_t gpio_init;
memset(&gpio_init,0,sizeof(gpio_init)); //清零结构体
gpio_init.dir = GPIO_OUTPUT; //配置GPIO方向，输入或者输出
gpio_init.pin = GPIO_PIN_8; //配置GPIO引脚号
gpio_init.speed = GPIO_HIGH_SPEED; //设置GPIO速度
hal_gpio_init(GPIOB_BASE,&gpio_init); //初始化GPIO
/* PWM参数初始化 */
adv_tim_init_t_def adv_tim_init;
memset(&adv_tim_init,0,sizeof(adv_tim_init));
adv_tim_init.adv_tim_clk_div = 0; //设置时钟分
adv_tim_init.adv_tim_load_value = 40000 - 1; //设置PWM频1 / (40 / (1 / 40000)) = 1k
adv_tim_init.adv_tim_cmp_a_value = 20000 ; //设置通道a比较值，占空比为 50%
adv_tim_init.adv_tim_cmp_b_value = 20000 ; //设置通道b比较值，占空比为 50%
adv_tim_init.adv_tim_dead_gap_value = 1000; //设置死区时间
adv_tim_init.adv_tim_dead_en = ADV_TIMER_DEAD_DIS; //不开启死区
adv_tim_init.adv_tim_cnt_mode = ADV_TIMER_CNT_MODE_INC; //向上计数模式
adv_tim_init.adv_tim_cha_en = ADV_TIMER_CHA_EN; //使能通道a
adv_tim_init.adv_tim_chb_en = ADV_TIMER_CHB_EN; //使能通道b
adv_tim_init.adv_tim_cha_it_mode = ADV_TIMER_CHA_IT_MODE_INC; //使能通道a向上计数中断
adv_tim_init.adv_tim_chb_it_mode = ADV_TIMER_CHB_IT_MODE_INC; //使能通道b向上计数中断
hal_adv_tim_init(ADV_TIMER_0_BASE,&adv_tim_init); //初始化
ADV_TIMER0
/* 输入捕获-脉冲计数配置初始化 */
memset(&adv_tim_init,0,sizeof(adv_tim_init));
adv_tim_init.adv_tim_clk_div = 0; //设置时钟分频，0为不分频
adv_tim_init.adv_tim_cap_edg = ADV_TIMER_EDG_RISE; //设置捕获上升沿
adv_tim_init.adv_tim_cap_mode = ADV_TIMER_CAP_MODE_2; //设置捕获模式2
adv_tim_init.adv_tim_cap_en = ADV_TIMER_CAP_EN; //捕获模式使能
adv_tim_init.adv_tim_load_value = 0xFFFF;
hal_adv_tim_init(ADV_TIMER_1_BASE,&adv_tim_init); //初始化ADV_TIMER1
/* enable the pwm interrupt */
hal_adv_tim_it_cfg(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_CMPA,HAL_ENABLE); //使能通道a中断
hal_adv_tim_it_cfg(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_CMPB,HAL_ENABLE); //使能通道b中断
hal_adv_tim_it_cfg(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_LOAD,HAL_ENABLE); //使能加载值中断
// hal_adv_tim_it_cfg(ADV_TIMER_1_BASE,ADV_TIMER_IT_FLAG_CMPA,HAL_ENABLE); //使能通道a中断
// hal_adv_tim_it_cfg(ADV_TIMER_1_BASE,ADV_TIMER_IT_FLAG_CMPB,HAL_ENABLE); //使能通道b中断
// hal_adv_tim_it_cfg(ADV_TIMER_1_BASE,ADV_TIMER_IT_FLAG_LOAD,HAL_ENABLE); //使能加载值中断
NVIC_SetPriority(ADV_TIMER_IRQn, 4);
NVIC_EnableIRQ(ADV_TIMER_IRQn);
uint32_t cmp_a_value = 0; //通道a的比较值
float duty_value = 0; //占空比
uint8_t inc_dir = 0; //占空比递增/减方向
uint32_t pulse_cnt = 0; //脉冲计数
while(1)
{
if(inc_dir == 0)
duty_value += 0.01;
else
duty_value -= 0.01;
if(duty_value >= 0.99)
inc_dir = 1;
if(duty_value <= 0.01)
inc_dir = 0;
hal_adv_tim_set_comp_a(ADV_TIMER_0_BASE,duty_value * 40000);
ln_delay_ms(500);
pulse_cnt = hal_adv_tim_get_trig_value(ADV_TIMER_1_BASE);
}
}
void ADV_TIMER_IRQHandler()
{
if(hal_adv_tim_get_it_flag(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_CMPB) == 1)
{
hal_adv_tim_clear_it_flag(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_CMPB);
//hal_gpio_toggle(GPIOB_BASE,GPIO_PIN_7);
}
if(hal_adv_tim_get_it_flag(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_CMPA) == 1)
{
hal_adv_tim_clear_it_flag(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_CMPA);
//hal_gpio_toggle(GPIOB_BASE,GPIO_PIN_8);
}
if(hal_adv_tim_get_it_flag(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_LOAD) == 1)
{
hal_adv_tim_clear_it_flag(ADV_TIMER_0_BASE,ADV_TIMER_IT_FLAG_LOAD);
//hal_gpio_toggle(GPIOB_BASE,GPIO_PIN_7);
}
if(hal_adv_tim_get_it_flag(ADV_TIMER_1_BASE,ADV_TIMER_IT_FLAG_LOAD) == 1)
{
hal_adv_tim_clear_it_flag(ADV_TIMER_1_BASE,ADV_TIMER_IT_FLAG_LOAD);
hal_gpio_toggle(GPIOB_BASE,GPIO_PIN_8);
}
}