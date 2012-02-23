/* linux/drivers/usb/gadget/u_lgeusb.h
 *
 * Copyright (C) 2008 Google, Inc.
 * Copyright (C) 2010 LGE.
 * Author : Young Kyoung KIM <yk.kim@lge.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __U_LGEUSB_H__
#define __U_LGEUSB_H__

#ifdef CONFIG_LGE_USB_GADGET_SUPPORT_FACTORY_USB
/* Common Interface */
int lge_get_usb_serial_number(char *serial_number);
int lge_detect_factory_cable(void);
#endif /* LG_FW_COMPILE_ERROR */

#define LG_UNKNOWN_CABLE			0
#define LG_WALL_CHARGER_CABLE		1
#define LG_NORMAL_USB_CABLE			2
#define LG_FACTORY_CABLE_56K_TYPE	3
#define LG_FACTORY_CABLE_130K_TYPE	4
#define LG_FACTORY_CABLE_910K_TYPE	5
#define LG_RESERVED1_CABLE			6
#define LG_RESERVED2_CABLE			7
#define LG_NONE_CABLE				8

#endif /* __U_LGE_USB_H__ */
