/*
 *  felica_rfs.c
 *  
 */

/*
 *  INCLUDE FILES FOR MODULE
 */

#include "felica_rfs.h"
#include "felica_gpio.h"

#include "felica_test.h"

/*
 *  DEFINE
 */

/*
 *   FUNCTION PROTOTYPE
 */

/*
 *   INTERNAL DEFINITION
 */


/*
 *   INTERNAL VARIABLE
 */
static int isopen = 0; // 0 : No open 1 : Open

/*
 *   FUNCTION DEFINITION

 */

/*
 * Description: MFC calls this function using open method of FileInputStream class
 * Input: None
 * Output: Success : 0 Fail : Others 
 */
static int felica_rfs_open (struct inode *inode, struct file *fp)
{
  int rc = 0;

#ifdef FUNCTION_DRIVER_SECUIRY
  if(0 == felica_check_uid_mfc())return -1;
#endif

  if(1 == isopen)
  {
    #ifdef FEATURE_DEBUG_LOW 
    FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_open - already open \n");
    #endif
    
    return -1;
  }
  else
  {
    #ifdef FEATURE_DEBUG_LOW 
    FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_open - start \n");
    #endif

    isopen = 1;
  }

  rc = felica_gpio_open(GPIO_FELICA_RFS, GPIO_DIRECTION_IN, GPIO_LOW_VALUE);

  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_open - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_open - result(%d) \n",result_open_rfs);
  return result_open_rfs;
#else
  return rc;
#endif

}

/*
 * Description: MFC calls this function using read method(int read()) of FileInputStream class
 * Input: None
 * Output: RFS low : 1 RFS high : 0
 */
static ssize_t felica_rfs_read(struct file *fp, char *buf, size_t count, loff_t *pos)
{
  int rc = 0;
  int getvalue = GPIO_LOW_VALUE;

#ifdef FUNCTION_DRIVER_SECUIRY
  if(0 == felica_check_uid_mfc())return -1;
#endif

  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_read - start \n");
  #endif

/* Check error */
  if(NULL == fp)
  {
    FELICA_DEBUG_MSG("[FELICA_RFS] ERROR fp \n");
    return -1;    
  }

  if(NULL == buf)
  {
    FELICA_DEBUG_MSG("[FELICA_RFS] ERROR buf \n");
    return -2;    
  }

  if(1 != count)
  {
    FELICA_DEBUG_MSG("[FELICA_RFS] ERROR count \n");
    return -3;    
  }

  if(NULL == pos)
  {
    FELICA_DEBUG_MSG("[FELICA_RFS] ERROR file \n");
    return -4;    
  }

/* Get GPIO value */
  getvalue = felica_gpio_read(GPIO_FELICA_RFS);

  if((GPIO_LOW_VALUE != getvalue)&&(GPIO_HIGH_VALUE != getvalue))
  {
    FELICA_DEBUG_MSG("[FELICA_RFS] ERROR - getvalue is out of range \n");
    return -5;    
  }

/* Copy value to user memory */
  getvalue = getvalue ? GPIO_LOW_VALUE: GPIO_HIGH_VALUE;

  FELICA_DEBUG_MSG("[FELICA_RFS] RFS status : %d \n", getvalue);

  rc = copy_to_user((void*)buf, (void*)&getvalue, count);
  if(rc)
  {
    FELICA_DEBUG_MSG("[FELICA_RFS] ERROR -  copy_to_user \n");
    return rc;
  }

  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_read - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_read - result(%d) \n",result_read_rfs);
  if(result_read_rfs != -1)
    result_read_rfs = count;

  return result_read_rfs;
#else
    return count;
#endif
}
/*
 * Description: MFC calls this function using close method(int close()) of FileInputStream class
 * Input: None
 * Output: RFS low : 1 RFS high : 0
 */
static int felica_rfs_release (struct inode *inode, struct file *fp)
{

#ifdef FUNCTION_DRIVER_SECUIRY
  if(0 == felica_check_uid_mfc())return -1;
#endif

  if(0 == isopen)
  {
    #ifdef FEATURE_DEBUG_LOW 
    FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_release - not open \n");
    #endif

    return -1;
  }
  else
  {
    #ifdef FEATURE_DEBUG_LOW 
    FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_release - start \n");
    #endif

    isopen = 0;
  }

  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_release - end \n");
  #endif

#ifdef FELICA_FN_DEVICE_TEST
  FELICA_DEBUG_MSG("[FELICA_RFS] felica_rfs_release - result(%d) \n",result_close_rfs);
  return result_close_rfs;
#else
  return 0;
#endif
}

static struct file_operations felica_rfs_fops = 
{
  .owner    = THIS_MODULE,
  .open      = felica_rfs_open,
  .read      = felica_rfs_read,
  .release  = felica_rfs_release,
};

static struct miscdevice felica_rfs_device = {
  .minor = MISC_DYNAMIC_MINOR,
  .name = FELICA_RFS_NAME,
  .fops = &felica_rfs_fops,
};

static int felica_rfs_init(void)
{
  int rc;

  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA] felica_pon_init - start \n");
  #endif

  /* register the device file */
  rc = misc_register(&felica_rfs_device);
  if (rc < 0)
  {
    FELICA_DEBUG_MSG("[FELICA] FAIL!! can not register felica_pon \n");
    return rc;
  }

  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA] felica_pon_init - end \n");
  #endif

  return 0;
}

static void felica_rfs_exit(void)
{
  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA] felica_pon_exit - start \n");
  #endif

  /* deregister the device file */
  misc_deregister(&felica_rfs_device);
  
  #ifdef FEATURE_DEBUG_LOW 
  FELICA_DEBUG_MSG("[FELICA] felica_pon_exit - end \n");
  #endif
}

module_init(felica_rfs_init);
module_exit(felica_rfs_exit);

MODULE_LICENSE("Dual BSD/GPL");
