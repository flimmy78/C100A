
#include "lcd_panel_cfg.h"


static void LCD_cfg_panel_info(__panel_para_t * info)
{

	memset(info,0,sizeof(__panel_para_t));

	//���Ļ�����Ϣ
	info->lcd_x 				  = 1024;
	info->lcd_y 				  = 600;
	info->lcd_dclk_freq 		  = 51;  //MHz
	info->lcd_pwm_freq			  = 2;	//KHz
	info->lcd_srgb				  = 0x00202020;
	info->lcd_swap				  = 1;
	
	//���Ľӿ�������Ϣ
	info->lcd_if				  = 0;//0:HV , 1:8080 I/F, 2:TTL I/F, 3:LVDS
	
	//����HVģ�������Ϣ
	info->lcd_hv_if 			  = 0;
	info->lcd_hv_hspw			  = 0;
	info->lcd_hv_lde_iovalue	  = 0;
	info->lcd_hv_lde_used		  = 0;
	info->lcd_hv_smode			  = 0;
	info->lcd_hv_syuv_if		  = 0;
	info->lcd_hv_vspw			  = 0;
		
	//����HV������Ϣ
	info->lcd_hbp			= 158;
	//info->lcd_ht			= 1055;//1344;
	info->lcd_ht			= 1344;
	info->lcd_vbp			= 25;
	info->lcd_vt			= (2 * 635);

	//����IO������Ϣ
	info->lcd_io_cfg0			  = 0x00000000;
	info->lcd_io_cfg1			  = 0x00000000;
	info->lcd_io_strength		  = 0;

	//TTL��Ļ��������Ϣ
	info->lcd_ttl_ckvd			  = 0;
	info->lcd_ttl_ckvh			  = 0;
	info->lcd_ttl_ckvt			  = 0;
	info->lcd_ttl_datainv_en	  = 0;
	info->lcd_ttl_datainv_sel	  = 0;
	info->lcd_ttl_datarate		  = 0;
	info->lcd_ttl_oehd			  = 0;
	info->lcd_ttl_oehh			  = 0;
	info->lcd_ttl_oevd			  = 0;
	info->lcd_ttl_oevh			  = 0;
	info->lcd_ttl_oevt			  = 0;
	info->lcd_ttl_revd			  = 0;
	info->lcd_ttl_revsel		  = 0;
	info->lcd_ttl_sthd			  = 0;
	info->lcd_ttl_sthh			  = 0;
	info->lcd_ttl_stvdl 		  = 0;
	info->lcd_ttl_stvdp 		  = 0;
	info->lcd_ttl_stvh			  = 0; 
		
	//cpu��Ļ��������Ϣ
	info->lcd_frm			 = 1;	//0: disable; 1: enable rgb666 dither; 2:enable rgb656 dither


}      

static __s32 LCD_open_flow(__u32 sel)
{
	LCD_OPEN_FUNC(sel, LCD_power_on	, 10); 	//��LCD���磬����ʱ10ms
	LCD_OPEN_FUNC(sel, LCD_io_init	, 20); 	//����ͳ�ʼ��IO������ʱ20ms
	LCD_OPEN_FUNC(sel, TCON_open	, 200); //��LCD������������ʱ200ms
	LCD_OPEN_FUNC(sel, LCD_open_cmd	, 10); 	//ͨ��IO����LCDģ��Ϊ����ģʽ������ʱ10ms
	LCD_OPEN_FUNC(sel, LCD_bl_open	, 0); 	//�򿪱��⣬����ʱ0ms

	return 0;
}

static __s32 LCD_close_flow(__u32 sel)
{	
	LCD_CLOSE_FUNC(sel, LCD_bl_close    , 0); 	//�رձ��⣬����ʱ0ms
	LCD_CLOSE_FUNC(sel, LCD_close_cmd   , 0); 	//ͨ��IO����LCDģ��Ϊ����/����ģʽ������ʱ0ms
	LCD_CLOSE_FUNC(sel, TCON_close	    , 0); 	//�ر�LCD ������������ʱ0ms
	LCD_CLOSE_FUNC(sel, LCD_io_exit	    , 0); 	//����ʱIO��״̬���ͷţ�����ʱ0ms
	LCD_CLOSE_FUNC(sel, LCD_power_off   , 700); //�ر�LCD���磬����ʱ1700ms

	return 0;
}
static __u32 lcd_power = 0;


////////////////////////////////////////   POWER   ////////////////////////////////////////////////////////////////////
static void LCD_power_on(__u32 sel)//0 active
{
	__lcd_panel_init_para_t para;

	LCD_get_init_para(&para);
	para.power_on(sel);
}

static void LCD_power_off(__u32 sel)//0 active
{
	__lcd_panel_init_para_t para;

	LCD_get_init_para(&para);
	para.power_off(sel);
}

////////////////////////////////////////   back light   ////////////////////////////////////////////////////////////////////
static void LCD_bl_open(__u32 sel)
{
    __u32 tmp;
	__lcd_panel_init_para_t para;


	
	if(lcd_power)
	{
		esPINS_SetPinIO(lcd_power, 1, NULL);	//output
		esPINS_WritePinData(lcd_power, 1, NULL);
	}
	 esKRNL_TimeDly(40);

	LCD_get_init_para(&para);
	
    tmp = sys_get_wvalue(para.base_timer+0xa0);
    tmp |= (1<<4);
    sys_put_wvalue(para.base_timer+0xa0,tmp);
}

static void LCD_bl_close(__u32 sel)
{
    __u32 tmp;
    __lcd_panel_init_para_t para;

	LCD_get_init_para(&para);
	
    tmp = sys_get_wvalue(para.base_timer+0xa0);
    tmp &= (~(1<<4));
    sys_put_wvalue(para.base_timer+0xa0,tmp);
	 esKRNL_TimeDly(10);
	if(lcd_power)
	{
	esPINS_SetPinIO(lcd_power, 1, NULL);	//output
	esPINS_WritePinData(lcd_power, 0, NULL);
	}
}


//����IO,����IO״̬Ϊ���HIGHT
static void  LCD_io_init(__u32 sel)
{	
  __s32 ret;
  __s32 lcd_usb;
    user_gpio_set_t  gpio_set[1];
  //return;
    ret = esCFG_GetKeyValue("lcd0_para", "lcd_power", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        lcd_power = esPINS_PinGrpReq(gpio_set, 1);
        if (!lcd_power)
        {
            __wrn("request lcd_bl pin failed\n");
            return ;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return ;
    }

	esPINS_SetPinIO(lcd_power, 1, NULL);	//output
	esPINS_WritePinData(lcd_power, 0, NULL);

	ret = esCFG_GetKeyValue("lcd0_para", "lcd_usb", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
	lcd_usb = esPINS_PinGrpReq(gpio_set, 1);
	
	esPINS_SetPinIO(lcd_usb, 1, NULL);	//output
	esPINS_WritePinData(lcd_usb, 1, NULL);
	//esKRNL_TimeDly(200);
}

//����IO״̬Ϊ���LOW,�ͷ�IO
static void LCD_io_exit(__u32 sel)
{
}

static void  LCD_open_cmd(__u32 sel)
{
}

static void  LCD_close_cmd(__u32 sel)
{
}

void LCD_get_panel_funs_0(__lcd_panel_fun_t * fun)
{
    fun->cfg_panel_info = LCD_cfg_panel_info;
    fun->cfg_open_flow = LCD_open_flow;
    fun->cfg_close_flow = LCD_close_flow;
}

