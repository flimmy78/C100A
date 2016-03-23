#include "com_ctrl.h"
#include "string.h"

//#define DEBUG_BY_YUWENWEN
//#define TEST_PACKAGE
//#define BLUETOOTH_BUFLEN  256

ES_FILE *fp = NULL; // �����ļ�ָ��
ES_FILE *blue_fp = NULL;
//__s32 BufferGpio = 0;
//#define MODIFY_BLUETOOTH_BAUDRATE  1
ES_FILE* pfile_pwm;
//#define MODIFY_BLUETOOTH_NAME  1

static __bool fanFlag = EPDK_FALSE;
static __u8 j = 0;

static __u16 STRIDE_TABLE[13] = {
	31,52,74,93,108,123,141,155,168,182,201,213,220,
};

 void Send_BT_status(__u8 status) 
{
	__u8 sendbuf[10];

	sendbuf[0] = 0x5b;
	sendbuf[1] = 0x02;
	sendbuf[2] = 0x03;
	sendbuf[3] = status;
	sendbuf[4] = 0x5d;
	bluetooth_writebuf(sendbuf,5);

}
__s32 init_buffer() 
{
	 user_gpio_set_t  gpio_set[1];
	 __s32 ret;

	 esDEV_Plugin("\\drv\\pwm.drv",0,0,1);
	 esKRNL_TimeDly(10);
	 pfile_pwm = eLIBs_fopen("b:\\USER\\PWM","rb+");
	 
	 eLIBs_fioctrl(pfile_pwm,PWM_CMD_SET_PWM_FRE,4,(void *)0);
	 eLIBs_fioctrl(pfile_pwm,PWM_CMD_SET_ACTIVY_CYCLE,20,(void *)0);

	 
	 
#if 0
	 ret = esCFG_GetKeyValue("lcd0_para", "lcd_buffer", (int *)gpio_set, sizeof(user_gpio_set_t)/4);
    if (!ret)
    {
        BufferGpio = esPINS_PinGrpReq(gpio_set, 1);
        if (!BufferGpio)
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
	esPINS_SetPinIO(BufferGpio, 1, NULL); //��Ϊ���
	esPINS_WritePinData(BufferGpio, 1, NULL);
#endif	
	return EPDK_OK;
}

void open_buffer(com_data_para_t *p_cmd_data,__u16 count,__bool keyTonFlag)
{
	#if 0
	reg_root_para_t *para;

	para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);

	if (!para->keyTFlag) {
		  eLIBs_fioctrl(pfile_pwm,PWM_CMD_START,0,(void *)0);
		 esKRNL_TimeDly(sec);
		 eLIBs_fioctrl(pfile_pwm,PWM_CMD_STOP,0,(void *)0);
	}
	
	return EPDK_OK;
	#endif
	//reg_root_para_t *para;

	//para = (reg_root_para_t *)dsk_reg_get_para_by_app(REG_APP_ROOT);

	if (!keyTonFlag) {
		p_cmd_data->flag = 1;
		p_cmd_data->crtl_com = REG_ADDR_WR_BUFFER;
		p_cmd_data->bufferCount = count ;
		p_cmd_data->com_func = COM_FUNC_WR_ONE;
		#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
		#endif
		p_cmd_data->flag = 0;
	}
	
}

void buffer_time(com_data_para_t *p_cmd_data,__u16 sec)
{
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WR_BUFFER_TIME;
	p_cmd_data->bufferTime= sec ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
	#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
	#endif
	p_cmd_data->flag = 0;
	
}



/*********************************************************************************
���룺pSrcBufָ����У��͵������׵�ַ��len���ݳ��ȣ���λ�ֽڡ�
�����У���
���ܣ�ͨ����λ���㷨������pSrcBuf��ʼ������Ϊlen�����ݵ�CRCУ��͡�
      ���������ǵ�λ��ͷ�����Բ��÷�ת����ʽ������ʽΪ0x8408��
**********************************************************************************/
__u16 calCRCBitByBit(__u8 *pSrcBuf,__u8 len)
{
    __u16 crcReg;
	__u8 i,j;	
	
	j=0;
	crcReg = 0xFFFF;

	while (len-- !=0)
	{
	    __u8 temp;
	    temp = pSrcBuf[j];
	    
		for (i=0x01;i!=0;i=i<<1)
		{
			if ((crcReg & 0x0001) != 0)
			{
				crcReg >>= 1;
				crcReg ^= 0x8408;
			}
			else
			{
				crcReg >>= 1;
			}

			if ((temp&i)!=0)
			{
				crcReg ^= 0x8408;
			}
		}
		j++;
	}

	return crcReg;
}

/*
__u16 crc_tbl[16] = 
{
0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
};
*/
__u16 crcNibbleTbl[16] = 
{
	0x0000, 0x1081, 0x2102, 0x3183, 
	0x4204, 0x5285, 0x6306, 0x7387, 
	0x8408, 0x9489, 0xa50a, 0xb58b, 
	0xc60c, 0xd68d, 0xe70e, 0xf78f 
};
/*********************************************************************************
���룺pSrcBufָ����У��͵������׵�ַ��len���ݳ��ȣ���λ�ֽڡ�
�����У���
���ܣ�ͨ�������ÿ4λ��һ�α�����pSrcBuf��ʼ������Ϊlen�����ݵ�CRCУ��͡�
      ���������ǵ�λ��ͷ�����Բ��÷�ת����ʽ������ʽΪ0x8408��
**********************************************************************************/
__u16 calCRCByTalbe(__u8 *pSrcBuf,__u8 len)
{
    __u8 crcLower,i=0;
    __u16 crcReg=0;

    crcReg=0xFFFF;

	while (len-- !=0)
	{	    
	    crcLower = crcReg & 0x0F;
	    crcReg >>= 4;
	    crcReg ^= crcNibbleTbl[crcLower^(pSrcBuf[i]&0x0F)];
	    crcLower = crcReg & 0x0F;
	    crcReg >>= 4;
	    crcReg ^= crcNibbleTbl[crcLower^(pSrcBuf[i]>>4)];
	    i++;
	}

	return crcReg;
}


/*********************************************************************************
���룺pSrcBuf������������׵�ַ��pResultBuf����������׵�ַ��len��������ݳ��ȣ�
      ����λ�ֽڡ�
�������������ݰ��ĳ��ȣ����Ȱ�����ͷ�Ͱ�β��
���ܣ��������͵����ݴ��������Ҫ���͵����ݡ������ͷ��У����֮��(��������ͷ��У����)
      ��У���룬׷�ӵ���β�����Ž�FD��FF֮������ݽ��в�֣����������ݰ��ĳ��ȡ�
**********************************************************************************/
__u8 comPacket(__u8 *pSrcBuf,__u8 *pResultBuf,__u8 len)
{
    __u16 crc;
    __u8  resultLen;
	//__u8 *p_test;
	int i;

	//p_test = pResultBuf;
    crc = calCRCByTalbe(pSrcBuf+1,len-1); // У����
   
    pSrcBuf[len] = (__u8)crc; 
    pSrcBuf[len+1] = (__u8)(crc>>8);
	
    len += 2;// ����CRC�󣬳�������2��

	// 0xFF
    pResultBuf[0] = pSrcBuf[0];
    pSrcBuf++;
    pResultBuf++;
	
    len--;// ȥ����ͷ��һ���ֽں󣬶����ݽ��в�֡�
    resultLen = 1;//oxff
    while(len)
    {
        if (*pSrcBuf >= PACK_FRAME_MAX_DATA) // if data > 0xFD
        {
            *pResultBuf = PACK_FRAME_MAX_DATA;//��ǰֵ���ΪFD+X
            pResultBuf++;//ָ�����
            *pResultBuf = (*pSrcBuf) - PACK_FRAME_MAX_DATA;//���ΪX
            pResultBuf++;//ָ�����
            pSrcBuf++;//ָ�����
            resultLen += 2;//���ȼӲ��Ϊ�����ֽ�
        }
        else//��������
        {
            *pResultBuf = *pSrcBuf;
            pResultBuf++;
            pSrcBuf++;   
            resultLen++;
        }
        
        len--;
    }

    *pResultBuf = 0xFE;//���ϰ�β
    resultLen++;	

    return resultLen;
}


/*********************************************************************************
���룺pSrcBuf������������׵�ַ��pResultBuf����������׵�ַ��rawPacketlen�����
      ���ݳ��ȣ���λ�ֽڡ�
�������������ΪTRUE��ʾ�õ�һ����ȷ�����ݰ���ΪFALSE��ʾ�õ�һ��У���������ݰ�
���ܣ������յ���δ����������ݰ����н������������CRCУ��͡�
**********************************************************************************/
__u8 comUnpacket(__u8 *pSrcBuf,__u8 *pResultBuf,__u8 rawPacketlen)
{
    __u8  unpacketBufLen;
    __u16 receivedCRC;
    __u16 CRCByCal;
    __u8  *pResultBufTemp;
	int i;

    if (rawPacketlen>32)
    {
        return EPDK_FALSE;
    }

	if (rawPacketlen < 2) {
		return EPDK_FALSE;
	}

    pResultBufTemp = pResultBuf;
    pResultBufTemp[0] = pSrcBuf[0];
    pSrcBuf++;
    pResultBufTemp++;
    rawPacketlen-=2;// ȥ����ͷ��β��һ���ֽں󣬶����ݽ��в�֡�
    unpacketBufLen = 1;

    while(rawPacketlen)
    {
        if ( (*pSrcBuf) >= PACK_FRAME_MAX_DATA )
        {
            pSrcBuf++;
            *pResultBufTemp = PACK_FRAME_MAX_DATA + (*pSrcBuf);
            pSrcBuf++; 
            pResultBufTemp++;  
            
            unpacketBufLen++;
            rawPacketlen -= 2;
        }
        else
        {
            *pResultBufTemp = *pSrcBuf;
            pSrcBuf++;  
            pResultBufTemp++;
            
            unpacketBufLen++;
            rawPacketlen--;
        }        
        
    }

	if (unpacketBufLen > 32){
		return 0;
	}
    receivedCRC = pResultBuf[unpacketBufLen-2] + ((__u16)pResultBuf[unpacketBufLen-1]<<8);
    CRCByCal = calCRCByTalbe(pResultBuf+1,unpacketBufLen-3);// ��ͷ����β��CRC��У��	
   if (receivedCRC==CRCByCal)
   //if (1)
    {
    	// ����ɹ����򷵻ؽ�ѹ��ĳ���
        return unpacketBufLen;
    }
    else
    {
	//	__msg("receivedCRC = 0x%x, CRCByCal=0x%x\n",receivedCRC, CRCByCal);
        return EPDK_FALSE;
    }      
}

/*
  * open_uart1 �򿪴���1����ͨѶ
  */
__s32 open_uart2(void)
{
	__wrn("com_uart_init begin...\n");
	 
	 if(NULL != fp)
    {
        __wrn("uart1 alreadly init...\n");
        return EPDK_OK;
    }
	 
	esDEV_Plugin("\\drv\\uart.drv", 2, (void *)COM_BAUD_RATE, 1);

	fp = eLIBs_fopen("b:\\BUS\\UART2", "rb+");//��uart1
	if (NULL == fp)
	{
		__wrn("open b:\\BUS\\UART2 failed\n");
		esDEV_Plugout("\\drv\\uart.drv",1);
		return EPDK_FAIL;
	}
    __wrn("com_uart_init success...\n");
	return EPDK_OK;
}



//����ͨѶ����
__s32 open_uart0()
{	
	__msg("com_uart_init begin...\n");
	 
	 if(NULL != blue_fp)
    {
        __wrn("uart0 alreadly init...\n");
        return EPDK_OK;
    }

	esDEV_Plugin("\\drv\\uart.drv", 0, (void *)COM_BAUD_RATE_BLUETOOTH, 1);
	blue_fp = eLIBs_fopen("b:\\BUS\\UART0", "rb+");//��uart0
	if (NULL == blue_fp)
	{
		__msg("open b:\\BUS\\UART0 failed\n");
		esDEV_Plugout("\\drv\\uart.drv",0);
		return EPDK_FAIL;
	}
	
    __msg("BT success...\n");
	return EPDK_OK;
}

__s32 read_BTData(__u8 *ResultBuf)
{
	//__u16 SrcBuf[4];
	__u16 ResultLen;
	//__u8 head_offsize = 0;
	//__u8 end_offsize  = 0;
	__u8 i;
	//__u8 tmp;
	//__u32 time_count;

	if (blue_fp == NULL){
        __wrn("g_fp_uart is null...\n");
        return EPDK_FAIL;
	}

	if (ResultBuf == NULL){
        __err("ResultBuf is null ...\n");
        return EPDK_FAIL;
	}

	eLIBs_memset(ResultBuf, 0, 10);
	//eLIBs_memset(SrcBuf   , 0, 4);
	
	ResultLen = 0;
	ResultLen = eLIBs_fread(ResultBuf, 1, 1, blue_fp);

	//__msg("huang#############   ResultLen = 0x%x \n",ResultLen);
	
	//__msg("huang#############   ResultBuf = 0x%x \n",ResultBuf[0]);
	

	return ResultLen;
}

__s32 write_BTData(__u8 cmd,__u8 len,__u16 *SrcData) 
{
	__u8 srcLen;
	__u16 putData[12];
	__u8 i;
	__s8 ret;

	putData[0] = 0x5b;
	putData[1] = len+1;
	putData[2] = cmd;
	putData[len+3] = 0x5d;

	for(i=3;i++;i<(len+3)) {
		putData[i] = SrcData[i];
	}
	
	ret = eLIBs_fwrite(putData, 1,len+4, blue_fp);
	//__msg("huang####  ret = %d \n", ret);
	return ret;
}


int  bluetooth_writebuf(__u8 *pucData,__u8 len)
{
	
	__u8 ret = 0;
	//char run_buf[12] = {"start"};
	//char run_buf1[12] = {"start|"};
	//__msg("read_buf length  ret :%d    **********    \n",ret,sizeof(pucData));

	//eLIBs_memset(pucData, 0, BLUETOOTH_BUFLEN);()
//	__msg("huang########   write  BT  \n");
	ret = eLIBs_fwrite(pucData, 1,len, blue_fp);
	
	//__msg(" read  ret   is :%d ************\n",ret);
	//ret = eLIBs_fwrite(pucData, 1,4, fp);
	return ret;

}

__u8  bluetooth_write_tx(__u32 *num,int len)
{

	int i,sum,str_len,ret ;
	char p_num[30] = {0};
	char pack_head[10] = {"start|"};
	char pack_end[10] = {"end"};

	//����У���
	sum = 0;
		for(i = 0;i < len;i++)
			{
			sum +=  num[i];
			}
	  	num[len] = sum;
//write  head
		str_len = strlen(pack_head);
		eLIBs_fwrite(pack_head, 1, str_len, blue_fp);
		#if 0
				if (ret != str_len){
				__msg("uart1 send  head data faild\n");
				return EPDK_FAIL;
			}
		#endif

//write data
		for(i = 0;i <= len;i++)
			{
			eLIBs_int2str_dec(num[i],p_num);
			eLIBs_strcat(p_num,"|");
//add |		
			#if 0
			if(eLIBs_strcat(p_num,"|") == NULL){
				__msg("eLIBs_strcat error *******\n");
				return -1;
			}
			#endif
			str_len = strlen(p_num);
			//__msg("send to app string :%s     ###### \n",p_num);
			eLIBs_fwrite(p_num, 1, str_len, blue_fp);

			#if 0
				if (ret != str_len){
				__msg("uart1 send data faild\n");
				return EPDK_FAIL;
			}
			#endif
			g_memset(p_num, 0, sizeof(p_num));
			
		}

//write  end
		str_len = strlen(pack_end);
		eLIBs_fwrite(pack_end, 1, str_len, blue_fp);
		#if 0
				if (ret != str_len){
				__msg("uart1 send  end data faild\n");
				return EPDK_FAIL;
		}
		#endif

		return 0;
}

/*
  * close_uart1 �򿪴���1����ͨѶ
  */
__s32 close_uart1(void)
{
    __wrn("com_uart_deinit begin...\n");
    
    if(NULL == fp)
    {
        __wrn("uart alreadly deinit...\n");
        return EPDK_OK;
    }
	
    if(fp)
    {
        eLIBs_fclose(fp);
        fp = NULL;
    }

    esDEV_Plugout("\\drv\\uart.drv",1);

    __wrn("com_uart_deinit success...\n");
    
    return EPDK_OK;
}
//add by Tong 20150516  for bluetooth

int  bluetooth_sendbuf(char *pucData)
{
	
	__u8 ret = 0;
	//char run_buf[12] = {"start"};
	//char run_buf1[12] = {"start|"};
	//__msg("read_buf length  ret :%d    **********    \n",ret,sizeof(pucData));

	eLIBs_memset(pucData, 0, sizeof(pucData));
	ret = eLIBs_fread(pucData, 1, 1, blue_fp);
	
	//__msg(" read  ret   is :%d ************\n",ret);
	return ret;

}



/*
		��������:����һ��������ָ�����飬������apk
		src_pack :������ͷ��β�ķ�������
		pack_len :ָ������Ԫ�ظ���
**/
__u8 bluetooth_write_data_demo(char *src_pack[],int pack_len)
{	
	__u8 ret = 0;
	__u8 i = 0;
	__u8 str_len = 0;


	//__msg("------------com_send_thread @ send_data @ beginning--------!\n");
	if(blue_fp == NULL){		
        __wrn("g_fp_uart is null...\n");
		return EPDK_FAIL;
	}

	  
		//pack_len = sizeof(src_pack);//���ڶ�ά�����ÿ��Ԫ�ز��ȳ�����ʱ�ô�������
			for(i = 0 ;i < pack_len ;i++)
				{
				str_len = strlen(src_pack[i]);
				__msg("bluetooth_write_data : i:%d   %s########\n",i,src_pack[i]);
				ret = eLIBs_fwrite(src_pack[i], 1, str_len, blue_fp);
				if (ret != str_len){
				__msg("uart1 send data faild\n");
				return EPDK_FAIL;
			}
			
		}
	return EPDK_OK;
}



__u8 send_data(__u8 *tx_buf, __u8 tx_len)
{	
	int ret;
	int i;
	//__msg("------------com_send_thread @ send_data @ beginning--------!\n");
	if(fp == NULL){		
        __wrn("g_fp_uart is null...\n");
		return EPDK_FAIL;
	}

	//for (i=0;i < tx_len; i++)
	//	__msg("tx_buf[%d] = 0x%x\n", i , tx_buf[i]);
	ret = eLIBs_fwrite(tx_buf, 1, tx_len, fp);

	if (ret != tx_len){
		__msg("uart1 send data faild\n");
		return EPDK_FAIL;
	}
	//__msg("------------com_send_thread @ send_data @ ending--------!\n");

	return EPDK_OK;
}

/*
	���洫��ָ��OK 
**/
int ret_check(int order_id)
	{
		int i = 0;
		char *p = NULL;
		int my_data[] = {0};
		int check_num = 0;
		char ret_buf[]= {0};
		__u16 SILVIEW_CMD [ ] = {0};
		bluetooth_sendbuf(ret_buf);
		__msg("HERE we ret_check :%s  ####\n",ret_buf);
		p = strtok(ret_buf, "|");
		
while (p){
	if(i == 0){

				if(eLIBs_strcmp(p,"start") != 0){
					   __msg("############start failed !..........\n");
						return -1;
					}else if(eLIBs_strcmp(p,"start") == 0){
					   __msg("############start match ok !..........\n");
					}
				}
	if(i > 0 ){
				if(eLIBs_strcmp(p,"end")== 0 ){
				   __msg("############buffer end !..........\n");
					break;
				}else{
					my_data[i] = eLIBs_atoi(p); // ����ȡ���ַ�ת��Ϊ����
					check_num += my_data[i];
					__msg("eLIBs_atoi #####################:----->%d\n",my_data[i]);
			}
					
		}
			p = strtok(NULL, "|");
			i++;
	}

		i = i - 1;
		
		check_num = check_num/2;

	
		
		if(check_num == my_data[i] ){
				__msg("check_num:%d    mydata[i]:%d------i:%d---This package is ok-------\n",check_num,my_data[i],i);
				
				if((my_data[0] == 0)&&(my_data[i] == order_id))
					{
						*p = NULL;
						__msg("congratulations  ret ok !!!!! #####################:----->%d\n",my_data[i]);
						return order_id;	
					}else{
						*p = NULL;
						return -1;
				}
				
			}else{
			//__msg("This package is fause######--------\n");
			*p = NULL;
			return -1;
		}


	}



__u8 read_data(__u8 *ResultBuf)
{
	__u8 SrcBuf[RECEIVE_PACK_LEN_MAX];
	__u8 ResultLen;
	__u8 head_offsize = 0;
	__u8 end_offsize  = 0;
	__u8 i;
	__u8 tmp;
	__u32 time_count;

	if (fp == NULL){
        __wrn("g_fp_uart is null...\n");
        return EPDK_FAIL;
	}

	if (ResultBuf == NULL){
        __err("ResultBuf is null ...\n");
        return EPDK_FAIL;
	}

	eLIBs_memset(ResultBuf, 0, RECEIVE_PACK_LEN_MAX);
	eLIBs_memset(SrcBuf   , 0, RECEIVE_PACK_LEN_MAX);
	
	ResultLen = 0;
	ResultLen = eLIBs_fread(SrcBuf, 1, RECEIVE_PACK_LEN_MAX, fp);
	
	
	if (ResultLen >= RECEIVE_PACK_LEN_MIN)
	{
		for (i = 0; i < ResultLen; i++) {
		//	__msg("SrcBuf[%d] = 0x%x\n",i, SrcBuf[i]);	
			if (SrcBuf[i] == 0xFF)
				head_offsize = i;
			if (SrcBuf[i] == 0xFE)
				end_offsize = i;
		}

		if (SrcBuf[head_offsize] != 0xFF || SrcBuf[end_offsize] != 0xFE){
			return 0;
		}
	
		ResultLen = (end_offsize-head_offsize)+1;
	//__msg("ResultLen = %d\n", ResultLen);
//		ResultLen =  comUnpacket(SrcBuf+offsize, ResultBuf, ResultLen);
		ResultLen =  comUnpacket(SrcBuf+head_offsize, ResultBuf, ResultLen);

		return ResultLen;
	}
	
	return 0;
}





void getNormalData(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_NORMAL_DATA;
		p_cmd_data->com_func = COM_FUNC_RD_SOME;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;

}



/********************************************************************/
// �����¿�����ģʽ
// mode = 0: ����ת��ģʽ
//          = 1:�㶨ת��ģʽ
//          = 2: ����ģʽ
//          = 3:����ģʽ
//          = 4:����ģʽ
//          = 5:����PI����ģʽ
//          = 6:ת��PI����ģʽ
//          = 7:λ��PI����ģʽ
/********************************************************************/
void SetRunMode(com_data_para_t *p_cmd_data, __u16 mode)
{
	__u8 err;

#ifdef DEBUG_COM_CTRL
	//	__msg("run run mode = %d\n", mode);
#endif
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_RUN_MODE;
	p_cmd_data->run_mode = mode;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag  = 0;
}


void SeclectMac(com_data_para_t *p_cmd_data, __u16 mode)
{

	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_MAC_SELECT;
	p_cmd_data->run_mode = mode;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag  = 0;
}

void SetMaxADC(com_data_para_t *p_cmd_data, __u16 adcValue)
{

	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WR_MAX_ADC;
	p_cmd_data->adcValue = adcValue;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag  = 0;
}

void SetMinADC(com_data_para_t *p_cmd_data, __u16 adcValue)
{

	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WR_MIN_ADC;
	p_cmd_data->adcValue = adcValue;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag  = 0;
}



// ���ö���
void SetCounter(com_data_para_t *p_cmd_data , __u16 *counter ,__u8 maxCounter)
{
	int i;

	p_cmd_data->flag 	= 1;
	for (i = 0; i < 20; i++) {
		p_cmd_data->counter[i] = *(counter+i);
	}
	p_cmd_data->MaxCounter = maxCounter;
	p_cmd_data->com_func = COM_FUNC_WR_SOME;	
	p_cmd_data->crtl_com = REG_ADDR_WR_DATA;
	
#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;


}


// �������п���
void SetRunCtr(com_data_para_t *p_cmd_data, __u8 ctr_cmd)
{
	__u8 err;
#ifdef DEBUG_COM_CTRL
	//__msg("run ctr cmd = %d\n", ctr_cmd);
#endif
	p_cmd_data->flag 	= 1;
	p_cmd_data->crtl_com = ctr_cmd;
	p_cmd_data->com_func = COM_FUNC_WR_CTR_CMD;	

	if(ctr_cmd == RUN_CMD_RUN){ // ����������� �������״̬Ϊ����״̬
		p_cmd_data->motor_status = MOTOR_STATUS_RUN;
	}else if (ctr_cmd == RUN_CMD_SLEEP){
	p_cmd_data->motor_status = RUN_CMD_SLEEP;
	}
#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;

	//  add for test ....  
	if (ctr_cmd == RUN_CMD_RUN) {
		esKRNL_TimeDly(10); // ����100ms
	}
}

// ������������
void SetRunRpm(com_data_para_t *p_cmd_data, __u16 rpm)
{
#if 0
	__u8 err;
#ifdef DEBUG_COM_CTRL
	//__msg("set run rpm = %d\n", rpm);
#endif
	if (rpm > 120){
		__err("SetRunRpm INVALID PARAMETER VALUE\n");
		return;
	}
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_ROTATE_SPD_SET;
	p_cmd_data->rotate_spd_set = rpm * 10;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;	
#ifndef DEBUG_FOR_STOP_COM
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;
#endif
}

void GetRunRPM(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_ROTATE_SPD_NOW;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;

}

void calbrationINC(com_data_para_t *p_cmd_data)
{
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WR_INC_CMD;
	p_cmd_data->INCCmd = 3 ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
	#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
	#endif
	p_cmd_data->flag = 0;
}

void InitINC(com_data_para_t *p_cmd_data)
{
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WR_INC_CMD;
	p_cmd_data->INCCmd = 2 ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
	#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
	#endif
	p_cmd_data->flag = 0;
}

void SetCMDLevel(com_data_para_t *p_cmd_data,__u16 cmd)
{
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_COMMAD;
	p_cmd_data->LevelCmd = cmd ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
	#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
	#endif
	p_cmd_data->flag = 0;
}

void SetPWM(com_data_para_t *p_cmd_data,__u16 pwm)
{
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_PWM;
	p_cmd_data->pwm = pwm ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
	#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
	#endif
	p_cmd_data->flag = 0;
}
void GetINCstatus(com_data_para_t *p_cmd_data)
{
	p_cmd_data->flag = 1;
	
	p_cmd_data->crtl_com = REG_ADDR_RD_INC_STATUS;
	p_cmd_data->com_func = COM_FUNC_RD_ONE;
	#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
	#endif
	p_cmd_data->flag = 0;
}

void GetRunLevel(com_data_para_t *p_cmd_data)
{
#if 0
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_RD_LEVEL;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;
#endif

}

void GetRunIncAdc(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_RD_INC_ADC;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;

}

void GetRunIncMINAdc(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_RD_MIN_ADC_INC;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;

}

void GetRunIncMAXAdc(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_RD_MAX_ADC_INC;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;

}

void GetCounter(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_RD_COUNTER;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;

}



// ��������
void SetRunLevel(com_data_para_t *p_cmd_data, __u16 level)
{
	if (level > 20){
		__err("SetRunResistance INVALID PARAMETER VALUE\n");
		return;
	}
	p_cmd_data->flag = 1;
	__msg("huang###########   level = %d \n",level);
	p_cmd_data->crtl_com = REG_ADDR_WR_LEVEL;
	p_cmd_data->level_set = level ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;
}

// ��������
void SetRunINC(com_data_para_t *p_cmd_data, __u16 inc)
{
	
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WR_INC;
	p_cmd_data->inc_set = inc ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;
}


void SetRunSTRDIE(com_data_para_t *p_cmd_data, __u16 inc)
{
	
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WD_STRIDE;
	p_cmd_data->inc_set = STRIDE_TABLE[inc] ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;
}


// �����������
void SetRunMAXINC(com_data_para_t *p_cmd_data, __u16 inc)
{
	
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_WR_MAX_INC;
	p_cmd_data->max_inc= inc ;
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;
}

// �����������
void SetFanOnOff(com_data_para_t *p_cmd_data, __u16 flag)
{
	if (flag != 2) {
		fanFlag = !fanFlag;
	} else {
		fanFlag = EPDK_FALSE;
	}
	p_cmd_data->flag = 1;

	p_cmd_data->crtl_com = REG_ADDR_FAN;
	p_cmd_data->fan= fanFlag ;
	__msg("huang######## fan  =   %d \n",fanFlag);
	p_cmd_data->com_func = COM_FUNC_WR_ONE;
#ifndef DEBUG_FOR_STOP_COM	
	com_send_thread(p_cmd_data);
#endif
	p_cmd_data->flag = 0;
}





//add by Tong for controller version 20150430
void GetVersion(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_PROGRAMME_VERSION;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;

}

void GetYEAR(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_PROGRAMME_YEAR ;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;
}

void GetDay(com_data_para_t *p_cmd_data)
{
		p_cmd_data->flag = 1;
	
		p_cmd_data->crtl_com = REG_ADDR_PROGRAMME_MONTH_DAY ;
		p_cmd_data->com_func = COM_FUNC_RD_ONE;
#ifndef DEBUG_FOR_STOP_COM	
		com_send_thread(p_cmd_data);
#endif
		p_cmd_data->flag = 0;
}




#if 1
void com_send_thread(com_data_para_t *p_cmd_data)
{
	
	__u8 SrcBuf[RECEIVE_PACK_LEN_MAX];
	__u8 ResultBuf[RECEIVE_PACK_LEN_MAX];
	__u8 ReadBuf[RECEIVE_PACK_LEN_MAX];
	__u8 ReadLen = 0;
	__u8 ResultLen;		
	__u16 get_num;
	__u8 err;
	__u8 i;
	
	//__msg("------------*********com_send_thread beginning********--------!\n");

	SrcBuf[0] = PACK_FRAME_HEADER;
	SrcBuf[1] = p_cmd_data->com_func;
	SrcBuf[2] = p_cmd_data->crtl_com; // ƫ�Ƶ�ַ���߿�����
	
	switch(p_cmd_data->com_func){ // ������
		case COM_FUNC_WR_CTR_CMD: // 0X10, д����ָ��

			ResultLen = comPacket(SrcBuf,ResultBuf,3);	// ѹ����

			break;
		case COM_FUNC_RD_CTR_CMD: // 0X11��������ָ��
			break;
		case COM_FUNC_WR_ONE: // 0x20�� дһ������
			switch(p_cmd_data->crtl_com){
				case REG_ADDR_MAC_SELECT:
					SrcBuf[3] = p_cmd_data->run_mode & 0xFF;
					SrcBuf[4] =(p_cmd_data->run_mode>>8) & 0xFF;
					break;
				case REG_ADDR_WR_LEVEL:
					SrcBuf[3] = p_cmd_data->level_set & 0xFF;
					SrcBuf[4] =(p_cmd_data->level_set>>8) & 0xFF;
					break;
				case REG_ADDR_WR_INC:
					SrcBuf[3] = p_cmd_data->inc_set& 0xFF;
					SrcBuf[4] =(p_cmd_data->inc_set>>8) & 0xFF;
					break;
				case REG_ADDR_WD_STRIDE:
					SrcBuf[3] = p_cmd_data->inc_set& 0xFF;
					SrcBuf[4] =(p_cmd_data->inc_set>>8) & 0xFF;
					break;
				case REG_ADDR_WR_MAX_INC:
					SrcBuf[3] = p_cmd_data->max_inc& 0xFF;
					SrcBuf[4] =(p_cmd_data->max_inc>>8) & 0xFF;
					break;
				case REG_ADDR_WR_MAX_ADC:
				case REG_ADDR_WR_MIN_ADC:
					SrcBuf[3] = p_cmd_data->adcValue& 0xFF;
					SrcBuf[4] =(p_cmd_data->adcValue>>8) & 0xFF;
					break;
				case REG_ADDR_WR_INC_CMD:
					__msg("huang####    incCmd = %d \n", p_cmd_data->INCCmd);
					SrcBuf[3] = p_cmd_data->INCCmd & 0xFF;
					SrcBuf[4] =(p_cmd_data->INCCmd>>8) & 0xFF;
					break;
				case REG_ADDR_COMMAD:
					SrcBuf[3] = p_cmd_data->LevelCmd & 0xFF;
					SrcBuf[4] =(p_cmd_data->LevelCmd>>8) & 0xFF;
					break;
				case REG_ADDR_PWM:
					SrcBuf[3] = p_cmd_data->pwm & 0xFF;
					SrcBuf[4] =(p_cmd_data->pwm>>8) & 0xFF;
					break;
				case REG_ADDR_FAN:
					SrcBuf[3] = p_cmd_data->fan& 0xFF;
					SrcBuf[4] =(p_cmd_data->fan>>8) & 0xFF;
					break;
				case REG_ADDR_WR_BUFFER:
					SrcBuf[3] = p_cmd_data->bufferCount& 0xFF;
					SrcBuf[4] =(p_cmd_data->bufferCount>>8) & 0xFF;
					break;
				case REG_ADDR_WR_BUFFER_TIME:
					SrcBuf[3] = p_cmd_data->bufferTime& 0xFF;
					SrcBuf[4] =(p_cmd_data->bufferTime>>8) & 0xFF;
					break;
				default:
					break;
			}

			ResultLen = comPacket(SrcBuf,ResultBuf,5);	// ѹ����
			break;
		case COM_FUNC_WR_SOME:
			switch(p_cmd_data->crtl_com) {
				case REG_ADDR_WR_DATA:
					SrcBuf[3] = p_cmd_data->MaxCounter & 0xFF;
					//SrcBuf[4] = (p_cmd_data->MaxCounter >> 8) & 0xFF;

					for(i = 0 ; i < 20 ; i++) {
						//__msg("huang#####   counter = %d \n",p_cmd_data->counter[i]);
						SrcBuf[4+i*2] = p_cmd_data->counter[i] & 0xFF;
						SrcBuf[5+i*2] = (p_cmd_data->counter[i] >> 8) & 0xFF; 
					}

					ResultLen = comPacket(SrcBuf,ResultBuf,44);	
					//__msg("huang#####   ResultLen = %d \n", ResultLen);
					break;
			}
			break;
		case COM_FUNC_RD_ONE: // 0x21����һ������	
		case COM_FUNC_RD_SOME:      	
			ResultLen = comPacket(SrcBuf,ResultBuf,3);	// ѹ����
			break;			
	}

	send_data(ResultBuf, ResultLen);
	
	ReadLen  = 0;
	i = 0;
	while(ReadLen == 0){
		i++;
		
		esKRNL_TimeDly(1); //sleep 20ms	
		//__msg("------------com_send_thread @ read_data @ beginning--------!\n");
		ReadLen = read_data(ReadBuf);
		//__msg("------------com_send_thread @ read_data @ ending--------!\n");
		if((i % 5) == 0){
		//	__msg("can't get true data!\n");
			send_data(ResultBuf, ResultLen);
		}
		if (i>12)
			return;
	}
	

	if (ReadLen >=RECEIVE_PACK_LEN_MIN){
		switch(ReadBuf[1]){//�ڶ��ֽ�ִ�гɹ���ʧ��
		case EXC_SUCCEED://ִ�гɹ�  EXC_SUCCEED    	0x00
			
			switch (ReadBuf[2]){//�����ֽڹ�����
			case COM_FUNC_RD_ONE:
			case COM_FUNC_RD_SOME:
			
				switch(ReadBuf[3]){

				case REG_ADDR_NORMAL_DATA:
					#if 0
					__msg("huang#####   HR = %d \n" ,  ReadBuf[5]);
					__msg("huang#####   HR1 = %d \n" ,ReadBuf[6]);
					__msg("huang#####   HR 2= %d \n" ,ReadBuf[7]);
				//	__msg("huang#####   PWM1 = %d \n" ,  ReadBuf[8]);
				//	__msg("huang#####   PWM2 = %d \n" , ReadBuf[9]);
				//	__msg("huang#####   ERR = %d \n" , ReadBuf[10]);
					#endif
					p_cmd_data->key_value = ReadBuf[4];
					p_cmd_data->HrFlag = ReadBuf[5];
					p_cmd_data->HandHr = ReadBuf[6];
					p_cmd_data->WirHr  = ReadBuf[7];
					p_cmd_data->curRpm  =  (ReadBuf[9]<<8 )| ReadBuf[8];
					p_cmd_data->error_code = ReadBuf[10];
					
					break;
			
				case REG_ADDR_RD_INC_STATUS:
					p_cmd_data->INCstatus = ReadBuf[4];
					__msg("huang######   INCstatus  = %d \n" , p_cmd_data->INCstatus);
					break;
				case REG_ADDR_RD_INC_ADC:
					p_cmd_data->curInc_ADC= ReadBuf[4];
					__msg("huang######   curInc_ADC  = %d \n" , p_cmd_data->curInc_ADC);
					break;
				case REG_ADDR_RD_MIN_ADC_INC:
					p_cmd_data->minInc_ADC= ReadBuf[4];
					__msg("huang######   minInc_ADC  = %d \n" , p_cmd_data->minInc_ADC);
					break;
				case REG_ADDR_RD_MAX_ADC_INC:
					p_cmd_data->maxInc_ADC = ReadBuf[4];
					__msg("huang######   maxInc_ADC  = %d \n" , p_cmd_data->maxInc_ADC);
					break;
				case REG_ADDR_RD_COUNTER:
					p_cmd_data->curCounter = ReadBuf[4];
					__msg("huang######   curCounter  = %d \n" , p_cmd_data->curCounter);
					break;
				case REG_ADDR_EXCEPTION_MES: // ������Ϣ
					p_cmd_data->error_code = get_num;
					if (p_cmd_data->error_code){
						__msg("error code = %d\n", p_cmd_data->error_code);
					}
					break;
				case REG_ADDR_MOTOR_STATUS: // ���״̬
					p_cmd_data->motor_status = get_num;
					break;
				
				case REG_ADDR_OVERLOAD://����ģʽ
					p_cmd_data->overload_status = get_num;
					break;
			//	case REG_ADDR_DIRECTION_NOW: // ���з���
					//p_cmd_data->direction_now = get_num;
				//	break;
				//case REG_ADDR_ROTATE_SPD_NOW:  // �����ٶ�
				//	p_cmd_data->rotate_spd_now = get_num;
				//	break;
				//case REG_ADDR_RESISTANCE_NOW: // ��������
					//p_cmd_data->resistance_now = (short)(ReadBuf[5]<<8 | ReadBuf[4]);
				//	break;
				//case REG_ADDR_POWER_NOW: // ���й���
				//	p_cmd_data->power_now = (short)(ReadBuf[5]<<8 | ReadBuf[4]);
				//	break;	


				case REG_ADDR_PROGRAMME_VERSION:
					p_cmd_data->version = get_num;
					
				break;
				case REG_ADDR_PROGRAMME_YEAR:
					p_cmd_data->year = get_num;
					
				break;
				case REG_ADDR_PROGRAMME_MONTH_DAY:
					p_cmd_data->date = get_num;
					
				break;
					
			//add end
				}
			break;
			case COM_FUNC_WR_ONE://COM_FUNC_WR_ONE       	0x40
			case COM_FUNC_WR_SOME:
				
			//__msg("huang#####  WR SUCESS  \n", ReadBuf[3]);			
				
			break;
			case COM_FUNC_WR_CTR_CMD:
				//__msg("ResultBuf:%d\n", ReadBuf[2]);
				break;
			case COM_FUNC_RD_CTR_CMD:
				break;
			}
				break;
		case EXC_FAILURE:
			__msg("huang######   error!!! \n");
			switch (ReadBuf[2]){
				case COM_FUNC_WR_ONE:
					__msg("clear  error   fail  !!!!!!!!\n");
					break;
				}
			break;
		case EXC_NO_FUN_C:
		case EXC_WR_FAILURE:
		case EXC_WR_INHIBIT:
		case EXC_NO_PARAMETER:
		case EXC_CRC_ERROR:
			
					
			p_cmd_data->error_code = ReadBuf[1];
			if (p_cmd_data->error_code) {
				__msg("uart1 faile ,err code = %d\n", p_cmd_data->error_code);
			}
		break;
		default:
			break;
		}

	}

	//esKRNL_TimeDly(1);
	//__msg("-----------------------*********com_send_thread ending********------------!\n");
}
#else

#endif
