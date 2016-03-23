/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                           Matrix Key Driver Module
*
*                                    (c) Copyright 2006-2010, All winners Co,Ld.
*                                             All Rights Reserved
*
* File    : matrix_key.c
* By      : james deng <csjamesdeng@gmail.com>
* Version : 1.0.0
* Date    : 2011-04-25
* Descript:
* Update  : date              author         ver       notes
*           2011-04-25        james deng     1.0.0     build the file.
*********************************************************************************************************
*/
 
#include "drv_matrix_key_i.h"


#define MENU        0x00000001
#define VOL         0x00000002
#define LEFT        0x00000004
#define RIGHT       0x00000008

static __u32 g_pw_off_support = 1;//Ϊ1�����ʹ��PA3�ڼ��ػ�����jtag�ڸ���

static __u32 input_row1 = 0;
static __u32 input_row2 = 0;
static __u32 input_row3 = 0;
static __u32 input_row4 = 0;
static __u32 input_row5 = 0;
static __u32 input_row6 = 0;
static __u32 input_row7 = 0;
static __u32 input_row8 = 0;

static __u32 pw_off = 0;

/* ɨ������ */
static __s32 period;
/* �ػ�ʱ�� */
static __s32 pw_off_time;
/* ���������ӳٵ�ʱ�䣬��Ϊɨ�����ڵı���������û����� */
static __s32 key_down_delay;

static __s32 volatile is_power_off = EPDK_FALSE;
static __krnl_stmr_t *timer = NULL;

static __u8 matrix_key_tid = 0;
static void matrix_key_down(__u32 key_value);
static void matrix_key_up(__u32 key_value);

//���¶���Ϊ�˽������ϰ����������¿��ܳ��ֵİ���down��up��Ϣ����Ե����
static __u32 g_key_down_tbl[KEY_CNT] = {0};


void delay_ms(__u8 ms)
{
	__s32 j = 0;
	__s32 cnt = 3000;	//69600;//76800; //  1ms
	while(j < cnt)
	{
		{
			j++;
		}
	}
}

/********************************************************************************************************
* Function:    matrix_key_read
* Description: read routine
* Input��      
* Output��     
* Other��      
*********************************************************************************************************/
static __u32 matrix_key_read()
{
	esPINS_SetPinIO(input_row4, 1, NULL);
	esPINS_WritePinData(input_row4, 1, NULL);
	delay_ms(1);
	
	//esPINS_SetPinIO(input_row5, 1, NULL);
	//esPINS_WritePinData(input_row5, 1, NULL);
	//delay_ms(1);
	
	//���
	esPINS_SetPinIO(input_row1, 1, NULL);
	esPINS_WritePinData(input_row1, 0, NULL);
	//����
	esPINS_SetPinIO(input_row2, 0, NULL);
	esPINS_SetPinIO(input_row3, 0, NULL);
	esPINS_SetPinIO(input_row4, 0, NULL);
	//esPINS_SetPinIO(input_row5, 0, NULL);

	delay_ms(1);
	delay_ms(1);
	if(!esPINS_ReadPinData(input_row2, 0))
	{
		__msg(" -- KPAD_ENTER -- \n");
		return KPAD_ENTER;
	}
	else if(!esPINS_ReadPinData(input_row3, 0))
	{
		__msg(" -- KPAD_LEFT -- \n");
		return KPAD_LEFT;
	}
	else if(!esPINS_ReadPinData(input_row4, 0))
	{	
		__msg(" -- KPAD_MENU -- \n");
		return KPAD_MENU;
	}
	//else if(!esPINS_ReadPinData(input_row5, 0))
	//{
	//	__msg(" -- KPAD_NUM8 -- \n");
	//	return KPAD_NUM8;
	//}
	else
	{
		esPINS_SetPinIO(input_row2, 1, NULL);
		esPINS_WritePinData(input_row2, 0, NULL);
		delay_ms(1);
		
		if(!esPINS_ReadPinData(input_row3, 0))
		{
			__msg(" -- KPAD_RIGHT -- \n");
			return KPAD_RIGHT;
		}
		else if(!esPINS_ReadPinData(input_row4, 0))
		{	
			__msg(" -- KPAD_NUM6 -- \n");
			return KPAD_NUM6;
		}
		//else if(!esPINS_ReadPinData(input_row5, 0))
		//{
		//	__msg(" -- KPAD_NUM9 -- \n");
		//	return KPAD_NUM9;
		//}
		else
		{
			esPINS_SetPinIO(input_row3, 1, NULL);
			esPINS_WritePinData(input_row3, 0, NULL);
			delay_ms(1);

			if(!esPINS_ReadPinData(input_row4, 0))
			{	
				__msg(" -- KPAD_NUM7 -- \n");
				return KPAD_NUM7;
			}
			//else if(!esPINS_ReadPinData(input_row5, 0))
			//{
			//	__msg(" -- KPAD_NUM0 -- \n");
			//	return KPAD_NUM0;
			//}
		}
	}
	return EPDK_OK;
}

/********************************************************************************************************
* Function:    matrix_key_down
* Description: matrix key down
* Input��      
* Output��     
* Other��      
*********************************************************************************************************/
static void matrix_key_down(__u32 key_value)
{
    __key_msg_t msg;
    
    msg.key_value = key_value;
    msg.flag = 1;
    
    dev_matrix_key_ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);

    g_key_down_tbl[key_value]++;
}

/********************************************************************************************************
* Function:    matrix_key_up
* Description: matrix key up
* Input��      
* Output��     
* Other��      
*********************************************************************************************************/
static void matrix_key_up(__u32 key_value)
{
    __key_msg_t msg;

    if(g_key_down_tbl[key_value] > 0)
    {
        g_key_down_tbl[key_value]--;
    }
    else
    {
        return ;
    }
    
    msg.key_value = key_value;
    msg.flag = 0;
    
    dev_matrix_key_ioctrl(0, DRV_KEY_CMD_PUTKEY, 0, &msg);
}

//ȫ�������ɿ������ڲ��䷢��δ���͵�key up��Ϣ
static void matrix_match_key_down(void)
{
    __s32 i;
    __s32 key_cnt;

    key_cnt = sizeof(g_key_down_tbl)/sizeof(g_key_down_tbl[0]);

    for(i = 0 ; i < key_cnt ; i++)
    {
        while(g_key_down_tbl[i] > 0)
        {
            matrix_key_up(i);
        }
    }
}

/********************************************************************************************************
* Function:    power_off_timer
* Description: 
* Input��      
* Output��     
* Other��      
*********************************************************************************************************/
static void power_off_timer(void* arg)
{
#if 0
    matrix_key_down(KPAD_POWEROFF);
    matrix_key_up(KPAD_POWEROFF);
	is_power_off = EPDK_TRUE;
#endif
}

/********************************************************************************************************
* Function:    matrix_key_thread
* Description: matrix key running thread
* Input��      
* Output��     
* Other��      
*********************************************************************************************************/
static void matrix_key_thread(void *p_arg)
{    
    __u32 old_key, new_key, key = 0;
	__s32 delay;

    //���¶��������Զ�����
    static int auto_test_enable;
    
    {//�����debugģʽ�����Զ����ԣ������ϵͳ��������Ϣ
        if(EPDK_OK == esCFG_GetKeyValue("auto_test_para", "auto_test_enable", &auto_test_enable, 1))
        {
            __msg("----=%d\n", auto_test_enable);
        }
        else
        {
            auto_test_enable = 0;
        }
        __msg("----=%d\n", auto_test_enable);
    } 
    
    while (1)
    {
        if (esKRNL_TDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
        {
            break;
        }              

    
		delay = 0;
        __here__;
		while (1)
		{
			esKRNL_TimeDly(period);
			new_key = matrix_key_read();

			if (new_key) /* ����һ����Чֵ */
			{
		                __here__;
				/* ǰ�����ζ�����ֵ��ͬ������ǰ�������ֵ��Ч */
				if ((new_key == old_key) && old_key)
				{
                   // __msg("new_key=%d\n", new_key);
                   // __msg("old_key=%d\n", old_key);
                    __here__;
					break;
				}
                __here__;


				
				delay += period;

				if (!(delay ^ key_down_delay))
				{
                    __here__;
					/* ������Ϣǰ����Ƿ��ӳ��Ƿ�ﵽ�����趨����ֵ */
				    key = new_key;
				    matrix_key_down(key);
				}
                __here__;
			}
			else if (key) /* �������� */
			{
                __here__;
				{
                    __here__;
				    matrix_key_up(key);
				}
				
				key = 0;
			    break; /* �˳���ѭ�� */
			}
			else /* ����һ����Чֵ */
			{
			    delay = 0;
                matrix_match_key_down();
			}
		}
    }
}

/********************************************************************************************************
* Function:    matrix_key_init
* Description: initial matrix key
* Input��      
* Output��     
* Other��      
*********************************************************************************************************/
__s32 matrix_key_init(void)
{
    __s32            ret;
    user_gpio_set_t  gpio_set[1];

    {//�����debugģʽ�����pw-off�ڸ�jtag��
        __target_para_t target;
        esKSRV_GetTargetPara(&target);
        __msg("target.debugenable=%d\n", target.debugenable);
        if(target.debugenable == EPDK_TRUE)
        {
            g_pw_off_support = 0;
        }
        else
        {
            g_pw_off_support = 0;//it is change
        }
    }   

	/* ����input_row1 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row1", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row1 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row1)
        {
            __wrn("request input_row1 pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }

	ret = esPINS_SetPinPull(input_row1, PIN_PULL_UP, NULL);
	if (ret)
	{
		__wrn("pull input_row1 failed\n");
		return EPDK_FAIL;
	}

	/* ����input_row2 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row2", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row2 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row2)
        {
            __wrn("request input_row2 pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }

	ret = esPINS_SetPinPull(input_row2, PIN_PULL_UP, NULL);
	if (ret)
	{
		__wrn("pull input_row2 failed\n");
		return EPDK_FAIL;
	}

	/* ����input_row3 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row3", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row3 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row3)
        {
            __wrn("request output_col pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }
	
	ret = esPINS_SetPinPull(input_row3, PIN_PULL_UP, NULL);
	if (ret)
	{
		__wrn("pull input_row2 failed\n");
		return EPDK_FAIL;
	}

	/* ����input_row4 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row4", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row4 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row4)
        {
            __wrn("request output_col pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }
	
	ret = esPINS_SetPinPull(input_row4, PIN_PULL_UP, NULL);
	if (ret)
	{
		__wrn("pull input_row2 failed\n");
		return EPDK_FAIL;
	}

	/* ����input_row5 */
#if 0	
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row5", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row5 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row5)
        {
            __wrn("request output_col pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }
	
	ret = esPINS_SetPinPull(input_row5, PIN_PULL_DOWN, NULL);
	if (ret)
	{
		__wrn("pull input_row2 failed\n");
		return EPDK_FAIL;
	}

	/* ����input_row6 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row6", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row6 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row6)
        {
            __wrn("request output_col pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }
	
	ret = esPINS_SetPinPull(input_row6, PIN_PULL_DOWN, NULL);
	if (ret)
	{
		__wrn("pull input_row2 failed\n");
		return EPDK_FAIL;
	}
	
	/* ����input_row7 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row7", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row7 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row7)
        {
            __wrn("request output_col pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }
	
	ret = esPINS_SetPinPull(input_row7, PIN_PULL_DOWN, NULL);
	if (ret)
	{
		__wrn("pull input_row2 failed\n");
		return EPDK_FAIL;
	}

	/* ����input_row8 */
    eLIBs_memset(gpio_set, 0, sizeof(user_gpio_set_t));
    ret = esCFG_GetKeyValue("matrixkey_para", "input_row8", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        input_row8 = esPINS_PinGrpReq(gpio_set, 1);
        if (!input_row8)
        {
            __wrn("request output_col pin failed\n");
            return EPDK_FAIL;
        }
    }
    else
    {
        __wrn("fetch para from script failed\n");
        return EPDK_FAIL;
    }
	
	ret = esPINS_SetPinPull(input_row8, PIN_PULL_DOWN, NULL);
	if (ret)
	{
		__wrn("pull input_row2 failed\n");
		return EPDK_FAIL;
	}

#endif	
	/* ��ȡɨ������ */
	ret = esCFG_GetKeyValue("matrixkey_para", "period", &period, 1);
	if (ret)
	{
	    __wrn("fetch para: period from script failed\n");
	    period = 2;
	}
	__msg("matrix key update period: %d\n", period);
	
	/* ��ȡ�ػ�ʱ�� */
	ret = esCFG_GetKeyValue("matrixkey_para", "pw_off_time", &pw_off_time, 1);
	if (ret)
	{
	    __wrn("fetch para: pw_off_time from script failed\n");
		pw_off_time = 2000;
	}
	__msg("power off time: %d\n", pw_off_time);

	/* ��ȡ���������ӳٵ�ʱ�� */
	ret = esCFG_GetKeyValue("matrixkey_para", "key_down_delay", &key_down_delay, 1);
	if (ret)
	{
		__wrn("fetch para: key_down_delay from script failed\n");
		key_down_delay = 2 * period;//3//3
	}
	__msg("key down delay: %d\n", key_down_delay);

	/* ����timer */
	timer = esKRNL_TmrCreate(
				pw_off_time, 
				OS_TMR_OPT_PRIO_HIGH | OS_TMR_OPT_PERIODIC,
				(OS_TMR_CALLBACK)power_off_timer,
				NULL);
	if (!timer)
	{
		__wrn("create timer failed\n");
		esKRNL_TmrDel(timer);
		timer = NULL;
	}

    matrix_key_tid = esKRNL_TCreate(matrix_key_thread, 0, 0x800, KRNL_priolevel1);
    
    __msg("matrix key init finish\n");
    
    return EPDK_OK;
}

/********************************************************************************************************
* Function:    matrix_key_exit
* Description: exit matrix key
* Input��      
* Output��     
* Other��      
*********************************************************************************************************/
__s32 matrix_key_exit(void)
{
    esPINS_PinGrpRel(input_row1, 0);
    esPINS_PinGrpRel(input_row2, 0);
    esPINS_PinGrpRel(input_row3, 0);
    esPINS_PinGrpRel(input_row4, 0);
#if 0	
    esPINS_PinGrpRel(input_row5, 0);
    esPINS_PinGrpRel(input_row6, 0);
    esPINS_PinGrpRel(input_row7, 0);
    esPINS_PinGrpRel(input_row8, 0);
#endif
    esKRNL_TDel(matrix_key_tid);

	if (!timer)
	{
		esKRNL_TmrDel(timer);
		timer = NULL;
	}

    __msg("matrix key exit finish\n");
    
    return EPDK_OK;
}
