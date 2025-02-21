#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
/*#include <mach/gpio.h>*/
#include "../init-input.h"
#include <linux/input.h>
#include <linux/pinctrl/consumer.h>
#include <linux/sunxi-gpio.h>


#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif


#define SUSPEND_STATUS (1)
#define RESUME_STATUS (0)

#define EARLY_SUSPEND_STATUS (1)
#define EARLY_RESUME_STATUS (0)



#define DBG_EN 1
#if (DBG_EN == 1)
	#define __dbg(x, arg...) printk("[HAL_SWI_DBG]"x, ##arg)
	#define __err(x, arg...) printk(KERN_INFO"[HAL_SWI_ERR]"x, ##arg)
	#define __inf(x, arg...) printk(KERN_INFO"[HAL_SWI_INF]"x, ##arg)
#else
	#define __dbg(x, arg...)
	#define __err(x, arg...)
	#define __inf(x, arg...)
#endif

char phys[32];
u32 int_number;
struct gpio_config irq_gpio;
struct input_dev *input_dev;
struct work_struct  work;
static struct workqueue_struct *hal_wq;
/*static int trigger_mode;*/
static int sys_status;
static int early_status;
static spinlock_t irq_lock;
static char irq_pin_name[8];
/*static int resule_flag;*/
unsigned int old_level_status;

struct hal_data {
#ifdef CONFIG_HAS_EARLYSUSPEND
	struct early_suspend early_suspend;
#endif
};

#ifdef CONFIG_PM
static struct dev_pm_domain hal_pm_domain;
#endif

struct hal_data *hal_switch_data;
static unsigned int level_status; /*0:low level 1:high level*/
static unsigned int level_status1;
static unsigned int is_first = 1;
/*static unsigned long last_inter_time;
static unsigned long timeout = 30;
static unsigned long time_Interval = 0;
static int is_inter_noise = 0;
*/

static void hal_work_func(struct work_struct *work)
{
	unsigned int config;
	unsigned int irqflags = 0;
	unsigned int irq_num;
	/*int i;
	unsigned int count =0;
	*/
	unsigned int cd_detect;
	/*printk("%s ****enter timeout = %ld\n", __FUNCTION__, timeout);

	设置GPIO 为input功能
	*/
	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0);
	pin_config_set(SUNXI_PINCTRL, irq_pin_name, config);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, 0xFFFF);
	pin_config_get(SUNXI_PINCTRL, irq_pin_name, &config);


	cd_detect = level_status;
	level_status = gpio_get_value(int_number);

	printk("gpio value = %d\n", level_status);

    printk(" ********hal_work_func level_status = %d, old_level_status = %d, is_first =%d, sys_status = %d, early_status = %d\n", level_status, old_level_status, is_first, sys_status, early_status);
#if 0
	for (i = 0; i < 3; i++) {
		udelay(10);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0);
		pin_config_set(SUNXI_PINCTRL, irq_pin_name, config);

		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, 0xFFFF);
		pin_config_get(SUNXI_PINCTRL, irq_pin_name, &config);

/*		level_status = SUNXI_PINCFG_UNPACK_VALUE(config);*/
		level_status = gpio_get_value(val.gpio.gpio);

		if (cd_detect == level_status) {
			printk("count++, count:%d\n", count);
			count++;
		}
	}

	if (count != 3) {
		goto end;
	}
#endif
/*	if (is_first == 1){
		last_inter_time = jiffies;
		is_inter_noise = 0;
		printk("%s last_inter_time = %ld, jiffies = %ld\n ", __FUNCTION__, last_inter_time, jiffies);
	} else {
	    is_inter_noise = 0;
		printk("%s jiffies = %ld,last_inter_time = %ld\n", __FUNCTION__, jiffies, last_inter_time);
	    time_Interval = jiffies - last_inter_time;
		printk("%s	time_Interval = %ld\n", __FUNCTION__, time_Interval);
		if ( time_Interval < timeout) {
		   printk("%s ignor noise interrupt \n", __FUNCTION__);
		   is_inter_noise = 1;
	    }

	}
    if (is_inter_noise == 1) {
		printk(" is_inter_noise = %d", is_inter_noise);
    } else {
	  last_inter_time = jiffies;
*/
      /*printk(" hal_work_func level_status = %d, old_level_status = %d, is_first =%d ,sys_status = %d, early_status = %d\n", level_status, old_level_status, is_first, sys_status, early_status);*/
      /*int test = 11;
      test = EARLY_SUSPEND_STATUS;*/
      /*printk("EARLY_SUSPEND_STATUS=%d\n", EARLY_SUSPEND_STATUS);*/
		if ((level_status != old_level_status) || (is_first == 1)) {
			old_level_status = level_status;
		/*if ((level_status == 1) && (early_status == EARLY_SUSPEND_STATUS)) { /*leave off*/
		if (level_status == 1) { /*leave off*/
			printk("******leave off hall sensor, status is suspend, input KEY_WAKEUP******\n");
			input_report_key(input_dev, KEY_WAKEUP, 1);
			input_sync(input_dev);
			msleep(100);
			input_report_key(input_dev, KEY_WAKEUP, 0);
			input_sync(input_dev);
		/*} else if ((level_status == 0) && (sys_status == RESUME_STATUS) && (early_status == EARLY_RESUME_STATUS)) {/*close*/
		} else if (level_status == 0) {/*close*/
			printk("******close to hall sensor, status is resume, input KEY_SLEEP******\n");
			input_report_key(input_dev, KEY_SLEEP, 1);
			input_sync(input_dev);
			msleep(100);
			input_report_key(input_dev, KEY_SLEEP, 0);
			input_sync(input_dev);
		}

		is_first = 0;
	 }
/*	}
*/

end:
	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 6);
	pin_config_set(SUNXI_PINCTRL, irq_pin_name, config);

	spin_lock_irqsave(&irq_lock, irqflags);
	irq_num = gpio_to_irq(int_number);
	enable_irq(irq_num);
	spin_unlock_irqrestore(&irq_lock, irqflags);
}

irqreturn_t switch_handle(int irq, void *dev_id)
{
	unsigned long irqflags = 0;
	unsigned int irq_num;

	printk(" switch_handle enter\n");

	spin_lock_irqsave(&irq_lock, irqflags);
	irq_num = gpio_to_irq(int_number);
	disable_irq_nosync(irq_num);
	spin_unlock_irqrestore(&irq_lock, irqflags);

	queue_work(hal_wq, &work);

/*	flush_workqueue(hal_wq);
	cancel_work_sync(hal_wq);*/
	return IRQ_HANDLED;
}

/*
static void hal_switch_early_suspend(struct early_suspend *h)
{
	sys_status = SUSPEND_STATUS;
	early_status = EARLY_SUSPEND_STATUS;
	printk("%s early_status = %d \n", __FUNCTION__, early_status);
	flush_workqueue(hal_wq);
}
*/

/*
static void hal_switch_late_resume(struct early_suspend *h)
{

	sys_status = RESUME_STATUS;
	early_status = EARLY_RESUME_STATUS;
	printk("%s early_status = %d \n", __FUNCTION__, early_status);
}
*/

#ifdef CONFIG_PM
static int sunxi_hal_suspend(struct device *dev)
{
	sys_status = SUSPEND_STATUS;
	/*old_level_status = 0;*/
	printk("%s,old_level_status = %d \n", __FUNCTION__, old_level_status);
	/*resule_flag = 0;*/
    unsigned int config;

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, 0xFFFF);
	pin_config_get(SUNXI_PINCTRL, irq_pin_name, &config);

/*	level_status = SUNXI_PINCFG_UNPACK_VALUE(config);*/
	level_status = gpio_get_value(int_number);
	old_level_status = level_status;
	printk("%s sys_status = %d level_status = %d\n", __FUNCTION__, sys_status, level_status);

	flush_workqueue(hal_wq);
	enable_irq_wake(gpio_to_irq(int_number));
	return 0;
}
static int sunxi_hal_resume(struct device *dev)
{
	unsigned int config;
	/*unsigned int irqflags = 0;
	unsigned int irq_num;*/

	sys_status = RESUME_STATUS;
	/*resule_flag = 1;*/
	printk("%s sys_status = %d \n", __FUNCTION__, sys_status);
	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 0);
	pin_config_set(SUNXI_PINCTRL, irq_pin_name, config);

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, 0xFFFF);
	pin_config_get(SUNXI_PINCTRL, irq_pin_name, &config);

/*	level_status = SUNXI_PINCFG_UNPACK_VALUE(config);*/
	level_status = gpio_get_value(int_number);

	printk(" %s level_status = %d, old_level_status = %d , is_first = %d\n", __FUNCTION__, level_status, old_level_status, is_first);
	if ((level_status == 1) && (old_level_status != level_status) && (is_first == 0)) {
		old_level_status  = level_status;
		printk("**********2***************\n");
		input_report_key(input_dev, KEY_WAKEUP, 1);
		input_sync(input_dev);
		udelay(20);
		input_report_key(input_dev, KEY_WAKEUP, 0);
		input_sync(input_dev);
	}
	if ((old_level_status != level_status) && level_status == 0) {
		old_level_status  = level_status;
		printk("%s 00000000000 old_level_status = %d,\n", __FUNCTION__, old_level_status);
	}

	config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, 6);
	pin_config_set(SUNXI_PINCTRL, irq_pin_name, config);
	disable_irq_wake(gpio_to_irq(int_number));
	return 0;
}
#endif

static int switch_request_irq(void)
{
	int irq_number = 0;
	int ret = 0;

	input_dev = input_allocate_device();
	if (input_dev == NULL) {
		ret = -ENOMEM;
		printk("%s:Failed to allocate input device\n", __func__);
		return -1;
	}

	input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	sprintf(phys, "input/switch");

	input_dev->name = "switch";
	input_dev->phys = phys;
	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0002;
	input_dev->id.product = 0x0002;
	input_dev->id.version = 0x0100;
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(EV_REL, input_dev->evbit);
	set_bit(KEY_SLEEP, input_dev->keybit);
	set_bit(KEY_WAKEUP, input_dev->keybit);


#ifdef CONFIG_PM
	hal_pm_domain.ops.suspend = sunxi_hal_suspend;
	hal_pm_domain.ops.resume = sunxi_hal_resume;
	input_dev->dev.pm_domain = &hal_pm_domain;
#endif
	ret = input_register_device(input_dev);
	if (ret) {
		printk("Unable to register %s input device\n", input_dev->name);
		return -1;
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	hal_switch_data = kzalloc(sizeof(*hal_switch_data), GFP_KERNEL);
	if (hal_switch_data == NULL) {
		printk("Alloc GFP_KERNEL memory failed.");
		return -ENOMEM;
	}

	hal_switch_data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	hal_switch_data->early_suspend.suspend = hal_switch_early_suspend;
	hal_switch_data->early_suspend.resume = hal_switch_late_resume;
	register_early_suspend(&hal_switch_data->early_suspend);
#endif
    spin_lock_init(&irq_lock);
	/*注册中断*/
	irq_number = gpio_to_irq(int_number);
    printk("%s,irq_number = %d\n", __FUNCTION__, irq_number);
	if (IS_ERR_VALUE(irq_number)) {
			printk("map gpio [%d] to virq failed, errno = %d\n",
					int_number, irq_number);
			return -EINVAL;
	}

	ret = devm_request_irq(&(input_dev->dev), irq_number, switch_handle,
			       IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING|IRQF_NO_SUSPEND, "HAL_EINT", NULL);
		if (IS_ERR_VALUE(ret)) {
			printk("request virq %d failed, errno = %d\n",
				irq_number, ret);
			return -EINVAL;
		}
	return 0;
}

static int __init switch_init(void)
{

/*	script_item_value_type_e  type;*/
	int ret = -1;
	/*modify by clc*/
	struct device_node *np = NULL;

	np = of_find_node_by_name(NULL, "hall_para");
	if (!np) {
		pr_err("ERROR! get ctp_para failed, func:%s, line:%d\n", __FUNCTION__, __LINE__);
		goto devicetree_get_item_err;
	}
	if (!of_device_is_available(np)) {
		pr_err("%s: hall is not used\n", __func__);
		goto devicetree_get_item_err;
	}
	int_number = of_get_named_gpio_flags(np, "hall_int_port", 0, (enum of_gpio_flags *)&irq_gpio);
	if (!gpio_is_valid(int_number))
		pr_err("%s: hall_int_port is invalid. \n", __func__);


	printk("%s int_number = %d \n", __FUNCTION__, int_number);

	sunxi_gpio_to_name(int_number, irq_pin_name);
	INIT_WORK(&work, hal_work_func);
	printk("%s,irq_pin_name = %s\n", __FUNCTION__, irq_pin_name);

	hal_wq = create_singlethread_workqueue("hal_wq");
	if (!hal_wq) {
		printk("%s:Creat hal_wq workqueue failed.\n", __func__);
		return 0;
	}
	flush_workqueue(hal_wq);
	ret = switch_request_irq();
	if (!ret) {
		printk("*********Register IRQ OK!*********\n");
	}
	return 0;

	/*add by clc*/
devicetree_get_item_err:
		pr_notice("=========script_get_item_err============\n");
		return ret;
}

static void __exit switch_exit(void)
{
	unsigned int irq_num;
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&hal_switch_data->early_suspend);
	kfree(hal_switch_data);
#endif
    if (hal_wq) {
		destroy_workqueue(hal_wq);
		}


	irq_num = gpio_to_irq(int_number);
	devm_free_irq(&(input_dev->dev), irq_num, NULL);
	input_unregister_device(input_dev);
	return;
}

late_initcall(switch_init);
module_exit(switch_exit);

MODULE_DESCRIPTION("halswitch Driver");
MODULE_LICENSE("GPL v2");
