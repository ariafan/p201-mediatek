#ifdef BUILD_LK
#else
    #include <linux/string.h>
    #if defined(BUILD_UBOOT)
        #include <asm/arch/mt_gpio.h>
    #else
        #include <mach/mt_gpio.h>
    #endif
#endif
#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(854)	//854)	//jessen

#define REGFLAG_DELAY             							0XFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#define LCM_ID_ILI9806C	0x9816			//8009	//jessen

//#define LCM_DSI_CMD_MODE									0

#ifndef TRUE
    #define   TRUE     1
#endif
 
#ifndef FALSE
    #define   FALSE    0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))


// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)											lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

 struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = 
{
#if 0 //ili9806e  huarui //0923
//****************************** Page 1 Command ******************************//
{0xFF,5,{0xFF,0x98,0x06,0x04,0x01}},     // Change to Page 1
{0x08,1,{0x10}},                 // output SDA
{0x21,1,{0x01}},                 // DE = 1 Active
{0x30,1,{0x01}},                 // 480 X 800 2 854 1
{0x31,1,{0x02}},                 // 2-dot Inversion
{0x60,1,{0x04}},                // SDTI
{0x61,1,{0x00}},                // CRTI
{0x62,1,{0x06}},                 // EQTI
{0x63,1,{0x02}},                // PCTI
{0x40,1,{0x15}},                // BT  +2.5/-2.5 pump for DDVDH-L
{0x41,1,{0x33}},                 // DVDDH DVDDL clamp  
{0x42,1,{0x01}},                 // VGH/VGL 
{0x44,1,{0x09}},                 // VGH/VGL 
{0x50,1,{0x80}},                 // VGMP
{0x51,1,{0x80}},                 // VGMN
{0x52,1,{0x00}},                   //Flicker
{0x53,1,{0x64}},                   //Flicker
//++++++++++++++++++ Gamma Setting ++++++++++++++++++//
{0xA0,1,{0x00}},  // Gamma 0 
{0xA1,1,{0x03}},  // Gamma 4 
{0xA2,1,{0x07}},  // Gamma 8
{0xA3,1,{0x0F}},  // Gamma 16
{0xA4,1,{0x09}},  // Gamma 24
{0xA5,1,{0x1A}},  // Gamma 52
{0xA6,1,{0x0B}},  // Gamma 80
{0xA7,1,{0x0A}},  // Gamma 108
{0xA8,1,{0x03}},  // Gamma 147
{0xA9,1,{0x08}},  // Gamma 175
{0xAA,1,{0x06}},  // Gamma 203
{0xAB,1,{0x04}},  // Gamma 231
{0xAC,1,{0x0C}},  // Gamma 239
{0xAD,1,{0x2D}},  // Gamma 247
{0xAE,1,{0x28}},  // Gamma 251
{0xAF,1,{0x00}},  // Gamma 255
///==============Nagitive
{0xC0,1,{0x00}},  // Gamma 0 
{0xC1,1,{0x03}},  // Gamma 4
{0xC2,1,{0x08}},  // Gamma 8
{0xC3,1,{0x0F}},  // Gamma 16
{0xC4,1,{0x09}},  // Gamma 24
{0xC5,1,{0x19}},  // Gamma 52
{0xC6,1,{0x0A}},  // Gamma 80
{0xC7,1,{0x09}},  // Gamma 108
{0xC8,1,{0x03}},  // Gamma 147
{0xC9,1,{0x07}},  // Gamma 175
{0xCA,1,{0x05}},  // Gamma 203
{0xCB,1,{0x04}},  // Gamma 231
{0xCC,1,{0x0B}},  // Gamma 239
{0xCD,1,{0x2C}},  // Gamma 247
{0xCE,1,{0x28}},  // Gamma 251
{0xCF,1,{0x00}},  // Gamma 255
{0xFF,5,{0xFF,0x98,0x06,0x04,0x07}},     // Change to Page 7
{0x18,1,{0x1D}},
{0x26,1,{0x92}},
//****************************** Page 6 Command ******************************//
{0xFF,5,{0xFF,0x98,0x06,0x04,0x06}},     // Change to Page 6
{0x00,1,{0x21}},
{0x01,1,{0x06}},
{0x02,1,{0x00}},    
{0x03,1,{0x00}},
{0x04,1,{0x01}},
{0x05,1,{0x01}},
{0x06,1,{0x80}},    
{0x07,1,{0x02}},
{0x08,1,{0x05}},
{0x09,1,{0x00}},    
{0x0A,1,{0x00}},    
{0x0B,1,{0x00}},    
{0x0C,1,{0x01}},
{0x0D,1,{0x01}},
{0x0E,1,{0x00}},
{0x0F,1,{0x00}},
{0x10,1,{0xF0}},
{0x11,1,{0xF4}},
{0x12,1,{0x00}},
{0x13,1,{0x00}},
{0x14,1,{0x00}},
{0x15,1,{0xC0}},
{0x16,1,{0x08}},
{0x17,1,{0x00}},
{0x18,1,{0x00}},
{0x19,1,{0x00}},
{0x1A,1,{0x00}},
{0x1B,1,{0x00}},
{0x1C,1,{0x00}},
{0x1D,1,{0x00}},
{0x20,1,{0x02}},
{0x21,1,{0x13}},
{0x22,1,{0x45}},
{0x23,1,{0x67}},
{0x24,1,{0x01}},
{0x25,1,{0x23}},
{0x26,1,{0x45}},
{0x27,1,{0x67}},
{0x30,1,{0x13}},
{0x31,1,{0x22}},
{0x32,1,{0x22}},
{0x33,1,{0x22}},
{0x34,1,{0x22}},
{0x35,1,{0xBB}},
{0x36,1,{0xAA}},
{0x37,1,{0xDD}},
{0x38,1,{0xCC}},
{0x39,1,{0x66}},
{0x3A,1,{0x77}},
{0x3B,1,{0x22}},
{0x3C,1,{0x22}},
{0x3D,1,{0x22}},
{0x3E,1,{0x22}},
{0x3F,1,{0x22}},
{0x40,1,{0x22}},
{0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},     // Change to Page 0
#endif
#if 1 //ili9806e  dezhixin //0923
//****************************** Page 1 Command ******************************//

{0xFF,5,{0xFF,0x98,0x06,0x04,0x01}},
 
  
{0x08,1,{0x10}},

  
{0x21,1,{0x01}},
	
	
{0x30,1,{0x01}},//01 480*854
	
	
{0x31,1,{0x00}},//02
	
	
{0x40,1,{0x15}},//16
	
	
{0x41,1,{0x22}},
	
	
{0x42,1,{0x01}},
	
	
{0x43,1,{0x84}},
	
	
{0x44,1,{0x8a}},
	
	
{0x45,1,{0x1B}},
	
	
{0x50,1,{0x78}},
	
	
{0x51,1,{0x78}},
	
	
{0x52,1,{0x00}},
	
	
{0x53,1,{0x32}},//22

  
{0x57,1,{0x50}},

  
{0x60,1,{0x07}},
	
	
{0x61,1,{0x00}},
	
	
{0x62,1,{0x07}},
	
	
{0x63,1,{0x00}},
	

  
{0xA0,1,{0x00}},
	
{0xA1,1,{0x0B}},
	
{0xA2,1,{0x12}},
	
{0xA3,1,{0x0C}},
	
{0xA4,1,{0x05}},
	
{0xA5,1,{0x0C}},
	
{0xA6,1,{0x07}},
	
{0xA7,1,{0x16}},
	
{0xA8,1,{0x06}},
	
{0xA9,1,{0x0A}},
	
{0xAA,1,{0x0F}},
	
{0xAB,1,{0x06}},
	
{0xAC,1,{0x0E}},
	
{0xAD,1,{0x1A}},
	
{0xAE,1,{0x12}},
	
{0xAF,1,{0x00}},
	
{0xC0,1,{0x00}},
	
{0xC1,1,{0x0B}},
	
{0xC2,1,{0x12}},
	
{0xC3,1,{0x0C}},
	
{0xC4,1,{0x05}},
	
{0xC5,1,{0x0C}},
	
{0xC6,1,{0x07}},
	
{0xC7,1,{0x16}},
	
{0xC8,1,{0x06}},
	
{0xC9,1,{0x0A}},
	
{0xCA,1,{0x0F}},
	
{0xCB,1,{0x06}},
	
{0xCC,1,{0x0E}},
	
{0xCD,1,{0x1A}},
	
{0xCE,1,{0x12}},
	
{0xCF,1,{0x00}},
	
	
{0xFF,5,{0xFF,0x98,0x06,0x04,0x06}},

    
{0x00,1,{0xA0}},
	
	
{0x01,1,{0x05}},
	
	
{0x02,1,{0x00}},
	
	
{0x03,1,{0x00}},
	
	
{0x04,1,{0x01}},
	
	
{0x05,1,{0x01}},
	
	
{0x06,1,{0x88}},
	
	
{0x07,1,{0x04}},
	
	
{0x08,1,{0x01}},
	
	
{0x09,1,{0x90}},
	
	
{0x0A,1,{0x04}},
	
	
{0x0B,1,{0x01}},
	
	
{0x0C,1,{0x01}},
	
	
{0x0D,1,{0x01}},
	
	
{0x0E,1,{0x00}},
	
	
{0x0F,1,{0x00}},
	
	
{0x10,1,{0x55}},
	
	
{0x11,1,{0x50}},
	
	
{0x12,1,{0x01}},
	
	
{0x13,1,{0x85}},
	
	
{0x14,1,{0x85}},
	
	
{0x15,1,{0xC0}},
	
	
{0x16,1,{0x0B}},
	
	
{0x17,1,{0x00}},
	
	
{0x18,1,{0x00}},
	
	
{0x19,1,{0x00}},
	
	
{0x1A,1,{0x00}},
	
	
{0x1B,1,{0x00}},
	
	
{0x1C,1,{0x00}},
	
	
{0x1D,1,{0x00}},
	
	
	
{0x20,1,{0x01}},
	
	
{0x21,1,{0x23}},
	
	
{0x22,1,{0x45}},
	
	
{0x23,1,{0x67}},
	
	
{0x24,1,{0x01}},
	
	
{0x25,1,{0x23}},
	
	
{0x26,1,{0x45}},
	
	
{0x27,1,{0x67}},
	
	
	
{0x30,1,{0x02}},
	
	
{0x31,1,{0x22}},
	
	
{0x32,1,{0x11}},
	
	
{0x33,1,{0xAA}},
	
	
{0x34,1,{0xBB}},
	
	
{0x35,1,{0x66}},
	
	
{0x36,1,{0x00}},
	
	
{0x37,1,{0x22}},
	
	
{0x38,1,{0x22}},
	
	
{0x39,1,{0x22}},
	
	
{0x3A,1,{0x22}},
	
	
{0x3B,1,{0x22}},
	
	
{0x3C,1,{0x22}},
	
	
{0x3D,1,{0x22}},
	
	
{0x3E,1,{0x22}},
	
	
{0x3F,1,{0x22}},
	
	
{0x40,1,{0x22}},

{0x52,1,{0x10}},
	
	
{0x53,1,{0x10}},
  

{0xFF,5,{0xFF,0x98,0x06,0x04,0x07}},
	
{0x17,1,{0x22}},
	
{0x02,1,{0x77}},
	
{0xE1,1,{0x79}},
 
{0xFF,5,{0xFF,0x98,0x06,0x04,0x00}},
#endif

{0x11,	1,	{0x00}},
{REGFLAG_DELAY, 120, {}},
{0x29,	1,	{0x00}},
{REGFLAG_DELAY, 20, {}},
{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update)
{
	unsigned int i;

    for(i = 0; i < count; i++) {
		
        unsigned cmd;
        cmd = table[i].cmd;
		
        switch (cmd) {
			
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
				
            case REGFLAG_END_OF_TABLE :
                break;
				
            default:
                dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);				
            
        }
    }
}


// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		// enable tearing-free
    params->dbi.te_mode				= LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;
    
    params->dsi.mode   = SYNC_PULSE_VDO_MODE;
    
    // DSI
    /* Command mode setting */
    params->dsi.LANE_NUM				= LCM_TWO_LANE;
    
    //The following defined the fomat for data coming from LCD engine.
    params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dsi.data_format.trans_seq	= LCM_DSI_TRANS_SEQ_MSB_FIRST;
    params->dsi.data_format.padding 	= LCM_DSI_PADDING_ON_LSB;
    params->dsi.data_format.format	  = LCM_DSI_FORMAT_RGB888;
    
    // Video mode setting		
    params->dsi.intermediat_buffer_num = 2;
    
    params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
    
    params->dsi.word_count=480*3;
    params->dsi.vertical_active_line=854;		//jessen //854;	//800

    params->dsi.vertical_sync_active				= 4;	//3;
    params->dsi.vertical_backporch					= 16;	//12;
    params->dsi.vertical_frontporch					= 20;	//14;
    params->dsi.vertical_active_line				= FRAME_HEIGHT;
    
    params->dsi.horizontal_sync_active				= 10;
    params->dsi.horizontal_backporch				= 80;		//50;
    params->dsi.horizontal_frontporch				= 80;		//50;
    params->dsi.horizontal_blanking_pixel				= 60;
    params->dsi.horizontal_active_pixel				= FRAME_WIDTH;
    
 
    params->dsi.pll_div1=1;		// div1=0,1,2,3;div1_real=1,2,4,4
    params->dsi.pll_div2=0;		// div2=0,1,2,3;div2_real=1,2,4,4
    params->dsi.fbk_div =17;		// fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)		

}

static void lcm_init(void)
{
    SET_RESET_PIN(1);
    MDELAY(5);
    SET_RESET_PIN(0);
    MDELAY(20);
    SET_RESET_PIN(1);
    MDELAY(120);

    push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_suspend(void)
{
	SET_RESET_PIN(1);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(20);
//	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}


static void lcm_resume(void)
{
//	lcm_initialization_setting[17].para_list[0]+=2;		//// VcomVcomVcom used for test vcom
	lcm_init();
	
//	push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
}

/*
static void lcm_update(unsigned int x, unsigned int y,
                       unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0>>8)&0xFF);
	unsigned char x0_LSB = (x0&0xFF);
	unsigned char x1_MSB = ((x1>>8)&0xFF);
	unsigned char x1_LSB = (x1&0xFF);
	unsigned char y0_MSB = ((y0>>8)&0xFF);
	unsigned char y0_LSB = (y0&0xFF);
	unsigned char y1_MSB = ((y1>>8)&0xFF);
	unsigned char y1_LSB = (y1&0xFF);

	unsigned int data_array[16];

	data_array[0]= 0x00053902;
	data_array[1]= (x1_MSB<<24)|(x0_LSB<<16)|(x0_MSB<<8)|0x2a;
	data_array[2]= (x1_LSB);
	data_array[3]= 0x00053902;
	data_array[4]= (y1_MSB<<24)|(y0_LSB<<16)|(y0_MSB<<8)|0x2b;
	data_array[5]= (y1_LSB);
	data_array[6]= 0x002c3909;

	dsi_set_cmdq(data_array, 7, 0);

}


static void lcm_setbacklight(unsigned int level)
{
	unsigned int default_level = 145;
	unsigned int mapped_level = 0;

	//for LGE backlight IC mapping table
	if(level > 255) 
			level = 255;

	if(level >0) 
			mapped_level = default_level+(level)*(255-default_level)/(255);
	else
			mapped_level=0;

	// Refresh value of backlight level.
	lcm_backlight_level_setting[0].para_list[0] = mapped_level;

	push_table(lcm_backlight_level_setting, sizeof(lcm_backlight_level_setting) / sizeof(struct LCM_setting_table), 1);
}

*/


static unsigned int lcm_compare_id(void)
{
		int array[4];
			char buffer[5];
			char id_high=0;
			char id_low=0;
			int id=0;
	
			SET_RESET_PIN(1);
			SET_RESET_PIN(0);
			MDELAY(25); 	  
			SET_RESET_PIN(1);
			MDELAY(50); 	 
		   
			array[0]=0x00063902;
			array[1]=0x0698FFFF;
			array[2]=0x00000104;
			dsi_set_cmdq(array, 3, 1);
			MDELAY(10);
	 
			array[0]=0x00023700;
			dsi_set_cmdq(array, 1, 1);
		
			read_reg_v2(0x00, buffer,1);
			id_high = buffer[0];		 //0x98
	 
			read_reg_v2(0x01, buffer,1);
			id_low = buffer[0]; 		 //0x06
	 
			id =(id_high << 16) | id_low;
	
#if defined(BUILD_LK)
		printf("zrl_lk -- ili9806e 0x%x , 0x%x, 0x%x \n", id_high, id_low, id);
#else
		printk("zrl_kernel -- ili9806e 0x%x , 0x%x, 0x%x \n", id_high, id_low, id);
#endif
	
		return (id == 0x9806)?1:0; //xianliang add

	
}


LCM_DRIVER ili9806e_dsi_vdo_ylt_lcm_drv = 
{
        .name			= "ili9806e_dsi_vdo_ylt_lcm_drv",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id    = lcm_compare_id,	
};

