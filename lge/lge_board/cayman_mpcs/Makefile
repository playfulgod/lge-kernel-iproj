#platform.team@lge.com	2011.01

EXTRA_CFLAGS += -Iarch/arm/mach-msm
EXTRA_CFLAGS += -Iarch/arm/mach-msm/qdsp6v2
EXTRA_CFLAGS += -Ilge/include

obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= board_cayman_mpcs.o
#[LG_BTUI] To seperate bt power driver module form All in One board configuration  minwoo2.kim@lge.com
obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= platform_cayman_mpcs_bt.o
obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= platform_cayman_mpcs_camera.o
obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= platform_cayman_mpcs_input.o
#obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= platform_cayman_mpcs_misc.o
#obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= platform_cayman_mpcs_mmc.o
obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= platform_cayman_mpcs_panel.o
obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= platform_cayman_mpcs_snd.o
obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= gpiomux_cayman_mpcs.o
obj-$(CONFIG_MACH_LGE_C1_BOARD_MPS)	+= devices_cayman_mpcs.o
