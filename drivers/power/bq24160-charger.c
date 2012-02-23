/* 
 *  TI BQ24160 Charger Driver
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */
#define DEBUG
#include <linux/i2c.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/bq24160-charger.h>
#include <linux/power_supply.h>
#include <linux/pmic8058-batt-alarm.h>

#ifdef CONFIG_LGE_SWITCHING_CHARGER_BQ24160_DOCOMO_ONLY
#include <mach/gpio.h>
#include <linux/msm-charger.h>
#include <linux/msm_adc.h>
#endif


#define BQ24160_CABLE_PWRON_INT
//#define BQ24160_HZMODE_CTRL

// define register map
#define BQ24160_REG_STAT_CTRL 		0x00		/* Status / Control Register (read/write) */

#define BQ24160_WDOG_TMR_MASK		0x80
#define BQ24160_WDOG_TMR_SHFT		7
#define BQ24160_STAT_MASK			0x70
#define BQ24160_STAT_SHFT			4
#define BQ24160_SUPPLY_SEL_MASK		0x08
#define BQ24160_SUPPLY_SEL_SHFT		3
#define BQ24160_FAULT_MASK			0x07
#define BQ24160_FAULT_SHFT			0

#define BQ24160_REG_BATTNPS_STAT	0x01		/* Battery / Supply Status Register (read/write) */

#define BQ24160_INSTAT_MASK			0xC0
#define BQ24160_INSTAT_SHFT			6
#define BQ24160_USBSTAT_MASK		0x30
#define BQ24160_USBSTAT_SHFT		4
#define BQ24160_OTGLOCK_MASK		0x08
#define BQ24160_OTGLOCK_SHFT		3
#define BQ24160_BATTSTAT_MASK		0x06
#define BQ24160_BATTSTAT_SHFT		1

#define BQ24160_REG_CTRL			0x02		/* Control Register (read/write) */

#define BQ24160_RESET_MASK			0x80
#define BQ24160_RESET_SHFT			7
#define BQ24160_IUSB_LMT_MASK		0x70
#define BQ24160_IUSB_LMT_SHFT		4
#define BQ24160_ENSTAT_MASK			0x08
#define BQ24160_ENSTAT_SHFT			3
#define BQ24160_TE_MASK				0x04
#define BQ24160_TE_SHFT				2
#define BQ24160_CE_MASK				0x02
#define BQ24160_CE_SHFT				1
#define BQ24160_HZMODE_MASK			0x01
#define BQ24160_HZMODE_SHFT			0

#define BQ24160_REG_CTRL_BATVOLT	0x03		/* Control / Battery Voltage Register (read/write) */

#define BQ24160_VBATTREG_MASK		0xAE//0xB0
#define BQ24160_VBATTREG_SHFT		2
#define BQ24160_INLMT_IN_MASK		0x02
#define BQ24160_INLMT_IN_SHFT		1
#define BQ24160_DPDM_EN_MASK		0x01
#define BQ24160_DPDM_EN_SHFT		0

#define BQ24160_REG_VEND_PART_REV	0x04		/* Vendor / Part / Revision (read only) */

#define BQ24160_VENDOR_MASK			0xE0
#define BQ24160_VENDOR_SHFT			5
#define BQ24160_PN_MASK				0x18
#define BQ24160_PN_SHFT				3
#define BQ24160_REV_MASK			0x07
#define BQ24160_REV_SHFT			0

#define BQ24160_REG_BATTTERM_FCHGCRNT	0x05	/* Battery Termination / Fast Charge Current (read/write) */

#define BQ24160_ICHGCRNT_MASK		0xF8
#define BQ24160_ICHGCRNT_SHFT		3
#define BQ24160_ITERMCRNT_MASK		0x07
#define BQ24160_ITERMCRNT_SHFT		0

#define BQ24160_REG_VINDPM_STAT	0x06			/* Vin-dpm Voltage / DPPM Status */

#define BQ24160_MINSYS_STAT_MASK	0x80
#define BQ24160_MINSYS_STAT_SHFT	7
#define BQ24160_DPM_STAT_MASK		0x40
#define BQ24160_DPM_STAT_SHFT		6
#define BQ24160_USB_INDPM_MASK		0x38
#define BQ24160_USB_INDPM_SHFT		3
#define BQ24160_IN_INDPM_MASK		0x07
#define BQ24160_IN_INDPM_SHFT		0

#define BQ24160_REG_SAFETMR_NTCMON		0x07	/* Safety Timer / NTC Monitor (read/write) */

#define BQ24160_2XTMR_EN_MASK		0x80
#define BQ24160_2XTMR_EN_SHFT		7
#define BQ24160_TMR_MASK			0x60
#define BQ24160_TMR_SHFT			5
#define BQ24160_TS_EN_MASK			0x08
#define BQ24160_TS_EN_SHFT			3
#define BQ24160_TS_FAULT_MASK		0x06
#define BQ24160_TS_FAULT_SHFT		1

#define BQ24160_STAT_NO_VALID_SRC_DETECTED		0
#define BQ24160_STAT_IN_READY 					1
#define BQ24160_STAT_USB_READY 					2
#define BQ24160_STAT_CHARGING_FROM_IN 			3
#define BQ24160_STAT_CHARGING_FROM_USB 			4
#define BQ24160_STAT_CHARGE_DONE 				5
#define BQ24160_STAT_NA 						6
#define BQ24160_STAT_FAULT						7

#define BQ24160_CHG_WORK_PERIOD	 ((HZ) * 1)
#define BQ24160_CHG_TEMP_SENARIO ((HZ) * 1)
#define BQ24160_CHG_WATCH_DOG_RESET_PERIOD ((HZ) * 20)

#define WIRELESS_CHARGE_STATUS	71
#define WIRELESS_CHARGE_COMPLETE	141

#define SWITCHING_CHG_IRQ_N	124


#define BQ24160_ICHG_CURRENT_450    450
#define BQ24160_ICHG_CURRENT_500    500
#define BQ24160_ICHG_CURRENT_600    600
#define BQ24160_ICHG_CURRENT_800    800
#define BQ24160_ICHG_CURRENT_1500   1500


#define BQ24160_RESUME_CHG_VOLT     4250

enum
{
  BQ24160_CHG_STATE_NO_CABLE = 0,
  BQ24160_CHG_STATE_CHARGING,
  BQ24160_CHG_STATE_DONE,
  BQ24160_CHG_STATE_STOP,
};


struct bq24160_chip {
	struct i2c_client *client;
	struct delayed_work charge_work;
  struct delayed_work watch_dog_reset;
  struct delayed_work charge_start_from_temp_senario;
  struct delayed_work charge_stop_from_temp_senario;
#ifdef BQ24160_HZMODE_CTRL
	struct delayed_work hzmode_ctrl_work;
#endif	
	struct power_supply charger;
	struct bq24160_platform_data *pdata;
    int irq;
    int chg_online;
#ifdef CONFIG_LGE_SWITCHING_CHARGER_BQ24160_DOCOMO_ONLY
	struct msm_hardware_charger	adapter_hw_chg;
#endif
};


static bool at_cmd_chg = false;
static bool resume_chg_start = false;
static bool batt_high_temp = false;
static int ichg_current = 0;
static int old_ichg_current = 0;
static int old_charger = 0;
static int now_chg_state = 0;


static int bq24160_battery_gague_alarm_notify(struct notifier_block *nb,
					  unsigned long status, void *unused);

static struct notifier_block alarm_notifier = {
	.notifier_call = bq24160_battery_gague_alarm_notify,
};


extern unsigned msm_otg_get_chg_current(void);
extern int max17040_get_battery_mvolts(void);
extern int is_chg_plugged_in(void);



#ifdef CONFIG_LGE_SWITCHING_CHARGER_BQ24160_DOCOMO_ONLY
static void bq24160_irq_init(int gpio_num)
{
	int rc;
  
	rc = gpio_request(gpio_num, "bq24160_irq_gpio");
	if(rc < 0){
    printk(KERN_DEBUG "############ [bq24160_irq_init]: Can't Get BQ24160 IRQ GPIO #####################\n");
		return ;
	}
	gpio_direction_input(gpio_num);
	gpio_set_value(gpio_num, 1);

  printk(KERN_DEBUG "############ [bq24160_irq_init]: BQ24160 IRQ REGISTER SUCCESS #####################\n");
}
#endif


static int bq24160_write_reg(struct i2c_client *client, u8 reg, u8 value)
{
	int ret = i2c_smbus_write_byte_data(client, reg, value);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int bq24160_read_reg(struct i2c_client *client, u8 reg)
{
	int ret = i2c_smbus_read_byte_data(client, reg);

	if (ret < 0)
		dev_err(&client->dev, "%s: err %d\n", __func__, ret);

	return ret;
}

static int bq24160_set_bits(struct i2c_client *client, u8 reg, u8 mask, u8 data)
{
	u8 value = 0;
	int ret;

	value = bq24160_read_reg(client, reg);
	if (value < 0)
		goto out;
	value &= ~mask;
	value |= data;
	ret = bq24160_write_reg(client, reg, value);
	dev_dbg(&client->dev, "%s REG_#=0x%x value = 0x%x\n", __func__, reg, value);
out:
	return ret;
}

void bq24160_set_chg_current(int ichg)
{
  ichg_current = ichg;
}
EXPORT_SYMBOL(bq24160_set_chg_current);


static int bq24160_get_chg_current(void)
{
  return ichg_current;
}


static void bq24160_start_chg_from_temp_senario(struct work_struct *bq24160_work)
{
  u8 temp = 0;
	u8 status = 0;
  u8 reg_val= 0;
  int now_current = 0;
  struct bq24160_chip *bq24160_chg;


	bq24160_chg = container_of(bq24160_work, struct bq24160_chip,
			charge_start_from_temp_senario.work);


	/* Watchdog timer reset */
	bq24160_set_bits(bq24160_chg->client, BQ24160_REG_STAT_CTRL, BQ24160_WDOG_TMR_MASK, (1<<BQ24160_WDOG_TMR_SHFT));

  /* REG_#2  Set current limit and enable STAT in Control register */
	reg_val = ( (0b0 << BQ24160_RESET_SHFT) |
					(0b100 << BQ24160_IUSB_LMT_SHFT) |
					(0b1 << BQ24160_ENSTAT_SHFT) |
					(0b1 << BQ24160_TE_SHFT) |
					(0b0 << BQ24160_CE_SHFT) |
					(0b0 << BQ24160_HZMODE_SHFT) );
	bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL, reg_val);

  temp = bq24160_read_reg(bq24160_chg->client, BQ24160_REG_STAT_CTRL);
  dev_dbg(&bq24160_chg->client->dev, "%s STATUS Register[#0]: 0x%x\n", __func__,temp);

  status = temp & BQ24160_STAT_MASK;
	status = status >> BQ24160_STAT_SHFT;

  now_current = bq24160_get_chg_current();

  printk(KERN_DEBUG "############ [%s]: CHARGING START BECAUSE OF TEMPERATURE [[CHARGER STATUS = %d, OLD(mA) = %d NOW(mA) = %d #####################\n", 
    __func__, status, old_ichg_current, now_current);

  if((status == BQ24160_STAT_USB_READY || status == BQ24160_STAT_CHARGING_FROM_USB) && (old_ichg_current != now_current))
  {
    bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        
    switch(now_current)
    {
      case BQ24160_ICHG_CURRENT_450:
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x02); // Charging Current: 550mA Termination Current: 150mA
        break;
        
      case BQ24160_ICHG_CURRENT_500:
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x02); // Charging Current: 550mA Termination Current: 150mA
        break;
        
      case BQ24160_ICHG_CURRENT_600:
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x0A); // Charging Current: 625mA Termination Current: 150mA
        break;
        
      case BQ24160_ICHG_CURRENT_800:
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22); // Charging Current: 850mA Termination Current: 150mA
        break;
        
      case BQ24160_ICHG_CURRENT_1500:
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x62); // Charging Current: 1450mA Termination Current: 150mA
        break;

      default:
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x0A); // Charging Current: 625mA Termination Current: 150mA
        break;
    }
  }

  old_ichg_current = now_current;
}


static void bq24160_stop_chg_from_temp_senario(struct work_struct *bq24160_work)
{
  u8  reg_val= 0;
  struct bq24160_chip *bq24160_chg;

  printk(KERN_DEBUG "############ [%s]: CHARGING STOP BECAUSE OF TEMPERATURE #####################\n", __func__);

  bq24160_chg = container_of(bq24160_work, struct bq24160_chip,
			charge_stop_from_temp_senario.work);
    
  /* Watchdog timer reset */
	bq24160_set_bits(bq24160_chg->client, BQ24160_REG_STAT_CTRL, BQ24160_WDOG_TMR_MASK, (1<<BQ24160_WDOG_TMR_SHFT));

  /* REG_#2  Set current limit and enable STAT in Control register */
  reg_val = ( (0b0 << BQ24160_RESET_SHFT) |
					(0b100 << BQ24160_IUSB_LMT_SHFT) |
					(0b1 << BQ24160_ENSTAT_SHFT) |
					(0b1 << BQ24160_TE_SHFT) |
					(0b0 << BQ24160_CE_SHFT) |
					(0b0 << BQ24160_HZMODE_SHFT) );
	bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL, reg_val);
}


void bq24160_determine_the_collect_chg(bool start)
{
#if 0
  struct bq24160_chip *bq24160_chg;
  
  if(start)
  {
    schedule_delayed_work(&bq24160_chg->charge_start_from_temp_senario, BQ24160_CHG_TEMP_SENARIO);
  }
  else
  {
    schedule_delayed_work(&bq24160_chg->charge_stop_from_temp_senario, BQ24160_CHG_TEMP_SENARIO);
  }
#else
  batt_high_temp = start;
#endif
}
EXPORT_SYMBOL(bq24160_determine_the_collect_chg);


static void bq24160_watchdog_reset(struct work_struct *bq24160_work)
{
  struct bq24160_chip *bq24160_chg;
  int mv;
  int now_charger = 0;


	bq24160_chg = container_of(bq24160_work, struct bq24160_chip,
			watch_dog_reset.work);

  mv = max17040_get_battery_mvolts();

  printk(KERN_DEBUG "############ [bq24160_watchdog_reset]: WATCH-DOG RESET [CHG CUR = %d CHG STATE = %d VOLT = %d] #####################\n", 
    msm_otg_get_chg_current(), now_chg_state, mv);

  /* Stop Charging Condition Check for BATTERY */
  if(batt_high_temp)
  {
    u8  reg_val= 0;
    
    /* REG_#2  Set current limit and enable STAT in Control register */
		reg_val = ( (0b0 << BQ24160_RESET_SHFT) |
					(0b100 << BQ24160_IUSB_LMT_SHFT) |
					(0b1 << BQ24160_ENSTAT_SHFT) |
					(0b1 << BQ24160_TE_SHFT) |
					(0b0 << BQ24160_CE_SHFT) |
					(0b0 << BQ24160_HZMODE_SHFT) );
    
		bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL, reg_val);

    return;
  }

  /* Charging Current Setting */
  now_charger = msm_otg_get_chg_current();
  if(old_charger != now_charger)
  {
    if(now_charger < 500)
    {
      bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x02); // Charging Current: 625mA Termination Current: 150mA
    }
    else
    {
      bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22);  // Charging Current: 850mA Termination Current: 150mA
    }
  }

  /* Watchdog timer reset */
	bq24160_set_bits(bq24160_chg->client, BQ24160_REG_STAT_CTRL, BQ24160_WDOG_TMR_MASK, (1<<BQ24160_WDOG_TMR_SHFT));

  schedule_delayed_work(&bq24160_chg->watch_dog_reset, BQ24160_CHG_WATCH_DOG_RESET_PERIOD);
}



static int bq24160_battery_gague_alarm_notify(struct notifier_block *nb,
		unsigned long status, void *unused)
{
	int rc;

	pr_info("%s: status: %lu\n", __func__, status);

	switch (status) {
	case 0:
		break;
	/* expected case - trip of low threshold */
	case 1:
		if (is_chg_plugged_in()) {
			rc = pm8058_batt_alarm_state_set(0, 0);

      resume_chg_start = true;
			//msm_charger_notify_event(NULL, CHG_BATT_NEEDS_RECHARGING);
		}
		break;
	case 2:
		break;
	default:
		break;
	};

	return 0;
}



static void bq24160_charge(struct work_struct *bq24160_work)
{
  u8 temp = 0;
	u8 status = 0;
  int rc = 0;
  struct bq24160_chip *bq24160_chg;


	bq24160_chg = container_of(bq24160_work, struct bq24160_chip,
			charge_work.work);

	dev_dbg(&bq24160_chg->client->dev, "%s\n", __func__);

	/* Watchdog timer reset */
	bq24160_set_bits(bq24160_chg->client, BQ24160_REG_STAT_CTRL, BQ24160_WDOG_TMR_MASK, (1<<BQ24160_WDOG_TMR_SHFT));

  temp = bq24160_read_reg(bq24160_chg->client, BQ24160_REG_STAT_CTRL);
  dev_dbg(&bq24160_chg->client->dev, "%s STATUS Register[#0]: 0x%x\n", __func__,temp);

  status = temp & BQ24160_STAT_MASK;
	status = status >> BQ24160_STAT_SHFT;

  printk(KERN_DEBUG "############ [bq24160_charge]: CHARGE STATUS = %d Current = %d #####################\n", status, msm_otg_get_chg_current());

	switch(status)
	{
		case BQ24160_STAT_NO_VALID_SRC_DETECTED:
      now_chg_state = BQ24160_CHG_STATE_NO_CABLE;
      
      rc = pm8058_batt_alarm_state_set(1, 0);
      cancel_delayed_work_sync(&bq24160_chg->watch_dog_reset);
      bq24160_chg->chg_online = 0;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_REMOVED_EVENT);
			break;

#if 1
		case BQ24160_STAT_IN_READY:
      now_chg_state = BQ24160_CHG_STATE_CHARGING;
      
      if(msm_otg_get_chg_current() < 500)
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x02); // Charging Current: 625mA Termination Current: 150mA
      }
      else
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22);  // Charging Current: 850mA Termination Current: 150mA
      }

      rc = pm8058_batt_alarm_state_set(0, 0);
      bq24160_chg->chg_online = 1;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_INSERTED_EVENT);
      break;
#endif
      
		case BQ24160_STAT_USB_READY:
      now_chg_state = BQ24160_CHG_STATE_CHARGING;
      
      if(msm_otg_get_chg_current() < 500)
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x02); // Charging Current: 625mA Termination Current: 150mA
      }
      else
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22);  // Charging Current: 850mA Termination Current: 150mA
      }

      rc = pm8058_batt_alarm_state_set(0, 0);
      bq24160_chg->chg_online = 1;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_INSERTED_EVENT);
			break;

#if 1
		case BQ24160_STAT_CHARGING_FROM_IN:
      now_chg_state = BQ24160_CHG_STATE_CHARGING;
      
      if(msm_otg_get_chg_current() < 500)
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x02); // Charging Current: 625mA Termination Current: 150mA
      }
      else
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22);  // Charging Current: 850mA Termination Current: 150mA
      }

      rc = pm8058_batt_alarm_state_set(0, 0);
      bq24160_chg->chg_online = 1;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_INSERTED_EVENT);
      break;
#endif
     
		case BQ24160_STAT_CHARGING_FROM_USB:
      now_chg_state = BQ24160_CHG_STATE_CHARGING;
      
      if(msm_otg_get_chg_current() < 500)
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x02); // Charging Current: 625mA Termination Current: 150mA
      }
      else
      {
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
        bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22);  // Charging Current: 850mA Termination Current: 150mA
      }

      rc = pm8058_batt_alarm_state_set(0, 0);
      bq24160_chg->chg_online = 1;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_INSERTED_EVENT);
			break;
      
		case BQ24160_STAT_CHARGE_DONE:
      now_chg_state = BQ24160_CHG_STATE_DONE;
      
      //cancel_delayed_work_sync(&bq24160_chg->watch_dog_reset);
      bq24160_chg->chg_online = 1;
      rc = pm8058_batt_alarm_state_set(1, 0);
			msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_DONE_EVENT);
			break;
      
		case BQ24160_STAT_NA:
      now_chg_state = BQ24160_CHG_STATE_STOP;
      
      cancel_delayed_work_sync(&bq24160_chg->watch_dog_reset);
      bq24160_chg->chg_online = 0;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_REMOVED_EVENT);
			break;
      
		case BQ24160_STAT_FAULT:
      now_chg_state = BQ24160_CHG_STATE_STOP;
      
      cancel_delayed_work_sync(&bq24160_chg->watch_dog_reset);
      bq24160_chg->chg_online = 0;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_REMOVED_EVENT);
			break;
	}
}


static int __set_charger(struct bq24160_chip *chip, int enable)
{
    u8  reg_val= 0;

	if (enable) {
		/* enable charger */
    /* REG_#2  Set current limit and enable STAT in Control register */
		bq24160_write_reg(chip->client, BQ24160_REG_CTRL, 0x4D);
		//dev_dbg(&chip->client->dev, "%s REG_#2 =0x%x\n", __func__, reg_val);
    
    /* REG_#3  Set Battery Regulation Voltage */
		reg_val = ( (chip->pdata->vbatt_reg<< BQ24160_VBATTREG_SHFT) |
					(chip->pdata->inlimit_in << BQ24160_INLMT_IN_SHFT) |
					(chip->pdata->dpdm_en << BQ24160_DPDM_EN_SHFT) );
		bq24160_write_reg(chip->client, BQ24160_REG_CTRL_BATVOLT, reg_val);
		dev_dbg(&chip->client->dev, "%s REG_#3 =0x%x\n", __func__, reg_val);

    /* REG_#2  Set current limit and enable STAT in Control register */
		reg_val = ( (chip->pdata->reset << BQ24160_RESET_SHFT) |
					(chip->pdata->iusblimit << BQ24160_IUSB_LMT_SHFT) |
					(chip->pdata->enstat << BQ24160_ENSTAT_SHFT) |
					(chip->pdata->te << BQ24160_TE_SHFT) |
					(chip->pdata->ce << BQ24160_CE_SHFT) |
					(chip->pdata->hz_mode << BQ24160_HZMODE_SHFT) );
		bq24160_write_reg(chip->client, BQ24160_REG_CTRL, reg_val);
		dev_dbg(&chip->client->dev, "%s REG_#2 =0x%x\n", __func__, reg_val);

		/* REG_#0 */
		reg_val = ( (chip->pdata->tmr_rst << BQ24160_WDOG_TMR_SHFT) |
					(chip->pdata->supply_sel << BQ24160_SUPPLY_SEL_SHFT) );
		bq24160_write_reg(chip->client, BQ24160_REG_STAT_CTRL, reg_val);
		dev_dbg(&chip->client->dev, "%s REG_#0 =0x%x\n", __func__, reg_val);

		
		/* REG_#5  Set Battery Termination and Fast Chg current */
		reg_val = ( (chip->pdata->chgcrnt << BQ24160_ICHGCRNT_SHFT) |
					(chip->pdata->termcrnt << BQ24160_ITERMCRNT_SHFT) );
		bq24160_write_reg(chip->client, BQ24160_REG_BATTTERM_FCHGCRNT, reg_val);
		dev_dbg(&chip->client->dev, "%s REG_#5 =0x%x\n", __func__, reg_val);

		/* REG_#6  Set Vin-DPM voltage */
		reg_val = ( (chip->pdata->minsys_stat << BQ24160_MINSYS_STAT_SHFT) |
					(chip->pdata->dpm_stat << BQ24160_DPM_STAT_SHFT) |
					(chip->pdata->vindpm_usb << BQ24160_USB_INDPM_SHFT) |
					(chip->pdata->vindpm_in << BQ24160_IN_INDPM_SHFT) );
		bq24160_write_reg(chip->client, BQ24160_REG_VINDPM_STAT, reg_val);
		dev_dbg(&chip->client->dev, "%s REG_#6 =0x%x\n", __func__, reg_val);

		/* REG_#7  Set timer and monitor */
		reg_val = ( (chip->pdata->tmr2x_en << BQ24160_2XTMR_EN_SHFT) |
					(chip->pdata->safety_tmr << BQ24160_TMR_SHFT) |
					(chip->pdata->ts_en << BQ24160_TS_EN_SHFT) |
					(chip->pdata->ts_fault << BQ24160_TS_FAULT_SHFT) );
		bq24160_write_reg(chip->client, BQ24160_REG_SAFETMR_NTCMON, reg_val);
		dev_dbg(&chip->client->dev, "%s REG_#7 =0x%x\n", __func__, reg_val);
	} 
  else {
		/* disable charge */
	}
	dev_dbg(&chip->client->dev, "%s %s\n", __func__, (enable) ? "Enable charger" : "Disable charger");
	return 0;
}

static irqreturn_t bq24160_valid_handler(int irq, void *data)
{
  struct bq24160_chip *chip = (struct bq24160_chip *)data;

  printk(KERN_DEBUG "############ [bq24160_valid_handler] CHARGER INTERRUPT OCCURED #####################\n");

  schedule_delayed_work(&chip->watch_dog_reset, BQ24160_CHG_WATCH_DOG_RESET_PERIOD);
  
  schedule_delayed_work(&chip->charge_work, BQ24160_CHG_WORK_PERIOD);
  
  return IRQ_HANDLED;
}

static int bq24160_charger_get_property(struct power_supply *psy,
                                        enum power_supply_property psp,
                                        union power_supply_propval *val)
{
	struct bq24160_chip *chip = container_of(psy, struct bq24160_chip, charger);
	int ret = 0;
    int chg_status = 0;

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = chip->chg_online;
		break;
    case POWER_SUPPLY_PROP_STATUS:
		ret = bq24160_read_reg(chip->client, BQ24160_REG_STAT_CTRL);
		if(ret >= 0)
		{
        	chg_status = ret & BQ24160_STAT_MASK;
			chg_status = chg_status >> BQ24160_STAT_SHFT;
		}
        if (chip->chg_online) {
            if (chg_status == BQ24160_STAT_CHARGE_DONE) {
                val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
            }
            else if ((chg_status == BQ24160_STAT_CHARGING_FROM_IN) ||
                     (chg_status == BQ24160_STAT_CHARGING_FROM_USB) ) {
                val->intval = POWER_SUPPLY_STATUS_CHARGING;
            }
            else {
                val->intval = POWER_SUPPLY_STATUS_NOT_CHARGING;
            }
        }
        else {
            val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
        }
        ret = 0;
		break;	
    default:
		ret = -ENODEV;
		break;
	}
	return ret;
}

bool bq24160_charger_get_type(void)
{
  if(now_chg_state == BQ24160_CHG_STATE_STOP || now_chg_state == BQ24160_CHG_STATE_DONE || now_chg_state == BQ24160_CHG_STATE_NO_CABLE)
    return false;
  else if(now_chg_state == BQ24160_CHG_STATE_CHARGING)
    return true;
  else
    return false;
}
EXPORT_SYMBOL(bq24160_charger_get_type);

static enum power_supply_property bq24160_charger_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_STATUS,
};

static struct power_supply bq24160_charger_ps = {
   .name = "bq24160-charger",
   .type = POWER_SUPPLY_TYPE_MAINS,
   .properties = bq24160_charger_props,
   .num_properties = ARRAY_SIZE(bq24160_charger_props),
   .get_property = bq24160_charger_get_property,
};

#ifdef CONFIG_LGE_SWITCHING_CHARGER_BQ24160_DOCOMO_ONLY
/*=========================================================================*/

extern int pm8058_get_battery_mvolts(void);

extern int batt_read_adc(int channel, int *mv_reading);

extern int pm8058_get_battery_temperature(void);

extern int pm8058_is_battery_present(void);

extern int pm8058_is_battery_temp_within_range(void);

extern int pm8058_is_battery_id_valid(void);


#define BATT_THERM_OPEN_MV  2000
static int bq24160_is_battery_present(void)
{
	int mv_reading;

	mv_reading = 0;
	batt_read_adc(CHANNEL_ADC_BATT_THERM, &mv_reading);
	pr_info("%s: therm_raw is %d\n", __func__, mv_reading);
#ifndef CONFIG_LGE_FUEL_GAUGE
	if (mv_reading > 0 && mv_reading < BATT_THERM_OPEN_MV)
#endif
		return 1;

	return 0;
}


#ifdef CONFIG_LGE_CHARGER_TEMP_SCENARIO
#define BATT_THERM_OPERATIONAL_MAX_CELCIUS 100
#else
#define BATT_THERM_OPERATIONAL_MAX_CELCIUS 40
#endif
#define BATT_THERM_OPERATIONAL_MIN_CELCIUS 0
static int bq24160_is_battery_temp_within_range(void)
{
	int therm_celcius;

	therm_celcius = bq24160_is_battery_present();
	pr_info("%s: therm_celcius is %d\n", __func__, therm_celcius);
#ifndef CONFIG_LGE_FUEL_GAUGE 
	if (therm_celcius > 0
		&& therm_celcius > BATT_THERM_OPERATIONAL_MIN_CELCIUS
		&& therm_celcius < BATT_THERM_OPERATIONAL_MAX_CELCIUS)
#endif
		return 1;

	return 0;
}


void bq24160_at_cmd_chg_set(bool set_value)
{
  at_cmd_chg = set_value;
}
EXPORT_SYMBOL(bq24160_at_cmd_chg_set);


bool bq24160_at_cmd_chg_get(void)
{
  return at_cmd_chg;
}


static struct msm_battery_gauge bq24160_batt_gauge = {
#ifdef CONFIG_LGE_FUEL_GAUGE
	.get_battery_mvolts = pm8058_get_battery_mvolts,
#endif
	.get_battery_temperature = pm8058_get_battery_temperature,
	.is_battery_present = pm8058_is_battery_present,
	.is_battery_temp_within_range = bq24160_is_battery_temp_within_range,
	.is_battery_id_valid = pm8058_is_battery_id_valid,
};
/*=======================================================================*/

static int bq24160_start_charging(struct msm_hardware_charger *hw_chg,
		int chg_voltage, int chg_current)
{
	struct bq24160_chip *bq24160_chg;
	int ret = 0;
  u8  reg_val= 0;

	bq24160_chg = container_of(hw_chg, struct bq24160_chip, adapter_hw_chg);

  printk(KERN_DEBUG "############ [bq24160_start_charging] Online = %d AT CMD = %d Resume = %d #####################\n", 
    bq24160_chg->chg_online, bq24160_at_cmd_chg_get(), resume_chg_start);
  
	if (bq24160_chg->chg_online && !bq24160_at_cmd_chg_get())
  { 
    if(resume_chg_start)
    {
      /* Watchdog timer reset */
	    bq24160_set_bits(bq24160_chg->client, BQ24160_REG_STAT_CTRL, BQ24160_WDOG_TMR_MASK, (1<<BQ24160_WDOG_TMR_SHFT));

      /* REG_#2  Set current limit and enable STAT in Control register */
		  reg_val = ( (0b0 << BQ24160_RESET_SHFT) |
					(0b100 << BQ24160_IUSB_LMT_SHFT) |
					(0b1 << BQ24160_ENSTAT_SHFT) |
					(0b1 << BQ24160_TE_SHFT) |
					(0b0 << BQ24160_CE_SHFT) |
					(0b0 << BQ24160_HZMODE_SHFT) );
		  bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL, reg_val);

      bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
      bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22);  // Charging Current: 850mA Termination Current: 100mA

      bq24160_chg->chg_online = 1;
      msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_INSERTED_EVENT);
    
      resume_chg_start = false;
    }
    else
    {
		  /* we are already charging */
		  return 0;
    }
  }
  else
  {
    /* Watchdog timer reset */
	  bq24160_set_bits(bq24160_chg->client, BQ24160_REG_STAT_CTRL, BQ24160_WDOG_TMR_MASK, (1<<BQ24160_WDOG_TMR_SHFT));

    /* REG_#2  Set current limit and enable STAT in Control register */
		reg_val = ( (0b0 << BQ24160_RESET_SHFT) |
					(0b100 << BQ24160_IUSB_LMT_SHFT) |
					(0b1 << BQ24160_ENSTAT_SHFT) |
					(0b1 << BQ24160_TE_SHFT) |
					(0b0 << BQ24160_CE_SHFT) |
					(0b0 << BQ24160_HZMODE_SHFT) );
		bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL, reg_val);

    bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL_BATVOLT, BQ24160_VBATTREG_MASK); // Batt Regulation Volt: 4.38V
    bq24160_write_reg(bq24160_chg->client, BQ24160_REG_BATTTERM_FCHGCRNT, 0x22);  // Charging Current: 850mA Termination Current: 100mA

    bq24160_chg->chg_online = 1;
    msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_INSERTED_EVENT);

    schedule_delayed_work(&bq24160_chg->watch_dog_reset, BQ24160_CHG_WATCH_DOG_RESET_PERIOD);
  }

	dev_dbg(&bq24160_chg->client->dev, "%s\n", __func__);
	return ret;
}

static int bq24160_stop_charging(struct msm_hardware_charger *hw_chg)
{
	struct bq24160_chip *bq24160_chg;
	int ret = 0;

	bq24160_chg = container_of(hw_chg, struct bq24160_chip, adapter_hw_chg);
	if (!(bq24160_chg->chg_online)  && !bq24160_at_cmd_chg_get())
		/* we arent charging */
		return 0;
  else
  {
    u8  reg_val= 0;
    
    /* Watchdog timer reset */
	  bq24160_set_bits(bq24160_chg->client, BQ24160_REG_STAT_CTRL, BQ24160_WDOG_TMR_MASK, (1<<BQ24160_WDOG_TMR_SHFT));

    /* REG_#2  Set current limit and enable STAT in Control register */
		reg_val = ( (0b0 << BQ24160_RESET_SHFT) |
					(0b100 << BQ24160_IUSB_LMT_SHFT) |
					(0b1 << BQ24160_ENSTAT_SHFT) |
					(0b1 << BQ24160_TE_SHFT) |
					(0b0 << BQ24160_CE_SHFT) |
					(0b0 << BQ24160_HZMODE_SHFT) );
		bq24160_write_reg(bq24160_chg->client, BQ24160_REG_CTRL, reg_val);

    bq24160_chg->chg_online = 0;
    msm_charger_notify_event(&bq24160_chg->adapter_hw_chg, CHG_REMOVED_EVENT);

    cancel_delayed_work_sync(&bq24160_chg->watch_dog_reset);
  }

	dev_dbg(&bq24160_chg->client->dev, "%s\n", __func__);
	return ret;
}

static int bq24160_charging_switched(struct msm_hardware_charger *hw_chg)
{
	struct bq24160_chip *bq24160_chg;

	bq24160_chg = container_of(hw_chg, struct bq24160_chip, adapter_hw_chg);
	dev_dbg(&bq24160_chg->client->dev, "%s\n", __func__);
	return 0;
}

#endif

static __devinit int bq24160_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{

	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	struct bq24160_chip *chip;
  int ret = 0;

  printk(KERN_DEBUG "############ [bq24160_probe]: BQ24160 CHARGER PROBE START #####################\n");

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE))
		return -EIO;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	INIT_DELAYED_WORK(&chip->charge_work, bq24160_charge);
  INIT_DELAYED_WORK(&chip->watch_dog_reset, bq24160_watchdog_reset);
  INIT_DELAYED_WORK(&chip->charge_start_from_temp_senario, bq24160_start_chg_from_temp_senario);
  INIT_DELAYED_WORK(&chip->charge_stop_from_temp_senario, bq24160_stop_chg_from_temp_senario);

	chip->client = client;
	chip->pdata = client->dev.platform_data;

	i2c_set_clientdata(client, chip);

#ifdef CONFIG_LGE_SWITCHING_CHARGER_BQ24160_DOCOMO_ONLY
	/* fill hw_chg structure for registering msm_charger */
	chip->adapter_hw_chg.type = CHG_TYPE_AC;
	chip->adapter_hw_chg.rating = 1;
	chip->adapter_hw_chg.name = "bq24160-adapter";
	chip->adapter_hw_chg.start_charging = bq24160_start_charging;
	chip->adapter_hw_chg.stop_charging = bq24160_stop_charging;
	chip->adapter_hw_chg.charging_switched = bq24160_charging_switched;
	
	ret = msm_charger_register(&chip->adapter_hw_chg);
	if (ret) {
		dev_err(&client->dev,
			"%s msm_charger_register failed for ret =%d\n",
			__func__, ret);
		goto out;
	}

  msm_battery_gauge_register(&bq24160_batt_gauge);

	bq24160_irq_init(SWITCHING_CHG_IRQ_N);
#endif


	ret = request_threaded_irq(client->irq, NULL, bq24160_valid_handler,
				   IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, client->name, chip);


  ret = pm8058_batt_alarm_state_set(0, 0);
	if (ret) {
		pr_err("%s: unable to set batt alarm state\n", __func__);
		goto out;
	}

	/*
	 * The batt-alarm driver requires sane values for both min / max,
	 * regardless of whether they're both activated.
	 */
	ret = pm8058_batt_alarm_threshold_set(BQ24160_RESUME_CHG_VOLT, 4300);
	if (ret) {
		pr_err("%s: unable to set batt alarm threshold\n", __func__);
		goto out;
	}

	ret = pm8058_batt_alarm_hold_time_set(PM8058_BATT_ALARM_HOLD_TIME_16_MS);
	if (ret) {
		pr_err("%s: unable to set batt alarm hold time\n", __func__);
		goto out;
	}

	/* PWM enabled at 2Hz */
	ret = pm8058_batt_alarm_pwm_rate_set(1, 7, 4);
	if (ret) {
		pr_err("%s: unable to set batt alarm pwm rate\n", __func__);
		goto out;
	}

	ret = pm8058_batt_alarm_register_notifier(&alarm_notifier);
	if (ret) {
		pr_err("%s: unable to register alarm notifier\n", __func__);
		goto out;
	}

	chip->chg_online = 0;
	__set_charger(chip, 1);	//just for test


  schedule_delayed_work(&chip->charge_work, BQ24160_CHG_WORK_PERIOD);
  
	return 0;
out:
	kfree(chip);
	return ret;
}

static __devexit int bq24160_remove(struct i2c_client *client)
{
  int rc = 0;
  struct bq24160_chip *chip = i2c_get_clientdata(client);

	free_irq(client->irq, chip);
  cancel_delayed_work_sync(&chip->watch_dog_reset);
	power_supply_unregister(&bq24160_charger_ps);
	kfree(chip);

  rc = pm8058_batt_alarm_state_set(0, 0);

	return 0;
}

static const struct i2c_device_id bq24160_id[] = {
	{ "bq24160", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, bq24160_id);

static struct i2c_driver bq24160_i2c_driver = {
	.driver = {
		.name = "bq24160",
	},
	.probe		= bq24160_probe,
	.remove		= __devexit_p(bq24160_remove),
	.id_table	= bq24160_id,
};

static int __init bq24160_init(void)
{
	return i2c_add_driver(&bq24160_i2c_driver);
}
module_init(bq24160_init);

static void __exit bq24160_exit(void)
{
	i2c_del_driver(&bq24160_i2c_driver);
}
module_exit(bq24160_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sungwook Park <sungwook.park@lge.com>");
MODULE_DESCRIPTION("Power supply driver for BQ24160");
MODULE_ALIAS("platform:bq24160-charger");
