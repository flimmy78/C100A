#ifndef _COM_CTRL_H_
#define _COM_CTRL_H_

//#include "app_home_i.h"
#include "apps.h"
#include "beetles_app.h"


//#define DEBUG_COM_CTRL

// ������
#define COM_BAUD_RATE 38400
#define COM_BAUD_RATE_BLUETOOTH 115200

// ���ݽ���״̬�����հ�ͷ�����հ�ͷ�������
#define RECEIVE_CHECK_HEADER 0
#define RECEIVE_PACK_DATA    1

// ���ݰ���ͷ���������
#define PACK_FRAME_HEADER    0xFF //    ֡��ͷ
#define PACK_FRAME_END       0xFE //    ֡����
#define PACK_FRAME_MAX_DATA  0xFD //    ֡���������

// ���ջ���״̬�������ջ���Ϊ��ʱ���趨Ϊ0���ǿ�ʱ�趨Ϊ���յ������ݰ�����
#define RX_BUF_STATUS_EMPTY   0

// ���ݰ������С�����޶�
#define RECEIVE_PACK_LEN_MIN    5
#define RECEIVE_PACK_LEN_MAX    64

// ���ݰ����ճ�ʱʱ�䣬��λ 100ms
#define RECEIVE_PACK_TIME_MAX    3

/***********************************************************************************/
#define COM_ST_IDLE  0 // ���Ϳ���
#define COM_ST_TX    1 // ������
#define COM_ST_WAP   2 // �ȴ�Ӧ��

/*
0x00������ִ�гɹ�
0x01������ִ��ʧ��
0x02�������벻����
0x03������д��ʧ��
0x04�����ݷ�д
0x05������������
0x80������У�����
*/
#define  EXC_SUCCEED    	0x00
#define  EXC_FAILURE    	0x01
#define  EXC_NO_FUN_C   	0x02
#define  EXC_WR_FAILURE 	0x03
#define  EXC_WR_INHIBIT 	0x04
#define  EXC_NO_PARAMETER 	0x05
#define  EXC_CRC_ERROR  	0x80


#define REG_ADDR_OVERLOAD          0xA8



// ���ݰ��ڸ��������ݰ���ƫ����
#define TX_HEAHER_OFFSET    0  //  
#define TX_FUNCTION_OFFSET  1  //  

#define RX_HEAHER_OFFSET      0 //  
#define RX_EXC_RESULT_OFFSET  1 //  
#define RX_FUNCTION_OFFSET    2 //  

// ͨ�Ź�����
#define COM_FUNC_WR_CTR_CMD   	0x10  //   д����ָ� ���¿�д���������״ָ̬��
#define COM_FUNC_RD_CTR_CMD   	0x11  //   ������ָ���ȡ�¿�����յ��Ŀ���ָ��
#define COM_FUNC_WR_ONE       		0x20  //   дһ��������д���¿�ָ����ŵĲ���
#define COM_FUNC_RD_ONE       		0x21  //   ��һ���������ȡ�¿�ָ����ŵĲ���
#define COM_FUNC_WR_SOME      	0x40  //   д���ɸ�������д���ɸ��������¿�
#define COM_FUNC_RD_SOME      	0x41  //   �����ɸ����������¿ض����ɸ�����
#define COM_FUNC_IDLE				0xFF

// ������Աר��
#define COM_FUNC_RD_ONE_DEVELOPER       0x46
#define COM_FUNC_WR_ONE_DEVELOPER       0x47

/*
	����λ��
*/
#define REG_ADDR_DIRECTION_SET    0  // ת�������趨R/W
#define REG_ADDR_DIRECTION_NOW    1  // ʵ��ת������R
#define REG_ADDR_ROTATE_SPD_SET   2  // ת���趨��R/W
#define REG_ADDR_ROTATE_SPD_NOW   3  
#define REG_ADDR_RESISTANCE_SET   4	 // �����趨
#define REG_ADDR_RESISTANCE_NOW   5
#define REG_ADDR_POWER_SET        6  // �����趨
#define REG_ADDR_POWER_NOW        7

#define REG_ADDR_MAC_SELECT		0x0 //ѡ�����
#define REG_ADDR_ERR				0x23 //��Բ��������Ϣ
#define REG_ADDR_WR_DATA			0x4B //��Բ����д�����������COUNTER��
#define REG_ADDR_NORMAL_DATA     0x4C //����Բ����̬���ݰ����

#define REG_ADDR_RD_INC_ADC		0x16 //����ǰ��ADCֵ
#define REG_ADDR_RD_INC_STATUS   0x17 //����Բ������״̬
#define REG_ADDR_RD_MIN_ADC_INC	0x18 //���������ADCֵ
#define REG_ADDR_RD_MAX_ADC_INC	0x19
#define REG_ADDR_WR_INC			0x15	//���øö�����
#define REG_ADDR_WR_MAX_ADC		0x19
#define REG_ADDR_WR_MIN_ADC		0x18
#define	REG_ADDR_WR_MAX_INC		0x1A	//�����������
#define REG_ADDR_WR_INC_CMD		0x1B //д����������
#define REG_ADDR_WR_BUFFER		0x1D //д��������
#define REG_ADDR_WR_BUFFER_TIME	0X1F //д����ʱ��
#define REG_ADDR_WR_LEVEL			0x20 //�趨��ǰlevel����
#define	REG_ADDR_WD_STRIDE		0x21 //дSTRIDE ADֵ
#define	REG_ADDR_RD_COUNTER		0x22 //��ȡ��ǰʵ��COUNTER��

#define	REG_ADDR_RD_ERR			0x23 //��Բ��������Ϣ
#define	REG_ADDR_COMMAD			0x24 //����������
#define	REG_ADDR_STATUS			0x25 //������״̬
#define REG_ADDR_PWM				0x29 //pwm ��Ƶ��

#define REG_ADDR_FAN				0x32	//���ȿ���

#define	REG_ADDR_LEVEL_1			0x50 //��д�öζ���
#define	REG_ADDR_LEVEL_2			0x51
#define	REG_ADDR_LEVEL_3			0x52
#define	REG_ADDR_LEVEL_4			0x53
#define	REG_ADDR_LEVEL_5			0x54
#define	REG_ADDR_LEVEL_6			0x55
#define	REG_ADDR_LEVEL_7			0x56
#define	REG_ADDR_LEVEL_8			0x57
#define	REG_ADDR_LEVEL_9			0x58
#define	REG_ADDR_LEVEL_10			0x59
#define	REG_ADDR_LEVEL_11			0x5A
#define	REG_ADDR_LEVEL_12			0x5B
#define	REG_ADDR_LEVEL_13			0x5C
#define	REG_ADDR_LEVEL_14			0x5D
#define	REG_ADDR_LEVEL_15			0x5E
#define	REG_ADDR_LEVEL_16			0x5F
#define	REG_ADDR_LEVEL_17			0x60
#define	REG_ADDR_LEVEL_18			0x61
#define	REG_ADDR_LEVEL_19			0x62
#define	REG_ADDR_LEVEL_20			0x63


//add  for controllor 20150430
//#define REG_ADDR_PROGRAMME_IDENTIFIER   140
#define REG_ADDR_PROGRAMME_VERSION      141
#define REG_ADDR_PROGRAMME_YEAR         142
#define REG_ADDR_PROGRAMME_MONTH_DAY    143

//add  end





#define REG_ADDR_MOTOR_STATUS    10  // ������״̬
#define REG_ADDR_RELAY_STATUS    11  // ---
#define REG_ADDR_EXCEPTION_MES	 13  // �쳣��Ϣ
#define REG_ADDR_STATUS_CHANGE   18  //  �������״̬�ı��־

#define REG_ADDR_CURRENT_SET     20  // �趨����
#define REG_ADDR_CURRENT_NOW     21	 // ʵ�ʵ���

#define REG_ADDR_RUN_MODE        26  // ����ģʽ��������
#define REG_ADDR_RUN_CMD         27  // ��������
#define REG_ADDR_CURRENT_ADC     28
#define REG_ADDR_MAIN_VOLT_ADC   29
#define REG_ADDR_MAIN_VOLTAGE    30
#define REG_ADDR_H_BRIDGE_PWM    31
#define REG_ADDR_NO_CURRENT_ADC  32

#define REG_ADDR_POWER_ON_1MS_TICK  40 // �ϵ��ʱ����λ1ms
#define REG_ADDR_POWER_ON_10S_TICK  41 //  �ϵ��ʱ����λ10s

#define REG_ADDR_MAIN_WHILE_TIME      45
#define REG_ADDR_MAIN_WHILE_MAX_TIME  46


#define REG_ADDR_ADC_REF_VOLTAGE   100
#define REG_ADDR_ROTATE_SPD_P      101
#define REG_ADDR_ROTATE_SPD_I      102
#define REG_ADDR_CURRENT_P         103
#define REG_ADDR_CURRENT_I         104

// ����ģʽ
#define RUN_MODE_FREE_SPEED    0
#define RUN_MODE_CONST_SPEED   1
#define RUN_MODE_RESISTANCE    2
#define RUN_MODE_POWER         3
#define RUN_MODE_POSITION_LOCK 4
#define RUN_MODE_OC_DEBUG	   0x0A
#define RUN_MODE_RPM_DEBUG	   0x0B
#define RUN_MODE_PLACE_DEBUG   0x0C

// ��������
#define RUN_CMD_STOP       0
#define RUN_CMD_RUN        1
#define RUN_CMD_EMER_STOP  2
#define RUN_CMD_LOCK       3
#define RUN_CMD_SLEEP      4

//�쳣��Ϣ
#define ERROR_NO          	0x00 	// ���쳣
#define ERROR_OUTPUT_OC   	0x01    //���������
#define ERROR_MOTOR_OL    	0x02    //������
#define ERROR_BRAKE_OL    	0x03    //�ƶ��������
#define ERROR_LOW_VOLTAGE 	0x04    //����Դ��ѹ
#define ERROR_OVER_VOLTAGE 	0x05    //����Դ��ѹ
#define ERROR_BRAKE_LOSS  	0x06    //�ƶ���·ʧЧ
#define ERROR_SPD_SENSE   	0x07    //�ٶȼ���쳣
#define ERROR_NO_CURRENT  	0x08    //���δ���ӻ������쳣
#define ERROR_OUTPUT_OL   	0x09    //�������
#define ERROR_SYS_OL      	0x0A    //ϵͳ����
#define ERROR_MCU         	0x0B    //�����쳣
#define ERROR_SAFEKEY_OFF 	0x20    //��ȫ���ضϿ�
#define ERROR_COM_TIMEOUT 	0x21    //ͨ�ų�ʱ


// ���״̬
#define MOTOR_STATUS_IDLE	0  //��ﴦ�ڿ���״̬
#define MOTOR_STATUS_RUN    1  // ��ﴦ������״̬
#define MOTOR_STATUS_STOP   2  // ��ﴦ��ֹͣ״̬���޷�����
#define MOTOR_STATUS_LOCK   3  // �Ͽ��Ѿ�����ָֹͣ�ת�����״̬


// ���з���
#define RUN_DIR_FORWARD		0  // ����
#define RUN_DIR_REVERSE		1  // ����

// �������ֵ
#define RESISTANCE_MAX 2000

typedef struct tag_communicate_para {
	__u8	com_func; //������

	// д����ָ��
	__u8	crtl_com; 			//����ָ��, ƫ�Ƶ�ַ

	//__u16   direction_set;  		// �������з���
	//__u16   direction_now;			// ʵ�����з���
	//__u16   rotate_spd_set;			// ���������ٶ�
	//__u16   rotate_spd_now;			// ʵ�������ٶ�
	//__s16   rotate_spd_now;			// ʵ�������ٶ�
	__u16   level_set;			// ������������
	//short	level_now;			// ʵ����������
	__u16   inc_set;
	__u16   max_inc;
	__u16   minInc_ADC;
	__u16   maxInc_ADC;
	__u16   curInc_ADC;
	__u16	power_set;				// �������й���
	short	power_now;				// ʵ�����й���
	__u16	motor_status;			// ���״̬
	__u16	overload_status;       // ����״̬
	__u16	relay_status;			// 
	__u16   status_change;
	__u16  fan;     //���ȿ���
	__u16  adcValue;

	__u16	curRpm; //��ǰrpm
//add by Tong for controllor  20150430
	__u16   version;//�汾
	__u16   year;//��
	__u16   date;//����
//add end
	__u16   error_code;              // ������
	__u32	run_mode;  				 // ����ģʽ

	__krnl_event_t  *sem;     	// ͨѶ�ź�������, �����¼�
	__bool  flag;				// ͨѶ����λ, 0���ʾû�б�ռ�ã�1���ʾ��ռ��

	__u16 MaxCounter;   		//��Բ�����������
	__u16 counter[20];        //20��ÿ�ε�ֵ
	__u16 curCounter;			//��ǰcounterֵ
	__u16 HrFlag;
	__u16 HandHr;			//��������
	__u16 WirHr;			//��������
	__u16 INCCmd;          //����������
	__u16 bufferCount;   //buffer��������
	__u16 bufferTime;		//buffer����ʱ��
	__u16 LevelCmd;
	__u16 pwm;
	//__bool keyTonFlag;
	__u8 INCstatus;      //����״̬
	__bool E2;

	__u8 key_value; //����ֵ
}com_data_para_t;

void com_send_thread(com_data_para_t *p_cmd_data);
void com_rece_thread(com_data_para_t *p_cmd_data);

//void com_rece_thread(void *arg);
__s32 open_uart0();
__s32 open_uart2();

int  bluetooth_sendbuf(char *pucData);
int  bluetooth_writebuf(__u8 *pucData,__u8 len);
__u8  bluetooth_write_tx(__u32 *num,int len);


void SetRunMode(com_data_para_t *p_cmd_data, __u16 mode);
void SetRunCtr(com_data_para_t *p_cmd_data, __u8 ctr_cmd);
void SetRunRpm(com_data_para_t *p_cmd_data, __u16 rpm);
void GetRunRPM(com_data_para_t *p_cmd_data);

void GetVersion(com_data_para_t *p_cmd_data);
void GetYEAR(com_data_para_t *p_cmd_data);
void GetDay(com_data_para_t *p_cmd_data);

void getNormalData(com_data_para_t *p_cmd_data);
void SeclectMac(com_data_para_t *p_cmd_data, __u16 mode);
void SetCounter(com_data_para_t *p_cmd_data, __u16 *counter,__u8 maxCounter);
void GetRunLevel(com_data_para_t *p_cmd_data);
void SetRunLevel(com_data_para_t *p_cmd_data, __u16 level);
void GetINCstatus(com_data_para_t *p_cmd_data);
void calbrationINC(com_data_para_t *p_cmd_data);
void InitINC(com_data_para_t *p_cmd_data);


//__s32 init_buffer();
void open_buffer(com_data_para_t *p_cmd_data,__u16 count,__bool keyTonFlag);

void buffer_time(com_data_para_t *p_cmd_data,__u16 sec);



void SetRunINC(com_data_para_t *p_cmd_data, __u16 inc);
void SetRunMAXINC(com_data_para_t *p_cmd_data, __u16 inc);

void SetFanOnOff(com_data_para_t *p_cmd_data, __u16 falg);
void GetRunIncAdc(com_data_para_t *p_cmd_data);
void GetRunIncMINAdc(com_data_para_t *p_cmd_data);
__s32 read_BTData(__u8 *ResultBuf);
__s32 write_BTData(__u8 cmd,__u8 len,__u16 *SrcData) ;

void Send_BT_status(__u8 status) ;

void SetMaxADC(com_data_para_t *p_cmd_data, __u16 adcValue);
void SetMinADC(com_data_para_t *p_cmd_data, __u16 adcValue);
void SetCMDLevel(com_data_para_t *p_cmd_data,__u16 cmd);
void SetPWM(com_data_para_t *p_cmd_data,__u16 pwm);
void GetCounter(com_data_para_t *p_cmd_data);
void SetRunSTRDIE(com_data_para_t *p_cmd_data, __u16 inc);














#endif
