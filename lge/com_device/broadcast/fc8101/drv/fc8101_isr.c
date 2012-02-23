/*****************************************************************************
 Copyright(c) 2009 FCI Inc. All Rights Reserved

 File name : fc8101_isr.c

 Description : fc8110 interrupt service routine

 History :
 ----------------------------------------------------------------------
 2009/08/29 	jason		initial
*******************************************************************************/
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#include "fci_types.h"
#include "fci_hal.h"
#include "fc8101_regs.h"
#include "fci_oal.h"
#include "fc8101_bb.h"

u8 *tsBuffer;

int (*pTSCallback)(u32 userdata, u8 *data, int length) = NULL;

u32 gTSUserData;

#define SPI_READ_BLK_SZ		8192

void fc8101_buffer_init(u8 alloc)
{
	if(alloc)
	{
		if(tsBuffer==NULL)
			tsBuffer=(u8 *)kmalloc((188*8*2)+4, GFP_DMA | GFP_KERNEL);

		if(tsBuffer==NULL)
			PRINTF(0, "tsBuffer malloc Fail ");
	}
	else if(tsBuffer!=NULL)
	{
		kfree(tsBuffer);
		tsBuffer =NULL;
	}
}

void fc8101_isr(HANDLE hDevice)
{
	u8	extIntStatus = 0;
	u8	extOverStatus = 0; // FCI buffer overflow reset patch 2011-08-30
	int mod, div;
	int i;

	bbm_ext_read(hDevice, BBM_COM_INT_STATUS, &extIntStatus);
	bbm_ext_write(hDevice, BBM_COM_INT_STATUS, extIntStatus);
	bbm_ext_write(hDevice, BBM_COM_INT_STATUS, 0x00);

	bbm_ext_read(hDevice, BBM_COM_OVERFLOW, &extOverStatus); // 2011-08-30 FCI patch to check buffer overflow

	// Buffer overflow -> buffer reset 2011-08-29 FCI add for buffer overflow reset taew00k.kang
	if(extOverStatus) {
		PRINTF(0, "FC8101 Buffer Overflow : 0x%x\n", extOverStatus);
		fc8101_reset(hDevice);		
	}
	else {
		if(extIntStatus & BBM_MF_INT)
		{
			u16	size;

			bbm_ext_word_read(hDevice, BBM_BUF_THR, &size);		
			size += 1;
			
			if(size) {
				
				mod = size / SPI_READ_BLK_SZ;
				div = size % SPI_READ_BLK_SZ;

				for(i=0; i<mod; i++)
					bbm_ext_data(hDevice, BBM_BUF_RD, &tsBuffer[4+SPI_READ_BLK_SZ*i], SPI_READ_BLK_SZ);

				if(div) {
					bbm_ext_data(hDevice, BBM_BUF_RD, &tsBuffer[4+SPI_READ_BLK_SZ*mod], div);
				}				
	
				if(pTSCallback) 
				{
					(*pTSCallback)(gTSUserData, &tsBuffer[4], (size));
				}	
			}
		}
	}

}

