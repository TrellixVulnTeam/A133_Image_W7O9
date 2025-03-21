/*
 * Allwinner SoCs hdmi2.0 driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
 #include "config.h"

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/of_irq.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
//#include <linux/power/scenelock.h>
#if defined(CONFIG_EXTCON)
#include <linux/extcon.h>
#endif

#include "hdmi_tx.h"
#include "hdmi_core/core_hdcp.h"
/*#include "hdmi_test.h"*/

#define DDC_PIN_ACTIVE "ddc_active"
#define DDC_PIN_SLEEP "ddc_sleep"
#define CEC_PIN_ACTIVE "cec_active"
#define CEC_PIN_SLEEP "cec_sleep"

#define ESM_REG_BASE_OFFSET 0x8000
#define HDCP22_FIRMWARE_SIZE	(1024 * 256)
#define HDCP22_DATA_SIZE	(1024 * 128)

static dev_t devid;
static struct cdev *hdmi_cdev;

static struct class *hdmi_class;
static struct device *hdev;

static struct hdmi_tx_drv	*hdmi_drv;

/*0x10: force unplug;
* 0x11: force plug;
* 0x1xx: unreport hpd state
* 0x1xxx: mask hpd
*/
u32 hdmi_hpd_mask_pre;
u32 hdmi_hpd_mask;
static u32 hpd_out_count;
static u32 re_read_edid_delay_count;

u32 hdmi_clk_enable_mask;
static u32 hdmi_pin_config_mask;
u32 hdmi_enable_mask;
u32 hdmi_suspend_mask;
/*static struct scene_lock hdmi_standby_lock;*/

static bool boot_hdmi;
static bool video_on;
static u32 hpd_state;
static u8 hdcp_encrypt_status;

static u8 cec_wakeup;

u32 hdmi_printf;

DEFINE_MUTEX(ddc_analog_lock);

#if defined(CONFIG_EXTCON)
static const unsigned int hdmi_cable[] = {
	EXTCON_DISP_HDMI,
	EXTCON_NONE,
};
static struct extcon_dev *hdmi_extcon_dev;
#endif

static int hdmi_run_thread(void *parg);

static void hdmi_set_ddc_analog(unsigned char set)
{
#ifdef TCON_PAN_SEL
	mutex_lock(&ddc_analog_lock);
	if (set)
		disp_hdmi_pad_sel(1);
	else
		disp_hdmi_pad_release();
	mutex_unlock(&ddc_analog_lock);
#endif
}

#ifdef TCON_PAN_SEL
extern u32 disp_hdmi_pad_get(void);
#endif
unsigned char hdmi_get_ddc_analog(void)
{
#ifdef TCON_PAN_SEL
	return (unsigned char)disp_hdmi_pad_get();
#else
	return 0;
#endif
}

struct hdmi_tx_drv *get_hdmi_drv(void)
{
	return hdmi_drv;
}

u32 get_drv_hpd_state(void)
{
#ifdef __FPGA_PLAT__
    pr_err("%s: force hpd in\n", __func__);
    return 1;
#else
    return hpd_state;
#endif
}

/**
 * @short List of the devices
 * Linked list that contains the installed devices
 */
static LIST_HEAD(devlist_global);

static void hdmi_msleep(unsigned int ms)
{
	set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(msecs_to_jiffies(ms));
}

#if defined(CONFIG_AW_AXP) || defined(CONFIG_REGULATOR)
int hdmi_power_enable(char *name)
{
	struct regulator *regu = NULL;
	int ret = -1;

#ifdef CONFIG_SUNXI_REGULATOR_DT
	regu = regulator_get(&hdmi_drv->pdev->dev, name);
#else
	regu = regulator_get(NULL, name);
#endif
	if (IS_ERR(regu)) {
		pr_err("%s: some error happen, fail to get regulator %s\n", __func__, name);
		goto exit;
	}

	/* enalbe regulator */
	ret = regulator_enable(regu);
	if (ret != 0) {
		pr_err("%s: some error happen, fail to enable regulator %s!\n", __func__, name);
		goto exit1;
	} else {
		VIDEO_INF("suceess to enable regulator %s!\n", name);
	}

exit1:
	/* put regulater, when module exit */
	regulator_put(regu);
exit:
	return ret;
}

int hdmi_power_disable(char *name)
{
	struct regulator *regu = NULL;
	int ret = 0;

#ifdef CONFIG_SUNXI_REGULATOR_DT
	regu = regulator_get(&hdmi_drv->pdev->dev, name);
#else
	regu = regulator_get(NULL, name);
#endif
	if (IS_ERR(regu)) {
		pr_err("%s: some error happen, fail to get regulator %s\n", __func__, name);
		goto exit;
	}

	/*disalbe regulator*/
	ret = regulator_disable(regu);
	if (ret != 0) {
		pr_err("%s: some error happen, fail to disable regulator %s!\n", __func__, name);
		goto exit1;
	} else {
		VIDEO_INF("suceess to disable regulator %s!\n", name);
	}

exit1:
	/*put regulater, when module exit*/
	regulator_put(regu);
exit:
	return ret;
}
#else
int hdmi_power_enable(char *name) {return 0; }
int hdmi_power_disable(char *name) {return 0; }
#endif

static void hdmi_clk_enable(void)
{
	struct clk *clk_parent = NULL;

	if (hdmi_clk_enable_mask)
		return;

	if (hdmi_drv->hdmi_clk != NULL) {
		clk_parent = clk_get(NULL, "tcon_tv");
		if (clk_parent == NULL || IS_ERR(clk_parent))
			pr_err("tcon_tv clk get failed\n");
		else
			clk_set_rate(hdmi_drv->hdmi_clk,
				clk_get_rate(clk_parent));

		if (clk_prepare_enable(hdmi_drv->hdmi_clk) != 0)
			pr_info("hdmi clk enable failed!\n");
	}

	if (hdmi_drv->hdmi_ddc_clk != NULL)
		if (clk_prepare_enable(hdmi_drv->hdmi_ddc_clk) != 0)
			pr_info("hdmi ddc clk enable failed!\n");

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	if (hdmi_drv->hdmi_hdcp_clk)
		clk_set_rate(hdmi_drv->hdmi_hdcp_clk, 300000000);

	if (hdmi_drv->hdmi_hdcp_clk != NULL)
		if (clk_prepare_enable(hdmi_drv->hdmi_hdcp_clk) != 0)
			pr_info("hdmi ddc clk enable failed!\n");
#endif

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if (hdmi_drv->hdmi_cec_clk != NULL) {
		if (clk_prepare_enable(hdmi_drv->hdmi_cec_clk) != 0)
			pr_info("hdmi cec clk enable failed!\n");
	}
#endif

	hdmi_clk_enable_mask = 1;
}

static void hdmi_resume_clk_enable(void)
{
	struct clk *clk_parent = NULL;

	if (hdmi_clk_enable_mask)
		return;

	if (hdmi_drv->hdmi_clk != NULL) {
		clk_parent = clk_get(NULL, "tcon_tv");
		if (clk_parent == NULL || IS_ERR(clk_parent))
			pr_err("tcon_tv clk get failed\n");
		else
			clk_set_rate(hdmi_drv->hdmi_clk,
				clk_get_rate(clk_parent));

		if (clk_prepare_enable(hdmi_drv->hdmi_clk) != 0)
			pr_info("hdmi clk enable failed!\n");
	}

	if ((hdmi_drv->hdmi_ddc_clk != NULL) && (!hdmi_drv->cec_super_standby))
		if (clk_prepare_enable(hdmi_drv->hdmi_ddc_clk) != 0)
			pr_info("hdmi ddc clk enable failed!\n");

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	if (hdmi_drv->hdmi_hdcp_clk)
		clk_set_rate(hdmi_drv->hdmi_hdcp_clk, 300000000);

	if (hdmi_drv->hdmi_hdcp_clk != NULL)
		if (clk_prepare_enable(hdmi_drv->hdmi_hdcp_clk) != 0)
			pr_info("hdmi ddc clk enable failed!\n");
#endif

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if ((hdmi_drv->hdmi_cec_clk != NULL) &&
				(!hdmi_drv->cec_super_standby)) {
		if (clk_prepare_enable(hdmi_drv->hdmi_cec_clk) != 0)
			pr_info("hdmi cec clk enable failed!\n");
	}
#endif

	hdmi_clk_enable_mask = 1;
}


static void hdmi_clk_disable(void)
{
	if (!hdmi_clk_enable_mask)
		return;

	hdmi_clk_enable_mask = 0;

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if (hdmi_drv->hdmi_cec_clk != NULL)
		clk_disable_unprepare(hdmi_drv->hdmi_cec_clk);
#endif

#ifdef CONFIG_HDMI2_HDCP_SUNXI
		if (hdmi_drv->hdmi_hdcp_clk != NULL)
			clk_disable_unprepare(hdmi_drv->hdmi_hdcp_clk);
#endif

	if (hdmi_drv->hdmi_ddc_clk != NULL)
		clk_disable_unprepare(hdmi_drv->hdmi_ddc_clk);

	if (hdmi_drv->hdmi_clk != NULL)
		clk_disable_unprepare(hdmi_drv->hdmi_clk);

}

static void hdmi_suspend_clk_disable(void)
{
	if (!hdmi_clk_enable_mask)
		return;

	hdmi_clk_enable_mask = 0;

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if ((hdmi_drv->hdmi_cec_clk != NULL)
		&& (!hdmi_drv->cec_super_standby))
		clk_disable_unprepare(hdmi_drv->hdmi_cec_clk);
#endif

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	if (hdmi_drv->hdmi_hdcp_clk != NULL)
		clk_disable_unprepare(hdmi_drv->hdmi_hdcp_clk);
#endif

	if ((hdmi_drv->hdmi_ddc_clk != NULL)
		&& (!hdmi_drv->cec_super_standby))
		clk_disable_unprepare(hdmi_drv->hdmi_ddc_clk);
	if (hdmi_drv->hdmi_clk != NULL)
		clk_disable_unprepare(hdmi_drv->hdmi_clk);
}

static void hdmi_pin_configure(void)
{
	s32 ret = 0;
	struct pinctrl_state *state;
#ifdef CONFIG_HDMI2_CEC_SUNXI
	struct pinctrl_state *cec_state;
#endif
	if (hdmi_pin_config_mask)
		return;

	/*pin configuration for ddc*/
	if (!IS_ERR(hdmi_drv->pctl)) {
		state = pinctrl_lookup_state(hdmi_drv->pctl, DDC_PIN_ACTIVE);
		if (IS_ERR(state)) {
			pr_info("pinctrl_lookup_state for HDMI2.0 SCL fail\n");
			return;
		}

		ret = pinctrl_select_state(hdmi_drv->pctl, state);
		if (ret < 0) {
			pr_info("pinctrl_select_state for HDMI2.0 DDC fail\n");
			return;
		}

#ifdef CONFIG_HDMI2_CEC_SUNXI
		cec_state = pinctrl_lookup_state(hdmi_drv->pctl, CEC_PIN_ACTIVE);
		if (IS_ERR(state)) {
			pr_info("pinctrl_lookup_state for HDMI2.0 CEC active fail\n");
			return;
		}

		ret = pinctrl_select_state(hdmi_drv->pctl, cec_state);
		if (ret < 0) {
			pr_info("pinctrl_select_state for HDMI2.0 CEC active fail\n");
			return;
		}
#endif

	}

	hdmi_pin_config_mask = 1;
}

static void hdmi_resume_pin_configure(void)
{
	s32 ret = 0;
	struct pinctrl_state *state;
#ifdef CONFIG_HDMI2_CEC_SUNXI
	struct pinctrl_state *cec_state;
#endif

	if (hdmi_pin_config_mask)
		return;

	/*pin configuration for ddc*/
	if (!IS_ERR(hdmi_drv->pctl)) {
		state = pinctrl_lookup_state(hdmi_drv->pctl, DDC_PIN_ACTIVE);
		if (IS_ERR(state)) {
			pr_info("pinctrl_lookup_state for HDMI2.0 SCL fail\n");
			return;
		}

		ret = pinctrl_select_state(hdmi_drv->pctl, state);
		if (ret < 0) {
			pr_info("pinctrl_select_state for HDMI2.0 DDC fail\n");
			return;
		}

#ifdef CONFIG_HDMI2_CEC_SUNXI
		if (!hdmi_drv->cec_super_standby) {
			cec_state = pinctrl_lookup_state(hdmi_drv->pctl, CEC_PIN_ACTIVE);
			if (IS_ERR(state)) {
				pr_info("pinctrl_lookup_state for HDMI2.0 CEC active fail\n");
				return;
			}

			ret = pinctrl_select_state(hdmi_drv->pctl, cec_state);
			if (ret < 0) {
				pr_info("pinctrl_select_state for HDMI2.0 CEC active fail\n");
				return;
			}
		}
#endif
	}

	hdmi_pin_config_mask = 1;
}


static void hdmi_pin_release(void)
{
	s32 ret = 0;
	struct pinctrl_state *state;

	if (!hdmi_pin_config_mask)
		return;

	/*pin configuration for ddc*/
	if (!IS_ERR(hdmi_drv->pctl)) {
		state = pinctrl_lookup_state(hdmi_drv->pctl, DDC_PIN_SLEEP);
		if (IS_ERR(state)) {
			pr_info("pinctrl_lookup_state for HDMI2.0 SCL fail\n");
			return;
		}

		ret = pinctrl_select_state(hdmi_drv->pctl, state);
		if (ret < 0) {
			pr_info("pinctrl_select_state for HDMI2.0 DDC fail\n");
			return;
		}

#ifdef CONFIG_HDMI2_CEC_SUNXI
		state = pinctrl_lookup_state(hdmi_drv->pctl, CEC_PIN_SLEEP);
		if (IS_ERR(state)) {
			pr_info("pinctrl_lookup_state for HDMI2.0 CEC SLEEP fail\n");
			return;
		}

		ret = pinctrl_select_state(hdmi_drv->pctl, state);
		if (ret < 0) {
			pr_info("pinctrl_select_state for HDMI2.0 CEC SLEEP fail\n");
			return;
		}
#endif
	}

	hdmi_pin_config_mask = 0;
}

static void hdmi_suspend_pin_release(void)
{
	s32 ret = 0;
	struct pinctrl_state *state;

	LOG_TRACE();
	if (!hdmi_pin_config_mask)
		return;

	/*pin configuration for ddc*/
	if (!IS_ERR(hdmi_drv->pctl)) {
		state = pinctrl_lookup_state(hdmi_drv->pctl, DDC_PIN_SLEEP);
		if (IS_ERR(state)) {
			pr_info("pinctrl_lookup_state for HDMI2.0 SCL fail\n");
			return;
		}

		ret = pinctrl_select_state(hdmi_drv->pctl, state);
		if (ret < 0) {
			pr_info("pinctrl_select_state for HDMI2.0 DDC fail\n");
			return;
		}

#ifdef CONFIG_HDMI2_CEC_SUNXI
		if (!hdmi_drv->cec_super_standby) {
			state = pinctrl_lookup_state(hdmi_drv->pctl, CEC_PIN_SLEEP);
			if (IS_ERR(state)) {
				pr_info("pinctrl_lookup_state for HDMI2.0 CEC SLEEP fail\n");
				return;
			}

			ret = pinctrl_select_state(hdmi_drv->pctl, state);
			if (ret < 0) {
				pr_info("pinctrl_select_state for HDMI2.0 CEC SLEEP fail\n");
				return;
			}
		}
#endif
	}

	hdmi_pin_config_mask = 0;
}


static void hdmi_sys_source_configure(void)
{
	LOG_TRACE();
	hdmi_clk_enable();
	hdmi_pin_configure();
}

static void hdmi_resume_sys_source_configure(void)
{
	LOG_TRACE();
	hdmi_resume_clk_enable();
	hdmi_resume_pin_configure();
}

static void hdmi_sys_source_release(void)
{
	LOG_TRACE();
	hdmi_clk_disable();
	hdmi_pin_release();
}

static void hdmi_suspend_sys_source_release(void)
{
	LOG_TRACE();
	hdmi_suspend_clk_disable();
	hdmi_suspend_pin_release();
}

static void hdmi_clk_reset(void)
{
	hdmi_clk_disable();
	udelay(10);
	hdmi_clk_enable();

	hdmi_clk_disable();
	udelay(10);
	hdmi_clk_enable();
}

/*static void hdmi_sys_source_reset(void)
{
	hdmi_sys_source_release();
	mdelay(10);
	hdmi_sys_source_configure();
}*/

#ifdef CONFIG_HDMI2_HDCP_SUNXI
static void set_hdcp_status(u8 status)
{
	hdcp_encrypt_status = status;
}
#endif

#ifdef CONFIG_HDMI2_FREQ_SPREAD_SPECTRUM
extern u32 hdmi_set_spread_spectrum(u32 pixel_clk);
#endif

static s32 hdmi_enable(void)
{
	s32 ret = 0;
	struct clk *clk_parent = NULL;
#ifdef CONFIG_HDMI2_FREQ_SPREAD_SPECTRUM
	struct disp_video_timings *video_info;
	u32 clk_rate = 0;
	videoParams_t *pVideo = &hdmi_drv->hdmi_core->mode.pVideo;
#endif

	LOG_TRACE();

	mutex_lock(&hdmi_drv->ctrl_mutex);

	if (hdmi_enable_mask == 1) {
		mutex_unlock(&hdmi_drv->ctrl_mutex);
		return 0;
	}

	if (hpd_state && (!video_on)) {
		if (hdmi_drv->hdmi_clk != NULL) {
			clk_parent = clk_get(NULL, "tcon_tv");
			if (clk_parent == NULL || IS_ERR(clk_parent))
				pr_err("tcon_tv clk get failed\n");
			else
				clk_set_rate(hdmi_drv->hdmi_clk,
					clk_get_rate(clk_parent));
#ifdef CONFIG_HDMI2_FREQ_SPREAD_SPECTRUM
			hdmi_get_video_timming_info(&video_info);
			clk_rate = video_info->pixel_clk
				* (video_info->pixel_repeat + 1);

			if (pVideo->mEncodingOut == YCC420)
				clk_rate /= 2;
			hdmi_set_spread_spectrum(clk_rate);
#endif
		}

		hdmi_set_ddc_analog(1);

		ret = hdmi_enable_core();
		video_on = true;
	}

	hdmi_enable_mask = 1;

	mutex_unlock(&hdmi_drv->ctrl_mutex);
	return ret;
}

static s32 hdmi_smooth_enable(void)
{
	s32 ret = 0;

	LOG_TRACE();

	mutex_lock(&hdmi_drv->ctrl_mutex);

	if (hpd_state && video_on)
		ret = hdmi_smooth_enable_core();

	mutex_unlock(&hdmi_drv->ctrl_mutex);

	return ret;
}

static s32 hdmi_disable(void)
{
	s32 ret;

	LOG_TRACE();

	mutex_lock(&hdmi_drv->ctrl_mutex);
	if (hdmi_enable_mask == 0) {
		mutex_unlock(&hdmi_drv->ctrl_mutex);
		return 0;
	}

	ret = hdmi_disable_core();

	hdmi_set_ddc_analog(0);

	video_on = false;
	hdmi_enable_mask = 0;

	mutex_unlock(&hdmi_drv->ctrl_mutex);
	return ret;
}

#if defined(CONFIG_SND_SUNXI_SOC_SUNXI_HDMIAUDIO)
s32 hdmi_audio_enable(u8 enable, u8 channel)
{
	s32 ret = 0;

	ret = hdmi_core_audio_enable(enable, channel);

	return ret;
}
#endif

static s32 hdmi_suspend(void)
{
	int i = 0;

	LOG_TRACE();

	if (hdmi_suspend_mask)
		return 0;

	if (hdmi_drv->hdmi_task) {
		kthread_stop(hdmi_drv->hdmi_task);
		hdmi_drv->hdmi_task = NULL;
	}

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if (hdmi_drv->cec_support) {
		cec_thread_exit();
#ifndef CONFIG_HDMI2_CEC_USER
		hdmi_cec_send_inactive_source();
		/*hdmi_cec_set_phyaddr_to_cpus();*/
#endif
		if (!hdmi_drv->cec_super_standby)
			hdmi_cec_enable(0);
		else
			hdmi_cec_soft_disable();
	}
#endif

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	if (hdmi_drv->hdmi_core->mode.pHdcp.hdcp_on)
		set_hdcp_status(HDCP_ING);
	if (hdmi_drv->hdmi_core->mode.pHdcp.use_hdcp)
		hdmi_drv->hdmi_core->dev_func.hdcp_close();
#endif

	hdmi_suspend_sys_source_release();
	if (hdmi_drv->cec_support
		&& hdmi_drv->cec_super_standby) {
		/*enable_wakeup_src(CPUS_HDMICEC_SRC, 0);
		scene_lock(&hdmi_standby_lock);*/
	}

	for (i = 0; i < hdmi_drv->power_count; i++)
		hdmi_power_disable(hdmi_drv->power[i]);

	hdmi_set_ddc_analog(0);

	mutex_lock(&hdmi_drv->ctrl_mutex);
	hdmi_suspend_mask = 1;
	cec_wakeup = 0;
	mutex_unlock(&hdmi_drv->ctrl_mutex);
	return 0;
}

/*extern int sunxi_smc_refresh_hdcp(void);*/
static s32 hdmi_resume(void)
{
	s32 ret = 0, i = 0;

	LOG_TRACE();
#ifdef CONFIG_SUNXI_SMC
	if (sunxi_smc_refresh_hdcp())
		pr_err("refresh hdcp key failed!!!\n");
#endif
	mutex_lock(&hdmi_drv->ctrl_mutex);
	if (hdmi_suspend_mask == 0) {
		mutex_unlock(&hdmi_drv->ctrl_mutex);
		return 0;
	}

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if (hdmi_drv->cec_support
		&& hdmi_drv->cec_super_standby) {
		/*scene_unlock(&hdmi_standby_lock);
		disable_wakeup_src(CPUS_HDMICEC_SRC, 0);*/
	}
#endif

	hdmi_set_ddc_analog(1);

	for (i = 0; i < hdmi_drv->power_count; i++)
		hdmi_power_enable(hdmi_drv->power[i]);
	hdmi_resume_sys_source_configure();
	hdmi_clk_reset();

	/*enable hpd sense*/
	hpd_sense_enbale_core(hdmi_drv->hdmi_core);

	hpd_state = 0;
#if defined(CONFIG_EXTCON)
	if (!(hdmi_hpd_mask & 0x100))
		extcon_set_cable_state_(hdmi_extcon_dev,
				EXTCON_DISP_HDMI,
				hpd_state ? STATUE_OPEN : STATUE_CLOSE);
#endif

	hdmi_drv->hdmi_task = kthread_create(hdmi_run_thread, (void *)0, "hdmi proc");
	if (IS_ERR(hdmi_drv->hdmi_task)) {
		pr_info("Unable to start kernel thread %s.\n\n", "hdmi proc");
		hdmi_drv->hdmi_task = NULL;
	}
	wake_up_process(hdmi_drv->hdmi_task);

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if (hdmi_drv->cec_support) {
		cec_thread_init(hdmi_drv->parent_dev);
		hdmi_cec_enable(1);
#ifndef CONFIG_HDMI2_CEC_USER
		if (!cec_wakeup) {
			hdmi_cec_wakup_request();
			cec_wakeup = 1;
		}
#endif
	}
#endif

	hdmi_suspend_mask = 0;
	mutex_unlock(&hdmi_drv->ctrl_mutex);

	return ret;
}

static void edid_check(void)
{
	if (!hdmi_drv->hdmi_core->mode.edid_done) {
		hdmi_edid_release();
		mutex_lock(&hdmi_drv->hdcp_mutex);
		edid_read_cap();
		mutex_unlock(&hdmi_drv->hdcp_mutex);
#if defined(CONFIG_EXTCON)
		if (!(hdmi_hpd_mask & 0x100))
			extcon_set_cable_state_(hdmi_extcon_dev,
						EXTCON_DISP_HDMI,
						STATUE_OPEN);
#endif
		edid_correct_hardware_config();
	}
}


static void hdmi_configure(struct hdmi_tx_drv *drv)
{
#ifdef __FPGA_PLAT__
    /*FPGA PHY Config*/
    resistor_calibration_core(hdmi_drv->hdmi_core, 0x10000, 0x0001);
    resistor_calibration_core(hdmi_drv->hdmi_core, 0x10004, 0x03e0);
#else
	resistor_calibration_core(drv->hdmi_core, 0x10004, 0x80c00000);
#endif
    hdmi_configure_core(drv->hdmi_core);
}

/*sys_config.fex setting for hpd*/
static void hdmi_hpd_sys_config_set(void)
{
	/*To enhance the ddc ability*/
	if (hdmi_drv->ddc_ctrl_en == 1) {
		if (gpio_direction_output(hdmi_drv->ddc_ctrl.gpio, 1) != 0) {
			pr_info("ddc ctrl gpio set 1 error!\n");
			return;
		}
	}
}

/*release sys_config.fex setting for hpd*/
static void hdmi_hpd_sys_config_release(void)
{
	pr_info("%s\n", __func__);
	if (hdmi_drv->ddc_ctrl_en == 1) {
		if (gpio_direction_output(hdmi_drv->ddc_ctrl.gpio, 0) != 0) {
			pr_info("ddc ctrl gpio set 0 error!\n");
			return;
		}
	}
}

static void hdmi_plugin_proc(void)
{
	LOG_TRACE();
	pr_info("HDMI cable is connected\n");
	if (!(hdmi_hpd_mask & 0x10))
		hpd_state = 1;

	hdmi_set_ddc_analog(1);
	hdmi_hpd_sys_config_set();
	mutex_lock(&hdmi_drv->hdcp_mutex);
	edid_read_cap();
	mutex_unlock(&hdmi_drv->hdcp_mutex);

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if (hdmi_drv->cec_support) {
		hdmi_cec_enable(1);
#ifndef CONFIG_HDMI2_CEC_USER
		if (!cec_wakeup) {
			hdmi_cec_wakup_request();
			cec_wakeup = 1;
		}
#endif
	}
#endif

	if (hdmi_drv->hdmi_core->mode.edid_done)
		edid_set_video_prefered_core();/*update some configs by edid*/
	mutex_lock(&hdmi_drv->ctrl_mutex);
	if ((!video_on) && (hdmi_enable_mask)) {
		hdmi_configure(hdmi_drv);
		video_on = true;
	}
	mutex_unlock(&hdmi_drv->ctrl_mutex);
#if defined(CONFIG_EXTCON)
	if ((!(hdmi_hpd_mask & 0x100)) && hdmi_drv->hdmi_core->mode.edid_done)
		extcon_set_cable_state_(hdmi_extcon_dev,
					EXTCON_DISP_HDMI,
					STATUE_OPEN);
#endif
	edid_correct_hardware_config();

#ifdef CONFIG_HDMI2_CEC_SUNXI
#ifndef CONFIG_HDMI2_CEC_USER
	hdmi_cec_send_active_source();
#endif
#endif
}

static void hdmi_plugout_proc(void)
{
	LOG_TRACE();
	pr_info("HDMI cable is disconnected\n");
	if (!(hdmi_hpd_mask & 0x10))
		hpd_state = 0;
	video_on = false;

#ifdef CONFIG_HDMI2_CEC_SUNXI
	cec_wakeup = 0;
	if (hdmi_drv->cec_support
		&& !hdmi_core_get_rxsense_state())
		hdmi_cec_enable(0);
#endif

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	if (hdmi_drv->hdmi_core->mode.pHdcp.hdcp_on) {
		hdmi_drv->hdmi_core->dev_func.hdcp_disconfigure();
		set_hdcp_status(HDCP_ING);
	}
#endif

	hdmi_hpd_out_core_process(hdmi_drv->hdmi_core);
	hdmi_clk_reset();
	hpd_sense_enbale_core(hdmi_drv->hdmi_core);

	hdmi_edid_release();
	hdmi_set_ddc_analog(0);

#if defined(CONFIG_EXTCON)
	if (!(hdmi_hpd_mask & 0x100))
		extcon_set_cable_state_(hdmi_extcon_dev, EXTCON_DISP_HDMI, STATUE_CLOSE);
#endif
}

/*
 * hpd_mask:
 * 1. 0x1x: force to plug in or out
 * 0x10-force to hotplug_out  0x11-force to hotplug_in
 *
 * 2. 0x1xx: do NOT report plug event to user-space
 * 0x110: force to hotplug_out and do NOT report plug event to user-space
 * 0x111: force to hotplug_in and do NOT report plug event to user-space
 *
 * 3. 0x1xxx: disable plugin or out
 *  0x1000: disable plugin or out
 */
static void hdmi_hpd_mask_process(void)
{
	if (hdmi_hpd_mask == 0x10
		|| hdmi_hpd_mask == 0x110
		|| hdmi_hpd_mask == 0x1010) {
		hdmi_plugout_proc();
	} else if (hdmi_hpd_mask == 0x11
		|| hdmi_hpd_mask == 0x111
		|| hdmi_hpd_mask == 0x1011) {
		msleep(400);
		hdmi_plugin_proc();
	} else {
		pr_info("Unknow hpd event\n");
	}

	/*hdmi_hpd_mask &= 0x0f0f;*/
}

static int hdmi_run_thread(void *parg)
{
	u32 hpd_state_now = 0;
	u32 i;
#ifdef CONFIG_HDMI2_HDCP_SUNXI
	u32 hdcp_status = 0;
#endif

	while (1) {
		if (kthread_should_stop())
			break;

		if (hdmi_hpd_mask & 0x1000) {
			hdmi_msleep(200);
			continue;
		}

		if (hdmi_hpd_mask & 0x10) {
			if (hdmi_hpd_mask != hdmi_hpd_mask_pre) {
				hdmi_hpd_mask_pre = hdmi_hpd_mask;
				hdmi_hpd_mask_process();
			}
			hdmi_msleep(80);
			continue;
		}


		if (!hdmi_clk_enable_mask) {
			hdmi_msleep(80);
			continue;
		}

		hpd_state_now = hdmi_core_get_hpd_state();
		if (hpd_state_now != hpd_state) {
			/*HPD Event Happen*/
			if (!hpd_state_now) {
				re_read_edid_delay_count = 20;
				hdmi_plugout_proc();
				hpd_out_count = 0;
			} else {
				for (i = 0; i < 2; i++) {
					hdmi_msleep(200);
					hpd_state_now = hdmi_core_get_hpd_state();
					if (hpd_state_now == hpd_state)
						break;/*it's not a real hpd event*/
				}

				if (i >= 2) {
					hpd_out_count = 41;
					/*it's a real hpd event*/
					hdmi_plugin_proc();
					re_read_edid_delay_count = 0;
				}
			}
		} else {
#ifdef CONFIG_HDMI2_HDCP_SUNXI
			if (hpd_state_now && (hpd_state || (hdmi_hpd_mask == 0x1)) &&
					hdmi_drv->hdmi_core->mode.pHdcp.hdcp_on &&
					hdmi_drv->hdmi_core->mode.pHdcp.use_hdcp) {
				mutex_lock(&hdmi_drv->hdcp_mutex);
				hdcp_status = get_hdcp_status_core();
				if (hdcp_status == 0)
					set_hdcp_status(HDCP_SUCCESS);
				else if (hdcp_status == -1)
					set_hdcp_status(HDCP_FAILED);
				else if (hdcp_status == 1)
					set_hdcp_status(HDCP_ING);
				else
					;/*pr_info("Error: Unkown hdcp status\n");*/
				mutex_unlock(&hdmi_drv->hdcp_mutex);
			}
#endif
		}

		hdmi_msleep(80);

		hpd_out_count++;
		if (hpd_out_count == 0)
			hpd_out_count = 41;
		if ((!hpd_state) && (hpd_out_count == 40))
			hdmi_hpd_sys_config_release();

		re_read_edid_delay_count++;
		if (re_read_edid_delay_count == 0)
			re_read_edid_delay_count = 20;
		if (hpd_state && (re_read_edid_delay_count == 19))
			edid_check();
	}

	return 0;
}

#ifdef __FPGA_PLAT__
static void hdmi_fpga_config(void)
{
	unsigned int __iomem *pin_addr;

	/*FPGA PHY Config*/
	resistor_calibration_core(hdmi_drv->hdmi_core, 0x10000, 0x0001);
	resistor_calibration_core(hdmi_drv->hdmi_core, 0x10004, 0x03e0);

	/*FPGA CEC GPIO Config*/
	pin_addr = ioremap(0x0300b024, 4);
	writel(0x400055, (void __iomem *)(pin_addr));

	/*FPGA DDC GPIO Config*/
	pin_addr = ioremap(0x0300b030, 4);
	writel(0x55777777, pin_addr);
}
#endif

static bool disp_is_hdmi_boot(void)
{
	unsigned int value;
	unsigned int output_type0, output_mode0;
	unsigned int output_type1, output_mode1;

	/*Read video booting params from disp device tree*/
	value = disp_boot_para_parse("boot_disp");

	/*To check if hdmi has been configured in uboot*/
	output_type0 = (value >> 8) & 0xff;
	output_mode0 = (value) & 0xff;
	output_type1 = (value >> 24) & 0xff;
	output_mode1 = (value >> 16) & 0xff;
	if ((output_type0 == DISP_OUTPUT_TYPE_HDMI) ||
		(output_type1 == DISP_OUTPUT_TYPE_HDMI))
		return true;
	else
		return false;
}

static int hdmi_dts_parse_basic_info(struct platform_device *pdev)
{
	/* iomap */
	uintptr_t reg_base = 0x0;
	int ret = 0;

	reg_base = (uintptr_t __force)of_iomap(pdev->dev.of_node, 0);
	if (reg_base == 0) {
		pr_err("unable to map hdmi registers\n");
		ret = -EINVAL;
	}
	hdmi_drv->reg_base = reg_base;

	/*get cts config*/
	if (of_property_read_u32(pdev->dev.of_node,
				       "hdmi_cts_compatibility",
				       &hdmi_drv->is_cts))
		pr_err("ERROR: can not get hdmi_cts_compatibility\n");
	hdmi_drv->hdmi_core->is_cts = hdmi_drv->is_cts;

	return ret;
}

#ifdef CONFIG_HDMI2_CEC_SUNXI
static void hdmi_dts_parse_cec(struct platform_device *pdev)
{
	/*get cec config*/
	if (of_property_read_u32(pdev->dev.of_node,
						   "hdmi_cec_support",
						   &hdmi_drv->cec_support))
			pr_err("ERROR: can not get hdmi_cec_support node\n");

	if (of_property_read_u32(pdev->dev.of_node,
					   "hdmi_cec_super_standby",
					   &hdmi_drv->cec_super_standby))
		pr_err("ERROR: can not get hdmi_cec_super_standby node\n");
	hdmi_drv->hdmi_core->cec_super_standby = hdmi_drv->cec_super_standby;
}
#endif

#ifdef CONFIG_HDMI2_HDCP_SUNXI
static int hdmi_dts_parse_hdcp(struct platform_device *pdev,
						hdcpParams_t *hdcp)
{
#ifdef CONFIG_HDMI2_HDCP22_SUNXI
	struct device_node *esm_np;
	u32 dts_esm_buff_phy_addr = 0, dts_esm_size_phy_addr = 0;
	void *dts_esm_buff_vir_addr = NULL, *dts_esm_size_vir_addr = NULL;
#endif

	/*get hdcp configs*/
	if (of_property_read_u32_array(pdev->dev.of_node, "hdmi_hdcp_enable",
			(u32 *)&hdcp->use_hdcp, 1))
			pr_info("ERROR: can not get hdmi_hdcp_enable\n");
	else
		hdmi_drv->support_hdcp = hdcp->use_hdcp;
#ifdef CONFIG_HDMI2_HDCP22_SUNXI
	if (of_property_read_u32_array(pdev->dev.of_node, "hdmi_hdcp22_enable",
			(u32 *)&hdcp->use_hdcp22, 1))
			pr_info("ERROR: can not get hdmi_hdcp22_enable\n");

	esm_np = of_find_node_by_name(NULL, "esm");
	if (esm_np == NULL) {
		pr_info("Can not find the node of esm\n");
	} else if (hdcp->use_hdcp22) {
		if (!of_property_read_u32_array(esm_np,
						"esm_img_size_addr",
						&dts_esm_size_phy_addr,
						1)) {
			/*obtain esm firmware size*/
			dts_esm_size_vir_addr = __va(dts_esm_size_phy_addr);
			memcpy((void *)(&hdcp->esm_firm_size), dts_esm_size_vir_addr, 4);

			if (!of_property_read_u32_array(esm_np,
							"esm_img_buff_addr",
							&dts_esm_buff_phy_addr,
							1)) {
				/*obtain esm firmware*/
				dts_esm_buff_vir_addr = __va(dts_esm_buff_phy_addr);
				if (hdcp->esm_firm_size <= HDCP22_FIRMWARE_SIZE)
					memcpy((void *)hdcp->esm_firm_vir_addr,
						dts_esm_buff_vir_addr,
						hdcp->esm_firm_size);
				else
					pr_info("WARN: esm_firm_size is too big\n");
			} else {
				pr_info("ERROR: Can not read esm_img_buff_addr\n");
			}
		} else {
			pr_info("ERROR: Can not read esm_img_size_addr\n");
		}
	}
#endif
	return 0;
}

#ifdef CONFIG_HDMI2_HDCP22_SUNXI
static char *esm_firm_vir_addr;
static u32 esm_firm_size;
#endif

static int hdmi_tx_hdcp_init(struct platform_device *pdev,
						hdcpParams_t *hdcp)
{
#ifdef CONFIG_HDMI2_HDCP22_SUNXI
	hdcp->esm_firm_vir_addr =  (unsigned long)dma_alloc_coherent(&pdev->dev,
					HDCP22_FIRMWARE_SIZE,
					&hdcp->esm_firm_phy_addr,
					GFP_KERNEL | __GFP_ZERO);
	hdcp->esm_firm_size = HDCP22_FIRMWARE_SIZE;

	esm_firm_vir_addr = (char *)hdcp->esm_firm_vir_addr;
	esm_firm_size = hdcp->esm_firm_size;

	hdcp->esm_data_vir_addr =  (unsigned long)dma_alloc_coherent(&pdev->dev,
					HDCP22_DATA_SIZE,
					&hdcp->esm_data_phy_addr,
					GFP_KERNEL | __GFP_ZERO);
	hdcp->esm_data_size = HDCP22_DATA_SIZE;

	hdcp->esm_hpi_base = hdmi_drv->reg_base + ESM_REG_BASE_OFFSET;

	hdcp->esm_firm_phy_addr -= 0x40000000;
	hdcp->esm_data_phy_addr -= 0x40000000;
#endif
	return 0;
}
#endif

static void hdmi_dts_parse_pin_config(struct platform_device *pdev)
{
	/*Get DCC GPIO*/
	hdmi_drv->pctl = pinctrl_get(&pdev->dev);
	if (IS_ERR(hdmi_drv->pctl))
		pr_err("ERROR: pinctrl_get for HDMI2.0 DDC fail\n");


	/*get ddc control gpio enable config*/
	if (of_property_read_u32(pdev->dev.of_node,
				       "ddc_en_io_ctrl",
				       &hdmi_drv->ddc_ctrl_en))
		pr_info("ERROR: can not get ddc_en_io_ctrl\n");

	if (hdmi_drv->ddc_ctrl_en) {
		hdmi_drv->ddc_ctrl.gpio =
			of_get_named_gpio_flags(pdev->dev.of_node,
				"ddc_io_ctrl", 0,
				(enum of_gpio_flags *)(&(hdmi_drv->ddc_ctrl)));

		if (gpio_request(hdmi_drv->ddc_ctrl.gpio, NULL) != 0) {
			pr_info("ddc ctrl gpio_request is failed\n");
			return;
		}
	}
}

static int hdmi_dts_parse_clk(struct platform_device *pdev)
{
	int index = 0;

	hdmi_drv->hdmi_clk = of_clk_get(pdev->dev.of_node, index);
	if (IS_ERR(hdmi_drv->hdmi_clk)) {
		dev_err(&pdev->dev, "fail to get clk for hdmi\n");
		return -1;
	}

	index++;
	hdmi_drv->hdmi_ddc_clk = of_clk_get(pdev->dev.of_node, index);
	if (IS_ERR(hdmi_drv->hdmi_ddc_clk)) {
		dev_err(&pdev->dev, "fail to get clk for hdmi ddc\n");
		return -1;
	}

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	if (hdmi_drv->support_hdcp) {
		index++;
		hdmi_drv->hdmi_hdcp_clk = of_clk_get(pdev->dev.of_node, index);
		if (IS_ERR(hdmi_drv->hdmi_hdcp_clk)) {
			dev_err(&pdev->dev, "fail to get clk for hdmi hdcp\n");
			/*return -1;*/
		}
	}
#endif

#ifdef CONFIG_HDMI2_CEC_SUNXI
	if (hdmi_drv->cec_support) {
		index++;
		hdmi_drv->hdmi_cec_clk = of_clk_get(pdev->dev.of_node, index);
		if (IS_ERR(hdmi_drv->hdmi_cec_clk)) {
			dev_err(&pdev->dev, "fail to get clk for hdmi cec\n");
			/*return -1;*/
		}
	}
#endif

	return 0;
}

static int hdmi_dts_parse_power(struct platform_device *pdev)
{
	int ret = 0, i;
	const char *hdmi_power;
	char power_name[20];

	if (of_property_read_u32(pdev->dev.of_node,
						   "hdmi_power_cnt",
						   &hdmi_drv->power_count)) {
		pr_err("ERROR: can not get hdmi_power_cnt\n");
		return -1;
	}

	for (i = 0; i < hdmi_drv->power_count; i++) {
		sprintf(power_name, "hdmi_power%d", i);
		if (of_property_read_string(pdev->dev.of_node,
					 power_name, &hdmi_power)) {
			pr_err("Error: get %s failed\n", power_name);
			ret = -1;
		} else {
			pr_info("Get hdmi_power%d:%s\n", i, hdmi_power);
			memcpy((void *)hdmi_drv->power[i], hdmi_power,
					strlen(hdmi_power) + 1);
			hdmi_power_enable(hdmi_drv->power[i]);
		}
	}

	return ret;
}

static void drv_global_value_init(void)
{
	hdmi_printf = 0;
	hdmi_hpd_mask_pre = 0;
	hdmi_hpd_mask = 0;
	hpd_out_count = 0;
	re_read_edid_delay_count = 0;
	hdmi_clk_enable_mask = 0;
	hdmi_pin_config_mask = 0;
	hdmi_enable_mask = 0;
	hdmi_suspend_mask = 0;

	boot_hdmi = false;
	video_on = false;
	hpd_state = 0;
	hdcp_encrypt_status = HDCP_DISABLE;
}

static int hdmi_tx_init(struct platform_device *pdev)
{
	int ret = 0;
	int phy_model = 301;
	struct disp_device_func disp_func;
	hdcpParams_t hdcp;

#if defined(CONFIG_SND_SUNXI_SOC_SUNXI_HDMIAUDIO)
	__audio_hdmi_func audio_func;
#if defined(CONFIG_SND_SUNXI_SOC_AUDIOHUB_INTERFACE)
	__audio_hdmi_func audio_func_muti;
#endif
#endif

	LOG_TRACE();

	pr_info("HDMI 2.0 driver init start!\n");
	drv_global_value_init();

	hdmi_drv = kzalloc(sizeof(struct hdmi_tx_drv), GFP_KERNEL);
	if (!hdmi_drv) {
		pr_err("%s:Could not allocated hdmi_tx_dev\n", FUNC_NAME);
		return -1;
	}

	hdmi_drv->hdmi_core = kzalloc(sizeof(struct hdmi_tx_core), GFP_KERNEL);
	if (!hdmi_drv->hdmi_core) {
		pr_err("%s:Could not allocated hdmi_tx_core\n", __func__);
		goto free_mem;
	}

	hdmi_drv->hdmi_core->blacklist_sink = -1;

	hdmi_drv->pdev = pdev;
	hdmi_drv->parent_dev = &pdev->dev;

	boot_hdmi = disp_is_hdmi_boot();
	pr_info("boot_hdmi=%s\n", boot_hdmi ? "true" : "false");

	if (hdmi_dts_parse_basic_info(pdev))
		goto free_mem;

	hdmi_core_set_base_addr(hdmi_drv->reg_base);

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	hdmi_tx_hdcp_init(pdev, &hdcp);
#endif

#ifdef CONFIG_HDMI2_CEC_SUNXI
	hdmi_dts_parse_cec(pdev);
#endif
#ifdef CONFIG_HDMI2_HDCP_SUNXI
	hdmi_dts_parse_hdcp(pdev, &hdcp);
#endif

	hdmi_dts_parse_pin_config(pdev);
	if (hdmi_dts_parse_clk(pdev))
		goto free_mem;
	hdmi_dts_parse_power(pdev);

#if defined(CONFIG_EXTCON)
	hdmi_extcon_dev = devm_extcon_dev_allocate(&pdev->dev, hdmi_cable);
	hdmi_extcon_dev->name = "hdmi";
	devm_extcon_dev_register(&pdev->dev, hdmi_extcon_dev);
#endif

	/*Check if hdmi has been configured during booting*/
	if (boot_hdmi) {
		VIDEO_INF("uboot has configured hdmi\n");
		video_on = true;
		hdmi_pin_config_mask = 1;
		hdmi_enable_mask = 1;
#if !defined(CONFIG_COMMON_CLK_ENABLE_SYNCBOOT)
		hdmi_clk_enable_mask = 0;
		hdmi_clk_enable();
		hdmi_clk_enable_mask = 1;
#endif
	} else {
		VIDEO_INF("uboot has not configured hdmi\n");
		video_on = false;
		hdmi_clk_enable_mask = 0;
		hdmi_pin_config_mask = 0;
		hdmi_sys_source_configure();
	}

	/*Init hdmi core and core params*/
	if (hdmi_tx_core_init(hdmi_drv->hdmi_core,
				phy_model, NULL,
				NULL, &hdcp)) {
		pr_err("Application init failed\n");
		goto free_mem;
	}

#ifdef __FPGA_PLAT__
	hdmi_fpga_config();
#else
	resistor_calibration_core(hdmi_drv->hdmi_core, 0x10004, 0x80c00000);
#endif

	if (!boot_hdmi)
		/*enable hpd sense*/
		hpd_sense_enbale_core(hdmi_drv->hdmi_core);

#if defined(CONFIG_EXTCON)
	if (!(hdmi_hpd_mask & 0x100))
		extcon_set_cable_state_(hdmi_extcon_dev,
						EXTCON_DISP_HDMI, hpd_state ?
						STATUE_OPEN : STATUE_CLOSE);
#endif

	hdmi_set_ddc_analog(1);

	/*Now that everything is fine, let's add it to device list*/
	list_add_tail(&hdmi_drv->devlist, &devlist_global);

	mutex_init(&hdmi_drv->ctrl_mutex);
	mutex_init(&hdmi_drv->hdcp_mutex);

	/*if (hdmi_drv->cec_super_standby)
		scene_lock_init(&hdmi_standby_lock, SCENE_HDMI_CEC_STANDBY, "hdmi_cec_standby");*/
#ifdef CONFIG_HDMI2_CEC_SUNXI
	hdmi_cec_init();
#endif

	/*Create hdmi thread to poll hpd and hdcp status and handle hdcp and hpd event*/
	hdmi_drv->hdmi_task = kthread_create(hdmi_run_thread, (void *)0, "hdmi proc");
	if (IS_ERR(hdmi_drv->hdmi_task)) {
		pr_info("Unable to start kernel thread %s.\n\n", "hdmi proc");
		hdmi_drv->hdmi_task = NULL;
		goto free_mem;
	}
	wake_up_process(hdmi_drv->hdmi_task);

#ifdef CONFIG_HDMI2_CEC_SUNXI
	cec_thread_init(hdmi_drv->parent_dev);
#endif

	memset(&disp_func, 0, sizeof(struct disp_device_func));
	disp_func.enable = hdmi_enable;
	disp_func.smooth_enable = hdmi_smooth_enable;
	disp_func.disable = hdmi_disable;
	disp_func.mode_support = hdmi_mode_support;
	disp_func.get_HPD_status = hdmi_get_HPD_status;
	disp_func.get_input_csc = hdmi_core_get_csc_type;
	disp_func.get_input_color_range = hdmi_core_get_color_range;
	disp_func.get_video_timing_info = hdmi_get_video_timming_info;
	disp_func.suspend = hdmi_suspend;
	disp_func.resume = hdmi_resume;
	disp_func.set_static_config = set_static_config;
	disp_func.get_static_config = get_static_config;
	disp_func.set_dynamic_config = set_dynamic_config;
	disp_func.get_dynamic_config = get_dynamic_config;

#ifdef CONFIG_HDMI2_CEC_SUNXI
#ifndef CONFIG_HDMI2_CEC_USER
	disp_func.cec_standby_request = hdmi_cec_standby_request;
	disp_func.cec_send_one_touch_play = hdmi_cec_send_one_touch_play;
#endif
#endif

	disp_set_hdmi_func(&disp_func);

#if defined(CONFIG_SND_SUNXI_SOC_SUNXI_HDMIAUDIO)
	audio_func.hdmi_audio_enable = hdmi_audio_enable;
	audio_func.hdmi_set_audio_para = hdmi_set_audio_para;
	audio_set_hdmi_func(&audio_func);
	AUDIO_INF("audio_set_hdmi_func\n");

#if defined(CONFIG_SND_SUNXI_SOC_AUDIOHUB_INTERFACE)
	audio_func_muti.hdmi_audio_enable = hdmi_audio_enable;
	audio_func_muti.hdmi_set_audio_para = hdmi_set_audio_para;
	audio_set_muti_hdmi_func(&audio_func_muti);
#endif
#endif
	VIDEO_INF("HDMI2.0 DRIVER PROBE END\n");

	return ret;

free_mem:
	kfree(hdmi_drv->hdmi_core);
	kfree(hdmi_drv);
	pr_info("Free core and drv memory\n");
	return -1;
}

static int hdmi_tx_exit(struct platform_device *pdev)
{
	struct hdmi_tx_drv *dev;
	struct list_head *list;
	int i = 0;

#ifdef CONFIG_HDMI2_CEC_SUNXI
	cec_thread_exit();
#endif

	while (!list_empty(&devlist_global)) {
		list = devlist_global.next;
		list_del(list);
		dev = list_entry(list, struct hdmi_tx_drv, devlist);

		if (dev == NULL)
			continue;
	}

	hdmi_set_ddc_analog(0);

	if (hdmi_drv->hdmi_task) {
		kthread_stop(hdmi_drv->hdmi_task);
		hdmi_drv->hdmi_task = NULL;
	}

	/*scene_lock_destroy(&hdmi_standby_lock);*/
	hdmi_sys_source_release();

	for (i = 0; i < hdmi_drv->power_count; i++)
		hdmi_power_enable(hdmi_drv->power[i]);

	return 0;
}

/**
 * @short of_device_id structure
 */
static const struct of_device_id dw_hdmi_tx[] = {
	{ .compatible =	"allwinner,sunxi-hdmi" },
	{ }
};
MODULE_DEVICE_TABLE(of, dw_hdmi_tx);

/**
 * @short Platform driver structure
 */
static struct platform_driver __refdata dwc_hdmi_tx_pdrv = {
	.remove = hdmi_tx_exit,
	.probe = hdmi_tx_init,
	.driver = {
		.name = "allwinner,sunxi-hdmi",
		.owner = THIS_MODULE,
		.of_match_table = dw_hdmi_tx,
	},
};


static int hdmi_open(struct inode *inode, struct file *filp)
{
	struct file_ops *fops;

	fops = kmalloc(sizeof(struct file_ops), GFP_KERNEL | __GFP_ZERO);
	if (!fops) {
		pr_err("Error:memory allocated for hdmi fops failed!\n");
		return -EINVAL;
	}

	filp->private_data = fops;

	return 0;
}

static int hdmi_release(struct inode *inode, struct file *filp)
{
	struct file_ops *fops = (struct file_ops *)filp->private_data;

	kfree(fops);
	return 0;
}

static ssize_t hdmi_read(struct file *file, char __user *buf,
						size_t count,
						loff_t *ppos)
{
	return -EINVAL;
}

static ssize_t hdmi_write(struct file *file, const char __user *buf,
						size_t count,
						loff_t *ppos)
{
	return -EINVAL;
}

static int hdmi_mmap(struct file *filp, struct vm_area_struct *vma)
{
	return 0;
}

static long hdmi_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	unsigned long long p_arg[3];

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	struct hdmi_hdcp_info hdcp_info;
#ifdef CONFIG_HDMI2_HDCP22_SUNXI
	/*for hdcp keys*/
	unsigned int key_size;
#endif
#endif
	struct file_ops *fops = (struct file_ops *)filp->private_data;

	if (!fops) {
		pr_err("%s: can get fops\n", __func__);
		return -EINVAL;
	}

	if (arg && copy_from_user((void *)p_arg, (void __user *)arg,
					3 * sizeof(unsigned long))) {
		pr_warn("copy_from_user fail\n");
		return -EINVAL;
	}

	switch (cmd) {
	case CMD_NONE:
		fops->ioctl_cmd = CMD_NONE;
		break;
#ifdef CONFIG_HDMI2_HDCP_SUNXI
#ifdef CONFIG_HDMI2_HDCP22_SUNXI
	case HDCP22_LOAD_FIRMWARE:
		fops->ioctl_cmd = HDCP22_LOAD_FIRMWARE;

		if (p_arg[1] > esm_firm_size) {
			pr_info("Error: hdcp22 firmware is too big! "
				"arg_size:%lld  esm_size:%d\n", p_arg[1],
					esm_firm_size);
			return -EINVAL;
		}
		key_size = p_arg[1];

		memset(esm_firm_vir_addr, 0, esm_firm_size);
		if (copy_from_user((void *)esm_firm_vir_addr,
					(void __user *)p_arg[0],
					key_size)) {
			pr_warn("hdcp2.2 firmware, copy_from_user fail\n");
			return -EINVAL;
		}

		pr_info("loading firmware has commpleted!");

#ifdef CONFIG_SUNXI_SMC
		/*sunxi_smc_load_hdcp_key(hdcp_key, key_size);*/
#endif
		break;
#endif
	case HDMI_HDCP_ENABLE:
		set_hdcp_status(HDCP_ING);
		hdcp_enable_core(hdmi_drv->hdmi_core, 1);
		break;

	case HDMI_HDCP_DISABLE:
		hdcp_enable_core(hdmi_drv->hdmi_core, 0);
		set_hdcp_status(HDCP_DISABLE);
		break;

	case HDMI_HDCP_INFO:
		hdcp_info.hdcp_type = get_hdcp_type_core(hdmi_drv->hdmi_core);
		hdcp_info.hdcp_status = (unsigned int)hdcp_encrypt_status;

		if (copy_to_user((void __user *)p_arg[0], (void *)&hdcp_info,
			sizeof(struct hdmi_hdcp_info))) {
			pr_err("copy hdcp info to user failed\n");
			return -EINVAL;
		}

		break;
#endif

	default:
		pr_err("%s ERROR cmd:%d!\n", __func__, cmd);
		return -EINVAL;
	}

	return 0;
}

#ifdef CONFIG_COMPAT
static long hdmi_compat_ioctl(struct file *filp, unsigned int cmd,
						unsigned long arg)
{
	compat_uptr_t karg[3];
	unsigned long __user *ubuffer;

	if (!arg)
		return hdmi_ioctl(filp, cmd, 0);

	if (copy_from_user((void *)karg, (void __user *)arg,
		3 * sizeof(compat_uptr_t))) {
		pr_warn("copy_from_user fail\n");
		return -EFAULT;
	}

	ubuffer = compat_alloc_user_space(3 * sizeof(unsigned long));
	if (!access_ok(VERIFY_WRITE, ubuffer, 3 * sizeof(unsigned long)))
		return -EFAULT;

	if (put_user(karg[0], &ubuffer[0]) ||
		put_user(karg[1], &ubuffer[1]) ||
		put_user(karg[2], &ubuffer[2])) {
		pr_warn("put_user fail\n");
		return -EFAULT;
	}

	return hdmi_ioctl(filp, cmd, (unsigned long)ubuffer);
}
#endif


static const struct file_operations hdmi_fops = {
	.owner		= THIS_MODULE,
	.open		= hdmi_open,
	.release	= hdmi_release,
	.write		= hdmi_write,
	.read		= hdmi_read,
	.unlocked_ioctl	= hdmi_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= hdmi_compat_ioctl,
#endif
	.mmap		= hdmi_mmap,
};

static int __parse_dump_str(const char *buf, size_t size,
				unsigned long *start, unsigned long *end)
{
	char *ptr = NULL;
	char *ptr2 = (char *)buf;
	int ret = 0, times = 0;

	/* Support single address mode, some time it haven't ',' */
next:

	/*Default dump only one register(*start =*end).
	If ptr is not NULL, we will cover the default value of end.*/
	if (times == 1)
		*start = *end;

	if (!ptr2 || (ptr2 - buf) >= size)
		goto out;

	ptr = ptr2;
	ptr2 = strnchr(ptr, size - (ptr - buf), ',');
	if (ptr2) {
		*ptr2 = '\0';
		ptr2++;
	}

	ptr = strim(ptr);
	if (!strlen(ptr))
		goto next;

	ret = kstrtoul(ptr, 16, end);
	if (!ret) {
		times++;
		goto next;
	} else
	pr_warn("String syntax errors: \"%s\"\n", ptr);

out:
	return ret;
}

static ssize_t hdmi_hpd_mask_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	return sprintf(buf, "0x%x\n", hdmi_hpd_mask);
}

static ssize_t hdmi_hpd_mask_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	int err;
	unsigned long val;

	if (count < 1)
		return -EINVAL;

	err = kstrtoul(buf, 16, &val);
	if (err) {
		pr_info("Invalid size\n");
		return err;
	}

	pr_info("val=0x%x\n", (u32)val);
	hdmi_hpd_mask = val;

	return count;
}

static DEVICE_ATTR(hpd_mask, 0664, hdmi_hpd_mask_show,
							hdmi_hpd_mask_store);


static ssize_t hdmi_edid_show(struct device *dev, struct device_attribute *attr,
							char *buf)
{
	u8 *edid = NULL, *edid_ext = NULL;

	/*EDID_block0*/
	edid = (u8 *)hdmi_drv->hdmi_core->mode.edid;
	if (edid != NULL)
		memcpy(buf, edid, 0x80);

	/*EDID extension block*/
	edid_ext =  hdmi_drv->hdmi_core->mode.edid_ext;
	if (edid_ext != NULL)
		memcpy(buf + 0x80, edid_ext,
				0x80 * ((struct edid *)edid)->extensions);

	if (edid && (!edid_ext))
		return 0x80;
	if (edid && edid_ext)
		return 0x80 * (1 + ((struct edid *)edid)->extensions);
	else
		return 0;
}

static ssize_t hdmi_edid_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	return count;
}

static DEVICE_ATTR(edid, 0664,
			hdmi_edid_show,
			hdmi_edid_store);

static ssize_t hdmi_test_reg_read_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%s\n", "echo [0x(address offset), 0x(count)] > hdmi_test_reg_read");
}

ssize_t hdmi_test_reg_read_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	unsigned long start_reg = 0;
	unsigned long read_count = 0;
	u32 i;
	u8 *separator;
	u32 data = 0;

	separator = strchr(buf, ',');
	if (separator != NULL) {
		if (__parse_dump_str(buf, count, &start_reg, &read_count))
			pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);

		pr_info("start_reg=0x%lx  read_count=%ld\n", start_reg, read_count);
		for (i = 0; i < read_count; i++) {
			pr_info("hdmi_addr_offset: 0x%lx = 0x%x\n", start_reg,
					hdmitx_read(start_reg * 4));

			start_reg++;
		}
	} else {
		separator = strchr(buf, ' ');
		if (separator != NULL) {
			start_reg = simple_strtoul(buf, NULL, 0);
			read_count = simple_strtoul(separator + 1, NULL, 0);
			for (i = 0; i < read_count; i += 4) {
				data = (u8)hdmitx_read((start_reg + i) * 4);
				data |= ((u8)hdmitx_read((start_reg + i + 1) * 4)) << 8;
				data |= ((u8)hdmitx_read((start_reg + i + 2) * 4)) << 16;
				data |= ((u8)hdmitx_read((start_reg + i + 3) * 4)) << 24;
				if ((i % 16) == 0)
					printk("\n0x%08lx: 0x%08x",
							(start_reg + i), data);
				else
					printk(" 0x%08x", data);
			}
		} else {
			start_reg = simple_strtoul(buf, NULL, 0);
			pr_info("hdmi_addr_offset: 0x%lx = 0x%x\n", start_reg,
					hdmitx_read(start_reg * 4));
		}
	}
	pr_info("\n");

	return count;
}

static DEVICE_ATTR(read, 0664,
					hdmi_test_reg_read_show,
					hdmi_test_reg_read_store);


static ssize_t hdmi_test_reg_write_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%s\n", "echo [0x(address offset), 0x(value)] > hdmi_test_write");
}

ssize_t hdmi_test_reg_write_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	unsigned long reg_addr = 0;
	unsigned long value = 0;
	u8 *separator1 = NULL;
	u8 *separator2 = NULL;

	separator1 = strchr(buf, ',');
	separator2 = strchr(buf, ' ');
	if (separator1 != NULL) {
		if (__parse_dump_str(buf, count, &reg_addr, &value))
			pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);

		pr_info("reg_addr=0x%lx  write_value=0x%lx\n", reg_addr, value);
		hdmitx_write((reg_addr * 4), value);

		mdelay(1);
		pr_info("after write,red(%lx)=%x\n", reg_addr,
				hdmitx_read(reg_addr * 4));
	} else if (separator2 != NULL) {
		reg_addr = simple_strtoul(buf, NULL, 0);
		value = simple_strtoul(separator2 + 1, NULL, 0);
		pr_info("reg_addr=0x%lx  write_value=0x%lx\n", reg_addr, value);
		hdmitx_write((reg_addr * 4), value);

		mdelay(1);
		pr_info("after write,red(%lx)=%x\n", reg_addr,
				hdmitx_read(reg_addr * 4));
	} else {
		pr_info("ERROR:error input\n");
	}

	return count;
}

static DEVICE_ATTR(write, 0664,
					hdmi_test_reg_write_show,
					hdmi_test_reg_write_store);

static ssize_t phy_write_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	pr_info("OPMODE_PLLCFG-0x16\n");
	pr_info("CKSYMTXCTRL-0x09\n");
	pr_info("PLLCURRCTRL-0x10\n");
	pr_info("VLEVCTRL-0x0E\n");
	pr_info("PLLGMPCTRL-0x15\n");
	pr_info("TXTERM-0x19\n");

	return sprintf(buf, "%s\n", "echo [0x(address offset), 0x(value)] > phy_write");
}

static ssize_t phy_write_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	u8 reg_addr = 0;
	u16 value = 0;
	char *end;
	struct hdmi_tx_core *core = NULL;

	core = hdmi_drv->hdmi_core;

	reg_addr = (u8)simple_strtoull(buf, &end, 0);

	if ((*end != ' ') && (*end != ',')) {
		pr_err("error separator:%c\n", *end);
		return count;
	}

	value = (u16)simple_strtoull(end + 1, &end, 0);

	pr_info("reg_addr=0x%x  write_value=0x%x\n", (u32)reg_addr, (u32)value);
	core->dev_func.phy_write(reg_addr, value);
	return count;
}

static DEVICE_ATTR(phy_write, 0664,
					phy_write_show,
					phy_write_store);


/*static DEVICE_ATTR(hdmi_test_print_core_structure, S_IRUGO|S_IWUSR|S_IWGRP,
					hdmi_test_print_core_structure_show,
					NULL);*/

static ssize_t phy_read_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	pr_info("OPMODE_PLLCFG-0x16\n");
	pr_info("CKSYMTXCTRL-0x09\n");
	pr_info("PLLCURRCTRL-0x10\n");
	pr_info("VLEVCTRL-0x0E\n");
	pr_info("PLLGMPCTRL-0x15\n");
	pr_info("TXTERM-0x19\n");

	return sprintf(buf, "%s\n", "echo [0x(address offset), 0x(count)] > phy_read");
}

ssize_t phy_read_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	u8 start_reg = 0;
	u16 value = 0;
	unsigned long read_count = 0;
	u32 i;
	struct hdmi_tx_core *core = get_platform();

	if (__parse_dump_str(buf, count, (unsigned long *)&start_reg, &read_count))
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);

	pr_info("start_reg=0x%x  read_count=%ld\n", (u32)start_reg, read_count);
	for (i = 0; i < read_count; i++) {
		core->dev_func.phy_read(start_reg, &value);
		pr_info("hdmi_addr_offset: 0x%x = 0x%x\n", (u32)start_reg, value);
		start_reg++;
	}
	pr_info("\n");

	return count;
}

static DEVICE_ATTR(phy_read, 0664,
					phy_read_show,
					phy_read_store);

extern u16 i2c_min_ss_scl_low_time;
extern u16 i2c_min_ss_scl_high_time;
static ssize_t hdmi_set_ddc_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	ssize_t n = 0;

	n += sprintf(buf + n, "low:%d high:%d\n", i2c_min_ss_scl_low_time,
		i2c_min_ss_scl_high_time);
	n += sprintf(buf + n, "%s\n",
		"echo [low_time, high_time] > set_ddc");

	return n;
}

ssize_t hdmi_set_ddc_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	char *end;

	i2c_min_ss_scl_low_time = (u16)simple_strtoull(buf, &end, 0);

	if ((*end != ' ') && (*end != ',')) {
		pr_err("error separator:%c\n", *end);
		return count;
	}

	i2c_min_ss_scl_high_time = (u16)simple_strtoull(end + 1, &end, 0);

	pr_info("low:%d  high:%d\n", i2c_min_ss_scl_low_time,
			i2c_min_ss_scl_high_time);
	return count;
}

static DEVICE_ATTR(set_ddc, 0664, hdmi_set_ddc_show, hdmi_set_ddc_store);

#ifndef SUPPORT_ONLY_HDMI14
static ssize_t scdc_read_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%s\n", "echo [0x(address offset), 0x(count)] > scdc_read");
}

ssize_t scdc_read_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	u8 start_reg = 0;
	u8 value = 0;
	unsigned long read_count = 0;
	u32 i;
	struct hdmi_tx_core *core = get_platform();

	if (__parse_dump_str(buf, count, (unsigned long *)&start_reg, &read_count))
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);

	pr_info("start_reg=0x%x  read_count=%ld\n", (u32)start_reg, read_count);
	for (i = 0; i < read_count; i++) {
		core->dev_func.scdc_read(start_reg, 1, &value);
		pr_info("hdmi_addr_offset: 0x%x = 0x%x\n", (u32)start_reg, value);
		start_reg++;
	}
	pr_info("\n");

	return count;
}

static DEVICE_ATTR(scdc_read, 0664,
					scdc_read_show,
					scdc_read_store);

static ssize_t scdc_write_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%s\n", "echo [0x(address offset), 0x(value)] > scdc_write");
}

static ssize_t scdc_write_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	u8 reg_addr = 0;
	u8 value = 0;
	struct hdmi_tx_core *core = NULL;

	core = hdmi_drv->hdmi_core;

	if (__parse_dump_str(buf, count, (unsigned long *)&reg_addr, (unsigned long *)&value))
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);

	pr_info("reg_addr=0x%x  write_value=0x%x\n", reg_addr, value);
	core->dev_func.scdc_write(reg_addr, 1, &value);
	return count;
}

static DEVICE_ATTR(scdc_write, 0664,
					scdc_write_show,
					scdc_write_store);
#endif

static ssize_t hdmi_debug_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	ssize_t n = 0;

	n += sprintf(buf + n, "Current debug=%d\n\n", hdmi_printf);

	n += sprintf(buf + n, "hdmi log debug level:\n");
	n += sprintf(buf + n, "debug = 1, print video log\n");
	n += sprintf(buf + n, "debug = 2, print edid log\n");
	n += sprintf(buf + n, "debug = 3, print audio log\n");
	n += sprintf(buf + n, "debug = 4, print video+edid+audio log\n");
	n += sprintf(buf + n, "debug = 5, print cec log\n");
	n += sprintf(buf + n, "debug = 6, print hdcp log\n");
	n += sprintf(buf + n, "debug = 7, print all of the logs above\n");
	n += sprintf(buf + n, "debug = 8, print all of the logs above and trace log\n");

	return n;
}

static ssize_t hdmi_debug_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	if (strncmp(buf, "9", 1) == 0)
		hdmi_printf = 9;
	else if (strncmp(buf, "8", 1) == 0)
		hdmi_printf = 8;
	else if (strncmp(buf, "7", 1) == 0)
		hdmi_printf = 7;
	else if (strncmp(buf, "6", 1) == 0)
		hdmi_printf = 6;
	else if (strncmp(buf, "5", 1) == 0)
			hdmi_printf = 5;
	else if (strncmp(buf, "4", 1) == 0)
		hdmi_printf = 4;
	else if (strncmp(buf, "3", 1) == 0)
		hdmi_printf = 3;
	else if (strncmp(buf, "2", 1) == 0)
		hdmi_printf = 2;
	else if (strncmp(buf, "1", 1) == 0)
		hdmi_printf = 1;
	else if (strncmp(buf, "0", 1) == 0)
		hdmi_printf = 0;
	else
		pr_err("Error Input!\n");

	pr_info("debug=%d\n", hdmi_printf);

	return count;
}

static DEVICE_ATTR(debug, 0664, hdmi_debug_show,
							hdmi_debug_store);

#ifdef CONFIG_HDMI2_CEC_SUNXI
#ifndef CONFIG_HDMI2_CEC_USER
static ssize_t hdmi_one_touch_play_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	return 0;
}

static ssize_t hdmi_one_touch_play_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	if (strncmp(buf, "1", 1) == 0)
		hdmi_cec_send_one_touch_play();
	else
		pr_err("Error Input!\n");

	return count;
}

static DEVICE_ATTR(one_touch_play, 0664,
				   hdmi_one_touch_play_show,
				   hdmi_one_touch_play_store);


static ssize_t hdmi_cec_local_standby_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	ssize_t ret = 0;

	ret = sprintf(buf, "%d", (unsigned int)cec_get_local_standby());

	return ret;
}

static ssize_t hdmi_cec_local_standby_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	if (strncmp(buf, "1", 1) == 0)
		cec_set_local_standby(true);
	else if (strncmp(buf, "1", 0) == 0)
		cec_set_local_standby(false);
	else
		pr_err("Error Input!\n");

	return count;
}

static DEVICE_ATTR(cec_local_standby_mode, 0664,
				   hdmi_cec_local_standby_show,
				   hdmi_cec_local_standby_store);
#endif


static ssize_t hdmi_cec_dump_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	ssize_t n = 0;

	n += cec_dump_core(buf + n);

	return n;
}

static DEVICE_ATTR(cec_dump, 0664,
				   hdmi_cec_dump_show, NULL);

static ssize_t hdmi_cec_enable_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%d\n",
				hdmi_drv->cec_support);
}

static ssize_t hdmi_cec_enable_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	if (strncmp(buf, "1", 1) == 0) {
		hdmi_drv->cec_support = 1;
		hdmi_cec_enable(1);
	} else {
		hdmi_drv->cec_support = 0;
		hdmi_cec_enable(0);
	}

	return count;
}

static DEVICE_ATTR(cec_enable, 0664,
				hdmi_cec_enable_show,
				hdmi_cec_enable_store);
#endif

#ifdef CONFIG_HDMI2_HDCP_SUNXI
static ssize_t hdmi_hdcp_type_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	char hdcp_type = (char)get_hdcp_type_core(hdmi_drv->hdmi_core);

	mutex_lock(&hdmi_drv->hdcp_mutex);
	memcpy(buf, &hdcp_type, 1);
	mutex_unlock(&hdmi_drv->hdcp_mutex);

	return 1;
}

static ssize_t hdmi_hdcp_type_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	return count;
}

static DEVICE_ATTR(hdcp_type, 0664, hdmi_hdcp_type_show,
							hdmi_hdcp_type_store);

static ssize_t hdmi_hdcp_enable_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%d\n",
				hdmi_drv->hdmi_core->mode.pHdcp.hdcp_on);
}

static ssize_t hdmi_hdcp_enable_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	mutex_lock(&hdmi_drv->hdcp_mutex);
	if (strncmp(buf, "1", 1) == 0) {
		set_hdcp_status(HDCP_ING);
		hdcp_enable_core(hdmi_drv->hdmi_core, 1);
	} else {
		hdcp_enable_core(hdmi_drv->hdmi_core, 0);
		set_hdcp_status(HDCP_DISABLE);
	}
	mutex_unlock(&hdmi_drv->hdcp_mutex);

	return count;
}

static DEVICE_ATTR(hdcp_enable, 0664,
				hdmi_hdcp_enable_show,
				hdmi_hdcp_enable_store);

static ssize_t hdcp_status_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	u32 count = sizeof(u8);

	mutex_lock(&hdmi_drv->hdcp_mutex);
	memcpy(buf, &hdcp_encrypt_status, count);
	mutex_unlock(&hdmi_drv->hdcp_mutex);

	return count;
}

static ssize_t hdcp_status_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	return count;
}

static DEVICE_ATTR(hdcp_status, 0664, hdcp_status_show,
								hdcp_status_store);

static ssize_t hdcp_dump_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	ssize_t ret = 0;

	ret += hdcp_dump_core(buf + ret);
	ret += sprintf(buf + ret, "\n");
	ret += hdmi_drv->hdmi_core->dev_func.hdcp_config_dump(buf + ret);

	return ret;
}

static DEVICE_ATTR(hdcp_dump, 0664,
				hdcp_dump_show,
				NULL);


#ifdef CONFIG_HDMI2_HDCP22_SUNXI
static char *esm_addr;
static ssize_t esm_dump_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
    ssize_t n = 0;

	n += sprintf(buf+n, "addr=%p, size=0x%04x\n",
		esm_firm_vir_addr, esm_firm_size);
    esm_addr = esm_firm_vir_addr;

    return n;
}

static ssize_t esm_dump_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
    if (esm_addr == 0 || esm_firm_size == 0)
    return -1;

    memcpy(esm_addr, buf, count);
    esm_addr = esm_addr + count;
    pr_info("esm_addr=%p, count=0x%04x\n", esm_addr, (unsigned int)count);
    return count;
}

static DEVICE_ATTR(esm_dump, 0664, esm_dump_show, esm_dump_store);

static ssize_t hpi_write_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%s\n", "echo [0x(address offset), 0x(value)] > hpi_write");
}

static ssize_t hpi_write_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	u32 reg_addr = 0;
	u32 value = 0;
	struct hdmi_tx_core *core = NULL;

	core = hdmi_drv->hdmi_core;

	if (__parse_dump_str(buf, count, (unsigned long *)&reg_addr, (unsigned long *)&value))
		pr_err("%s,%d err, invalid para!\n", __func__, __LINE__);

	pr_info("reg_addr=0x%x  write_value=0x%x\n", reg_addr, value);
	*((u32 *)(core->mode.pHdcp.esm_hpi_base + reg_addr)) = (u32)value;
	return count;
}

static DEVICE_ATTR(hpi_write, 0664, hpi_write_show, hpi_write_store);

static ssize_t hpi_read_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%s\n", "echo [0x(address offset)] > hpi_read");
}

ssize_t hpi_read_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	u32 start_reg = 0;
	u32 value = 0;
	char *end;
	struct hdmi_tx_core *core = get_platform();

	start_reg = (u32)simple_strtoull(buf, &end, 0);

	if ((*end != ' ') && (*end != ',')) {
		pr_err("error separator:%c\n", *end);
		return count;
	}

	pr_info("start_reg=0x%x\n", (u32)start_reg);
	value = *((u32 *)(core->mode.pHdcp.esm_hpi_base + start_reg));
	pr_info("hdmi_addr_offset: 0x%x = 0x%x\n", (u32)start_reg, value);

	return count;
}

static DEVICE_ATTR(hpi_read, 0664, hpi_read_show, hpi_read_store);
#endif
#endif

#ifdef CONFIG_HDMI2_FREQ_SPREAD_SPECTRUM
extern u32 ss_amp;
static ssize_t freq_spread_spectrum_amp_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	ssize_t n = 0;

	n += sprintf(buf + n, "frequency spread spectrum amp:\n");
	n += sprintf(buf + n, "2/4/6/../30\n\n");

	n += sprintf(buf + n, "echo [amp] > /sys/clas/hdmi/hdmi/attr/ss_amp\n\n");

	n += sprintf(buf + n, "NOTE: if echo 0 > /sys/clas/hdmi/hdmi/attr/ss_amp,\n");
	n += sprintf(buf + n, "disable sprum spectrum\n\n");

	n += sprintf(buf + n, "The current amp is:%d\n", ss_amp);

	return n;
}

static ssize_t freq_spread_spectrum_amp_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	char *end;
	struct disp_video_timings *video_info;

	ss_amp = (u32)simple_strtoull(buf, &end, 0);

	hdmi_get_video_timming_info(&video_info);

	hdmi_set_spread_spectrum(video_info->pixel_clk);

	return count;
}

static DEVICE_ATTR(ss_amp, 0664, freq_spread_spectrum_amp_show,
				freq_spread_spectrum_amp_store);

extern bool old_ss;
static ssize_t freq_spread_spectrum_version_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	ssize_t n = 0;

	n += sprintf(buf + n, "Use the old frequency spread spectrum:\n");
	n += sprintf(buf + n, "echo old > /sys/clas/hdmi/hdmi/attr/ss_version\n\n");

	n += sprintf(buf + n, "Use the new frequency spread spectrum:\n");
	n += sprintf(buf + n, "echo new > /sys/clas/hdmi/hdmi/attr/ss_version\n\n");

	n += sprintf(buf + n, "The current version of "
		"frequency spread spectrum  is:%s\n", old_ss ? "old" : "new");

	return n;
}

static ssize_t freq_spread_spectrum_version_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct disp_video_timings *video_info;

	if (strncmp(buf, "old", 1) == 0) {
		pr_info("set old version freq_spread_spectrum\n");
		old_ss = 1;
	} else if (strncmp(buf, "new", 1) == 0) {
		pr_info("set new version freq_spread_spectrum\n");
		old_ss = 0;
	} else {
		pr_err("[HDMI] ERROR INPUT：%s\n", buf);
		return count;
	}

	hdmi_get_video_timming_info(&video_info);

	hdmi_set_spread_spectrum(video_info->pixel_clk);

	return count;
}

static DEVICE_ATTR(ss_version, 0664, freq_spread_spectrum_version_show,
				freq_spread_spectrum_version_store);
#endif

struct hdmi_debug_video_mode {
	int hdmi_mode;/* vic */
	char *name;
};


static struct hdmi_debug_video_mode debug_video_mode[] = {
	{HDMI_VIC_720x480I_4_3, "480I"},
	{HDMI_VIC_720x480I_16_9, "480I"},
	{HDMI_VIC_720x576I_4_3,	"576I"},
	{HDMI_VIC_720x576I_16_9, "576I"},
	{HDMI_VIC_720x480P60_4_3, "720x480P"},
	{HDMI_VIC_720x480P60_16_9, "720x480P"},
	{HDMI_VIC_640x480P60, "640x480P"},
	{HDMI_VIC_720x576P_4_3,	"576P"},
	{HDMI_VIC_720x576P_16_9, "576P"},
	{HDMI_VIC_1280x720P24, "720P24"},
	{HDMI_VIC_1280x720P25, "720P25"},
	{HDMI_VIC_1280x720P30, "720P30"},
	{HDMI_VIC_1280x720P50, "720P50"},
	{HDMI_VIC_1280x720P60, "720P60"},
	{HDMI_VIC_1920x1080I50,	"1080I50"},
	{HDMI_VIC_1920x1080I60,	"1080I60"},
	{HDMI_VIC_1920x1080P24,	"1080P24"},
	{HDMI_VIC_1920x1080P50,	"1080P50"},
	{HDMI_VIC_1920x1080P60,	"1080P60"},
	{HDMI_VIC_1920x1080P25,	"1080P25"},
	{HDMI_VIC_1920x1080P30,	"1080P30"},
	{HDMI_VIC_3840x2160P30,	"2160P30"},
	{HDMI_VIC_3840x2160P25,	"2160PP25"},
	{HDMI_VIC_3840x2160P24,	"2160P24"},
	{HDMI_VIC_4096x2160P24,	"4096x2160P24"},
	{HDMI_VIC_4096x2160P25,	"4096x2160P25"},
	{HDMI_VIC_4096x2160P30, "4096x2160P30"},
	{HDMI_VIC_3840x2160P50, "2160P50"},
	{HDMI_VIC_4096x2160P50, "4096x2160P50"},
	{HDMI_VIC_3840x2160P60, "2160P60"},
	{HDMI_VIC_4096x2160P60, "4096x2160P60"},
};



static char *hdmi_vic_name[] = {
	"2160P30",
	"2160P25",
	"2160P24",
	"4096x2160P24",
};


static char *hdmi_audio_code_name[] = {
	"LPCM",
	"AC-3",
	"MPEG1",
	"MP3",
	"MPEG2",
	"AAC",
	"DTS",
	"ATRAC",
	"OneBitAudio",
	"DolbyDigital+",
	"DTS-HD",
	"MAT",
	"DST",
	"WMAPro",
};
static char *debug_get_video_name(int hdmi_mode)
{
	int i = 0;

	for (i = 0;
	i < sizeof(debug_video_mode)/sizeof(struct hdmi_debug_video_mode);
	 i++) {
		if (debug_video_mode[i].hdmi_mode == hdmi_mode)
			return debug_video_mode[i].name;
	}

	return NULL;
}
static ssize_t hdmi_sink_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	ssize_t n = 0;
	int i = 0;
	sink_edid_t	      *sink_cap =  hdmi_drv->hdmi_core->mode.sink_cap;

	if (!sink_cap) {
		n += sprintf(buf+n, "%s\n", "Do not read edid from sink");
		return n;
	}

	/* Video Data Block */
	n += sprintf(buf+n, "\n\n%s", "Video Mode:");
	for (i = 0; i < sink_cap->edid_mSvdIndex; i++) {
		if (sink_cap->edid_mSvd[i].mLimitedToYcc420
		|| sink_cap->edid_mSvd[i].mYcc420)
			continue;

		n += sprintf(buf+n, "  %s",
			debug_get_video_name(
			(int)sink_cap->edid_mSvd[i].mCode));


		if (sink_cap->edid_mSvd[i].mNative)
			n += sprintf(buf+n, "%s", "(native)");
	}


	for (i = 0; i < sink_cap->edid_mHdmivsdb.mHdmiVicCount; i++) {
		if (sink_cap->edid_mHdmivsdb.mHdmiVic[i] <= 0x4) {
			n += sprintf(buf+n, "%s",
			hdmi_vic_name[
			sink_cap->edid_mHdmivsdb.mHdmiVic[i]-1]);
		}
	}


	/* YCC420 VDB  */
	n += sprintf(buf+n, "\n\n%s", "Only Support YUV420:");
	for (i = 0; i < sink_cap->edid_mSvdIndex; i++) {
		if (sink_cap->edid_mSvd[i].mLimitedToYcc420) {
			n += sprintf(buf+n, "  %s",
			debug_get_video_name(
			(int)sink_cap->edid_mSvd[i].mCode));
		}
	}

	/*YCC420 CMDB */
	n += sprintf(buf+n, "\n\n%s", "Also Support YUV420:");
	for (i = 0; i < sink_cap->edid_mSvdIndex; i++) {
		if (sink_cap->edid_mSvd[i].mYcc420) {
			n += sprintf(buf+n, "  %s",
			debug_get_video_name(
			(int)sink_cap->edid_mSvd[i].mCode));
		}
	}


	/*video pixel format */
	n += sprintf(buf+n, "\n\n%s", "Pixel Format: RGB");
	if (sink_cap->edid_mYcc444Support)
		n += sprintf(buf+n, "  %s", "YUV444");
	if (sink_cap->edid_mYcc422Support)
		n += sprintf(buf+n, "  %s", "YUV422");
	/*deepcolor */
	n += sprintf(buf+n, "\n\n%s", "Deep Color:");
	if (sink_cap->edid_mHdmivsdb.mDeepColor30) {
		n += sprintf(buf+n, "  %s", "RGB444_30bit");
		if (sink_cap->edid_mHdmivsdb.mDeepColorY444)
			n += sprintf(buf+n, "  %s", "YUV444_30bit");
	}

	if (sink_cap->edid_mHdmivsdb.mDeepColor36) {
		n += sprintf(buf+n, "  %s", "RGB444_36bit");
		if (sink_cap->edid_mHdmivsdb.mDeepColorY444)
			n += sprintf(buf+n, "  %s", "YUV444_36bit");
	}
	if (sink_cap->edid_mHdmivsdb.mDeepColor48) {
		n += sprintf(buf+n, "  %s", "RGB444_48bit");
		if (sink_cap->edid_mHdmivsdb.mDeepColorY444)
			n += sprintf(buf+n, "  %s", "YUV444_48bit");
	}

	if (sink_cap->edid_mHdmiForumvsdb.mDC_30bit_420)
		n += sprintf(buf+n, "  %s", "YUV420_30bit");
	if (sink_cap->edid_mHdmiForumvsdb.mDC_36bit_420)
		n += sprintf(buf+n, "  %s", "YUV420_36bit");
	if (sink_cap->edid_mHdmiForumvsdb.mDC_48bit_420)
		n += sprintf(buf+n, "  %s", "YUV420_48bit");

	/*3D format */
	if (sink_cap->edid_mHdmivsdb.m3dPresent) {
		n += sprintf(buf+n, "\n\n%s", "3D Mode:");
		for (i = 0; i < 16; i++) {
			if (sink_cap->edid_mHdmivsdb.mVideo3dStruct[i][0] == 1
			&& i < sink_cap->edid_mSvdIndex) {
				n += sprintf(buf+n, "  %s_FP",
				debug_get_video_name(
				(int)sink_cap->edid_mSvd[i].mCode));
			}
			if (sink_cap->edid_mHdmivsdb.mVideo3dStruct[i][6] == 1
			&& i < sink_cap->edid_mSvdIndex) {
				n += sprintf(buf+n, "  %s_SBS",
				debug_get_video_name(
				(int)sink_cap->edid_mSvd[i].mCode));
			}
			if (sink_cap->edid_mHdmivsdb.mVideo3dStruct[i][8] == 1
			&& i < sink_cap->edid_mSvdIndex) {
				n += sprintf(buf+n, "  %s_TAB",
				debug_get_video_name(
				(int)sink_cap->edid_mSvd[i].mCode));
			}
		}
	}

	/*TMDS clk rate */
	if (sink_cap->edid_mHdmiForumvsdb.mValid) {
		n += sprintf(buf+n, "\n\n%s", "MaxTmdsCharRate:");
		n += sprintf(buf+n, "  %d",
			sink_cap->edid_mHdmiForumvsdb.mMaxTmdsCharRate);
	}
	/*audio*/
	n += sprintf(buf+n, "\n\n%s", "Basic Audio Support:");
	n += sprintf(buf+n, "  %s",
		sink_cap->edid_mBasicAudioSupport ? "YES" : "NO");
	if (sink_cap->edid_mBasicAudioSupport) {
		n += sprintf(buf+n, "\n\n%s", "Audio Code:");
		for (i = 0; i < sink_cap->edid_mSadIndex; i++) {
			n += sprintf(buf+n, "  %s",
		hdmi_audio_code_name[sink_cap->edid_mSad[i].mFormat-1]);
		}
	}

	/*hdcp*/
	n += sprintf(buf+n, "\n\n%s", "HDCP Tpye:");

	n += sprintf(buf+n, "%c", '\n');

	return n;

}



static DEVICE_ATTR(hdmi_sink, 0664,
				hdmi_sink_show,
				NULL);


static char *pixel_format_name[] = {
	"RGB",
	"YUV422",
	"YUV444",
	"YUV420",
};

static char *colorimetry_name[] = {
	"NULL",
	"ITU601",
	"ITU709",
	"XV_YCC601",
	"XV_YCC709",
	"S_YCC601",
	"ADOBE_YCC601",
	"ADOBE_RGB",
	"BT2020_Yc_Cbc_Crc",
	"BT2020_Y_CB_CR",
};

static ssize_t hdmi_rxsense_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%u", hdmi_core_get_rxsense_state());
}

static DEVICE_ATTR(rxsense, 0664, hdmi_rxsense_show, NULL);

static ssize_t hdmi_source_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	ssize_t n = 0;

	n += sprintf(buf+n, "\n%s%d\n",
		"HPD:  ",
		hdmi_core_get_hpd_state());
	n += sprintf(buf+n, "\n%s%d\n",
		"RxSense:  ",
		hdmi_core_get_rxsense_state());
	n += sprintf(buf+n, "\n%s%d\n",
		"PhyLock:  ",
		hdmi_core_get_phy_pll_lock_state());
	n += sprintf(buf+n, "\n%s%d\n",
		"PhyPower:  ",
		hdmi_core_get_phy_power_state());
	n += sprintf(buf+n, "\n%s%s\n",
		"TmdsMode:  ",
		hdmi_core_get_tmds_mode() ? "HDMI" : "DVI");
#ifndef SUPPORT_ONLY_HDMI14
	n += sprintf(buf+n, "\n%s%d\n",
		 "Scramble:  ",
		hdmi_core_get_scramble_state());
#endif
	n += sprintf(buf+n, "\n%s%d\n",
		"AvMute:  ",
		hdmi_core_get_avmute_state());
	n += sprintf(buf+n, "\n%s%d\n",
		"PixelRepetion:  ",
		 hdmi_core_get_pixelrepetion());
	n += sprintf(buf+n, "\n%s%d\n",
		"BitDepth:  ",
		hdmi_core_get_color_depth());
	n += sprintf(buf+n, "\n%s%s\n",
		"PixelFormat:  ",
		pixel_format_name[hdmi_core_get_pixel_format()]);
	n += sprintf(buf+n, "\n%s%s\n",
		"Colorimetry:  ",
		 colorimetry_name[hdmi_core_get_colorimetry()]);
	n += sprintf(buf+n, "\n%s%s\n",
		"VideoFormat:  ",
		 debug_get_video_name((int)hdmi_core_get_video_code()));
	n += sprintf(buf+n, "\n%s%d\n",
		"AudioLayout:  ",
		 hdmi_core_get_audio_layout());
	n += sprintf(buf+n, "\n%s%d\n",
		"AudioChannelCnt:  ",
		hdmi_core_get_audio_channel_count());
	n += sprintf(buf+n, "\n%s%d\n",
		"AudioSamplingFreq:  ",
		hdmi_core_get_audio_sample_freq());
	n += sprintf(buf+n, "\n%s%d\n",
		"AudioSampleSize:  ",
		 hdmi_core_get_audio_sample_size());
	n += sprintf(buf+n, "\n%s%d\n",
		"AudioNvalue:  ",
		hdmi_core_get_audio_n());
	return n;
}


static DEVICE_ATTR(hdmi_source, 0664,
				hdmi_source_show,
				NULL);

static ssize_t hdmi_avmute_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%s\n%s\n",
		"echo [value] > avmute",
		"-----value =1:avmute on; =0:avmute off");
}

static ssize_t hdmi_avmute_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	if (strncmp(buf, "1", 1) == 0)
		hdmi_core_avmute_enable(1);
	else
		hdmi_core_avmute_enable(0);

	return count;
}

static DEVICE_ATTR(avmute, 0664,
				hdmi_avmute_show,
				hdmi_avmute_store);



static ssize_t phy_power_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%s\n%s\n",
		 "echo [value] > phy_power",
		"-----value =1:phy power on; =0:phy power off");
}

static ssize_t phy_power_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	if (strncmp(buf, "1", 1) == 0)
		hdmi_core_phy_power_enable(1);
	else
		hdmi_core_phy_power_enable(0);

	return count;
}


static DEVICE_ATTR(phy_power, 0664,
				phy_power_show,
				phy_power_store);


static ssize_t dvi_mode_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	ssize_t n = 0;

	n += sprintf(buf + n, "%s\n%s\n",
			 "echo [value] > dvi_mode",
			"-----value =0:HDMI mode; =1:DVI mode");
	return n;
}

static ssize_t dvi_mode_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	if (count < 1)
		return -EINVAL;

	if (strncmp(buf, "1", 1) == 0)
		hdmi_core_dvimode_enable(1);
	else
		hdmi_core_dvimode_enable(0);

	return count;
}

static DEVICE_ATTR(dvi_mode, 0664,
				dvi_mode_show,
				dvi_mode_store);

static struct attribute *hdmi_attributes[] = {
	&dev_attr_read.attr,
	&dev_attr_write.attr,
	&dev_attr_phy_write.attr,
	&dev_attr_phy_read.attr,
	&dev_attr_set_ddc.attr,
#ifndef SUPPORT_ONLY_HDMI14
	&dev_attr_scdc_read.attr,
	&dev_attr_scdc_write.attr,
#endif
	&dev_attr_debug.attr,
	&dev_attr_hpd_mask.attr,
	&dev_attr_edid.attr,

	&dev_attr_hdmi_sink.attr,
	&dev_attr_hdmi_source.attr,
	&dev_attr_rxsense.attr,
	&dev_attr_avmute.attr,
	&dev_attr_phy_power.attr,
	&dev_attr_dvi_mode.attr,

#ifdef CONFIG_HDMI2_HDCP_SUNXI
	&dev_attr_hdcp_status.attr,
	&dev_attr_hdcp_type.attr,
	&dev_attr_hdcp_enable.attr,
	&dev_attr_hdcp_dump.attr,
#ifdef CONFIG_HDMI2_HDCP22_SUNXI
	&dev_attr_esm_dump.attr,
	&dev_attr_hpi_read.attr,
	&dev_attr_hpi_write.attr,
#endif
#endif

#ifdef CONFIG_HDMI2_CEC_SUNXI
#ifndef CONFIG_HDMI2_CEC_USER
	&dev_attr_one_touch_play.attr,
	&dev_attr_cec_local_standby_mode.attr,
#endif
	&dev_attr_cec_dump.attr,
	&dev_attr_cec_enable.attr,
#endif

#ifdef CONFIG_HDMI2_FREQ_SPREAD_SPECTRUM
	&dev_attr_ss_amp.attr,
	&dev_attr_ss_version.attr,
#endif
	NULL
};

static struct attribute_group hdmi_attribute_group = {
	.name = "attr",
	.attrs = hdmi_attributes
};

static int __init hdmi_module_init(void)
{
	int ret = 0, err;

	/*Create and add a character device*/
	alloc_chrdev_region(&devid, 0, 1, "hdmi");/*corely for device number*/
	hdmi_cdev = cdev_alloc();
	cdev_init(hdmi_cdev, &hdmi_fops);
	hdmi_cdev->owner = THIS_MODULE;
	err = cdev_add(hdmi_cdev, devid, 1);/*/proc/device/hdmi*/
	if (err) {
		pr_err("Error: hdmi cdev_add fail.\n");
		return -1;
	}

	/*Create a path: sys/class/hdmi*/
	hdmi_class = class_create(THIS_MODULE, "hdmi");
	if (IS_ERR(hdmi_class)) {
		pr_err("Error:hdmi class_create fail\n");
		return -1;
	}

	/*Create a path "sys/class/hdmi/hdmi"*/
	hdev = device_create(hdmi_class, NULL, devid, NULL, "hdmi");

	/*Create a path: sys/class/hdmi/hdmi/attr*/
	ret = sysfs_create_group(&hdev->kobj, &hdmi_attribute_group);
	if (ret)
		pr_err("Error: hdmi sysfs_create_group failed!\n");

	ret = platform_driver_register(&dwc_hdmi_tx_pdrv);
	if (ret)
		pr_err("Error: hdmi driver register fail\n");

	pr_info("HDMI2.0 module init end\n");

	return ret;
}

static void __exit hdmi_module_exit(void)
{
	pr_info("hdmi_module_exit\n");

	hdmi_tx_exit(hdmi_drv->pdev);

#ifdef CONFIG_HDMI2_HDCP22_SUNXI
	dma_free_coherent(hdmi_drv->parent_dev,
				HDCP22_DATA_SIZE,
				&hdmi_drv->hdmi_core->mode.pHdcp.esm_data_phy_addr,
				GFP_KERNEL | __GFP_ZERO);
	dma_free_coherent(hdmi_drv->parent_dev,
					HDCP22_FIRMWARE_SIZE,
					&hdmi_drv->hdmi_core->mode.pHdcp.esm_firm_phy_addr,
					GFP_KERNEL | __GFP_ZERO);
#endif
	hdmi_core_exit(hdmi_drv->hdmi_core);

	kfree(hdmi_drv);

	platform_driver_unregister(&dwc_hdmi_tx_pdrv);

	sysfs_remove_group(&hdev->kobj, &hdmi_attribute_group);
	device_destroy(hdmi_class, devid);

	class_destroy(hdmi_class);
	cdev_del(hdmi_cdev);
}

late_initcall(hdmi_module_init);
module_exit(hdmi_module_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("zhengwanyu");
MODULE_DESCRIPTION("HDMI_TX20 module driver");
MODULE_VERSION("1.0");
