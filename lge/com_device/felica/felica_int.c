/*
 *  felica_int.c
 *  
 */
 
/*
 *    INCLUDE FILES FOR MODULE
 */
#include <linux/irq.h>
 
#include "felica_int.h"
#include "felica_gpio.h"
/*
 *  DEFINE
 */
#define FEATURE_DEBUG

/*
 *    INTERNAL DEFINITION
 */

/*
 *    FUNCTION DEFINITION
 */

/*
* Description : MFC calls this function using close method(void open()) of FileOutputStream class
*               When this fuction is excuted, set PON to Low.
* Input : None
* Output : Success : 0 Fail : Other
*/
static int felica_int_open (struct inode *inode, struct file *fp)
{
#ifdef FEATURE_DEBUG
  int rc = 0;

  FELICA_DEBUG_MSG("[FELICA_INT] felica_int_open - start \n");

  FELICA_DEBUG_MSG("[FELICA_INT] felica_int_open - end \n");
#endif

	return rc;
}

/*
* Description : MFC calls this function using close method(void close()) of FileOutputStream class
*               When this fuction is excuted, set PON to Low.
* Input : None
* Output : Success : 0 Fail : Other
*/
static int felica_int_release (struct inode *inode, struct file *fp)
{
#ifdef FEATURE_DEBUG
  FELICA_DEBUG_MSG("[FELICA_INT] felica_int_release - start \n");

  FELICA_DEBUG_MSG("[FELICA_INT] felica_int_release - end \n");
#endif
	return 0;
}

/*
 *    STRUCT DEFINITION
 */


static struct file_operations felica_int_fops = 
{
	.owner		= THIS_MODULE,
	.open		  = felica_int_open,
	.release	= felica_int_release,
};

static struct miscdevice felica_int_device = 
{
	MISC_DYNAMIC_MINOR,
	FELICA_INT_NAME,
	&felica_int_fops
};

static irqreturn_t felica_int_isr(int irq, void *dev_id)
{
	FELICA_DEBUG_MSG("[FELICA_INT] felica_int_isr - start \n");

	FELICA_DEBUG_MSG("[FELICA_INT] felica_int_isr - end \n");

	return IRQ_HANDLED;
}
static int felica_int_init(void)
{
#ifdef FEATURE_DEBUG
	int rc = 0;

	FELICA_DEBUG_MSG("[FELICA_INT] felica_int_init - start \n");

	/* register the device file */
	rc = misc_register(&felica_int_device);
	if (rc)
	{
  	FELICA_DEBUG_MSG("[FELICA_INT] FAIL!! can not register felica_int \n");
		return rc;
	}

	rc= request_irq(GPIO_FELICA_INT, felica_int_isr, IRQF_DISABLED | IRQF_TRIGGER_FALLING, FELICA_INT_NAME, NULL);
	if (rc)
	{
  	FELICA_DEBUG_MSG("[FELICA_INT] FAIL!! can not request_irq \n");
		return rc;
	}
	
	FELICA_DEBUG_MSG("[FELICA_INT] felica_int_init - end \n");
#endif
	return 0;
}

static void felica_int_exit(void)
{
#ifdef FEATURE_DEBUG
	FELICA_DEBUG_MSG("[FELICA_INT] felica_int_exit - start \n");

	free_irq(GPIO_FELICA_INT, NULL);
	
	misc_deregister(&felica_int_device);
	
	FELICA_DEBUG_MSG("[FELICA_INT] felica_int_exit - end \n");
#endif
}

module_init(felica_int_init);
module_exit(felica_int_exit);

MODULE_LICENSE("Dual BSD/GPL");

