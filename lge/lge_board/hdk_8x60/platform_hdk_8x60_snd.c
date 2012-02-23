/* lge/lge_board/hdk_8x60/platform_hdk_8x60_snd.c
 *
 * Copyright (C) 2010 LGE, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#define CONFIG_LGE_AUDIO_NO_NCP_MODE

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/mfd/pmic8058.h>
#include <linux/pmic8058-othc.h>
#include <linux/mfd/pmic8901.h>
#include <linux/mfd/msm-adie-codec.h>
#include <linux/regulator/pmic8058-regulator.h>
#include <linux/regulator/pmic8901-regulator.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>

#include <mach/qdsp6v2/audio_dev_ctl.h>
#include <mach/qdsp6v2/apr_audio.h>
#include <mach/mpp.h>
#include <asm/mach-types.h>
#include <asm/uaccess.h>

#include "snddev_icodec.h"
#include "snddev_ecodec.h"
#include "timpani_profile_8x60.h"
#include "snddev_hdmi.h"
#include "snddev_mi2s.h"

#ifdef CONFIG_LGE_HEADSET_DETECTION_FSA8008
#include "fsa8008.h"
#endif

#ifdef CONFIG_LGE_AUDIO_AMP_WM9093
#include <linux/i2c.h>
#include "devices_hdk_8x60.h"
#include "wm9093.h"

//#define LGE_AUDIO_COMMENT_OUT_FOR_REFERENCE

static wmCodecCmd seq_lin_to_spkout[] = 
{
	{0x39, 0x000D},
	{0x01, 0x000B},
	{0x02, 0x60C0},
	{0x16, 0x0001},  // IN1_CLAMP 0bit 0 : clamp de-activate 1: clamp activated
	{0x18, 0x0102}, // only test  6dB -> 18dB		{0x18, 0x0102}, // IN1A
	{0x19, 0x0102}, // only test  6dB ->18dB		{0x18, 0x0102}, // IN1A
	{0x36, 0x0050},
	{0x03, 0x0008},
	{0x22, 0x0000}, // {0x22, 0x0050},
	{0x03, 0x0108},
	{0x25, 0x0150},  //0x178 : 12dB ,, 150 ; +3dB
	{0x16, 0x0000},
	{0x01, 0x100B}
};

static wmCodecCmd seq_lin_to_headset[] = 
{
	{0x39, 0x000D},
	{0x01, 0x000B},
	{0x02, 0x60C0},
	{0x16, 0x0001},
	{0x18, 0x0102},
	{0x19, 0x0102},
	{0x2D, 0x0040},
	{0x2E, 0x0010},
	{0x03, 0x0030},
	{0x2F, 0x0000},
	{0x30, 0x0000},
	{0x16, 0x0000},
	{0x46, 0x0100},
	{0x49, 0x0100}
};

static wmCodecCmd seq_power_down[] = 
{
	{0x46, 0x0100},
	{0x49, 0x0110},
	{0x00, 0x0000},
	{0x02, 0x6000},
	{0x39, 0x0000}
};

struct wm9093_platform_data lge_audio_wm9093_platform = {
	.speaker_on = {
		.amp_function = seq_lin_to_spkout,
		.amp_function_size = ARRAY_SIZE(seq_lin_to_spkout),
	},
		
	.hph_on = {
		.amp_function = seq_lin_to_headset,
		.amp_function_size = ARRAY_SIZE(seq_lin_to_headset),
	},

	.power_down = {
		.amp_function = seq_power_down,
		.amp_function_size = ARRAY_SIZE(seq_power_down),
	},
	.set_amp_path = NULL
};

#define WM9093_I2C_SLAVE_ADDR   0xDC

static struct i2c_board_info lge_i2c_wm9093_info[] = {
        {
                I2C_BOARD_INFO("wm9093", WM9093_I2C_SLAVE_ADDR>>1),
				.platform_data = &lge_audio_wm9093_platform
        },
};
#endif

#ifdef CONFIG_DEBUG_FS
static struct dentry *debugfs_hsed_config;
static void snddev_hsed_config_modify_setting(int type);
static void snddev_hsed_config_restore_setting(void);
#endif

static struct resource msm_cdcclk_ctl_resources[] = {
	{
		.name   = "msm_snddev_tx_mclk",
		.start  = 108,
		.end    = 108,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "msm_snddev_rx_mclk",
		.start  = 109,
		.end    = 109,
		.flags  = IORESOURCE_IO,
	},
};

static struct platform_device msm_cdcclk_ctl_device = {
	.name   = "msm_cdcclk_ctl",
	.num_resources  = ARRAY_SIZE(msm_cdcclk_ctl_resources),
	.resource       = msm_cdcclk_ctl_resources,
};

static struct resource msm_aux_pcm_resources[] = {

	{
		.name   = "aux_pcm_dout",
		.start  = 111,
		.end    = 111,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "aux_pcm_din",
		.start  = 112,
		.end    = 112,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "aux_pcm_syncout",
		.start  = 113,
		.end    = 113,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "aux_pcm_clkin_a",
		.start  = 114,
		.end    = 114,
		.flags  = IORESOURCE_IO,
	},
};

static struct platform_device msm_aux_pcm_device = {
	.name   = "msm_aux_pcm",
	.num_resources  = ARRAY_SIZE(msm_aux_pcm_resources),
	.resource       = msm_aux_pcm_resources,
};

static struct resource msm_mi2s_gpio_resources[] = {

	{
		.name   = "mi2s_ws",
		.start  = 101,
		.end    = 101,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "mi2s_sclk",
		.start  = 102,
		.end    = 102,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "mi2s_mclk",
		.start  = 103,
		.end    = 103,
		.flags  = IORESOURCE_IO,
	},
	{
		.name   = "fm_mi2s_sd",
		.start  = 107,
		.end    = 107,
		.flags  = IORESOURCE_IO,
	},
};

static struct platform_device msm_mi2s_device = {
	.name		= "msm_mi2s",
	.num_resources	= ARRAY_SIZE(msm_mi2s_gpio_resources),
	.resource	= msm_mi2s_gpio_resources,
};

#ifdef LGE_AUDIO_COMMENT_OUT_FOR_REFERENCE
static struct regulator *s3;
static struct regulator *mvs;

static int msm_snddev_enable_dmic_power(void)
{
	int ret;

	s3 = regulator_get(NULL, "8058_s3");
	if (IS_ERR(s3)) {
		ret = -EBUSY;
		goto fail_get_s3;
	}

	ret = regulator_set_voltage(s3, 1800000, 1800000);
	if (ret) {
		pr_err("%s: error setting voltage\n", __func__);
		goto fail_s3;
	}

	ret = regulator_enable(s3);
	if (ret) {
		pr_err("%s: error enabling regulator\n", __func__);
		goto fail_s3;
	}

	mvs = regulator_get(NULL, "8901_mvs0");
	if (IS_ERR(mvs))
		goto fail_mvs0_get;

	ret = regulator_enable(mvs);
	if (ret) {
		pr_err("%s: error setting regulator\n", __func__);
		goto fail_mvs0_enable;
	}
	return ret;

fail_mvs0_enable:
	regulator_put(mvs);
	mvs = NULL;
fail_mvs0_get:
	regulator_disable(s3);
fail_s3:
	regulator_put(s3);
	s3 = NULL;
fail_get_s3:
	return ret;
}

static void msm_snddev_disable_dmic_power(void)
{
	int ret;

	if (mvs) {
		ret = regulator_disable(mvs);
		if (ret < 0)
			pr_err("%s: error disabling vreg mvs\n", __func__);
		regulator_put(mvs);
		mvs = NULL;
	}

	if (s3) {
		ret = regulator_disable(s3);
		if (ret < 0)
			pr_err("%s: error disabling regulator s3\n", __func__);
		regulator_put(s3);
		s3 = NULL;
	}
}

static struct regulator *l11;

static int msm_snddev_enable_qt_dmic_power(void)
{
	int ret;

	l11 = regulator_get(NULL, "8058_l11");
	if (IS_ERR(l11))
		return -EBUSY;

	ret = regulator_set_voltage(l11, 1500000, 1500000);
	if (ret) {
		pr_err("%s: error setting regulator\n", __func__);
		goto fail_l11;
	}
	ret = regulator_enable(l11);
	if (ret) {
		pr_err("%s: error enabling regulator\n", __func__);
		goto fail_l11;
	}
	return 0;

fail_l11:
	regulator_put(l11);
	l11 = NULL;
	return ret;
}


static void msm_snddev_disable_qt_dmic_power(void)
{
	int ret;

	if (l11) {
		ret = regulator_disable(l11);
		if (ret < 0)
			pr_err("%s: error disabling regulator l11\n", __func__);
		regulator_put(l11);
		l11 = NULL;
	}
}
#endif

static int msm_snddev_poweramp_on_spk(void)
{
	pr_debug("%s\n", __func__);

#ifdef CONFIG_LGE_AUDIO_AMP_WM9093
	if (lge_audio_wm9093_platform.set_amp_path)
		lge_audio_wm9093_platform.set_amp_path(ICODEC_SPEAKER_RX);
#endif

	return 0;
}

static void msm_snddev_poweramp_off_spk(void)
{
#ifdef CONFIG_LGE_AUDIO_AMP_WM9093
	if (lge_audio_wm9093_platform.set_amp_path)
		lge_audio_wm9093_platform.set_amp_path(ICODEC_AMP_OFF);
#endif

	pr_debug("%s\n", __func__);
 }

#ifdef CONFIG_LGE_AUDIO_AMP_WM9093
static int msm_snddev_poweramp_on_hph(void)
{
	pr_debug("%s\n", __func__);

	if (lge_audio_wm9093_platform.set_amp_path)
		lge_audio_wm9093_platform.set_amp_path(ICODEC_HEADSET_ST_RX);

	return 0;
}

static void msm_snddev_poweramp_off_hph(void)
{
	if (lge_audio_wm9093_platform.set_amp_path)
		lge_audio_wm9093_platform.set_amp_path(ICODEC_AMP_OFF);

	pr_debug("%s\n", __func__);
}
#endif

/* Regulator 8058_l10 supplies regulator 8058_ncp. */
#ifndef CONFIG_LGE_AUDIO_NO_NCP_MODE
static struct regulator *snddev_reg_ncp;
#endif
static struct regulator *snddev_reg_l10;

static int msm_snddev_voltage_on(void)
{
	int rc;
	pr_debug("%s\n", __func__);

	snddev_reg_l10 = regulator_get(NULL, "8058_l10");
	if (IS_ERR(snddev_reg_l10)) {
		pr_err("%s: regulator_get(%s) failed (%ld)\n", __func__,
			"l10", PTR_ERR(snddev_reg_l10));
		return -EBUSY;
	}

	rc = regulator_set_voltage(snddev_reg_l10, 2600000, 2600000);
	if (rc < 0)
		pr_err("%s: regulator_set_voltage(l10) failed (%d)\n",
			__func__, rc);

	rc = regulator_enable(snddev_reg_l10);
	if (rc < 0)
		pr_err("%s: regulator_enable(l10) failed (%d)\n", __func__, rc);

#ifndef CONFIG_LGE_AUDIO_NO_NCP_MODE
	snddev_reg_ncp = regulator_get(NULL, "8058_ncp");
	if (IS_ERR(snddev_reg_ncp)) {
		pr_err("%s: regulator_get(%s) failed (%ld)\n", __func__,
			"ncp", PTR_ERR(snddev_reg_ncp));
		return -EBUSY;
	}

	rc = regulator_set_voltage(snddev_reg_ncp, 1800000, 1800000);
	if (rc < 0) {
		pr_err("%s: regulator_set_voltage(ncp) failed (%d)\n",
			__func__, rc);
		goto regulator_fail;
	}

	rc = regulator_enable(snddev_reg_ncp);
	if (rc < 0) {
		pr_err("%s: regulator_enable(ncp) failed (%d)\n", __func__, rc);
		goto regulator_fail;
	}

	return rc;

regulator_fail:
	regulator_put(snddev_reg_ncp);
	snddev_reg_ncp = NULL;
	return rc;
#else
	return rc;
#endif
}

static void msm_snddev_voltage_off(void)
{
	int rc;
	pr_debug("%s\n", __func__);

#ifndef CONFIG_LGE_AUDIO_NO_NCP_MODE
	if (!snddev_reg_ncp)
		goto done;

	rc = regulator_disable(snddev_reg_ncp);
	if (rc < 0)
		pr_err("%s: regulator_disable(ncp) failed (%d)\n",
			__func__, rc);

	regulator_put(snddev_reg_ncp);

	snddev_reg_ncp = NULL;

done:
#endif
	if (!snddev_reg_l10)
		return;

	rc = regulator_disable(snddev_reg_l10);
	if (rc < 0)
		pr_err("%s: regulator_disable(l10) failed (%d)\n",
			__func__, rc);

	regulator_put(snddev_reg_l10);

	snddev_reg_l10 = NULL;
}

static int msm_snddev_enable_amic_power(void)
{
/* sungwoo.cho@lge.com 2011. 01. 26 */
#ifdef CONFIG_LGE_AUDIO
	int ret;

#ifdef CONFIG_PMIC8058_OTHC
	ret = pm8058_micbias_enable(OTHC_MICBIAS_MAIN, OTHC_SIGNAL_ALWAYS_ON);

	if (ret)
		pr_err("%s: Enabling OTHC_MICBIAS_MAIN(%d) power failed\n", __func__, OTHC_MICBIAS_MAIN);
#endif

	ret = gpio_request(GPIO_CAMCORDER_MIC_EN, "CAMCORDER_MIC_EN");
	if (ret) {
		pr_err("%s: camcorder mic en %d request failed\n",
			__func__, GPIO_CAMCORDER_MIC_EN);
		return 0;
	}
	gpio_direction_output(GPIO_CAMCORDER_MIC_EN, 1);
#endif
	return 0;
/* sungwoo.cho@lge.com 2011. 01. 26 */
}

static void msm_snddev_disable_amic_power(void)
{
/* sungwoo.cho@lge.com 2011. 01. 26 */
#ifdef CONFIG_LGE_AUDIO
	gpio_set_value(GPIO_CAMCORDER_MIC_EN, 0);
	gpio_free(GPIO_CAMCORDER_MIC_EN);

#ifdef CONFIG_PMIC8058_OTHC
	{
		int ret;

		ret = pm8058_micbias_enable(OTHC_MICBIAS_MAIN, OTHC_SIGNAL_OFF);

		if (ret)
			pr_err("%s: Disabling OTHC_MICBIAS_MAIN(%d) power failed\n", __func__, OTHC_MICBIAS_MAIN);
	}
#endif
#endif
/* sungwoo.cho@lge.com 2011. 01. 26 */
}

#ifdef LGE_AUDIO_COMMENT_OUT_FOR_REFERENCE
static int msm_snddev_enable_dmic_sec_power(void)
{
	int ret;

	ret = msm_snddev_enable_dmic_power();
	if (ret) {
		pr_err("%s: Error: Enabling dmic power failed\n", __func__);
		return ret;
	}
#ifdef CONFIG_PMIC8058_OTHC
	ret = pm8058_micbias_enable(OTHC_MICBIAS_2, OTHC_SIGNAL_ALWAYS_ON);
	if (ret) {
		pr_err("%s: Error: Enabling micbias failed\n", __func__);
		msm_snddev_disable_dmic_power();
		return ret;
	}
#endif
	return 0;
}

static void msm_snddev_disable_dmic_sec_power(void)
{
	msm_snddev_disable_dmic_power();

#ifdef CONFIG_PMIC8058_OTHC
	pm8058_micbias_enable(OTHC_MICBIAS_2, OTHC_SIGNAL_OFF);
#endif
}
#endif

static struct adie_codec_action_unit iearpiece_48KHz_osr256_actions[] =
	EAR_PRI_MONO_8000_OSR_256;

static struct adie_codec_hwsetting_entry iearpiece_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = iearpiece_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(iearpiece_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile iearpiece_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = iearpiece_settings,
	.setting_sz = ARRAY_SIZE(iearpiece_settings),
};

static struct snddev_icodec_data snddev_iearpiece_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "handset_rx",
	.copp_id = 0,
	.profile = &iearpiece_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
};

static struct platform_device msm_iearpiece_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_iearpiece_data },
};

static struct adie_codec_action_unit imic_48KHz_osr256_actions[] =
	AMIC_PRI_MONO_OSR_256;

static struct adie_codec_hwsetting_entry imic_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = imic_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(imic_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile imic_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = imic_settings,
	.setting_sz = ARRAY_SIZE(imic_settings),
};

static struct snddev_icodec_data snddev_imic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_tx",
	.copp_id = 1,
	.profile = &imic_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_amic_power,
	.pamp_off = msm_snddev_disable_amic_power,
};

static struct platform_device msm_imic_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_imic_data },
};

static struct adie_codec_action_unit headset_ab_cpls_48KHz_osr256_actions[] =
#ifdef CONFIG_LGE_AUDIO_NO_NCP_MODE
	HPH_PRI_AB_LEG_STEREO;
#else
	HEADSET_AB_CPLS_48000_OSR_256;
#endif

static struct adie_codec_hwsetting_entry headset_ab_cpls_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = headset_ab_cpls_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(headset_ab_cpls_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile headset_ab_cpls_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = headset_ab_cpls_settings,
	.setting_sz = ARRAY_SIZE(headset_ab_cpls_settings),
};

static struct snddev_icodec_data snddev_ihs_stereo_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "headset_stereo_rx",
	.copp_id = 0,
	.profile = &headset_ab_cpls_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
/* LGE_CHANGE 2011. 01. 20 sungwoo.cho, dongsung.shin start */
#ifdef CONFIG_LGE_AUDIO_AMP_WM9093
	.pamp_on = msm_snddev_poweramp_on_hph,
	.pamp_off = msm_snddev_poweramp_off_hph,
#endif	
/* LGE_CHANGE 2011. 01. 20 sungwoo.cho, dongsung.shin end */
	.voltage_on = msm_snddev_voltage_on,
	.voltage_off = msm_snddev_voltage_off,
};

static struct platform_device msm_headset_stereo_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_ihs_stereo_rx_data },
};

#ifdef LGE_AUDIO_COMMENT_OUT_FOR_REFERENCE
static struct adie_codec_action_unit headset_anc_48KHz_osr256_actions[] =
	ANC_HEADSET_CPLS_AMIC1_AUXL_RX1_48000_OSR_256;

static struct adie_codec_hwsetting_entry headset_anc_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = headset_anc_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(headset_anc_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile headset_anc_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = headset_anc_settings,
	.setting_sz = ARRAY_SIZE(headset_anc_settings),
};

static struct snddev_icodec_data snddev_anc_headset_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE | SNDDEV_CAP_ANC),
	.name = "anc_headset_stereo_rx",
	.copp_id = PRIMARY_I2S_RX,
	.profile = &headset_anc_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_amic_power,
	.pamp_off = msm_snddev_disable_amic_power,
	.voltage_on = msm_snddev_voltage_on,
	.voltage_off = msm_snddev_voltage_off,
};

static struct platform_device msm_anc_headset_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_anc_headset_data },
};
#endif

static struct adie_codec_action_unit ispkr_stereo_48KHz_osr256_actions[] =
/* sungwoo.cho@lge.com 2011. 01. 26 */
#ifdef CONFIG_LGE_AUDIO_AMP_WM9093

#ifdef CONFIG_LGE_AUDIO_NO_NCP_MODE
	HPH_PRI_AB_LEG_STEREO;
#else
	HEADSET_AB_CPLS_48000_OSR_256;
#endif

#else
	SPEAKER_PRI_STEREO_48000_OSR_256;
#endif
/* sungwoo.cho@lge.com 2011. 01. 26 */

static struct adie_codec_hwsetting_entry ispkr_stereo_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ispkr_stereo_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(ispkr_stereo_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ispkr_stereo_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ispkr_stereo_settings,
	.setting_sz = ARRAY_SIZE(ispkr_stereo_settings),
};

static struct snddev_icodec_data snddev_ispkr_stereo_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "speaker_stereo_rx",
	.copp_id = 0,
	.profile = &ispkr_stereo_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_poweramp_on_spk,
	.pamp_off = msm_snddev_poweramp_off_spk,
/* sungwoo.cho@lge.com 2011. 01. 26 */
#ifdef CONFIG_LGE_AUDIO_AMP_WM9093
	.voltage_on = msm_snddev_voltage_on,
	.voltage_off = msm_snddev_voltage_off,
#endif
/* sungwoo.cho@lge.com 2011. 01. 26 */
};

static struct platform_device msm_ispkr_stereo_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_ispkr_stereo_data },
};

#ifdef CONFIG_LGE_AUDIO

static struct snddev_icodec_data snddev_ispkr_mic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_mono_tx",
	.copp_id = 1,
	.profile = &imic_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_amic_power,
	.pamp_off = msm_snddev_disable_amic_power,
};

#else

static struct adie_codec_action_unit idmic_mono_48KHz_osr256_actions[] =
	DMIC1_PRI_MONO_OSR_256;

static struct adie_codec_hwsetting_entry idmic_mono_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = idmic_mono_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(idmic_mono_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile idmic_mono_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = idmic_mono_settings,
	.setting_sz = ARRAY_SIZE(idmic_mono_settings),
};

static struct snddev_icodec_data snddev_ispkr_mic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_mono_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &idmic_mono_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_dmic_power,
	.pamp_off = msm_snddev_disable_dmic_power,
};

#endif

static struct platform_device msm_ispkr_mic_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_ispkr_mic_data },
};

#ifdef LGE_AUDIO_COMMENT_OUT_FOR_REFERENCE
static struct adie_codec_action_unit iearpiece_ffa_48KHz_osr256_actions[] =
	EAR_PRI_MONO_8000_OSR_256;

static struct adie_codec_hwsetting_entry iearpiece_ffa_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = iearpiece_ffa_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(iearpiece_ffa_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile iearpiece_ffa_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = iearpiece_ffa_settings,
	.setting_sz = ARRAY_SIZE(iearpiece_ffa_settings),
};

static struct snddev_icodec_data snddev_iearpiece_ffa_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "handset_rx",
	.copp_id = 0,
	.profile = &iearpiece_ffa_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
};

static struct platform_device msm_iearpiece_ffa_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_iearpiece_ffa_data },
};

static struct snddev_icodec_data snddev_imic_ffa_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &idmic_mono_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_dmic_power,
	.pamp_off = msm_snddev_disable_dmic_power,
};

static struct platform_device msm_imic_ffa_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_imic_ffa_data },
};

static struct snddev_icodec_data snddev_qt_dual_dmic_d0_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_mono_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &idmic_mono_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_qt_dmic_power,
	.pamp_off = msm_snddev_disable_qt_dmic_power,
};

static struct platform_device msm_qt_dual_dmic_d0_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_qt_dual_dmic_d0_data },
};

static struct adie_codec_action_unit dual_mic_endfire_8KHz_osr256_actions[] =
	DMIC1_PRI_STEREO_OSR_256;

static struct adie_codec_hwsetting_entry dual_mic_endfire_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = dual_mic_endfire_8KHz_osr256_actions,
		.action_sz = ARRAY_SIZE(dual_mic_endfire_8KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile dual_mic_endfire_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = dual_mic_endfire_settings,
	.setting_sz = ARRAY_SIZE(dual_mic_endfire_settings),
};

static struct snddev_icodec_data snddev_dual_mic_endfire_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_dual_mic_endfire_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &dual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_dmic_power,
	.pamp_off = msm_snddev_disable_dmic_power,
};

static struct platform_device msm_hs_dual_mic_endfire_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_dual_mic_endfire_data },
};

static struct snddev_icodec_data snddev_dual_mic_spkr_endfire_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_dual_mic_endfire_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &dual_mic_endfire_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_dmic_power,
	.pamp_off = msm_snddev_disable_dmic_power,
};

static struct platform_device msm_spkr_dual_mic_endfire_device = {
	.name = "snddev_icodec",
	.id = 15,
	.dev = { .platform_data = &snddev_dual_mic_spkr_endfire_data },
};

static struct adie_codec_action_unit dual_mic_broadside_8osr256_actions[] =
	HS_DMIC2_STEREO_OSR_256;

static struct adie_codec_hwsetting_entry dual_mic_broadside_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = dual_mic_broadside_8osr256_actions,
		.action_sz = ARRAY_SIZE(dual_mic_broadside_8osr256_actions),
	}
};

static struct adie_codec_dev_profile dual_mic_broadside_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = dual_mic_broadside_settings,
	.setting_sz = ARRAY_SIZE(dual_mic_broadside_settings),
};

static struct snddev_icodec_data snddev_hs_dual_mic_broadside_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "handset_dual_mic_broadside_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &dual_mic_broadside_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_dmic_sec_power,
	.pamp_off = msm_snddev_disable_dmic_sec_power,
};

static struct platform_device msm_hs_dual_mic_broadside_device = {
	.name = "snddev_icodec",
	.id = 21,
	.dev = { .platform_data = &snddev_hs_dual_mic_broadside_data },
};

static struct snddev_icodec_data snddev_spkr_dual_mic_broadside_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "speaker_dual_mic_broadside_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &dual_mic_broadside_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_enable_dmic_sec_power,
	.pamp_off = msm_snddev_disable_dmic_sec_power,
};

static struct platform_device msm_spkr_dual_mic_broadside_device = {
	.name = "snddev_icodec",
	.id = 18,
	.dev = { .platform_data = &snddev_spkr_dual_mic_broadside_data },
};

#endif

static struct snddev_hdmi_data snddev_hdmi_stereo_rx_data = {
	.capability = SNDDEV_CAP_RX ,
	.name = "hdmi_stereo_rx",
	.copp_id = HDMI_RX,
	.channel_mode = 0,
	.default_sample_rate = 48000,
};

static struct platform_device msm_snddev_hdmi_stereo_rx_device = {
	.name = "snddev_hdmi",
	.dev = { .platform_data = &snddev_hdmi_stereo_rx_data },
};

static struct snddev_mi2s_data snddev_mi2s_fm_tx_data = {
	.capability = SNDDEV_CAP_TX ,
	.name = "fmradio_stereo_tx",
	.copp_id = MI2S_TX,
	.channel_mode = 2, /* stereo */
	.sd_lines = MI2S_SD3, /* sd3 */
	.sample_rate = 48000,
};

static struct platform_device msm_mi2s_fm_tx_device = {
	.name = "snddev_mi2s",
	.dev = { .platform_data = &snddev_mi2s_fm_tx_data },
};

static struct snddev_mi2s_data snddev_mi2s_fm_rx_data = {
	.capability = SNDDEV_CAP_RX ,
	.name = "fmradio_stereo_rx",
	.copp_id = MI2S_RX,
	.channel_mode = 2, /* stereo */
	.sd_lines = MI2S_SD3, /* sd3 */
	.sample_rate = 48000,
};

static struct platform_device msm_mi2s_fm_rx_device = {
	.name = "snddev_mi2s",
	.id = 1,
	.dev = { .platform_data = &snddev_mi2s_fm_rx_data },
};

static struct adie_codec_action_unit iheadset_mic_tx_osr256_actions[] =
	HEADSET_AMIC2_TX_MONO_PRI_OSR_256;

static struct adie_codec_hwsetting_entry iheadset_mic_tx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = iheadset_mic_tx_osr256_actions,
		.action_sz = ARRAY_SIZE(iheadset_mic_tx_osr256_actions),
	}
};

static struct adie_codec_dev_profile iheadset_mic_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = iheadset_mic_tx_settings,
	.setting_sz = ARRAY_SIZE(iheadset_mic_tx_settings),
};

static struct snddev_icodec_data snddev_headset_mic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "headset_mono_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &iheadset_mic_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
};

static struct platform_device msm_headset_mic_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_headset_mic_data },
};

static struct adie_codec_action_unit
	ihs_stereo_speaker_stereo_rx_48KHz_osr256_actions[] =
	SPEAKER_HPH_AB_CPL_PRI_STEREO_48000_OSR_256;

static struct adie_codec_hwsetting_entry
	ihs_stereo_speaker_stereo_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = ihs_stereo_speaker_stereo_rx_48KHz_osr256_actions,
		.action_sz =
		ARRAY_SIZE(ihs_stereo_speaker_stereo_rx_48KHz_osr256_actions),
	}
};

static struct adie_codec_dev_profile ihs_stereo_speaker_stereo_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = ihs_stereo_speaker_stereo_rx_settings,
	.setting_sz = ARRAY_SIZE(ihs_stereo_speaker_stereo_rx_settings),
};

static struct snddev_icodec_data snddev_ihs_stereo_speaker_stereo_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "headset_stereo_speaker_stereo_rx",
	.copp_id = 0,
	.profile = &ihs_stereo_speaker_stereo_rx_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_poweramp_on_spk,
	.pamp_off = msm_snddev_poweramp_off_spk,
	.voltage_on = msm_snddev_voltage_on,
	.voltage_off = msm_snddev_voltage_off,
};

static struct platform_device msm_ihs_stereo_speaker_stereo_rx_device = {
	.name = "snddev_icodec",
	.id = 22,
	.dev = { .platform_data = &snddev_ihs_stereo_speaker_stereo_rx_data },
};

/* define the value for BT_SCO */

static struct snddev_ecodec_data snddev_bt_sco_earpiece_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE),
	.name = "bt_sco_rx",
	.copp_id = PCM_RX,
	.channel_mode = 1,
};

static struct snddev_ecodec_data snddev_bt_sco_mic_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE),
	.name = "bt_sco_tx",
	.copp_id = PCM_TX,
	.channel_mode = 1,
};

struct platform_device msm_bt_sco_earpiece_device = {
	.name = "msm_snddev_ecodec",
	.dev = { .platform_data = &snddev_bt_sco_earpiece_data },
};

struct platform_device msm_bt_sco_mic_device = {
	.name = "msm_snddev_ecodec",
	.dev = { .platform_data = &snddev_bt_sco_mic_data },
};

static struct adie_codec_action_unit itty_mono_tx_actions[] =
	TTY_HEADSET_MONO_TX_OSR_256;

static struct adie_codec_hwsetting_entry itty_mono_tx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = itty_mono_tx_actions,
		.action_sz = ARRAY_SIZE(itty_mono_tx_actions),
	},
};

static struct adie_codec_dev_profile itty_mono_tx_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = itty_mono_tx_settings,
	.setting_sz = ARRAY_SIZE(itty_mono_tx_settings),
};

static struct snddev_icodec_data snddev_itty_mono_tx_data = {
	.capability = (SNDDEV_CAP_TX | SNDDEV_CAP_VOICE | SNDDEV_CAP_TTY),
	.name = "tty_headset_mono_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &itty_mono_tx_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
};

static struct platform_device msm_itty_mono_tx_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_itty_mono_tx_data },
};

static struct adie_codec_action_unit itty_mono_rx_actions[] =
	TTY_HEADSET_MONO_RX_8000_OSR_256;

static struct adie_codec_hwsetting_entry itty_mono_rx_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = itty_mono_rx_actions,
		.action_sz = ARRAY_SIZE(itty_mono_rx_actions),
	},
};

static struct adie_codec_dev_profile itty_mono_rx_profile = {
	.path_type = ADIE_CODEC_RX,
	.settings = itty_mono_rx_settings,
	.setting_sz = ARRAY_SIZE(itty_mono_rx_settings),
};

static struct snddev_icodec_data snddev_itty_mono_rx_data = {
	.capability = (SNDDEV_CAP_RX | SNDDEV_CAP_VOICE | SNDDEV_CAP_TTY),
	.name = "tty_headset_mono_rx",
	.copp_id = PRIMARY_I2S_RX,
	.profile = &itty_mono_rx_profile,
	.channel_mode = 1,
	.default_sample_rate = 48000,
	.voltage_on = msm_snddev_voltage_on,
	.voltage_off = msm_snddev_voltage_off,
};

static struct platform_device msm_itty_mono_rx_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_itty_mono_rx_data },
};

static struct adie_codec_action_unit linein_pri_actions[] =
	LINEIN_PRI_STEREO_OSR_256;

static struct adie_codec_hwsetting_entry linein_pri_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = linein_pri_actions,
		.action_sz = ARRAY_SIZE(linein_pri_actions),
	},
};

static struct adie_codec_dev_profile linein_pri_profile = {
	.path_type = ADIE_CODEC_TX,
	.settings = linein_pri_settings,
	.setting_sz = ARRAY_SIZE(linein_pri_settings),
};

static struct snddev_icodec_data snddev_linein_pri_data = {
	.capability = SNDDEV_CAP_TX,
	.name = "linein_pri_tx",
	.copp_id = PRIMARY_I2S_TX,
	.profile = &linein_pri_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.voltage_on = msm_snddev_voltage_on,
	.voltage_off = msm_snddev_voltage_off,
};

static struct platform_device msm_linein_pri_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_linein_pri_data },
};

static struct adie_codec_action_unit auxpga_lb_lo_actions[] =
	LB_AUXPGA_LO_STEREO;

static struct adie_codec_hwsetting_entry auxpga_lb_lo_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = auxpga_lb_lo_actions,
		.action_sz = ARRAY_SIZE(auxpga_lb_lo_actions),
	},
};

static struct adie_codec_dev_profile auxpga_lb_lo_profile = {
	.path_type = ADIE_CODEC_LB,
	.settings = auxpga_lb_lo_settings,
	.setting_sz = ARRAY_SIZE(auxpga_lb_lo_settings),
};

static struct snddev_icodec_data snddev_auxpga_lb_lo_data = {
	.capability = SNDDEV_CAP_LB,
	.name = "speaker_stereo_lb",
	.copp_id = PRIMARY_I2S_RX,
	.profile = &auxpga_lb_lo_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.pamp_on = msm_snddev_poweramp_on_spk,
	.pamp_off = msm_snddev_poweramp_off_spk,
	.dev_vol_type = SNDDEV_DEV_VOL_ANALOG,
};

static struct platform_device msm_auxpga_lb_lo_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_auxpga_lb_lo_data },
};

static struct adie_codec_action_unit auxpga_lb_hs_actions[] =
	LB_AUXPGA_HPH_AB_CPLS_STEREO;

static struct adie_codec_hwsetting_entry auxpga_lb_hs_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions = auxpga_lb_hs_actions,
		.action_sz = ARRAY_SIZE(auxpga_lb_hs_actions),
	},
};

static struct adie_codec_dev_profile auxpga_lb_hs_profile = {
	.path_type = ADIE_CODEC_LB,
	.settings = auxpga_lb_hs_settings,
	.setting_sz = ARRAY_SIZE(auxpga_lb_hs_settings),
};

static struct snddev_icodec_data snddev_auxpga_lb_hs_data = {
	.capability = SNDDEV_CAP_LB,
	.name = "hs_stereo_lb",
	.copp_id = PRIMARY_I2S_RX,
	.profile = &auxpga_lb_hs_profile,
	.channel_mode = 2,
	.default_sample_rate = 48000,
	.voltage_on = msm_snddev_voltage_on,
	.voltage_off = msm_snddev_voltage_off,
	.dev_vol_type = SNDDEV_DEV_VOL_ANALOG,
};

static struct platform_device msm_auxpga_lb_hs_device = {
	.name = "snddev_icodec",
	.dev = { .platform_data = &snddev_auxpga_lb_hs_data },
};

#ifdef CONFIG_DEBUG_FS
static struct adie_codec_action_unit
	ihs_stereo_rx_class_d_legacy_48KHz_osr256_actions[] =
	HPH_PRI_D_LEG_STEREO;

static struct adie_codec_hwsetting_entry
	ihs_stereo_rx_class_d_legacy_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions =
		ihs_stereo_rx_class_d_legacy_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE
		(ihs_stereo_rx_class_d_legacy_48KHz_osr256_actions),
	}
};

static struct adie_codec_action_unit
	ihs_stereo_rx_class_ab_legacy_48KHz_osr256_actions[] =
	HPH_PRI_AB_LEG_STEREO;

static struct adie_codec_hwsetting_entry
	ihs_stereo_rx_class_ab_legacy_settings[] = {
	{
		.freq_plan = 48000,
		.osr = 256,
		.actions =
		ihs_stereo_rx_class_ab_legacy_48KHz_osr256_actions,
		.action_sz = ARRAY_SIZE
		(ihs_stereo_rx_class_ab_legacy_48KHz_osr256_actions),
	}
};

static void snddev_hsed_config_modify_setting(int type)
{
	struct platform_device *device;
	struct snddev_icodec_data *icodec_data;

	device = &msm_headset_stereo_device;
	icodec_data = (struct snddev_icodec_data *)device->dev.platform_data;

	if (icodec_data) {
		if (type == 1) {
			icodec_data->voltage_on = NULL;
			icodec_data->voltage_off = NULL;
			icodec_data->profile->settings =
				ihs_stereo_rx_class_d_legacy_settings;
			icodec_data->profile->setting_sz =
			ARRAY_SIZE(ihs_stereo_rx_class_d_legacy_settings);
		} else if (type == 2) {
			icodec_data->voltage_on = NULL;
			icodec_data->voltage_off = NULL;
			icodec_data->profile->settings =
				ihs_stereo_rx_class_ab_legacy_settings;
			icodec_data->profile->setting_sz =
			ARRAY_SIZE(ihs_stereo_rx_class_ab_legacy_settings);
		}
	}
}

static void snddev_hsed_config_restore_setting(void)
{
	struct platform_device *device;
	struct snddev_icodec_data *icodec_data;

	device = &msm_headset_stereo_device;
	icodec_data = (struct snddev_icodec_data *)device->dev.platform_data;

	if (icodec_data) {
		icodec_data->voltage_on = msm_snddev_voltage_on;
		icodec_data->voltage_off = msm_snddev_voltage_off;
		icodec_data->profile->settings = headset_ab_cpls_settings;
		icodec_data->profile->setting_sz =
			ARRAY_SIZE(headset_ab_cpls_settings);
	}
}

static ssize_t snddev_hsed_config_debug_write(struct file *filp,
	const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	char *lb_str = filp->private_data;
	char cmd;

	if (get_user(cmd, ubuf))
		return -EFAULT;

	if (!strcmp(lb_str, "msm_hsed_config")) {
		switch (cmd) {
		case '0':
			snddev_hsed_config_restore_setting();
			break;

		case '1':
			snddev_hsed_config_modify_setting(1);
			break;

		case '2':
			snddev_hsed_config_modify_setting(2);
			break;

		default:
			break;
		}
	}
	return cnt;
}

static int snddev_hsed_config_debug_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}

static const struct file_operations snddev_hsed_config_debug_fops = {
	.open = snddev_hsed_config_debug_open,
	.write = snddev_hsed_config_debug_write
};
#endif

#ifdef CONFIG_LGE_HEADSET_DETECTION_FSA8008
#include <linux/input.h>

static void fsa8008_set_headset_mic_bias(int enable) {
	int rc;

	if (enable)
		rc = pm8058_micbias_enable(OTHC_MICBIAS_HEADSET, OTHC_SIGNAL_ALWAYS_ON);
	else
		rc = pm8058_micbias_enable(OTHC_MICBIAS_HEADSET, OTHC_SIGNAL_OFF);

	if (rc)
		pr_err("%s: Enabling amic power failed\n", __func__);
}

static struct fsa8008_platform_data fsa8008_platform_data = {
	.switch_name = "h2w",

	.keypad_name = "ffa-keypad_qwerty", // "qwerty", // "hsd_headset"; // just for testing

	.key_code = KEY_SPACE, // KEY_MEDIA // just for testing

	.gpio_detect = GPIO_EAR_SENSE_N,
	.gpio_mic_en = GPIO_EAR_MIC_EN,
	.gpio_jpole  = GPIO_EARPOL_DETECT,
	.gpio_key    = GPIO_EAR_KEY_INT,

	.set_headset_mic_bias = fsa8008_set_headset_mic_bias,

	.latency_for_detection = 75,
};

static struct platform_device lge_hsd_device = {
   .name = "fsa8008",
   .id   = -1,
   .dev = {
      .platform_data = &fsa8008_platform_data,
   },
};

#endif

#ifdef LGE_AUDIO_COMMENT_OUT_FOR_REFERENCE
static struct platform_device *snd_devices_ffa[] __initdata = {
	&msm_iearpiece_ffa_device,
	&msm_imic_ffa_device,
	&msm_ispkr_stereo_device,
	&msm_snddev_hdmi_stereo_rx_device,
	&msm_headset_mic_device,
	&msm_ispkr_mic_device,
	&msm_bt_sco_earpiece_device,
	&msm_bt_sco_mic_device,
	&msm_headset_stereo_device,
	&msm_itty_mono_tx_device,
	&msm_itty_mono_rx_device,
	&msm_mi2s_fm_tx_device,
	&msm_mi2s_fm_rx_device,
	&msm_hs_dual_mic_endfire_device,
	&msm_spkr_dual_mic_endfire_device,
	&msm_hs_dual_mic_broadside_device,
	&msm_spkr_dual_mic_broadside_device,
	&msm_ihs_stereo_speaker_stereo_rx_device,
	&msm_anc_headset_device,
	&msm_auxpga_lb_hs_device,
	&msm_auxpga_lb_lo_device,
	&msm_linein_pri_device,
};
#endif

static struct platform_device *snd_devices_lge_i_board[] __initdata = {
	&msm_iearpiece_device,
	&msm_imic_device,
	&msm_ispkr_stereo_device,
	&msm_snddev_hdmi_stereo_rx_device,
	&msm_headset_mic_device,
	&msm_ispkr_mic_device,
	&msm_bt_sco_earpiece_device,
	&msm_bt_sco_mic_device,
	&msm_headset_stereo_device,
	&msm_itty_mono_tx_device,
	&msm_itty_mono_rx_device,
	&msm_mi2s_fm_tx_device,
	&msm_mi2s_fm_rx_device,
	&msm_ihs_stereo_speaker_stereo_rx_device,
	&msm_auxpga_lb_hs_device,
	&msm_auxpga_lb_lo_device,
	&msm_linein_pri_device,
#ifdef CONFIG_LGE_HEADSET_DETECTION_FSA8008
	&lge_hsd_device,
#endif
};

#ifdef LGE_AUDIO_COMMENT_OUT_FOR_REFERENCE
static struct platform_device *snd_devices_fluid[] __initdata = {
	&msm_iearpiece_device,
	&msm_imic_device,
	&msm_ispkr_stereo_device,
	&msm_snddev_hdmi_stereo_rx_device,
	&msm_headset_stereo_device,
	&msm_headset_mic_device,
	&msm_bt_sco_earpiece_device,
	&msm_bt_sco_mic_device,
	&msm_mi2s_fm_tx_device,
	&msm_mi2s_fm_rx_device,
	&msm_anc_headset_device,
	&msm_auxpga_lb_hs_device,
	&msm_auxpga_lb_lo_device,
};

static struct platform_device *snd_devices_qt[] __initdata = {
	&msm_headset_stereo_device,
	&msm_headset_mic_device,
	&msm_ispkr_stereo_device,
	&msm_qt_dual_dmic_d0_device,
	&msm_snddev_hdmi_stereo_rx_device,
};
#endif

static struct platform_device *snd_devices_common[] __initdata = {
	&msm_aux_pcm_device,
	&msm_cdcclk_ctl_device,
	&msm_mi2s_device,
};

void __init msm_snddev_init(void)
{
	int i;
	int dev_id;

#ifdef CONFIG_LGE_AUDIO_AMP_WM9093
	/* wm9093 initialization code */
	i2c_register_board_info(LGE_GSBI_BUS_ID_AUDIO_AMP_WM9093,
		lge_i2c_wm9093_info,
		ARRAY_SIZE(lge_i2c_wm9093_info));
#endif

	for (i = 0, dev_id = 0; i < ARRAY_SIZE(snd_devices_common); i++)
		snd_devices_common[i]->id = dev_id++;

	platform_add_devices(snd_devices_common,
		ARRAY_SIZE(snd_devices_common));

	for (i = 0; i < ARRAY_SIZE(snd_devices_lge_i_board); i++)
		snd_devices_lge_i_board[i]->id = dev_id++;

	platform_add_devices(snd_devices_lge_i_board, ARRAY_SIZE(snd_devices_lge_i_board));

#ifdef CONFIG_DEBUG_FS
	debugfs_hsed_config = debugfs_create_file("msm_hsed_config",
				S_IFREG | S_IRUGO, NULL,
		(void *) "msm_hsed_config", &snddev_hsed_config_debug_fops);
#endif
}
