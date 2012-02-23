/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/reboot.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/mfd/pmic8058.h>
#include <linux/mfd/pmic8901.h>

#include <mach/msm_iomap.h>
#include <mach/restart.h>
#include <mach/scm-io.h>
#include <asm/mach-types.h>

#define TCSR_WDT_CFG 0x30

#define WDT0_RST       (MSM_TMR0_BASE + 0x38)
#define WDT0_EN        (MSM_TMR0_BASE + 0x40)
#define WDT0_BARK_TIME (MSM_TMR0_BASE + 0x4C)
#define WDT0_BITE_TIME (MSM_TMR0_BASE + 0x5C)

#define PSHOLD_CTL_SU (MSM_TLMM_BASE + 0x820)

#define IMEM_BASE           0x2A05F000

#define RESTART_REASON_ADDR 0x65C
#define DLOAD_MODE_ADDR     0x0

// START sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP : 56K_DLOAD_TRANSITION {
#ifdef CONFIG_LGE_USB_FACTORY
#define SHARED_IMEM_BOOT_SIZE 0X00C8
#endif
// END sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP }

static int restart_mode;
void *restart_reason;

#ifdef CONFIG_MSM_DLOAD_MODE
static int in_panic;
static void *dload_mode_addr;
// START sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP : 56K_DLOAD_TRANSITION {
#ifdef CONFIG_LGE_USB_FACTORY
static void *dload_hsu_info_addr;
#endif
// END sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP }

// START sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP : 56K_DLOAD_TRANSITION {
#ifdef CONFIG_LGE_USB_GADGET_DRIVER
extern u16 android_get_product_id(void);
#endif
// END sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP }

/* Download mode master kill-switch */
static int dload_set(const char *val, struct kernel_param *kp);
static int download_mode = 1;
module_param_call(download_mode, dload_set, param_get_int,
			&download_mode, 0644);

static int panic_prep_restart(struct notifier_block *this,
			      unsigned long event, void *ptr)
{
	in_panic = 1;
	return NOTIFY_DONE;
}

static struct notifier_block panic_blk = {
	.notifier_call	= panic_prep_restart,
};

static void set_dload_mode(int on)
{
	/* neo.kang@lge.com 2011-06-24, for knowing when set dload */
	printk("%s : %d\n", __func__, on);

	if (dload_mode_addr) {
		writel(on ? 0xE47B337D : 0, dload_mode_addr);
		writel(on ? 0xCE14091A : 0,
		       dload_mode_addr + sizeof(unsigned int));

#ifdef CONFIG_LGE_USB_FACTORY
// START sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP : 56K_DLOAD_TRANSITION {
        if (1==on && dload_hsu_info_addr) {
            u16 pid = 0;
#ifdef CONFIG_LGE_USB_GADGET_DRIVER
            pid = android_get_product_id();
#endif
    		writel((0x6000==pid) ? 0x6000 : 0, dload_hsu_info_addr);
        }
// END sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP }
#endif
		mb();
	}
}

static int dload_set(const char *val, struct kernel_param *kp)
{
	int ret;
	int old_val = download_mode;

	ret = param_set_int(val, kp);

	if (ret)
		return ret;

	/* If download_mode is not zero or one, ignore. */
	if (download_mode >> 1) {
		download_mode = old_val;
		return -EINVAL;
	}

	set_dload_mode(download_mode);

	return 0;
}
#else
#define set_dload_mode(x) do {} while (0)
#endif

void msm_set_restart_mode(int mode)
{
	restart_mode = mode;
}
EXPORT_SYMBOL(msm_set_restart_mode);

static void msm_power_off(void)
{
	printk(KERN_NOTICE "Powering off the SoC\n");
#ifdef CONFIG_MSM_DLOAD_MODE
	set_dload_mode(0);
#endif

#if defined(CONFIG_LGE_ERROR_HANDLER)
		writel(0x77665504, restart_reason);
#endif

	pm8058_reset_pwr_off(0);
	pm8901_reset_pwr_off(0);
	writel(0, PSHOLD_CTL_SU);
	mdelay(10000);
	printk(KERN_ERR "Powering off has failed\n");
	return;
}

void arch_reset(char mode, const char *cmd)
{

#ifdef CONFIG_MSM_DLOAD_MODE

	/* This looks like a normal reboot at this point. */
	set_dload_mode(0);

	/* Write download mode flags if we're panic'ing */
	set_dload_mode(in_panic);

	/* Write download mode flags if restart_mode says so */
	if (restart_mode == RESTART_DLOAD)
#ifdef CONFIG_LGE_ERROR_HANDLER
    {
		set_dload_mode(1);
        writel(0x6d63c421, restart_reason);
        goto reset;
    }
#else
		set_dload_mode(1);
#endif

	/* Kill download mode if master-kill switch is set */
	if (!download_mode)
		set_dload_mode(0);
#endif

	printk(KERN_NOTICE "Going down for restart now : %s\n", cmd);

	pm8058_reset_pwr_off(1);

	/* neo.kang@lge.com 2011-06-01
	 * add the error handler */
#if defined(CONFIG_LGE_ERROR_HANDLER)
	if( in_panic == 1 ) {
		if( restart_mode == SUB_THD_F_PWR)
			writel(0x6d630040, restart_reason);
		else if( restart_mode == SUB_THD_F_SD)
			writel(0x6d630020, restart_reason);
		else if( restart_mode == SUB_THD_F_PWR)
			writel(0x6d630010, restart_reason);
		else if( restart_mode == SUB_UNAB_THD)
			writel(0x6d630008, restart_reason);
		else if( restart_mode == SUB_RESET_SOC)
			writel(0x6d630004, restart_reason);
		else if( restart_mode == SUB_UNKNOWN)
			writel(0x6d630002, restart_reason);
		else
			writel(0x6d630100, restart_reason);
	} else {
		if (cmd != NULL) {
			if (!strncmp(cmd, "bootloader", 10)) {
				writel(0x77665500, restart_reason);
			} else if (!strncmp(cmd, "recovery", 8)) {
				writel(0x77665502, restart_reason);
			} else if (!strncmp(cmd, "oem-", 4)) {
				unsigned long code;
				strict_strtoul(cmd + 4, 16, &code);
				code = code & 0xff;
				writel(0x6f656d00 | code, restart_reason);
			} else {
				writel(0x77665501, restart_reason);
			}
		} else {
			writel(0x00000000, restart_reason);
		}
	}
reset:
#else // qct original
	if (cmd != NULL) {
		if (!strncmp(cmd, "bootloader", 10)) {
			writel(0x77665500, restart_reason);
		} else if (!strncmp(cmd, "recovery", 8)) {
			writel(0x77665502, restart_reason);
		} else if (!strncmp(cmd, "oem-", 4)) {
			unsigned long code;
			strict_strtoul(cmd + 4, 16, &code);
			code = code & 0xff;
			writel(0x6f656d00 | code, restart_reason);
		} else {
			writel(0x77665501, restart_reason);
		}
	}
#endif // CONFIG_LGE_ERROR_HANDLER

	writel(0, WDT0_EN);

	if (!(machine_is_msm8x60_charm_surf() || machine_is_lge_i_board() ||
	      machine_is_msm8x60_charm_ffa())) {
		dsb();
		writel(0, PSHOLD_CTL_SU); /* Actually reset the chip */
		mdelay(5000);
		pr_notice("PS_HOLD didn't work, falling back to watchdog\n");
	}

	__raw_writel(1, WDT0_RST);
	__raw_writel(5*0x31F3, WDT0_BARK_TIME);
	__raw_writel(0x31F3, WDT0_BITE_TIME);
	__raw_writel(3, WDT0_EN);
	secure_writel(3, MSM_TCSR_BASE + TCSR_WDT_CFG);

	mdelay(10000);
	printk(KERN_ERR "Restarting has failed\n");
}

#ifdef CONFIG_LGE_CHARGER_TEMP_SCENARIO
EXPORT_SYMBOL(arch_reset);
#endif

static int __init msm_restart_init(void)
{
	void *imem = ioremap_nocache(IMEM_BASE, SZ_4K);

#ifdef CONFIG_MSM_DLOAD_MODE
	atomic_notifier_chain_register(&panic_notifier_list, &panic_blk);
	dload_mode_addr = imem + DLOAD_MODE_ADDR;

// START sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP : 56K_DLOAD_TRANSITION {
#ifdef CONFIG_LGE_USB_FACTORY
    dload_hsu_info_addr = dload_mode_addr + SHARED_IMEM_BOOT_SIZE;
#endif
// END sungchae.koo@lge.com 2011/07/14 P1_LAB_BSP }

	/* Reset detection is switched on below.*/
	set_dload_mode(1);
#endif
	restart_reason = imem + RESTART_REASON_ADDR;
	pm_power_off = msm_power_off;

	/* neo.kang@lge.com 2011-06-01
	 * add the error handler */
#if defined(CONFIG_LGE_ERROR_HANDLER)
	//writel(0x6d63ad00, restart_reason);
	set_dload_mode(0);
#endif

	return 0;
}

late_initcall(msm_restart_init);
