/*
 *  felica_common.c
 *  
 */
 
/*
 *    INCLUDE FILES FOR MODULE
 */
#include "felica_common.h"

/*
 *  DEFINE
 */

/*
 *    INTERNAL DEFINITION
 */
 
#ifdef FUNCTION_DRIVER_SECUIRY
#define UID_MFC 4003
#define UID_FL 4007

/*
 *    FUNCTION DEFINITION
 */

/*
* Description : 
* Input : None
* Output : Success : 1 Fail : 0
*/
int felica_check_uid_mfc(void)
{
  if(UID_MFC == current_uid())
  {
    return 1;
  }
  else
   FELICA_DEBUG_MSG("[FELICA_PON] felica_check_uid_mfc - can not access \n");
  
  return 0;
}

/*
* Description : 
* Input : None
* Output : Success : 1 Fail : 0
*/
int felica_check_uid_fl(void)
{
  if((UID_MFC == current_uid())||(UID_FL == current_uid()))
  {
    return 1;
  }
  else
   FELICA_DEBUG_MSG("[FELICA_PON] felica_check_uid_fl - can not access \n");
  
  return 0;
}
#endif