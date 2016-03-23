/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: main_menu.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/

#include "app_home_i.h"
#include "manuel_bar.h"
#include "com_ctrl.h"

#if 0
    #define __here__            eLIBs_printf("@L%d(%s)\n", __LINE__, __FILE__);
	#define __msg(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __wrn(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
	#define __err(...)    		(eLIBs_printf("MSG:L%d(%s):", __LINE__, __FILE__),                 \
							     eLIBs_printf(__VA_ARGS__)									        )
#else
    #define __msg(...)
#endif

//32λͼ����8λͼ����ɫ���岻ͬ  �Լ���Ӷ���
#define MAIN_32_WHITE         0xffFFFFFF
#define MAIN_32_YELLOW		0xff00FFFF

#define SEL_ICON_WIDTH		22
#define SEL_ICON_HEIGH		150

static __u16 NoKeyCount = 0;

__bool SET_FLAG  = EPDK_FALSE;

static H_WIN h_deswin_old;

static __bool unitFlag = EPDK_FALSE;

static __s32 focus = 1;

static __u8 stopFocus = 0;

static __u8  numKeyCount = 0; //�������´���

static __u16 numTmp = 0;	//�������ֱ���

static __u16 ageTmp;
static __u16 weightTmp;
static __u16 timeTmp;

static __u8 fanFlag = 0;

static __u16 ageSrc;
static __u16 weightSrc;
static __bool keyTonFlag;
static __bool BTSET_FLAG;
static __u16  BT_weight;
static __u16  BT_age;
static __u16  BT_time;

static __bool KEYNUM_Flag = EPDK_FALSE;

static __u8 writeData[32];
static __u8 ShareData[32];
static __u8 pageData[2];

static __u16 SP_Data[5];

static __bool bufferOn = EPDK_TRUE;

#if 0
static __s32 setNUM_ID[] = {
	ID_HOME_TV_NUMBER_SETTING_0_BMP,
	ID_HOME_TV_NUMBER_SETTING_1_BMP,
	ID_HOME_TV_NUMBER_SETTING_2_BMP,
	ID_HOME_TV_NUMBER_SETTING_3_BMP,
	ID_HOME_TV_NUMBER_SETTING_4_BMP,
	ID_HOME_TV_NUMBER_SETTING_5_BMP,
	ID_HOME_TV_NUMBER_SETTING_6_BMP,
	ID_HOME_TV_NUMBER_SETTING_7_BMP,
	ID_HOME_TV_NUMBER_SETTING_8_BMP,
	ID_HOME_TV_NUMBER_SETTING_9_BMP,
};
#endif

static mm_res_id_t ui_res_manuel[] =
{
	{ID_HOME_BTN_SETTING_AGE_1_BMP,ID_HOME_BTN_SETTING_AGE_2_BMP},
	{ID_HOME_BTN_SETTING_WEIGHT_1_BMP,ID_HOME_BTN_SETTING_WEIGHT_2_BMP},
	{ID_HOME_BTN_SETTING_TIME_1_BMP,ID_HOME_BTN_SETTING_TIME_2_BMP},
};
 
static mm_res_id_t* manuel_get_ui_res(__s32 rotate)
{
	return ui_res_manuel;	
}



/*
	������Դ
*/
static __s32 init_manuel_res(mmenu_attr_t *mmenu_attr)
{
    __u32 	i;    
   // home_uipara_t* home_ui_para;
    mm_res_id_t* main_ui_res;

    if(EPDK_TRUE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }

   // home_ui_para = home_get_ui_para(GUI_GetScnDir());
  //  if(!home_ui_para)
  //  {
   //     __err("home_ui_para is null..\n");
   //     return EPDK_FAIL;
   // }    
//__msg("��������\n");
    main_ui_res = manuel_get_ui_res(GUI_GetScnDir());
    if(!main_ui_res)
    {
        __err("main_ui_para is null...\n");
        return EPDK_FAIL;
    }

	//for(i = 0 ; i < 10; i++)
	//{
		mmenu_attr->settingnum_bmp[0] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_0_BMP);
		mmenu_attr->settingnum_bmp[1] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_1_BMP);
		mmenu_attr->settingnum_bmp[2] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_2_BMP);
		mmenu_attr->settingnum_bmp[3] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_3_BMP);
		mmenu_attr->settingnum_bmp[4] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_4_BMP);
		mmenu_attr->settingnum_bmp[5] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_5_BMP);
		mmenu_attr->settingnum_bmp[6] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_6_BMP);
		mmenu_attr->settingnum_bmp[7] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_7_BMP);
		mmenu_attr->settingnum_bmp[8] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_8_BMP);
		mmenu_attr->settingnum_bmp[9] = dsk_theme_open(ID_HOME_TV_NUMBER_SETTING_9_BMP);
	//}
	
	mmenu_attr->item_nr = 3; 
	
	for(i = 0; i < mmenu_attr->item_nr; i++)
	{       				
		mmenu_attr->unfocus_bmp[i] = dsk_theme_open(main_ui_res[i].unfocus_bmp_id);				

		mmenu_attr->focus_bmp[i] = dsk_theme_open(main_ui_res[i].focus_bmp_id);	
	}    
	
	mmenu_attr->message_bmp_1 = dsk_theme_open(ID_HOME_TV_MESSAGE_SETTING_01_BMP);
	mmenu_attr->message_bmp_2 = dsk_theme_open(ID_HOME_TV_MESSAGE_SETTING_02_BMP);

	mmenu_attr->title_bmp = dsk_theme_open(ID_HOME_TV_SETTING_MANUAL_BMP);
    mmenu_attr->res_init = EPDK_TRUE;
	 
	return EPDK_OK;
}

/*
	�ͷ���Դ
*/
static __s32 uninit_manuel_res(mmenu_attr_t *mmenu_attr)
{
	  __u32 	i; 

    if(EPDK_FALSE == mmenu_attr->res_init)
    {
        return EPDK_OK;
    }
#if 1	  
	mmenu_attr->item_nr = 3;

	for (i=0; i<mmenu_attr->item_nr; i++)
	{
		__msg("i = %d\n", i);
		dsk_theme_close(mmenu_attr->unfocus_bmp[i]);
		dsk_theme_close(mmenu_attr->focus_bmp[i]);		
	}

	for (i = 0; i < 10; i++)
	{
		dsk_theme_close(mmenu_attr->settingnum_bmp[i]);
	}
	
	dsk_theme_close(mmenu_attr->message_bmp_1);
	dsk_theme_close(mmenu_attr->message_bmp_2);
#endif	
	dsk_theme_close(mmenu_attr->title_bmp);
    mmenu_attr->res_init = EPDK_FALSE;
    
	return EPDK_OK;
}

/*
	������
*/

static void draw_manuel_age(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 269;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (KEYNUM_Flag) {
		if (mmenu_attr->manuel_set_p[0].manuel_setNUM < AGE_MIN || mmenu_attr->manuel_set_p[0].manuel_setNUM > AGE_MAX) {
			mmenu_attr->manuel_set_p[0].manuel_setNUM = ageTmp;
		}
	}

	if (mmenu_attr->manuel_set_p[0].manuel_setNUM < AGE_MIN) {
		mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_MIN;
	} else if (mmenu_attr->manuel_set_p[0].manuel_setNUM > AGE_MAX) {
		mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_MAX;
	}
	

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM/10]);
	GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);

	ageTmp = mmenu_attr->manuel_set_p[0].manuel_setNUM;
	
}

static void draw_KEYNUM_manuel_age(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 269;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[0].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
}


/*
	������
*/
static void draw_manuel_weight(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 458;
	__s32 setNUM_y = 312;
	void *pbmp;

	if (KEYNUM_Flag) {
		if (!unitFlag) {
			if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = weightTmp;
			}
		} else {
			if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = weightTmp;
			}
		}		
	}

	if (!unitFlag) {
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MIN_B;
		} else if (mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MAX_B;
		}

	}else {
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MIN_M;
		} else if (mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
			mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_MAX_M;
		}
	}
	
	GUI_SetBkColor(0x00000000);
	GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*3, setNUM_y+125);

	if(mmenu_attr->manuel_set_p[1].manuel_setNUM >= 100)
	{
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	} else {
		//GUI_SetBkColor(0x00000000);
		//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
		} else {
			//GUI_SetBkColor(0x00000000);
			//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	}

	weightTmp = mmenu_attr->manuel_set_p[1].manuel_setNUM;
	}

static void draw_KEYNUM_manuel_weight(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 458;
	__s32 setNUM_y = 312;
	void *pbmp;

	GUI_SetBkColor(0x00000000);
	GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*3, setNUM_y+125);
	if(mmenu_attr->manuel_set_p[1].manuel_setNUM >= 100)
	{
		
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/100]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);

		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%100%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	} else {
		//GUI_SetBkColor(0x00000000);
		//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
		if (mmenu_attr->manuel_set_p[1].manuel_setNUM > 9) {
			pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM/10%10]);
			GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
		} else {
			//GUI_SetBkColor(0x00000000);
			//GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*3, setNUM_y+125);
		}
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[1].manuel_setNUM%10]);
		GUI_BMP_Draw(pbmp,setNUM_x+37*2,setNUM_y);
	}
}


/*
	��ʱ��
*/

static void draw_manuel_time(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 685;
	__s32 setNUM_y = 312;
	void *pbmp;

	if(KEYNUM_Flag) {
		if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
			mmenu_attr->manuel_set_p[2].manuel_setNUM = timeTmp;
		}
	}

	if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10 && mmenu_attr->manuel_set_p[2].manuel_setNUM > 0) {
		mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
	}
	
	if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= TIME_MIN && mmenu_attr->manuel_set_p[2].manuel_setNUM <= TIME_MAX) {

	} else {
		if (mmenu_attr->manuel_set_p[2].manuel_setNUM < TIME_MIN) {
			mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_MAX;
		} else if (mmenu_attr->manuel_set_p[2].manuel_setNUM > TIME_MAX) {
			mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_MIN;
		}
	}
	if (mmenu_attr->manuel_set_p[2].manuel_setNUM == 0) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);

	timeTmp = mmenu_attr->manuel_set_p[2].manuel_setNUM ;
	
}

static void draw_KEYNUM_manuel_time(mmenu_attr_t *mmenu_attr)
{
	__s32 setNUM_x = 685;
	__s32 setNUM_y = 312;
	void *pbmp;
	

	if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
		GUI_SetBkColor(0x00000000);
		GUI_ClearRect(setNUM_x, setNUM_y, setNUM_x+37*2, setNUM_y+125);
	} else  {
		pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM/10]);
		GUI_BMP_Draw(pbmp,setNUM_x,setNUM_y);
	}

	pbmp = dsk_theme_hdl2buf(mmenu_attr->settingnum_bmp[mmenu_attr->manuel_set_p[2].manuel_setNUM%10]);
	GUI_BMP_Draw(pbmp,setNUM_x+37,setNUM_y);
}



/*
	�����õ�����
*/

static void draw_setNUM(mmenu_attr_t *mmenu_attr, int num)
{
	if (num == 0)
	{
		draw_manuel_age(mmenu_attr);
	}
	
	if (num == 1)
	{
		draw_manuel_weight(mmenu_attr);
	}

	if (num == 2)
	{
		draw_manuel_time(mmenu_attr);
	}
	
	if(num == 3) 
	{
		
		draw_manuel_age(mmenu_attr);
		draw_manuel_weight(mmenu_attr);
		draw_manuel_time(mmenu_attr);
	}
	
	 
}

/*
	�������������õ�����
*/

static void draw_KEYNUM_setNUM(mmenu_attr_t *mmenu_attr, int num)
{
	if (num == 0)
	{
		draw_KEYNUM_manuel_age(mmenu_attr);
	}
	
	if (num == 1)
	{
		draw_KEYNUM_manuel_weight(mmenu_attr);
	}

	if (num == 2)
	{
		draw_KEYNUM_manuel_time(mmenu_attr);
	}

	 
}


static void draw_manuel_set(mmenu_attr_t *mmenu_attr)
{
	__s32 pbmp_x = 216;
	__s32 pbmp_y = 237;
	void *pbmp;
	__s32 i;

	for (i=0; i<3; i++)
	{
		if (mmenu_attr->manuel_set_p[i].manuel_focus)
		{
			pbmp = dsk_theme_hdl2buf(mmenu_attr->focus_bmp[i]);
		} else {
	 		pbmp = dsk_theme_hdl2buf(mmenu_attr->unfocus_bmp[i]);
		}
	
		GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);

		pbmp_x += 208;
	 }
}

static void draw_message(mmenu_attr_t *mmenu_attr, int num)
{
	__s32 pbmp_x = 0;
	__s32 pbmp_y = 504;
	void *pbmp;

	if (num == 1) {
		SET_FLAG = EPDK_TRUE;
		pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_1);
		
	}else {
		SET_FLAG = EPDK_FALSE;
		pbmp = dsk_theme_hdl2buf(mmenu_attr->message_bmp_2);
	}

	GUI_BMP_Draw(pbmp,pbmp_x,pbmp_y);
}

/*
	��ʾManuel����
*/

static void paint_manuel_all(mmenu_attr_t *mmenu_attr)
{
	void *pbmp;
	GUI_RECT gui_rect;
	
	
	if(EPDK_FALSE == mmenu_attr->res_init)
    {
        init_manuel_res(mmenu_attr);
    }

	GUI_LyrWinSel(mmenu_attr->layer);
	GUI_SetBkColor(0);
    //GUI_SetBkColor(TB_BACKCOLOR);

    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
	
	pbmp = dsk_theme_hdl2buf(mmenu_attr->title_bmp);
	gui_rect.x0 = 0;
    gui_rect.y0 = 70;  
    gui_rect.x1 = 1024;
    gui_rect.y1 = 600;
    GUI_ClearRectEx(&gui_rect);
	
	GUI_BMP_Draw(pbmp,0, 70);

	draw_manuel_set(mmenu_attr);

	draw_setNUM(mmenu_attr,3);

	if (BTSET_FLAG) {
		draw_message(mmenu_attr,2);
	} else {
		draw_message(mmenu_attr,1);
	}
	
}


/*
	�����水����Ӧ
*/
 __s32  manuelbar_key_proc(__gui_msg_t *msg)
{
	__u32 keycode;
	mmenu_attr_t *mmenu_attr;
	__s32 i;
	
	
	mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(h_deswin_old);
	if (!mmenu_attr)
	{
		__err("invalid para...\n");
		return EPDK_FAIL;
	}
	
	keycode = msg->dwAddData1;
	//__msg("huang####  keycode = %d \n",keycode);
	if (keycode != 4096) {
		NoKeyCount = 0;
	} else {
		NoKeyCount++;
		if (NoKeyCount > 6000) {
			main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
		}
	} 
	switch(keycode)
	{
		case FAN_KEY:
			fanFlag = !fanFlag;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			//__msg("huang#####  keyTonFlag = %d\n", keyTonFlag);
			SetFanOnOff(mmenu_attr->p_cmd_data,fanFlag);
			break;
		case START_KEY:
			if (BTSET_FLAG) {
				Send_BT_status(4); 
			}
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);

			
			if (mmenu_attr->manuel_set_p[0].manuel_setNUM > 99 || mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
				mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
			} 

			if (!unitFlag) {
				if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
					mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_B;
					
				}
			} else {
				if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
					mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
				
				}
			}	

			if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
				mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_INIT;
			}
			writeData[0] = mmenu_attr->manuel_set_p[0].manuel_setNUM;
			if (unitFlag) {
				writeData[2] = mmenu_attr->manuel_set_p[1].manuel_setNUM;
				writeData[3] = ((mmenu_attr->manuel_set_p[1].manuel_setNUM*11/5)>>8)&0xFF;
				writeData[4] = (mmenu_attr->manuel_set_p[1].manuel_setNUM*11/5)&0xFF;
			} else {
				writeData[2] = mmenu_attr->manuel_set_p[1].manuel_setNUM*5/11;
				writeData[3] = (mmenu_attr->manuel_set_p[1].manuel_setNUM>>8)&0xFF;
				writeData[4] = mmenu_attr->manuel_set_p[1].manuel_setNUM&0xFF;
			}
			writeData[1] = mmenu_attr->manuel_set_p[2].manuel_setNUM;
			
			if (!unitFlag) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = mmenu_attr->manuel_set_p[1].manuel_setNUM * 5 / 11;
			}
			
			SP_Data[0] = mmenu_attr->manuel_set_p[0].manuel_setNUM;
			SP_Data[1] = mmenu_attr->manuel_set_p[1].manuel_setNUM;
			SP_Data[2] = mmenu_attr->manuel_set_p[2].manuel_setNUM;
			SP_Data[3] = mmenu_attr->manuel_set_p[3].manuel_setNUM;
			
			SP_Data[4] = mmenu_attr->curID;

			pageData[0] = 0x20;
			writePage(writeData,pageData);
			
			main_cmd2manuel(h_deswin_old,ID_SPORTS,0,SP_Data); //����
			
			
			break;
		case STOP_KEY:
		{
			numKeyCount = 0;
			if (mmenu_attr->manuel_set_p[0].manuel_focus) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			
				main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
				break;
			} else {
				switch(focus) {
					case 1:
						if (mmenu_attr->manuel_set_p[0].manuel_setNUM > 99 || mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
							draw_manuel_age(mmenu_attr);
							return EPDK_OK;
						} 
						break;
					case 2:
						if (!unitFlag) {
							if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
								mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_B;
								draw_manuel_weight(mmenu_attr);
								return EPDK_OK;
							}
						} else {
							if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
								mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
								draw_manuel_weight(mmenu_attr);
								return EPDK_OK;
							}
						}	
						
						break;
					case 3:
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_INIT;
							draw_manuel_time(mmenu_attr);
							return EPDK_OK;
						}
						break;
				
			}
				stopFocus--;
				focus--;
				if (focus < 0) {
					focus = 3;
				}
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
				for (i = 0; i < 3; i++)
				{
					if (i != stopFocus) 
					{
						mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_FALSE;
					} else {
						mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_TRUE;
					}	
				}	
				if (stopFocus == 2) {
					draw_message(mmenu_attr,1);
				}
				draw_manuel_set(mmenu_attr);
				draw_setNUM(mmenu_attr,3);
			}
			break;
		}
		case ENTER_KEY:
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			numKeyCount = 0;
				switch(focus) {
					case 1:
						if (mmenu_attr->manuel_set_p[0].manuel_setNUM > 99 || mmenu_attr->manuel_set_p[0].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[0].manuel_setNUM = AGE_INIT;
							draw_manuel_age(mmenu_attr);
							return EPDK_OK;
						} 
						break;
					case 2:
						if (!unitFlag) {
							if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_B || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_B) {
								mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_B;
								draw_manuel_weight(mmenu_attr);
								return EPDK_OK;
							}
						} else {
							if (mmenu_attr->manuel_set_p[1].manuel_setNUM < WEIGHT_MIN_M || mmenu_attr->manuel_set_p[1].manuel_setNUM > WEIGHT_MAX_M) {
								mmenu_attr->manuel_set_p[1].manuel_setNUM = WEIGHT_INIT_M;
								draw_manuel_weight(mmenu_attr);
								return EPDK_OK;
							}
						}	
						
						break;
					case 3:
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM > 0 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = TIME_INIT;
							draw_manuel_time(mmenu_attr);
							return EPDK_OK;
						}
						break;
				
			}
			for (i = 0; i < 3; i++)
			{
				if (i != focus) 
				{
					mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_FALSE;
				} else {
					mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_TRUE;
				}	
			}	
			
			draw_manuel_set(mmenu_attr);
			draw_setNUM(mmenu_attr,3);
			if(focus == 0) {
				draw_message(mmenu_attr,1);
			}

			if (focus == 3) {
				draw_message(mmenu_attr,2);
			}
		
			focus ++;
			stopFocus = focus -1;
			if (focus > 3)
			{
				focus  = 0;
				stopFocus = 3;
			}
			
			numKeyCount = 0;
			break;
		case DOWN_LONG_KEY:
		case DOWN_QUICK_LONG_KEY:
		case INCDEC_LONG_KEY:
		case INCDEC_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case DOWN_KEY:
		case INCDEC_KEY:
			if (!SET_FLAG|| (focus == 0)) {
				return EPDK_TRUE;
			}
			
			numKeyCount = 0;
			KEYNUM_Flag = EPDK_FALSE;
			for (i = 0; i < 3; i++)
			{
			
				if (mmenu_attr->manuel_set_p[i].manuel_focus == EPDK_TRUE )
				{
					mmenu_attr->manuel_set_p[i].manuel_setNUM--;
					
					
					if (mmenu_attr->manuel_set_p[2].manuel_setNUM < 10 && mmenu_attr->manuel_set_p[2].manuel_setNUM > 0) {
						mmenu_attr->manuel_set_p[2].manuel_setNUM = 0;
					}
					
					draw_setNUM(mmenu_attr,i);
				}
				
			
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			break;
		
		case UP_LONG_KEY:
		case UP_QUICK_LONG_KEY:
		case INCPLUSE_LONG_KEY:
		case INCPLUSE_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case UP_KEY:
		case INCPLUSE_KEY:
			
			numKeyCount = 0;
			if (!SET_FLAG|| (focus == 0)) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			for (i = 0; i < 3; i++)
			{
			
				if (mmenu_attr->manuel_set_p[i].manuel_focus)
				{
					mmenu_attr->manuel_set_p[i].manuel_setNUM++;
					
					if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
						mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
					}
					
					draw_setNUM(mmenu_attr,i);
				}
				
			
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			break;

		
			
		case FAST_LONG_KEY:
		case FAST_LONG_QUICK_KEY:
		case LEVELPLUSE_LONG_KEY:
		case LEVELPLUSE_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case FAST_KEY:
		case LEVELPLUSE_KEY:
			numKeyCount = 0;
			if (!SET_FLAG|| (focus == 0)) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_FALSE;
			for (i = 0; i < 3; i++)
			{
			
				if (mmenu_attr->manuel_set_p[i].manuel_focus)
				{
					mmenu_attr->manuel_set_p[i].manuel_setNUM++;
					
					if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
						mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
					}
					
					draw_setNUM(mmenu_attr,i);
				}
				
			
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			break;

		
			
		case SLOW_LONG_KEY:
		case SLOW_LONG_QUICK_KEY:
		case LEVELDEC_LONG_KEY:
		case LEVELDEC_QUICK_LONG_KEY:
			bufferOn = EPDK_FALSE;
		case SLOW_KEY:
		case LEVELDEC_KEY:
			if (!SET_FLAG|| (focus == 0)) {
				return EPDK_TRUE;
			}
			numKeyCount = 0;
			KEYNUM_Flag = EPDK_FALSE;
			for (i = 0; i < 3; i++)
			{
			
				if (mmenu_attr->manuel_set_p[i].manuel_focus == EPDK_TRUE )
				{
					mmenu_attr->manuel_set_p[i].manuel_setNUM--;
					
					draw_setNUM(mmenu_attr,i);
				}
				
			
			}
			if (bufferOn) {
				open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			} else {
				bufferOn = EPDK_TRUE;
			}
			break;

		case MANUAL_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 1;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+1;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) { //����������أ��������ν���
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+1;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case HILL_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 2;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+2;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+2;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case FATBURN_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 3;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+3;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+3;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case CARDIO_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 4;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+4;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+4;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case STRENGTH_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 5;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+5;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+5;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case INTERUAL_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 6;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+6;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+6;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case USER1_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 7;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+7;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+7;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case USER2_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 8;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+8;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+8;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case HR1_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 9;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10+9;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+9;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;
		case HR2_KEY:
			if (!SET_FLAG) {
				return EPDK_TRUE;
			}
			KEYNUM_Flag = EPDK_TRUE;
			numKeyCount ++;
			open_buffer(mmenu_attr->p_cmd_data,1,keyTonFlag);
			if (numKeyCount == 1) {
				numTmp = 0;
			} else if (numKeyCount == 2) {
				numTmp = numTmp*10;
				if (!mmenu_attr->manuel_set_p[1].manuel_focus) {
					numKeyCount = 0;
				}
			} else if (numKeyCount == 3) {
				numTmp = numTmp*10+0;
				numKeyCount = 0;
			}

			for (i = 0; i < 3; i++)
				{
					if (mmenu_attr->manuel_set_p[i].manuel_focus)
					{
						mmenu_attr->manuel_set_p[i].manuel_setNUM = numTmp;
						/*
						if (mmenu_attr->manuel_set_p[2].manuel_setNUM >= 1 && mmenu_attr->manuel_set_p[2].manuel_setNUM < 10) {
							mmenu_attr->manuel_set_p[2].manuel_setNUM = 10;
						}
						*/
						draw_KEYNUM_setNUM(mmenu_attr,i);
					}	
				}
			break;

		case BT_DISCONNECT:
			main_cmd2manuel(h_deswin_old,ID_MAIN,0,SP_Data); 
			break;
		default:
			break;

	}


	return EPDK_OK;
}
	
/*
	
*/
static __s32 _manuel_bar_Proc(__gui_msg_t *msg)
{
   // home_uipara_t* home_ui_para;
   // __here__;
    
  //  home_ui_para = home_get_ui_para(GUI_GetScnDir());
  //  if(!home_ui_para)
   // {
   // __here__;
    //    __err("home_ui_para is null..\n");
    //    return ;
   // }   
	switch(msg->id)
	{
	case GUI_MSG_CREATE:
		{
			return EPDK_OK;
			#if 0
			mmenu_attr_t *mmenu_attr;
			mmenu_para_t *mmenu_para;

			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			
			mmenu_attr = (mmenu_attr_t *)esMEMS_Balloc(sizeof(mmenu_attr_t));
            if(!mmenu_attr)
            {
                return EPDK_FAIL;                
            }
			eLIBs_memset((void *)mmenu_attr, 0, sizeof(mmenu_attr));
			init_manuel_res(mmenu_attr);
			
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;
			mmenu_attr->item_w = home_ui_para->item_width;
			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
			mmenu_attr->focus_txt_color = MAIN_32_YELLOW;//APP_COLOR_YELLOW;
			mmenu_attr->unfocus_txt_color =   MAIN_32_WHITE;//APP_COLOR_BLACK; //cai modify//APP_COLOR_WHITE;
            mmenu_attr->res_init = EPDK_FALSE;

			
			GUI_WinSetAddData(msg->h_deswin, (__s32)mmenu_attr);
            
		}
		return EPDK_OK;
		#endif
		
		//	reg_sport_para_t *para;	
		//	__s32 i;
			//para = (reg_sport_para_t*)dsk_reg_get_para_by_app(REG_APP_SPORT);
			//for(i = 0; i < 2; i++){
			//	para->test[i] = 5;
			//}
			break;
		}
	case GUI_MSG_CLOSE:
		{
			
			GUI_FrmWinDelete(msg->h_deswin);
		}
		return EPDK_OK;
	case GUI_MSG_DESTROY:
		{
			mmenu_para_t *mmenu_para;
			mmenu_attr_t *mmenu_attr;
			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);
			uninit_manuel_res(mmenu_attr);
		
			esMEMS_Bfree(mmenu_attr, sizeof(mmenu_attr_t));
			esMEMS_Bfree(mmenu_para, sizeof(mmenu_para_t));//esMEMS_Bfree(mmenu_para, sizeof(mmenu_para));//�ڴ��ͷ�����
			
		}
		return EPDK_OK;
	case GUI_MSG_PAINT:
        {
			mmenu_attr_t *mmenu_attr;
			mmenu_para_t *mmenu_para;
			__s32 i;

			mmenu_para = (mmenu_para_t *)GUI_WinGetAttr(msg->h_deswin);
			
			mmenu_attr = (mmenu_attr_t *)esMEMS_Balloc(sizeof(mmenu_attr_t));
            if(!mmenu_attr)
            {
                return EPDK_FAIL;                
            }
			eLIBs_memset((void *)mmenu_attr, 0, sizeof(mmenu_attr));
			init_manuel_res(mmenu_attr);
			
			mmenu_attr->font = mmenu_para->mmenu_font;
			mmenu_attr->layer = mmenu_para->layer;
			//mmenu_attr->item_w = home_ui_para->item_width;
			mmenu_attr->focus_item = mmenu_para->focus_id;
			mmenu_attr->first_item = 0;
			NoKeyCount = 0;
			focus = 1;
			stopFocus = 0;
			KEYNUM_Flag = EPDK_FALSE;
			pageData[0] = 0x20;
			readPage(ShareData,pageData);
			for(i=0;i<32;i++) {
				writeData[i] = ShareData[i];
			}
			
			mmenu_attr->manuel_set_p[0].manuel_setNUM = ShareData[0];
			
            mmenu_attr->res_init = EPDK_FALSE;	
			__msg("huang#########3  unitFlag = %d \n",unitFlag);
			if (unitFlag) {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = ShareData[2];
			} else {
				mmenu_attr->manuel_set_p[1].manuel_setNUM = ShareData[3]*256+ShareData[4];
			}
		
			mmenu_attr->manuel_set_p[2].manuel_setNUM = ShareData[1];
			
			mmenu_attr->manuel_set_p[0].manuel_focus = EPDK_TRUE;
			for(i = 1 ; i < 4; i++)
			{
				mmenu_attr->manuel_set_p[i].manuel_focus = EPDK_FALSE;
			}

		
			GUI_WinSetAddData(msg->h_deswin, (__s32)mmenu_attr);

            if (GUI_LYRWIN_STA_ON != GUI_LyrWinGetSta(GUI_WinGetLyrWin(msg->h_deswin)))
            {
                __msg("layer not on, not need paint...\n");
    			return EPDK_OK;
            }

			h_deswin_old = msg->h_deswin;
            mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(msg->h_deswin);

            if (!mmenu_attr)
    		{
    			__err("invalid para...\n");
    			return EPDK_FAIL;
    		}
		
			__here__;
           
    		if (BTSET_FLAG) {
				mmenu_attr->manuel_set_p[0].manuel_setNUM = BT_age;
				mmenu_attr->manuel_set_p[1].manuel_setNUM = BT_weight;
				mmenu_attr->manuel_set_p[2].manuel_setNUM = BT_time;
			}
    		paint_manuel_all(mmenu_attr);
		
			
    		return EPDK_OK;
	    }

	case GUI_MSG_KEY:
    {
		manuelbar_key_proc(msg);
		return EPDK_OK;
	}
    case GUI_MSG_TOUCH:	
	{
		//mmenu_touch_proc(msg);
        return EPDK_OK;
	}			
    case GUI_MSG_TIMER:
    {        
        __msg("main_menu_proc, GUI_MSG_TIMER\n");
        return EPDK_OK;
    }
	case GUI_MSG_COMMAND:
		return EPDK_OK;
	case GUI_MSG_WIN_WAKEUP:
		GUI_InvalidateRect(msg->h_deswin, NULL, EPDK_TRUE);
		return EPDK_OK;
	}
	
	return GUI_FrmWinDefaultProc(msg);	
}

__s32 manuel_bar_win_create(H_WIN h_parent, mmenu_para_t *para)
{
	__gui_framewincreate_para_t framewin_para;
	mmenu_para_t *mmenu_para;
	FB fb;

	
	GUI_LyrWinGetFB(para->layer, &fb);
    __here__;

	mmenu_para = (mmenu_para_t *)esMEMS_Balloc(sizeof(mmenu_para_t));
    if(!mmenu_para)
    {
    __here__;
        return EPDK_FAIL;
    }
	eLIBs_memset((void*)mmenu_para, 0, sizeof(mmenu_para_t));
	mmenu_para->layer = para->layer;
	mmenu_para->focus_id = para->focus_id;
	BTSET_FLAG = para->BTSET_Flag;
	BT_weight = para->weight;
	BT_time = para->time;
	BT_age = para->age;
	pageData[0] = 0x60;
	readPage(writeData,pageData);
	unitFlag = writeData[0];
	keyTonFlag = writeData[3];
	eLIBs_memset(&framewin_para, 0, sizeof(__gui_framewincreate_para_t));
	framewin_para.name =	"manuel_bar_win",
	framewin_para.dwExStyle = WS_EX_NONE;
	framewin_para.dwStyle = WS_NONE|WS_VISIBLE;
	framewin_para.spCaption =  NULL;
	framewin_para.hOwner	= NULL;
	framewin_para.id         = MANUEL_BAR_ID;
	framewin_para.hHosting = h_parent;
	framewin_para.FrameWinProc = (__pGUI_WIN_CB)esKRNL_GetCallBack((__pCBK_t)_manuel_bar_Proc);
	framewin_para.rect.x =0;
	framewin_para.rect.y = 0;
	framewin_para.rect.width= fb.size.width;
	framewin_para.rect.height = fb.size.height;
	framewin_para.BkColor.alpha =  0;
	framewin_para.BkColor.red = 0;
	framewin_para.BkColor.green = 0;
	framewin_para.BkColor.blue = 0;
	framewin_para.attr =  (void *)mmenu_para;
	framewin_para.hLayer = para->layer;
    __here__;

	return (GUI_FrmWinCreate(&framewin_para));
}

void manuel_bar_win_delete(H_WIN mm_win)
{
	GUI_FrmWinDelete(mm_win);
}

void manuel_bar_res_uninit(H_WIN hwin)
{    
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        uninit_manuel_res(mmenu_attr);
    }
}

void manuel_bar_res_init(H_WIN hwin)
{
    mmenu_attr_t * mmenu_attr = (mmenu_attr_t *)GUI_WinGetAddData(hwin);

    if(mmenu_attr)
    {
        init_manuel_res(mmenu_attr);
    }
}

