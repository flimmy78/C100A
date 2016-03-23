/*
**************************************************************************************************************
*											         ePDK
*						            the Easy Portable/Player Develop Kits
*									           desktop system 
*
*						        	 (c) Copyright 2007-2010, ANDY, China
*											 All Rights Reserved
*
* File    	: main_uipara.c
* By      	: Andy.zhang
* Func		: desk main thread
* Version	: v1.0
* ============================================================================================================
* 2009-7-20 8:51:52  andy.zhang  create this file, implements the fundemental interface;
**************************************************************************************************************
*/
#include "home_uipara.h"
#include "main_menu.h"
//#include "sub_menu.h"

static home_uipara_t uipara_400_240 =
{
    {
        0, 171, 400, 240-171
    },
    {
        0, 40, 118, 123
    },
    400/5,//һ��5��
    8, //7
    5,//һ��5��
    45,//�����
    45,//���߶�
        
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};

#if 0
static home_uipara_t uipara_480_272 =
{
    {
        0, 185, 480, 272-185
    },
    {
        0, 56, 118, 123
    },
    480/5,//һ��5��
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
    5,//һ��5��
    45,//���ͼ����
    45,//���ͼ��߶�
    
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#else
  //������ʹ��2x4����
  static home_uipara_t uipara_480_272 =
{
    {
        0, 42, 480, 230
    },
    {
        0, 0, 480, 272
    },
    480/5,//һ��5��
    230/2,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
#ifdef HIDE_RECORD
    7,//�ܹ�7��ͼ��
#else
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#endif    
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#else
    10,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#endif
    2,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    4,//һ��5��
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
	4,
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
	4,
#else
    5,//һ��5��
#endif
    65,//���ͼ����
    55,//���ͼ��߶�

  //��Ŀ����  //dsk_type_string
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#endif
#if (MAX_ROW_NUM==1)
  static home_uipara_t uipara_800_480 =
{
    {
        0, 240, 800, 240
    },
    {
        0, 0, 200, 480
    },
    800/10,//һ��5��
    480/4,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
#ifdef HIDE_RECORD
    7,//�ܹ�7��ͼ��
#else
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#endif    
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#else
    10,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#endif
    1,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    4,//һ��5��
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
	5,
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
	4,
#else
    5,//һ��5��
#endif
    100,//���ͼ����
    100,//���ͼ��߶�

//sub menu
  //��Ŀ����  //dsk_type_string
    118,//item width
    24,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#else
  static home_uipara_t uipara_800_480 =
{
    {
        0, 0, 1024, 600
    },
    {
        0, 0, 1024, 600
    },
    800/10,//һ��5��
    480/3,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
#ifdef HIDE_RECORD
    7,//�ܹ�7��ͼ��
#else
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#endif    
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
    8,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#else
    10,//�ܹ�7��ͼ��//cai++  �ӵ�8��ͼ��
#endif
    2,
#if defined(NO_EXPLORE)&&defined(NO_CALENDAR)    
    4,//һ��5��
#elif defined(NO_CALENDAR)&&defined(HIDE_LINEIN)    
	4,
#elif defined(NO_CALENDAR)&&defined(HIDE_EBOOK)    
	4,
#else
    5,//һ��5��
#endif
    130,//���ͼ����
    130,//���ͼ��߶�

//sub menu
  //��Ŀ����  //dsk_type_string
    118,//item width
    30,//item height
    118,//line width
    3,//line height
    118,//top width
    2,//top height
    118,//bottom width
    6//bottom height
};
#endif
home_uipara_t* home_get_ui_para(__s32 rotate)
{
    __s32           screen_width;
    __s32 			screen_height;
	
	/* get lcd size*/
	dsk_display_get_size(&screen_width, &screen_height);
    
	switch(rotate)
	{
	case GUI_SCNDIR_NORMAL:
	case GUI_SCNDIR_ROTATE180:
		{
			if((screen_width == 400 )&&( screen_height == 240))
				return &uipara_400_240;	
			else if((screen_width == 480 )&&( screen_height == 272))
				return &uipara_480_272;	
			else if((screen_width == 800 )&&( screen_height == 480))
				return &uipara_800_480;	
			
		}		
	case GUI_SCNDIR_ROTATE90:
	case GUI_SCNDIR_ROTATE270:
		
		break;
	}	

#ifdef FOR_800X480_UI
	return &uipara_800_480;		
#else
	return &uipara_480_272;		
#endif
}



