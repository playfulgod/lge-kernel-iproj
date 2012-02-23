/* Copyright (c) 2010,  LG Electronics Inc. All rights reserved. */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/types.h>
#include <asm/system.h>
#include <asm/gpio.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/wakelock.h>
#include <linux/i2c/twl.h>		//for akm power
#include <linux/regulator/consumer.h>	//for akm power
#include <linux/mfd/pmic8058.h>
#include "muic.h"

/* Macros assume PMIC GPIOs start at 0 */
#define PM8058_GPIO_BASE                    NR_MSM_GPIOS
#define PM8058_GPIO_PM_TO_SYS(pm_gpio)      (pm_gpio + PM8058_GPIO_BASE)
#define PM8058_GPIO_SYS_TO_PM(sys_gpio)     (sys_gpio - PM8058_GPIO_BASE)
#define PM8058_MPP_BASE                     (PM8058_GPIO_BASE + PM8058_GPIOS)
#define PM8058_MPP_PM_TO_SYS(pm_gpio)       (pm_gpio + PM8058_MPP_BASE)
#define PM8058_MPP_SYS_TO_PM(sys_gpio)      (sys_gpio - PM8058_MPP_BASE)
#define PM8058_IRQ_BASE	                    (NR_MSM_IRQS + NR_GPIO_IRQS)

/*******************************************************************************
 * Initialize MUIC - Default setting.
 *
 * CONTROL_1:
 * 
 *	ID_2P2	= 0. Enable to distinguish MUIC_EARMIC from MUIC_TV_OUT_LOAD and MUIC_OTG.
 *			     Enable for MUIC_EARMIC operation.
 *	ID_620	= 0. Enable only to distinguish MUIC_TV_OUT_LOAD from MUIC_OTG.
 *	ID_200	= 1.
 *	VLDO	= 0. Enable to apply 2.3V for MUIC_EARMIC operation.
 *	SEMREN	= 1.
 *	ADC_EN	= 0. Because it is automatically enabled upon any change in ID resistor.
 *	CP_EN	= 0. Enalbe for USB 2.0 (MUIC_AP_USB, MUIC_CP_USB, and MUIC_OTG).
 *			     Enable for Audio charge pump (MUIC_EARMIC, MUIC_TV_OUT_LOAD).
 * 
 * CONTROL_2: 
 *
 *	INTPOL	= 0.
 *	INT_EN	= 1.
 *	MIC_LP	= 0.
 *	CP_AUD	= 1. Disable for Audio operation (MUIC_EARMIC, MUIC_TV_OUT_LOAD).
 *	CHG_TYP = 1.
 *	USB_DET_DIS = 0. Negative enabled.
 *
 * SW_CONTROL: 
 *
 *	MIC_ON	= 0. Enable for MUIC_EARMIC and MUIC_TV_OUT_LOAD.
 *	DP	= 111 (open).
 *	DM	= 111 (open).
 *
 ******************************************************************************/

static struct i2c_client *muic_client;
static struct work_struct muic_wq;

/* store INT Register value */
//static uint8_t int_stat_val;
/* store STATUS register value */
//static uint8_t status_val;
charger_type charge_id;
usb_id_type usb_id;

/*
 * Function: Read the MUIC register whose internal address is addr
 *           and save the uint8_t content into value.
 * Return value: Negative errno upon failure, 0 upon success.
 */
static uint8_t ts5usba_muic_read(uint8_t addr)
{
	return i2c_smbus_read_byte_data(muic_client, addr);
}

/*
 * Function: Write uint8_t value to the MUIC register whose internal address is addr.
 * Return value: Negative errno upon failure, 0 upon success.
 */
static void ts5usba_muic_write(uint8_t addr, uint8_t val)
{
	i2c_smbus_write_byte_data(muic_client, addr, val);
}

static void ts5usba_muic_initialize(void)
{
    uint8_t vendor_id = 0;
    usb_id = INVALID;
    charge_id = CHARGER_TYPE_NONE;

    pr_info("%s\n", __func__);

    vendor_id = ts5usba_muic_read(TS5USBA_DEVICE_ID_ADDR);
    vendor_id &= TS5USBA_VENDOR_MASK;

    if(vendor_id != TS5USBA_VENDOR_TI)
    {
        pr_err("%s: vendor_id does not match (0x%02X)\n", __func__, vendor_id);
        return;
    }

    ts5usba_muic_write(TS5USBA_SW_CONTROL, TS5USBA_DP_DM_OPEN<<TS5USBA_DP_MASK_SHIFT|
            TS5USBA_DP_DM_OPEN<<TS5USBA_DM_MASK_SHIFT);
    ts5usba_muic_write(TS5USBA_CONTROL_1_ADDR, TS5USBA_ID_200_MASK|TS5USBA_SEMREN_MASK);
    ts5usba_muic_write(TS5USBA_CONTROL_2_ADDR, TS5USBA_CHG_TYP_MASK);
    mdelay(250);
    ts5usba_muic_write(TS5USBA_CONTROL_2_ADDR, TS5USBA_INT_EN_MASK|TS5USBA_CHG_TYP_MASK);
}

static uint8_t ts5usba_muic_read_vbus(void)
{
    uint8_t reg_val;

    reg_val = ts5usba_muic_read(TS5USBA_INT_STATUS);

    pr_info("%s: %d\n", __func__, (reg_val&TS5USBA_VBUS_MASK) ? 1:0);
    return ((reg_val&TS5USBA_VBUS_MASK) ? 1:0);
}

static uint8_t ts5usba_muic_read_chg_det(void)
{
    uint8_t reg_val;

    reg_val = ts5usba_muic_read(TS5USBA_INT_STATUS);

    pr_info("%s: %d\n", __func__, (reg_val&TS5USBA_CHGDET_MASK) ? 1:0);
    return ((reg_val&TS5USBA_CHGDET_MASK) ? 1:0);
}

static charger_type ts5usba_muic_read_charge_type(void)
{
    uint8_t reg_val;
    charger_type charger_val;

    reg_val = ts5usba_muic_read(TS5USBA_CHIP_STATUS);

    if (reg_val & TS5USBA_DC_PORT_MASK)
    {
        pr_info("%s: DEDICATED_CHARGER\n", __func__);
        charger_val = DEDICATED_CHARGER;
    }
    else if (reg_val & TS5USBA_CH_PORT_MASK)
    {
        pr_info("%s: CHARGING_HOST\n", __func__);
        charger_val = CHARGING_HOST;
    }
    else
    {
        pr_info("%s: INVALID_CHARGER\n", __func__);
        charger_val = INVALID_CHARGER;
    }

    return charger_val;
}

static usb_id_type ts5usba_muic_read_usb_id(void)
{
    uint8_t reg_val;
    usb_id_type usb_id_val;
    char *impedance[] = {
        "0 ohm (grounded)",
        "24k ohm",
        "56k ohm",
        "100k ohm",
        "130k ohm",
        "180k ohm",
        "240k ohm",
        "330k ohm",
        "430k ohm",
        "620k ohm",
        "910k ohm",
        "open"
    };

    reg_val = ts5usba_muic_read(TS5USBA_INT_STATUS);
    reg_val &= TS5USBA_IDNO_MASK;

    /* 
     * INT_Status: IDNO
     *
     * 0000: ADC determined ID impedance = 0 ohm (grounded)
     * 0001: ADC determined ID impedance = 24k ohm
     * 0010: ADC determined ID impedance = 56k ohm
     * 0011: ADC determined ID impedance = 100k ohm
     * 0100: ADC determined ID impedance = 130k ohm
     * 0101: ADC determined ID impedance = 180k ohm
     * 0110: ADC determined ID impedance = 240k ohm
     * 0111: ADC determined ID impedance = 330k ohm
     * 1000: ADC determined ID impedance = 430k ohm
     * 1001: ADC determined ID impedance = 620k ohm
     * 1010: ADC determined ID impedance = 910k ohm
     * 1011: ADC determined ID impedance = open
     */
    pr_info("%s: IDNO = %d%d%d%d: ADC determined ID impedance = %s\n", __func__,
            (reg_val>>3)&1,
            (reg_val>>2)&1,
            (reg_val>>1)&1,
            (reg_val>>0)&1,
            impedance[reg_val]);

    if(reg_val == TS5USBA_ID_130K_VAL)
    {
        pr_info("%s: FACTORY_UART\n", __func__);
        usb_id_val = FACTORY_UART;
    }
    else if(reg_val == TS5USBA_ID_56K_VAL)
    {
        pr_info("%s: FACTORY_USB\n", __func__);
        usb_id_val = FACTORY_USB;
    }
    else if(reg_val == TS5USBA_ID_180K_VAL)
    {
        pr_info("%s: USB_ONLY(TA)\n", __func__);
        usb_id_val = USB_ONLY;//TA_ONLY;
    }
    else if(reg_val == TS5USBA_ID_GROUND_VAL)
    {
        pr_info("%s: INVALID\n", __func__);
        usb_id_val = INVALID;
    }
    else
    {
        pr_info("%s: USB_ONLY\n", __func__);
        usb_id_val = USB_ONLY;
    }

    return usb_id_val;
}

static void ts5usba_muic_set_idlemode(void)
{
    pr_info("%s\n", __func__);
    ts5usba_muic_write(TS5USBA_SW_CONTROL,TS5USBA_DP_DM_OPEN<<TS5USBA_DP_MASK_SHIFT|
            TS5USBA_DP_DM_OPEN<<TS5USBA_DM_MASK_SHIFT);
    ts5usba_muic_write(TS5USBA_CONTROL_1_ADDR,TS5USBA_ID_200_MASK|TS5USBA_SEMREN_MASK);
    ts5usba_muic_write(TS5USBA_CONTROL_2_ADDR,TS5USBA_INT_EN_MASK|TS5USBA_CHG_TYP_MASK);
}

static void ts5usba_muic_set_TAmode(void)
{
    pr_info("%s\n", __func__);
    ts5usba_muic_write(TS5USBA_CONTROL_1_ADDR,TS5USBA_ID_2P2_MASK|TS5USBA_SEMREN_MASK);  
    ts5usba_muic_write(TS5USBA_SW_CONTROL,TS5USBA_DP_DM_OPEN<<TS5USBA_DP_MASK_SHIFT|
            TS5USBA_DP_DM_OPEN<<TS5USBA_DM_MASK_SHIFT);	
}

static void ts5usba_muic_set_UARTmode(void)
{
    pr_info("%s\n", __func__);
    ts5usba_muic_write(TS5USBA_CONTROL_1_ADDR,TS5USBA_ID_2P2_MASK|TS5USBA_SEMREN_MASK);  
    ts5usba_muic_write(TS5USBA_SW_CONTROL,TS5USBA_DP_DM_UART<<TS5USBA_DP_MASK_SHIFT|
            TS5USBA_DP_DM_UART<<TS5USBA_DM_MASK_SHIFT);	
}

static void ts5usba_muic_set_LTmode(void)
{
    pr_info("%s\n", __func__);
    ts5usba_muic_write(TS5USBA_CONTROL_1_ADDR,TS5USBA_ID_2P2_MASK|
            TS5USBA_SEMREN_MASK|TS5USBA_CP_EN_MASK);  
    ts5usba_muic_write(TS5USBA_SW_CONTROL,TS5USBA_DP_DM_USB<<TS5USBA_DP_MASK_SHIFT|
            TS5USBA_DP_DM_USB<<TS5USBA_DM_MASK_SHIFT);		
}

static void ts5usba_muic_set_USBmode(void)
{
    pr_info("%s\n", __func__);
    ts5usba_muic_write(TS5USBA_CONTROL_1_ADDR,TS5USBA_ID_2P2_MASK|
            TS5USBA_SEMREN_MASK|TS5USBA_CP_EN_MASK);
    ts5usba_muic_write(TS5USBA_SW_CONTROL,TS5USBA_DP_DM_USB<<TS5USBA_DP_MASK_SHIFT|
            TS5USBA_DP_DM_USB<<TS5USBA_DM_MASK_SHIFT);	
}

static void ts5usba_muic_set_dp_dm_mode(usb_id_type usb_id)
{
    switch (usb_id)
    {
        case FACTORY_UART:
            ts5usba_muic_set_UARTmode();
            break;
        case FACTORY_USB:
            ts5usba_muic_set_LTmode();
            break;
        case TA_ONLY:
            ts5usba_muic_set_TAmode();
            break;
        case USB_ONLY:
            ts5usba_muic_set_USBmode();
            break;
        case AUDIO:
        case INVALID:
        default:
            ts5usba_muic_set_idlemode();
            break;
    }
}

static void ts5usba_muic_main(void)
{
    pr_info("%s\n", __func__);
    pr_info("%s: INT_STATUS = %02X\n", __func__, ts5usba_muic_read(TS5USBA_INT_STATUS));

    if (!ts5usba_muic_read_vbus())
    {
        usb_id = INVALID; //AUDIO;
        ts5usba_muic_set_idlemode();
        return;
    }

    if (!ts5usba_muic_read_chg_det())
    {
        charge_id = CHARGER_TYPE_NONE;
    }
    else
    {
        charge_id = ts5usba_muic_read_charge_type();;
    }

    usb_id = ts5usba_muic_read_usb_id();
    ts5usba_muic_set_dp_dm_mode(usb_id);

    return;
}

static void ts5usba_muic_wq_func(struct work_struct *muic_wq)
{
    pr_info("%s\n", __func__);

    /*
     * Upon an MUIC IRQ (MUIC_INT_N falls),
     * wait 70ms before reading INT_STAT and STATUS.
     * After the reads, MUIC_INT_N returns to high
     * (but the INT_STAT and STATUS contents will be held).
     * 
     * Do this only if TS5USBA33402_device_detection() was called upon IRQ.
     */
    mdelay(70);

    ts5usba_muic_main();
}

static irqreturn_t ts5usba_muic_isr(int irq, void *data)
{
	pr_info("%s: IRQ occurs!\n", __func__);
	schedule_work(&muic_wq);

	return IRQ_HANDLED;
}

static int ts5usba_muic_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
//	u32 retry_no;
	int sys_gpio; 
    unsigned int irq;
	int rc;
	
	muic_client = client;

	pr_info("%s\n", __func__);

//--------------------------------------
	sys_gpio = PM8058_GPIO_PM_TO_SYS(MUIC_INT_GPIO);
    irq = PM8058_GPIO_IRQ(PM8058_IRQ_BASE, MUIC_INT_GPIO);
//--------------------------------------

	/* Initialize GPIO 15 (MUIC_INT_N).
	 * Check if other driver already occupied it.
	 */
	rc = gpio_request(sys_gpio, "MUIC IRQ GPIO");
	if(rc < 0){
		printk(KERN_INFO "[MUIC] GPIO 40 MUIC_INT_N is already occupied by other driver!\n");
		return -ENOSYS;
	}

	/* Initialize GPIO direction before use or IRQ setting */
	rc = gpio_direction_input(sys_gpio);
	if(rc < 0){
		printk(KERN_INFO "[MUIC] GPIO 40 MUIC_INT_N direction initialization failed!\n");
		return -ENOSYS;
	}

	/* Register MUIC work queue function */
	INIT_WORK(&muic_wq, ts5usba_muic_wq_func);

	/* Initialize MUIC - Finally MUIC INT becomes enabled */
	ts5usba_muic_initialize();

	/* Set up an IRQ line and enable the involved interrupt handler.
	 * From this point, a MUIC_INT_N can invoke muic_interrupt_handler().
	 * muic_interrupt_handler merely calls schedule_work() with muic_wq_func().
	 * muic_wq_func() actually performs the accessory detection.
	 */
	rc = request_threaded_irq(irq, NULL, ts5usba_muic_isr,
			IRQF_TRIGGER_FALLING | IRQF_DISABLED,
            "muic_irq", &client->dev);
	if (rc < 0){
		printk(KERN_INFO "[MUIC] MUIC_INT_N IRQ line set up failed!\n");
		free_irq(gpio_to_irq(sys_gpio), &client->dev);
		return rc;
	}

    ts5usba_muic_main();

	return rc;
}

static int ts5usba_muic_remove(struct i2c_client *client)
{
	free_irq(gpio_to_irq(MUIC_INT_GPIO), &client->dev);
	gpio_free(MUIC_INT_GPIO);
	i2c_set_clientdata(client, NULL);

	return 0;
}

static int ts5usba_muic_suspend(struct i2c_client *client, pm_message_t state)
{
    /*
	client->dev.power.power_state = state;
    */
	return 0;
}
		
static int ts5usba_muic_resume(struct i2c_client *client)
{
    /*
	client->dev.power.power_state = PMSG_ON;
    */
	return 0;
}

static const struct i2c_device_id ts5usba_muic_ids[] = {
	{"ts5usba_i2c_muic", 0},
	{/* end of list */},
};

static struct i2c_driver ts5usba_muic_driver = {
	.probe	 	= ts5usba_muic_probe,
	.remove	 	= ts5usba_muic_remove,
	.suspend 	= ts5usba_muic_suspend,
	.resume  	= ts5usba_muic_resume,
	.id_table	= ts5usba_muic_ids,
    .driver	 	= {
        .name       = "ts5usba_i2c_muic",
        .owner      = THIS_MODULE,
    },
};

static int __init ts5usba_muic_init(void)
{
	return i2c_add_driver(&ts5usba_muic_driver);
}

static void __exit ts5usba_muic_exit(void)
{
	i2c_del_driver(&ts5usba_muic_driver);
}

module_init(ts5usba_muic_init);
module_exit(ts5usba_muic_exit);

MODULE_AUTHOR("LG Electronics");
MODULE_DESCRIPTION("TS5USBA MUIC Driver");
MODULE_LICENSE("GPL");

