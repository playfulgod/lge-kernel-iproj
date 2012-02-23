/* kwangdo.yi@lge.com [jointlab] Tue 05 Apr 2011 S
   add i_atnt gpio suspended/active config
   4/5 : still not used.
*/
#include <linux/module.h>
#include <mach/irqs.h>
#include <asm/mach-types.h>
#include "gpiomux.h"

static struct gpiomux_setting msm_gpio00_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio01_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio02_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio03_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio04_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio05_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio06_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio07_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio08_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio09_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio10_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio11_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio12_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio13_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio14_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio15_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio16_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio17_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio18_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio19_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio20_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio21_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio22_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio23_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio24_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio25_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio26_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio27_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio28_cfg_suspend = /* LCD_VSYNC */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio29_cfg_suspend = /* MDM_MSM_VFR */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio30_cfg_suspend = /* WLAN_WAKE_UP*/
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};


static struct gpiomux_setting msm_gpio31_cfg_suspend = /* LIN_PWM_FREQ */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio32_cfg_suspend = /* CAM_MCLK */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio33_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio34_cfg_suspend = /* WLAN_POWER_ON */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio35_cfg_suspend = /* TOUCH_I2C_SDA for gsbi1*/
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio36_cfg_suspend = /* TOUCH_I2C_SCL for gsbi1 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio37_cfg_suspend = /* GRYO_INT2 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio38_cfg_suspend = /* 3AXIS_INT */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio39_cfg_suspend = /* COMPASS_RESET */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio40_cfg_suspend = /* MDM_MSM_WAKEUP */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio41_cfg_suspend = /* PROX_AMBIENT_INT */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio42_cfg_suspend = /* LCD_ID */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio43_cfg_suspend = /* I2C_COMMON_SDA for gsbi3 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio44_cfg_suspend = /* I2C_COMMON_SCL for gsbi3 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio45_cfg_suspend = /* WLAN_HOST_WAKEUP */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio46_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio47_cfg_suspend = /* CAM_I2C_SDA for gsbi4 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio48_cfg_suspend = /* CAM_I2C_SCL for gsbi4 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio49_cfg_suspend = /* LCD_BL_PM_EN */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio50_cfg_suspend = /* LCD_RESET_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio51_cfg_suspend = /* FUEL_I2C_SDA for gsbi5 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio52_cfg_suspend = /* FUEL_I2C_SCL for gsbi5 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};

static struct gpiomux_setting msm_gpio53_cfg_active = /* BT_UART_TXD for gsbi6 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio53_cfg_suspend= /* BT_UART_TXD for gsbi6 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio54_cfg_active = /* BT_UART_RXD for gsbi6 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio54_cfg_suspend = /* BT_UART_RXD for gsbi6 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio55_cfg_active = /* BT_UART_CTS_N for gsbi6 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio55_cfg_suspend= /* BT_UART_CTS_N for gsbi6 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio56_cfg_active = /* BT_UART_RFR_N for gsbi6 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio56_cfg_suspend= /* BT_UART_RFR_N for gsbi6 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio57_cfg_suspend = /* VT_1.3M_CAM_RESET_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio58_cfg_suspend = /* EAR_MIC_EN */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio59_cfg_suspend = /* QTR_CODEC_I2C_SDA for gsbi7 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA, /* need to check */
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio60_cfg_suspend = /* QTR_CODEC_I2C_SCL for gsbi7 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA, /* need to check */
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio61_cfg_suspend = /* TOUCH_INT_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};

static struct gpiomux_setting msm_gpio62_cfg_suspend = /* BATTERY_ID_PULLUP */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio63_cfg_suspend = /* BATTERY_ID */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio64_cfg_suspend = /* I2C_MUIC_SDA for gsbi8  */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio65_cfg_suspend = /* I2C_MUIC_SCL for gsbi8 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio66_cfg_active= /* IPC_UART_TX for gsbi9 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA ,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio66_cfg_suspend= /* IPC_UART_TX for gsbi9 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio67_cfg_active= /* IPC_UART_RX for gsbi9 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA ,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio67_cfg_suspend= /* IPC_UART_RX for gsbi9 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio68_cfg_active= /* IPC_UART_CTS_N for gsbi9 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA ,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio68_cfg_suspend= /* IPC_UART_CTS_N for gsbi9 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio69_cfg_active= /* IPC_UART_RTS_N for gsbi9 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA ,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio69_cfg_suspend= /* IPC_UART_RTS_N for gsbi9 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio70_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio71_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio72_cfg_suspend = /* I2C_SENSOR_SCL for gsbi10 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio73_cfg_suspend = /* I2C_SENSOR_SDA for gsbi10 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio74_cfg_suspend = /* BOOT_FROM_ROM */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio75_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio76_cfg_suspend = /* BOOT_CONFIG_6*/
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio77_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio78_cfg_suspend = /* BOOT_CONFIG_5 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio79_cfg_suspend = /* BOOT_CONFIG_4 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio80_cfg_suspend = /* BOOT_CONFIG_3 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio81_cfg_suspend = /* BOOT_CONFIG_0 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio82_cfg_suspend = /* BOOT_CONFIG_2 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio83_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio84_cfg_suspend = /* BOOT_CONFIG_1 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio85_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio86_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio87_cfg_suspend = /* PM8058_APC_SEC_IRQ_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio88_cfg_suspend = /* PM8058_APC_USR_IRQ_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio89_cfg_suspend = /* PM8058_MDM_IRQ_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio90_cfg_suspend = /* PM8901_APC_SEC_IRQ_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio91_cfg_suspend = /* PM8901_APC_USR_IRQ_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio92_cfg_suspend = /* MSM_PS_HOLD */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_12MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio93_cfg_suspend = /* IPC_ERR_FATAL */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio94_cfg_suspend = /* 8M_CAM_VCM_EN */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio95_cfg_suspend = /* SUB_IPC_SDC_CMD */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio95_cfg_active= /* SUB_IPC_SDC_CMD */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio96_cfg_suspend = /* SUB_IPC_SDC_DAT3 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio96_cfg_active= /* SUB_IPC_SDC_DAT3 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio97_cfg_suspend = /* SUB_IPC_SDC_CLK */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio97_cfg_active= /* SUB_IPC_SDC_CLK */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio98_cfg_suspend = /* SUB_IPC_SDC_DAT2 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio98_cfg_active= /* SUB_IPC_SDC_DAT2 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio99_cfg_suspend = /* SUB_IPC_SDC_DAT1 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio99_cfg_active= /* SUB_IPC_SDC_DAT1 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio100_cfg_suspend = /* SUB_IPC_SDC_DAT0*/
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio100_cfg_active= /* SUB_IPC_SDC_DAT0*/
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio101_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio102_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio103_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio104_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio105_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio106_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio107_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio108_cfg_suspend = /* AUDIO_TX_MCLK */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio108_cfg_active= /* AUDIO_TX_MCLK */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio109_cfg_suspend = /* AUDIO_RX_MCLK1 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio109_cfg_active= /* AUDIO_RX_MCLK1 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio110_cfg_suspend = /* AUDIO_TX_I2S_SD2 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio111_cfg_suspend = /* BT_PCM_DOUT */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio111_cfg_active= /* BT_PCM_DOUT */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio112_cfg_suspend = /* BT_PCM_DIN */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio112_cfg_active= /* BT_PCM_DIN */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio113_cfg_suspend = /* BT_PCM_SYNC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio113_cfg_active= /* BT_PCM_SYNC */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio114_cfg_suspend = /* BT_PCM_CLK */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio114_cfg_active= /* BT_PCM_CLK */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio115_cfg_suspend = /* I2C_AXIS_SCL */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio116_cfg_suspend = /* I2C_AXIS_SDA */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_8MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio117_cfg_suspend = /* MSM_UART_RXD */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio117_cfg_active = /* MSM_UART_RXD */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio118_cfg_suspend = /* MSM_UART_TXD */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio118_cfg_active = /* MSM_UART_TXD */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio119_cfg_suspend = /* AUDIO_RX_I2S_SCK2 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio120_cfg_suspend = /* AUDIO_RX_I2S_WS2 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio121_cfg_suspend = /* AUDIO_RX_I2S_SD2 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio122_cfg_suspend = /* AUDIO_RX_MCLK2 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio123_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio124_cfg_suspend = /* COMPASS_INT */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio125_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio126_cfg_suspend = /* EARPOL_DETECT */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio127_cfg_suspend = /* BT_HOST_WAKEUP */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio128_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio129_cfg_suspend = /* IPC_SDIO_DETECT */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio130_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio131_cfg_suspend = /* MSM_PON_RESET */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio132_cfg_suspend = /* PM8028_PWRUP */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio133_cfg_suspend = /* IPC_ERR_FATAL_HOST */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio134_cfg_suspend = /* IPC_STATUS_HOST */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio135_cfg_suspend = /* IPC_WAKEUP */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio136_cfg_suspend = /* IPC_STATUS */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio137_cfg_suspend = /* BT_WAKEUP */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio138_cfg_suspend = /* BT_RESET_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio139_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio140_cfg_suspend = /* IPC_SYNC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio140_cfg_active = /* IPC_SYNC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio141_cfg_suspend = /* PM28_D0_EN */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio142_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio143_cfg_suspend = /* IPC_SDC_DATA0 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio143_cfg_active= /* IPC_SDC_DATA0 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio144_cfg_suspend = /* IPC_SDC_DATA1 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio144_cfg_active= /* IPC_SDC_DATA1 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio145_cfg_suspend = /* IPC_SDC_DATA2 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio145_cfg_active= /* IPC_SDC_DATA2 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio146_cfg_suspend = /* IPC_SDC_DATA3 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio146_cfg_active= /* IPC_SDC_DATA3 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio147_cfg_suspend = /* IPC_SDC_DATA4 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio147_cfg_active= /* IPC_SDC_DATA4 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio148_cfg_suspend = /* IPC_SDC_DATA5 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio148_cfg_active= /* IPC_SDC_DATA5 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio149_cfg_suspend = /* IPC_SDC_DATA6 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio149_cfg_active= /* IPC_SDC_DATA6 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio150_cfg_suspend = /* IPC_SDC_DATA7 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio150_cfg_active= /* IPC_SDC_DATA7 */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio151_cfg_suspend = /* IPC_SDIO_CMD */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio151_cfg_active= /* IPC_SDIO_CMD */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio152_cfg_suspend = /* IPC_SDIO_CLK */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio152_cfg_active= /* IPC_SDIO_CLK */
{
	.func = GPIOMUX_FUNC_2,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio153_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio154_cfg_suspend = /* FLASH_LED_EN */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio155_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio156_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio157_cfg_suspend = /* 8M_CAM_RESET_N */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio158_cfg_suspend = /* MOTOR_EN */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio159_cfg_suspend = /* EMMC_DATA0 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio159_cfg_active = /* EMMC_DATA0 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio160_cfg_suspend = /* EMMC_DATA1 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio160_cfg_active = /* EMMC_DATA1 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio161_cfg_suspend = /* EMMC_DATA2 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio161_cfg_active = /* EMMC_DATA2 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio162_cfg_suspend = /* EMMC_DATA3 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio162_cfg_active = /* EMMC_DATA3 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio163_cfg_suspend = /* EMMC_DATA4 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio163_cfg_active = /* EMMC_DATA4 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio164_cfg_suspend = /* EMMC_DATA5 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio164_cfg_active = /* EMMC_DATA5 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio165_cfg_suspend = /* EMMC_DATA6 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio165_cfg_active = /* EMMC_DATA6 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio166_cfg_suspend = /* EMMC_DATA7 */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio166_cfg_active = /* EMMC_DATA7 */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio167_cfg_suspend = /* EMMC_CLK */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio167_cfg_active = /* EMMC_CLK */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_16MA,
	.pull = GPIOMUX_PULL_NONE,
};
static struct gpiomux_setting msm_gpio168_cfg_suspend = /* EMMC_CMD */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio168_cfg_active = /* EMMC_CMD */
{
	.func = GPIOMUX_FUNC_1,
	.drv = GPIOMUX_DRV_10MA,
	.pull = GPIOMUX_PULL_UP,
};
static struct gpiomux_setting msm_gpio169_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio170_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio171_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};
static struct gpiomux_setting msm_gpio172_cfg_suspend = /* NC */
{
	.func = GPIOMUX_FUNC_GPIO,
	.drv = GPIOMUX_DRV_2MA,
	.pull = GPIOMUX_PULL_DOWN,
};


static struct msm_gpiomux_config msm8x60_p930_gpio_configs[] __initdata =
{
	{
		.gpio = 0,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio00_cfg_suspend,
		},
	},

	{
		.gpio = 1,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio01_cfg_suspend,
		},
	},
	{
		.gpio = 2,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio02_cfg_suspend,
		},
	},
	{
		.gpio = 3,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio03_cfg_suspend,
		},
	},
	{
		.gpio = 4,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio04_cfg_suspend,
		},
	},
	{
		.gpio = 5,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio05_cfg_suspend,
		},
	},
	{
		.gpio = 6,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio06_cfg_suspend,
		},
	},
	{
		.gpio = 7,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio07_cfg_suspend,
		},
	},
	{
		.gpio = 8,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio08_cfg_suspend,
		},
	},
	{
		.gpio = 9,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio09_cfg_suspend,
		},
	},
	{
		.gpio = 10,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio10_cfg_suspend,
		},
	},
	{
		.gpio = 11,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio11_cfg_suspend,
		},
	},
	{
		.gpio = 12,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio12_cfg_suspend,
		},
	},
	{
		.gpio = 13,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio13_cfg_suspend,
		},
	},
	{
		.gpio = 14,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio14_cfg_suspend,
		},
	},
	{
		.gpio = 15,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio15_cfg_suspend,
		},
	},
	{
		.gpio = 16,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio16_cfg_suspend,
		},
	},
	{
		.gpio = 17,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio17_cfg_suspend,
		},
	},
	{
		.gpio = 18,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio18_cfg_suspend,
		},
	},
	{
		.gpio = 19,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio19_cfg_suspend,
		},
	},
	{
		.gpio = 20,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio20_cfg_suspend,
		},
	},
	{
		.gpio = 21,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio21_cfg_suspend,
		},
	},
	{
		.gpio = 22,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio22_cfg_suspend,
		},
	},
	{
		.gpio = 23,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio23_cfg_suspend,
		},
	},
	{
		.gpio =24,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio24_cfg_suspend,
		},
	},
	{
		.gpio = 25,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio25_cfg_suspend,
		},
	},
	{
		.gpio = 26,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio26_cfg_suspend,
		},
	},
	{
		.gpio = 27,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio27_cfg_suspend,
		},
	},
	{
		.gpio = 28,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio28_cfg_suspend,
		},
	},
	{
		.gpio = 29,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio29_cfg_suspend,
		},
	},
	{
		.gpio = 30,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio30_cfg_suspend,
		},
	},
	{
		.gpio = 31,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio31_cfg_suspend,
		},
	},
	{
		.gpio = 32,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio32_cfg_suspend,
		},
	},
	{
		.gpio = 33,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio33_cfg_suspend,
		},
	},
	{
		.gpio = 34,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio34_cfg_suspend,
		},
	},
	{
		.gpio = 35,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio35_cfg_suspend,
		},
	},
	{
		.gpio = 36,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio36_cfg_suspend,
		},
	},
	{
		.gpio = 37,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio37_cfg_suspend,
		},
	},
	{
		.gpio = 38,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio38_cfg_suspend,
		},
	},
	{
		.gpio = 39,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio39_cfg_suspend,
		},
	},
	{
		.gpio = 40,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio40_cfg_suspend,
		},
	},
	{
		.gpio = 41,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio41_cfg_suspend,
		},
	},
	{
		.gpio = 42,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio42_cfg_suspend,
		},
	},
	{
		.gpio = 43,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio43_cfg_suspend,
		},
	},
	{
		.gpio = 44,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio44_cfg_suspend,
		},
	},
	{
		.gpio = 45,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio45_cfg_suspend,
		},
	},
	{
		.gpio = 46,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio46_cfg_suspend,
		},
	},
	{
		.gpio = 47,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio47_cfg_suspend,
		},
	},
	{
		.gpio = 48,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio48_cfg_suspend,
		},
	},
	{
		.gpio = 49,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio49_cfg_suspend,
		},
	},
	{
		.gpio = 50,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio50_cfg_suspend,
		},
	},
	{
		.gpio = 51,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio51_cfg_suspend,
		},
	},
	{
		.gpio = 52,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio52_cfg_suspend,
		},
	},
	{
		.gpio = 53,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio53_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio53_cfg_suspend,
		},
	},
	{
		.gpio = 54,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio54_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio54_cfg_suspend,
		},
	},
	{
		.gpio = 55,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio55_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio55_cfg_suspend,
		},
	},
	{
		.gpio = 56,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio56_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio56_cfg_suspend,
		},
	},
	{
		.gpio = 57,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio57_cfg_suspend,
		},
	},
	{
		.gpio = 58,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio58_cfg_suspend,
		},
	},
	{
		.gpio = 59,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio59_cfg_suspend,
		},
	},
	{
		.gpio = 60,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio60_cfg_suspend,
		},
	},
	{
		.gpio = 61,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio61_cfg_suspend,
		},
	},
	{
		.gpio = 62,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio62_cfg_suspend,
		},
	},
	{
		.gpio = 63,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio63_cfg_suspend,
		},
	},
	{
		.gpio = 64,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio64_cfg_suspend,
		},
	},
	{
		.gpio = 65,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio65_cfg_suspend,
		},
	},
	{
		.gpio = 66,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio66_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio66_cfg_suspend,
		},
	},
	{
		.gpio = 67,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio67_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio67_cfg_suspend,
		},
	},
	{
		.gpio = 68,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio68_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio68_cfg_suspend,
		},
	},
	{
		.gpio = 69,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio69_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio69_cfg_suspend,
		},
	},
	{
		.gpio = 70,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio70_cfg_suspend,
		},
	},
	{
		.gpio = 71,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio71_cfg_suspend,
		},
	},
	{
		.gpio = 72,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio72_cfg_suspend,
		},
	},
	{
		.gpio = 73,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio73_cfg_suspend,
		},
	},
	{
		.gpio = 74,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio74_cfg_suspend,
		},
	},
	{
		.gpio = 75,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio75_cfg_suspend,
		},
	},
	{
		.gpio = 76,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio76_cfg_suspend,
		},
	},
	{
		.gpio = 77,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio77_cfg_suspend,
		},
	},
	{
		.gpio = 78,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio78_cfg_suspend,
		},
	},
	{
		.gpio = 79,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio79_cfg_suspend,
		},
	},
	{
		.gpio = 80,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio80_cfg_suspend,
		},
	},
	{
		.gpio = 81,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio81_cfg_suspend,
		},
	},
	{
		.gpio = 82,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio82_cfg_suspend,
		},
	},
	{
		.gpio = 83,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio83_cfg_suspend,
		},
	},
	{
		.gpio = 84,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio84_cfg_suspend,
		},
	},
	{
		.gpio = 85,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio85_cfg_suspend,
		},
	},
	{
		.gpio = 86,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio86_cfg_suspend,
		},
	},
	{
		.gpio = 87,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio87_cfg_suspend,
		},
	},
	{
		.gpio = 88,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio88_cfg_suspend,
		},
	},
	{
		.gpio = 89,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio89_cfg_suspend,
		},
	},
	{
		.gpio = 90,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio90_cfg_suspend,
		},
	},
	{
		.gpio =91,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio91_cfg_suspend,
		},
	},
	{
		.gpio = 92,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio92_cfg_suspend,
		},
	},
	{
		.gpio = 93,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio93_cfg_suspend,
		},
	},
	{
		.gpio = 94,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio94_cfg_suspend,
		},
	},
	{
		.gpio = 95,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio95_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio95_cfg_suspend,
		},
	},
	{
		.gpio = 96,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio96_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio96_cfg_suspend,
		},
	},
	{
		.gpio = 97,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio97_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio97_cfg_suspend,
		},
	},
	{
		.gpio = 98,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio98_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio98_cfg_suspend,
		},
	},
	{
		.gpio = 99,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio99_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio99_cfg_suspend,
		},
	},
	{
		.gpio = 100,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio100_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio100_cfg_suspend,
		},
	},
	{
		.gpio = 101,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio101_cfg_suspend,
		},
	},
	{
		.gpio = 102,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio102_cfg_suspend,
		},
	},
	{
		.gpio = 103,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio103_cfg_suspend,
		},
	},
	{
		.gpio = 104,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio104_cfg_suspend,
		},
	},
	{
		.gpio = 105,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio105_cfg_suspend,
		},
	},
	{
		.gpio = 106,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio106_cfg_suspend,
		},
	},
	{
		.gpio = 107,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio107_cfg_suspend,
		},
	},
	{
		.gpio = 108,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio108_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio108_cfg_suspend,
		},
	},
	{
		.gpio = 109,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio109_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio109_cfg_suspend,
		},
	},
	{
		.gpio = 110,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio110_cfg_suspend,
		},
	},
	{
		.gpio = 111,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio111_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio111_cfg_suspend,
		},
	},
	{
		.gpio = 112,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio112_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio112_cfg_suspend,
		},
	},
	{
		.gpio = 113,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio113_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio113_cfg_suspend,
		},
	},
	{
		.gpio = 114,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio114_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio114_cfg_suspend,
		},
	},
	{
		.gpio = 115,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio115_cfg_suspend,
		},
	},
	{
		.gpio = 116,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio116_cfg_suspend,
		},
	},
	{
		.gpio = 117,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio117_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio117_cfg_suspend,
		},
	},
	{
		.gpio = 118,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio118_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio118_cfg_suspend,
		},
	},
	{
		.gpio = 119,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio119_cfg_suspend,
		},
	},
	{
		.gpio = 120,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio120_cfg_suspend,
		},
	},
	{
		.gpio = 121,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio121_cfg_suspend,
		},
	},
	{
		.gpio = 122,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio122_cfg_suspend,
		},
	},
	{
		.gpio = 123,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio123_cfg_suspend,
		},
	},
	{
		.gpio = 124,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio124_cfg_suspend,
		},
	},
	{
		.gpio = 125,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio125_cfg_suspend,
		},
	},
	{
		.gpio = 126,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio126_cfg_suspend,
		},
	},
	{
		.gpio = 127,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio127_cfg_suspend,
		},
	},
	{
		.gpio = 128,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio128_cfg_suspend,
		},
	},
	{
		.gpio = 129,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio129_cfg_suspend,
		},
	},
	{
		.gpio = 130,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio130_cfg_suspend,
		},
	},
	{
		.gpio = 131,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio131_cfg_suspend,
		},
	},
	{
		.gpio = 132,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio132_cfg_suspend,
		},
	},
	{
		.gpio = 133,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio133_cfg_suspend,
		},
	},
	{
		.gpio = 134,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio134_cfg_suspend,
		},
	},
	{
		.gpio = 135,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio135_cfg_suspend,
		},
	},
	{
		.gpio = 136,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio136_cfg_suspend,
		},
	},
	{
		.gpio = 137,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio137_cfg_suspend,
		},
	},
	{
		.gpio = 138,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio138_cfg_suspend,
		},
	},
	{
		.gpio = 139,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio139_cfg_suspend,
		},
	},
	{
		.gpio = 140,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio140_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio140_cfg_suspend,
		},
	},
	{
		.gpio = 141,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio141_cfg_suspend,
		},
	},
	{
		.gpio = 142,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio142_cfg_suspend,
		},
	},
	{
		.gpio = 143,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio143_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio143_cfg_suspend,
		},
	},	{
		.gpio = 144,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio144_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio144_cfg_suspend,
		},
	},
	{
		.gpio = 145,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio145_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio145_cfg_suspend,
		},
	},	{
		.gpio = 146,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio146_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio146_cfg_suspend,
		},
	},
	{
		.gpio = 147,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio147_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio147_cfg_suspend,
		},
	},
	{
		.gpio = 148,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio148_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio148_cfg_suspend,
		},
	},
	{
		.gpio = 149,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio149_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio149_cfg_suspend,
		},
	},
	{
		.gpio = 150,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio150_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio150_cfg_suspend,
		},
	},
	{
		.gpio = 151,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio151_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio151_cfg_suspend,
		},
	},	{
		.gpio = 152,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio152_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio152_cfg_suspend,
		},
	},
	{
		.gpio = 153,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio153_cfg_suspend,
		},
	},
	{
		.gpio = 154,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio154_cfg_suspend,
		},
	},
	{
		.gpio = 155,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio155_cfg_suspend,
		},
	},
	{
		.gpio = 156,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio156_cfg_suspend,
		},
	},
	{
		.gpio = 157,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio157_cfg_suspend,
		},
	},
	{
		.gpio = 158,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio158_cfg_suspend,
		},
	},
	{
		.gpio = 159,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio159_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio159_cfg_suspend,
		},
	},
	{
		.gpio = 160,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio160_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio160_cfg_suspend,
		},
	},
	{
		.gpio = 161,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio161_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio161_cfg_suspend,
		},
	},
	{
		.gpio = 162,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio162_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio162_cfg_suspend,
		},
	},
	{
		.gpio = 163,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio163_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio163_cfg_suspend,
		},
	},
	{
		.gpio = 164,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio164_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio164_cfg_suspend,
		},
	},
	{
		.gpio = 165,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio165_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio165_cfg_suspend,
		},
	},	
	{
		.gpio = 166,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio166_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio166_cfg_suspend,
		},
	},	
	{
		.gpio = 167,
		.settings = 
		{
			[GPIOMUX_ACTIVE] = &msm_gpio167_cfg_active,
			[GPIOMUX_SUSPENDED] = &msm_gpio167_cfg_suspend,
		},
	},	
	{
		.gpio = 168,
			.settings = 
			{
				[GPIOMUX_ACTIVE] = &msm_gpio168_cfg_active,
				[GPIOMUX_SUSPENDED] = &msm_gpio168_cfg_suspend,
			},
	},	
	{
		.gpio = 169,
			.settings = 
			{
				[GPIOMUX_SUSPENDED] = &msm_gpio169_cfg_suspend,
			},
	},
	{
		.gpio = 170,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio170_cfg_suspend,
		},
	},	
	{
		.gpio = 171,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio171_cfg_suspend,
		},
	},	
	{
		.gpio = 172,
		.settings = 
		{
			[GPIOMUX_SUSPENDED] = &msm_gpio172_cfg_suspend,
		},
	},
};
