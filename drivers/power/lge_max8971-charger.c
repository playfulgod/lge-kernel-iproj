/* 
 *  MAXIM MAX8971 Charger Driver
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/max8971-charger.h>
#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
#include <mach/gpio.h>
#include <linux/msm-charger.h>
#include <linux/msm_adc.h>
#endif

// define register map
#define MAX8971_REG_CHGINT      0x0F

#define MAX8971_REG_CHGINT_MASK 0x01

#define MAX8971_REG_CHG_STAT    0x02

#define MAX8971_DCV_MASK        0x80
#define MAX8971_DCV_SHIFT       7
#define MAX8971_DCI_MASK        0x40
#define MAX8971_DCI_SHIFT       6
#define MAX8971_DCOVP_MASK      0x20
#define MAX8971_DCOVP_SHIFT     5
#define MAX8971_DCUVP_MASK      0x10
#define MAX8971_DCUVP_SHIFT     4
#define MAX8971_CHG_MASK        0x08
#define MAX8971_CHG_SHIFT       3
#define MAX8971_BAT_MASK        0x04
#define MAX8971_BAT_SHIFT       2
#define MAX8971_THM_MASK        0x02
#define MAX8971_THM_SHIFT       1
#define MAX8971_PWRUP_OK_MASK   0x01
#define MAX8971_PWRUP_OK_SHIFT  0
#define MAX8971_I2CIN_MASK      0x01
#define MAX8971_I2CIN_SHIFT     0

#define MAX8971_REG_DETAILS1    0x03
#define MAX8971_DC_V_MASK       0x80
#define MAX8971_DC_V_SHIFT      7
#define MAX8971_DC_I_MASK       0x40
#define MAX8971_DC_I_SHIFT      6
#define MAX8971_DC_OVP_MASK     0x20
#define MAX8971_DC_OVP_SHIFT    5
#define MAX8971_DC_UVP_MASK     0x10
#define MAX8971_DC_UVP_SHIFT    4
#define MAX8971_THM_DTLS_MASK   0x07
#define MAX8971_THM_DTLS_SHIFT  0

#define MAX8971_THM_DTLS_COLD   1       // charging suspended(temperature<T1)
#define MAX8971_THM_DTLS_COOL   2       // (T1<temperature<T2)
#define MAX8971_THM_DTLS_NORMAL 3       // (T2<temperature<T3)
#define MAX8971_THM_DTLS_WARM   4       // (T3<temperature<T4)
#define MAX8971_THM_DTLS_HOT    5       // charging suspended(temperature>T4)

#define MAX8971_REG_DETAILS2    0x04
#define MAX8971_BAT_DTLS_MASK   0x30
#define MAX8971_BAT_DTLS_SHIFT  4
#define MAX8971_CHG_DTLS_MASK   0x0F
#define MAX8971_CHG_DTLS_SHIFT  0

#define MAX8971_BAT_DTLS_BATDEAD        0   // VBAT<2.1V
#define MAX8971_BAT_DTLS_TIMER_FAULT    1   // The battery is taking longer than expected to charge
#define MAX8971_BAT_DTLS_BATOK          2   // VBAT is okay.
#define MAX8971_BAT_DTLS_GTBATOV        3   // VBAT > BATOV

#define MAX8971_CHG_DTLS_DEAD_BAT           0   // VBAT<2.1V, TJSHDN<TJ<TJREG
#define MAX8971_CHG_DTLS_PREQUAL            1   // VBAT<3.0V, TJSHDN<TJ<TJREG
#define MAX8971_CHG_DTLS_FAST_CHARGE_CC     2   // VBAT>3.0V, TJSHDN<TJ<TJREG
#define MAX8971_CHG_DTLS_FAST_CHARGE_CV     3   // VBAT=VBATREG, TJSHDN<TJ<TJREG
#define MAX8971_CHG_DTLS_TOP_OFF            4   // VBAT>=VBATREG, TJSHDN<TJ<TJREG
#define MAX8971_CHG_DTLS_DONE               5   // VBAT>VBATREG, T>Ttopoff+16s, TJSHDN<TJ<TJREG
#define MAX8971_CHG_DTLS_TIMER_FAULT        6   // VBAT<VBATOV, TJ<TJSHDN
#define MAX8971_CHG_DTLS_TEMP_SUSPEND       7   // TEMP<T1 or TEMP>T4
#define MAX8971_CHG_DTLS_USB_SUSPEND        8   // charger is off, DC is invalid or chaarger is disabled(USBSUSPEND)
#define MAX8971_CHG_DTLS_THERMAL_LOOP_ACTIVE    9   // TJ > REGTEMP
#define MAX8971_CHG_DTLS_CHG_OFF            10  // charger is off and TJ >TSHDN

#define MAX8971_REG_CHGCNTL1    0x05
#define MAX8971_DCMON_DIS_MASK  0x02
#define MAX8971_DCMON_DIS_SHIFT 1
#define MAX8971_USB_SUS_MASK    0x01
#define MAX8971_USB_SUS_SHIFT   0

#define MAX8971_REG_FCHGCRNT    0x06
#define MAX8971_CHGCC_MASK      0x1F
#define MAX8971_CHGCC_SHIFT     0
#define MAX8971_FCHGTIME_MASK   0xE0
#define MAX8971_FCHGTIME_SHIFT  5


#define MAX8971_REG_DCCRNT      0x07
#define MAX8971_CHGRSTRT_MASK   0x40
#define MAX8971_CHGRSTRT_SHIFT  6
#define MAX8971_DCILMT_MASK     0x3F
#define MAX8971_DCILMT_SHIFT    0

#define MAX8971_REG_TOPOFF          0x08
#define MAX8971_TOPOFFTIME_MASK     0xE0
#define MAX8971_TOPOFFTIME_SHIFT    5
#define MAX8971_IFST2P8_MASK        0x10
#define MAX8971_IFST2P8_SHIFT       4
#define MAX8971_TOPOFFTSHLD_MASK    0x0C
#define MAX8971_TOPOFFTSHLD_SHIFT   2
#define MAX8971_CHGCV_MASK          0x03
#define MAX8971_CHGCV_SHIFT         0

#define MAX8971_REG_TEMPREG     0x09
#define MAX8971_REGTEMP_MASK    0xC0
#define MAX8971_REGTEMP_SHIFT   6
#define MAX8971_THM_CNFG_MASK   0x20
#define MAX8971_THM_CNFG_SHIFT  5
#define MAX8971_SAFETYREG_MASK  0x01
#define MAX8971_SAFETYREG_SHIFT 0

#define MAX8971_REG_PROTCMD     0x0A
#define MAX8971_CHGPROT_MASK    0x0C
#define MAX8971_CHGPROT_SHIFT   2


#define SWITCHING_CHG_IRQ_N	124


struct max8971_chip {
	struct i2c_client *client;
	struct power_supply charger;
	struct max8971_platform_data *pdata;
    int irq;
    int chg_online;
#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
	struct msm_hardware_charger	adapter_hw_chg;
#endif
};

#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
static void max8971_irq_init(int gpio_num)
{
	int rc;
  
	rc = gpio_request(gpio_num, "max8971_irq_gpio");
	if(rc < 0){
		pr_err("[S-CHG]Can't get max8971-charger irq gpio\n");
		return ;
	}
	gpio_direction_input(gpio_num);
	gpio_set_value(gpio_num, 1);
}
#endif


static int max8971_write_reg(struct i2c_client *client, u8 reg, u8 value)
{
	int ret = i2c_smbus_write_byte_data(client, reg, value);

	if (ret < 0)
		dev_err(&client->dev, "[S-CHG]%s: err %d\n", __func__, ret);

	return ret;
}



#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
#define I2C_MAX_ERR_COUNT  5
static int switching_charger_i2c_error = 0;

struct max8971_chip *g_max8971_chg;
#endif

static int max8971_read_reg(struct i2c_client *client, u8 reg)
{
	int ret = i2c_smbus_read_byte_data(client, reg);

	if (ret < 0)
  { 
		dev_err(&client->dev, "[S-CHG]%s: err %d count=%d\n", __func__, ret, switching_charger_i2c_error);

#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
    switching_charger_i2c_error++;

    if(switching_charger_i2c_error > I2C_MAX_ERR_COUNT)
    {      
      dev_err(&client->dev, "[S-CHG][SEND CHG REMOVE EVT]%s: err %d count=%d\n", __func__, ret, switching_charger_i2c_error);
      
      msm_charger_notify_event(&g_max8971_chg->adapter_hw_chg, CHG_REMOVED_EVENT);
      switching_charger_i2c_error = 0;

      disable_irq(client->irq);
    }
#endif
  }

	return ret;
}


static int __set_charger(struct max8971_chip *chip, int enable)
{
    u8  reg_val= 0;

    // unlock charger protection
    reg_val = MAX8971_CHGPROT_UNLOCKED<<MAX8971_CHGPROT_SHIFT;
    max8971_write_reg(chip->client, MAX8971_REG_PROTCMD, reg_val);   

	if (enable) {
		/* enable charger */

        // Set fast charge current and timer
        reg_val = ((chip->pdata->chgcc<<MAX8971_CHGCC_SHIFT) |
                   (chip->pdata->fchgtime<<MAX8971_FCHGTIME_SHIFT));
        max8971_write_reg(chip->client, MAX8971_REG_FCHGCRNT, reg_val);

        // Set input current limit and charger restart threshold
        reg_val = ((chip->pdata->chgrstrt<<MAX8971_CHGRSTRT_SHIFT) |
                   (chip->pdata->dcilmt<<MAX8971_DCILMT_SHIFT));
        max8971_write_reg(chip->client, MAX8971_REG_DCCRNT, reg_val);

        // Set topoff condition
        reg_val = ((chip->pdata->topofftime<<MAX8971_TOPOFFTIME_SHIFT) |
                   (chip->pdata->topofftshld<<MAX8971_TOPOFFTSHLD_SHIFT) |
                   (chip->pdata->chgcv<<MAX8971_CHGCV_SHIFT));
        max8971_write_reg(chip->client, MAX8971_REG_TOPOFF, reg_val);

        // Set temperature condition
        reg_val = ((chip->pdata->regtemp<<MAX8971_REGTEMP_SHIFT) |
                   (chip->pdata->thm_config<<MAX8971_THM_CNFG_SHIFT) |
                   (chip->pdata->safetyreg<<MAX8971_SAFETYREG_SHIFT));
        max8971_write_reg(chip->client, MAX8971_REG_TEMPREG, reg_val);       

        // USB Suspend
//        max8971_set_bits(chip->client, MAX8971_REG_CHGCNTL1, MAX8971_USB_SUS_MASK, 1);
//		dev_info(&chip->client->dev, "USB Suspend\n");
	} else {
		/* disable charge */
//        max8971_set_bits(chip->client, MAX8971_REG_CHGCNTL1, MAX8971_USB_SUS_MASK, 0);
	}
	dev_info(&chip->client->dev, "[S-CHG]%s\n", (enable) ? "Enable charger" : "Disable charger");
	return 0;
}


#if 0
static int max8971_set_bits(struct i2c_client *client, u8 reg, u8 mask, u8 data)
{
	u8 value = 0;
	int ret;

	ret = max8971_read_reg(client, reg);
	if (ret < 0)
		goto out;
	value &= ~mask;
	value |= data;
	ret = max8971_write_reg(client, reg, value);
out:
	return ret;
}
#endif


#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
int max8971_set_start_charging(void)
{
#if 0
  printk(KERN_DEBUG "############ [max8971_set_start_charging]: MAX8971 START CHARGING #####################\n");
  
  g_max8971_chg->chg_online = 0;
  __set_charger(g_max8971_chg, 1); 

  msm_charger_notify_event(&g_max8971_chg->adapter_hw_chg, CHG_INSERTED_EVENT);
#endif

  return 0;
}
EXPORT_SYMBOL(max8971_set_start_charging);


int max8971_set_stop_charging(void)
{
#if 0
  printk(KERN_DEBUG "############ [max8971_set_stop_charging]: MAX8971 STOP CHARGING #####################\n");
  
  g_max8971_chg->chg_online = 1;
  __set_charger(g_max8971_chg, 0);

  msm_charger_notify_event(&g_max8971_chg->adapter_hw_chg, CHG_REMOVED_EVENT);
#endif
          
  return 0;
}
EXPORT_SYMBOL(max8971_set_stop_charging);
#endif


static int max8971_charger_detail_irq(int irq, void *data)
{
	struct max8971_chip *chip = (struct max8971_chip *)data;
    int val;

    printk(KERN_DEBUG "############ [max8971_charger_detail_irq]: IRQ = %d #####################\n", irq);
    
    switch (irq) {
    case MAX8971_IRQ_PWRUP_OK:
#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971  
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS1);
        if ((val & MAX8971_DC_V_MASK) == 0) 
        {
          g_max8971_chg = chip;
            
    		  chip->chg_online = 1;
			    __set_charger(chip, 1);  
			    dev_info(&chip->client->dev, "[S-CHG]Valid Charger status: details-0x%x\n", val);

			    msm_charger_notify_event(&chip->adapter_hw_chg, CHG_INSERTED_EVENT);
        }
        else 
        {
          // VBUS is invalid. VDC > 4.5
          chip->chg_online = 0;
          __set_charger(chip, 0);
			    dev_info(&chip->client->dev, "[S-CHG]VBUS is invalid\n");

			    msm_charger_notify_event(&chip->adapter_hw_chg, CHG_REMOVED_EVENT);
        }
#endif
        dev_info(&chip->client->dev, "[S-CHG]Power Up OK Interrupt\n");
        break;
    case MAX8971_IRQ_THM:
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS1);
        dev_info(&chip->client->dev, "[S-CHG]Thermistor Interrupt: details-0x%x\n", (val & MAX8971_THM_DTLS_MASK));
        break;
    case MAX8971_IRQ_BAT:
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS2);
        dev_info(&chip->client->dev, "[S-CHG]Battery Interrupt: details-0x%x\n", (val & MAX8971_BAT_DTLS_MASK));
        break;
    case MAX8971_IRQ_CHG:
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS2);
        dev_info(&chip->client->dev, "[S-CHG]Fast Charge Interrupt: details-0x%x\n", (val & MAX8971_CHG_DTLS_MASK));
        break;
    case MAX8971_IRQ_DCUVP:
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS1);
        if (val & MAX8971_DC_UVP_MASK) {
            // VBUS is invalid. VDC < VDC_UVLO
            __set_charger(chip, 0);
        }
        dev_info(&chip->client->dev, "[S-CHG]DC Under voltage Interrupt: details-0x%x\n", (val & MAX8971_DC_UVP_MASK));
        break;
    case MAX8971_IRQ_DCOVP:
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS1);
        if (val & MAX8971_DC_OVP_MASK) {
            // VBUS is invalid. VDC > VDC_OVLO
            __set_charger(chip, 0);
        }
        dev_info(&chip->client->dev, "[S-CHG]DC Over voltage Interrupt: details-0x%x\n", (val & MAX8971_DC_OVP_MASK));
        break;
    case MAX8971_IRQ_DCI:
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS1);
        dev_info(&chip->client->dev, "[S-CHG]DC Input Current Limit Interrupt: details-0x%x\n", (val & MAX8971_DC_I_MASK));
        break;
    case MAX8971_IRQ_DCV:
        /* Temporary disabled becuase of MAXIM8971 IC problem.
                  This services is implemented at "MAX8971_IRQ_PWRUP_OK" interrupt. */
#ifndef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
        val = max8971_read_reg(chip->client, MAX8971_REG_DETAILS1);
        if ((val & MAX8971_DC_V_MASK) == 0) 
        {
    		  chip->chg_online = 1;
			    __set_charger(chip, 1);  
			    dev_info(&chip->client->dev, "[S-CHG]Valid Charger status: details-0x%x\n", val);
#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
			    msm_charger_notify_event(&chip->adapter_hw_chg, CHG_INSERTED_EVENT);
#endif
        }
        else 
        {
          // VBUS is invalid. VDC > 4.5
          chip->chg_online = 0;
          __set_charger(chip, 0);
			    dev_info(&chip->client->dev, "[S-CHG]VBUS is invalid\n");
#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
			    msm_charger_notify_event(&chip->adapter_hw_chg, CHG_REMOVED_EVENT);
#endif
        }
        dev_info(&chip->client->dev, "[S-CHG]DC Input Voltage Limit Interrupt: details-0x%x\n", (val & MAX8971_DC_V_MASK));
#endif
        break;
    }
    return 0;
}


static irqreturn_t max8971_charger_handler(int irq, void *data)
{
	struct max8971_chip *chip = (struct max8971_chip *)data;
	int irq_val, irq_mask, irq_name;

	irq_val = max8971_read_reg(chip->client, MAX8971_REG_CHGINT);
    irq_mask = max8971_read_reg(chip->client, MAX8971_REG_CHGINT_MASK);

	dev_info(&chip->client->dev, "[S-CHG]max8971_charger_handler irq_val = %x irq_mask = %x\n", irq_val, irq_mask );

    for (irq_name = MAX8971_IRQ_PWRUP_OK; irq_name<MAX8971_NR_IRQS; irq_name++) {
        if ((irq_val & (0x01<<irq_name)) && !(irq_mask & (0x01<<irq_name))) {
            max8971_charger_detail_irq(irq_name, data);
        }
    }
	return IRQ_HANDLED;
}


static int max8971_charger_get_property(struct power_supply *psy,
                                        enum power_supply_property psp,
                                        union power_supply_propval *val)
{
	struct max8971_chip *chip = container_of(psy, struct max8971_chip, charger);
	int ret = 0;
    int chg_dtls_val;

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = chip->chg_online;
		break;
    case POWER_SUPPLY_PROP_STATUS:
		ret = max8971_read_reg(chip->client, MAX8971_REG_DETAILS2);
        chg_dtls_val = (ret & MAX8971_CHG_DTLS_MASK);
        if (chip->chg_online) {
            if (chg_dtls_val == MAX8971_CHG_DTLS_DONE) {
                val->intval = POWER_SUPPLY_STATUS_DISCHARGING;
            }
            else if ((chg_dtls_val == MAX8971_CHG_DTLS_TIMER_FAULT) ||
                     (chg_dtls_val == MAX8971_CHG_DTLS_TEMP_SUSPEND) ||
                     (chg_dtls_val == MAX8971_CHG_DTLS_USB_SUSPEND) ||
                     (chg_dtls_val == MAX8971_CHG_DTLS_CHG_OFF)) {
                val->intval = POWER_SUPPLY_STATUS_NOT_CHARGING;
            }
            else {
                val->intval = POWER_SUPPLY_STATUS_CHARGING;
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

static enum power_supply_property max8971_charger_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_STATUS,
};

static struct power_supply max8971_charger_ps = {
   .name = "max8971-charger",
   .type = POWER_SUPPLY_TYPE_MAINS,
   .properties = max8971_charger_props,
   .num_properties = ARRAY_SIZE(max8971_charger_props),
   .get_property = max8971_charger_get_property,
};

#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
/*=========================================================================*/
#ifdef CONFIG_LGE_FUEL_GAUGE
extern int max17040_get_battery_mvolts(void);
#endif


static int batt_read_adc(int channel, int *mv_reading)
{
	int ret;
	void *h;
	struct adc_chan_result adc_chan_result;
	struct completion  conv_complete_evt;

	pr_debug("%s: called for %d\n", __func__, channel);
	ret = adc_channel_open(channel, &h);
	if (ret) {
		pr_err("%s: couldnt open channel %d ret=%d\n",
					__func__, channel, ret);
		goto out;
	}
	init_completion(&conv_complete_evt);
	ret = adc_channel_request_conv(h, &conv_complete_evt);
	if (ret) {
		pr_err("%s: couldnt request conv channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}
	wait_for_completion(&conv_complete_evt);
	ret = adc_channel_read_result(h, &adc_chan_result);
	if (ret) {
		pr_err("%s: couldnt read result channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}
	ret = adc_channel_close(h);
	if (ret) {
		pr_err("%s: couldnt close channel %d ret=%d\n",
						__func__, channel, ret);
	}
	if (mv_reading)
		*mv_reading = adc_chan_result.measurement;

	pr_debug("%s: done for %d\n", __func__, channel);
	return adc_chan_result.physical;
out:
	pr_debug("%s: done for %d\n", __func__, channel);
	return -EINVAL;

}


static int max8971_get_battery_temperature(void)
{
	return batt_read_adc(CHANNEL_ADC_BATT_THERM, NULL);
}


#define BATT_THERM_OPEN_MV  2000
static int max8971_is_battery_present(void)
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
static int max8971_is_battery_temp_within_range(void)
{
	int therm_celcius;

	therm_celcius = max8971_get_battery_temperature();
	pr_info("%s: therm_celcius is %d\n", __func__, therm_celcius);
#ifndef CONFIG_LGE_FUEL_GAUGE 
	if (therm_celcius > 0
		&& therm_celcius > BATT_THERM_OPERATIONAL_MIN_CELCIUS
		&& therm_celcius < BATT_THERM_OPERATIONAL_MAX_CELCIUS)
#endif
		return 1;

	return 0;
}


#define BATT_UNKNOWN    0
static int max8971_is_battery_id_valid(void)
{
    extern uint16_t battery_info_get(void);
    if(BATT_UNKNOWN == battery_info_get()) 
	    return 0;
    else
      return 1;
}



static struct msm_battery_gauge max8971_batt_gauge = {
#ifdef CONFIG_LGE_FUEL_GAUGE
	.get_battery_mvolts = max17040_get_battery_mvolts,
#endif
	.get_battery_temperature = max8971_get_battery_temperature,
	.is_battery_present = max8971_is_battery_present,
	.is_battery_temp_within_range = max8971_is_battery_temp_within_range,
	.is_battery_id_valid = max8971_is_battery_id_valid,
};
/*=======================================================================*/

static int max8971_start_charging(struct msm_hardware_charger *hw_chg,
		int chg_voltage, int chg_current)
{
	struct max8971_chip *max8971_chg;
	int ret = 0;

	max8971_chg = container_of(hw_chg, struct max8971_chip, adapter_hw_chg);

  printk(KERN_DEBUG "############ [max8971_start_charging]: ONLINE = %d #####################\n", max8971_chg->chg_online);
  
	if (max8971_chg->chg_online)
  { 
    dev_dbg(&max8971_chg->client->dev, "[S-CHG]%s - We are already charging!!!\n", __func__);
		/* we are already charging */
		return 0;
  }

#ifndef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
  max8971_irq_init(SWITCHING_CHG_IRQ_N);

  ret = request_threaded_irq(max8971_chg->irq, NULL, max8971_charger_handler,
            IRQF_ONESHOT | IRQF_TRIGGER_LOW, max8971_chg->client->name, max8971_chg);
  if (unlikely(ret < 0))
  {
    pr_debug("[S-CHG]max8971: failed to request IRQ	%X\n", ret);
    return 0;
  }

  ret = max8971_read_reg(max8971_chg->client, MAX8971_REG_CHG_STAT);
	if (ret >= 0) {
		max8971_chg->chg_online = (ret & MAX8971_DCV_MASK) ? 1 : 0;
	}

  // Set IRQ MASK register
  ret = max8971_write_reg(max8971_chg->client, MAX8971_REG_CHGINT_MASK, max8971_chg->pdata->int_mask);

  dev_err(&max8971_chg->client->dev, "[S-CHG]%s\n", __func__);

  ret = max8971_write_reg(max8971_chg->client, MAX8971_REG_FCHGCRNT, 0x4c);

  ret = max8971_write_reg(max8971_chg->client, MAX8971_REG_DCCRNT, 0x3f);

  ret = max8971_write_reg(max8971_chg->client, MAX8971_REG_TOPOFF, 0x6e);

  ret = max8971_read_reg(max8971_chg->client, 0x06);
  dev_err(&max8971_chg->client->dev, "[S-CHG]0x%x - [FCHGCRNT]!!!\n", ret);

  ret = max8971_read_reg(max8971_chg->client, 0x07);
  dev_err(&max8971_chg->client->dev, "[S-CHG]0x%x - [DCCRNT]!!!\n", ret);

  ret = max8971_read_reg(max8971_chg->client, 0x08);
  dev_err(&max8971_chg->client->dev, "[S-CHG]0x%x - [TOPOFF]!!!\n", ret);

  ret = max8971_read_reg(max8971_chg->client, 0x01);
  dev_err(&max8971_chg->client->dev, "[S-CHG]0x%x - [CHK_INT_MASK]!!!\n", ret);
#endif


	return ret;
}

static int max8971_stop_charging(struct msm_hardware_charger *hw_chg)
{
	struct max8971_chip *max8971_chg;
	int ret = 0;

	max8971_chg = container_of(hw_chg, struct max8971_chip, adapter_hw_chg);

  printk(KERN_DEBUG "############ [max8971_stop_charging]: ONLINE = %d #####################\n", max8971_chg->chg_online);
  
	if (!(max8971_chg->chg_online))
		/* we arent charging */
		return 0;

  //disable_irq(max8971_chg->client->irq);

	return ret;
}

static int max8971_charging_switched(struct msm_hardware_charger *hw_chg)
{
	struct max8971_chip *max8971_chg;

	max8971_chg = container_of(hw_chg, struct max8971_chip, adapter_hw_chg);
	dev_dbg(&max8971_chg->client->dev, "[S-CHG]%s\n", __func__);
	return 0;
}
#endif

static __devinit int max8971_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{

	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	struct max8971_chip *chip;
    int ret;


  pr_notice("[S-CHG] [FIRST] %s\n", __func__);

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE))
		return -EIO;

  pr_notice("[S-CHG] [SECOND] %s\n", __func__);

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

  pr_notice("[S-CHG] [THIRD] %s\n", __func__);

	chip->client = client;
	chip->pdata = client->dev.platform_data;

	i2c_set_clientdata(client, chip);

	chip->charger = max8971_charger_ps;

	ret = power_supply_register(&client->dev, &chip->charger);
	if (ret) {
		dev_err(&client->dev, "[S-CHG]failed: power supply register\n");
		i2c_set_clientdata(client, NULL);
		goto out;
	}
#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
	chip->adapter_hw_chg.type = CHG_TYPE_AC;
	chip->adapter_hw_chg.rating = 2;
	chip->adapter_hw_chg.name = "max8971-adapter";
	chip->adapter_hw_chg.start_charging = max8971_start_charging;
	chip->adapter_hw_chg.stop_charging = max8971_stop_charging;
	chip->adapter_hw_chg.charging_switched = max8971_charging_switched;

  g_max8971_chg = chip;
	
	ret = msm_charger_register(&chip->adapter_hw_chg);

	if (ret) {
		dev_err(&client->dev,
			"[S-CHG]%s msm_charger_register failed for ret =%d\n",
			__func__, ret);
		goto out;
	}


	msm_battery_gauge_register(&max8971_batt_gauge);

	max8971_irq_init(SWITCHING_CHG_IRQ_N);
#endif

#if 1
    ret = request_threaded_irq(client->irq, NULL, max8971_charger_handler,
            IRQF_ONESHOT | IRQF_TRIGGER_LOW, client->name, chip);
    if (unlikely(ret < 0))
    {
        pr_debug("[S-CHG]max8971: failed to request IRQ	%X\n", ret);
        goto out;
    }
#endif

	chip->chg_online = 0;
	ret = max8971_read_reg(client, MAX8971_REG_CHG_STAT);
	if (ret >= 0) {
		chip->chg_online = (ret & MAX8971_DCV_MASK) ? 1 : 0;
	}

  // Set IRQ MASK register
  ret = max8971_write_reg(client, MAX8971_REG_CHGINT_MASK, chip->pdata->int_mask);



#ifdef CONFIG_LGE_SWITCHING_CHARGER_MAX8971
	dev_err(&client->dev, "[S-CHG]%s\n", __func__);

  ret = max8971_write_reg(client, MAX8971_REG_FCHGCRNT, 0x4c);

  ret = max8971_write_reg(client, MAX8971_REG_DCCRNT, 0x3f);

  ret = max8971_write_reg(client, MAX8971_REG_TOPOFF, 0x6e);

  ret = max8971_read_reg(client, 0x06);
  dev_err(&client->dev, "[S-CHG]0x%x - [FCHGCRNT]!!!\n", ret);

  ret = max8971_read_reg(client, 0x07);
  dev_err(&client->dev, "[S-CHG]0x%x - [DCCRNT]!!!\n", ret);

  ret = max8971_read_reg(client, 0x08);
  dev_err(&client->dev, "[S-CHG]0x%x - [TOPOFF]!!!\n", ret);

  ret = max8971_read_reg(client, 0x01);
  dev_err(&client->dev, "[S-CHG]0x%x - [CHK_INT_MASK]!!!\n", ret);
#endif

	return 0;
out:
	kfree(chip);
	return ret;
}

static __devexit int max8971_remove(struct i2c_client *client)
{
    struct max8971_chip *chip = i2c_get_clientdata(client);

	free_irq(client->irq, chip);
	power_supply_unregister(&max8971_charger_ps);
	kfree(chip);

	return 0;
}

static const struct i2c_device_id max8971_id[] = {
	{ "max8971", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, max8971_id);

static struct i2c_driver max8971_i2c_driver = {
	.driver = {
		.name = "max8971",
	},
	.probe		= max8971_probe,
	.remove		= __devexit_p(max8971_remove),
	.id_table	= max8971_id,
};

static int __init max8971_init(void)
{
  pr_notice("[S-CHG]%s: i2c_add_driver\n", __func__);
	return i2c_add_driver(&max8971_i2c_driver);
}
module_init(max8971_init);

static void __exit max8971_exit(void)
{
	i2c_del_driver(&max8971_i2c_driver);
}
module_exit(max8971_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Clark Kim <clark.kim@maxim-ic.com>");
MODULE_DESCRIPTION("Power supply driver for MAX8971");
MODULE_ALIAS("platform:max8971-charger");
