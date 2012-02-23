/*
 *  felicagpio.c
 *  
 */
 
/*
 *    INCLUDE FILES FOR MODULE
 */

#include "felica_gpio.h"

/*
* Description : 
* Input : 
* Output : 
*/
int felica_gpio_open(int gpionum, int direction, int value)
{
  int rc = 0;
  
  /* Cofigure GPIO */
  rc = gpio_tlmm_config(gpionum, GPIO_CONFIG_ENABLE);
  if(rc)
  {
    FELICA_DEBUG_MSG("[FELICA] ERROR - gpio_tlmm_config \n");
    return rc;
  }

  /* Set output direction */
  if(GPIO_DIRECTION_IN == direction)
  {
    rc = gpio_direction_input(gpionum);
    if(rc)
    {
      FELICA_DEBUG_MSG("[FELICA] ERROR -  gpio_direction_input \n");
      return rc;
    }  
  }
  else
  {
    rc = gpio_direction_output(gpionum, value);
    if(rc)
    {
      FELICA_DEBUG_MSG("[FELICA] ERROR -  gpio_direction_output \n");
      return rc;
    }
  }

  return rc;
}

/*
* Description : 
* Input : 
* Output : 
*/
void felica_gpio_write(int gpionum, int value)
{
  gpio_set_value(gpionum, value);
}

/*
* Description : 
* Input : 
* Output : 
*/
int felica_gpio_read(int gpionum)
{
  return gpio_get_value(gpionum);
}

