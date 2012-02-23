/*
 *  felicacommon.h
 *  
 */

#ifndef __FELICACOMMON_H__
#define __FELICACOMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  INCLUDE FILES FOR MODULE
 */
#include <linux/module.h>/*THIS_MODULE*/
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>/* printk() */
#include <linux/types.h>/* size_t */
#include <linux/miscdevice.h>/*misc_register, misc_deregister*/
#include <linux/vmalloc.h>
#include <linux/fs.h>/*file_operations*/
#include <linux/delay.h>/*mdelay*/

#include <asm/uaccess.h>/*copy_from_user*/
#include <asm/io.h>/*static*/
#include <mach/gpio.h>

/*
 *  DEFINE
 */

/* FUNCTION FEATURE */
//#define FUNCTION_DRIVER_SECUIRY

/* debug message */
//#define FEATURE_DEBUG_LOW
#define FELICA_DEBUG_MSG printk


/* felica_pon */
#define FELICA_PON_NAME    "felica_pon"

/* felica */
#define FELICA_NAME    "felica"

/* felica_cen */
#define FELICA_CEN_NAME    "felica_cen"

/* felica_rfs */
#define FELICA_RFS_NAME    "felica_rfs"

/* felica_cal */
#define FELICA_CAL_NAME    "felica_cal"

/* felica I2C */
#define FELICA_I2C_NAME    "felica_i2c"

/* felica_int */
#define FELICA_RWS_NAME    "felica_rws"

#ifdef FUNCTION_DRIVER_SECUIRY
int felica_check_uid_mfc(void);
int felica_check_uid_fl(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // __FELICACOMMON_H__
