/*
********************************************************************************
*                                    eMOD
*                   the Easy Portable/Player Develop Kits
*                               mod_cedar sub-system
*                          (module name, e.g.mpeg4 decoder plug-in) module
*
*          (c) Copyright 2010-2012, Allwinner Microelectronic Co., Ltd.
*                              All Rights Reserved
*
* File   : mod_karaoke.h
* Version: V1.0
* By     : Eric_wang
* Date   : 2011-8-11
* Description:
    ����cedar�ĸ���������,�ʴ�������cedar�����ݽṹ����,�������Ͷ���,���.
********************************************************************************
*/
#ifndef _MOD_KARAOKE_H_
#define _MOD_KARAOKE_H_

#include "mod_cedar.h"
#include "mod_ginkgo.h"

typedef enum __KARAOKE_CMD_SET            /* ý�岥���м�������б�(IOCtrol)                              */
{
    //karaoke�м��,����cedar core�ĸ����,��cedarƽ��
    KARAOKE_CMD_SET_MEDIAFILE = 0x2000,         /* ���ð������ļ�·�� */
    KARAOKE_CMD_SET_RECORD_MEDIAFILE,           /* ����Ҫ¼�Ƶ��ļ�������Ϣ, ���еĲ����ʾͻ���Ϊmicrophone�Ĳ�����, pbuffer = __ginkgo_media_file_inf* */
    KARAOKE_CMD_SET_CHAN_OUTPUT_MODE,           // ���ò���ʱ���������ģʽ(���ҡ�������), aux = AudioChanOutputMode, return = EPDK_OK/EPDK_FAIL, (__audio_dev_chan_mode_t)
    KARAOKE_CMD_GET_MESSAGE_CHN,                // �õ�karaoke����Ϣ����ͨ��

    KARAOKE_CMD_ENABLE_MUSIC = 0x2100,          //(1)music��microphone����ѡһ��, aux = 1:enable; aux=0:disable,֪ͨmixer����,������play֮ǰ��stop״̬�����ú�,��;������Ч.
    KARAOKE_CMD_ENABLE_MICROPHONE,              //(2)aux = 1:֪ͨmixer, ����audio_rec; aux=0:֪ͨmixer,ֹͣaudio_rec,�����ܻ᷵��ʧ��.���ֻ����˷�Ļ�.
    KARAOKE_CMD_ENABLE_RECORD,                  //(4)aux = 1: enable��ζ�ż���¼�Ʋ��,����¼��; �����ڲ�����;��enable,���ز������¼��. aux = 0:disable���ǹرղ��,�ļ�д����.
    //KARAOKE_CMD_ENABLE_AUDIOPLYBK,              //(3)audio_plybk��record����ѡһ��, aux = 1:enable, aux=0:disable

    KARAOKE_CMD_PLAY = 0x2200,
    KARAOKE_CMD_STOP,   //����ֹͣ,���ź�¼��
    KARAOKE_CMD_PAUSE,  //Ԥ��,����ʵ��. �������ָmusic��¼�Ƶ���ͣ���������ʱmicrophone˵�������ǿ���ͨ�����䴫��.
    KARAOKE_CMD_GET_STATUS, 

    KARAOKE_CMD_GET_TOTAL_TIME = 0x2300,

    KARAOKE_CMD_ENABLE_ECHO = 0x2400,            //echo enable,��������ʹ�ܣ�aux = 1: enable; aux =0: disable; 
    KARAOKE_CMD_ENABLE_ECHO_EFFECT,              //echo effect enable������Ч��ʹ�ܣ�aux = 1: enable; aux =0: disable;
    KARAOKE_CMD_SET_ECHO_EFFECT_OUTPUT_MODE,     //���û���Ч��ģʽ��
    
    KARAOKE_CMD_SET_VID_LAYERHDL = 0x2504,         /* ������Ƶͼ���� pbuffer = video_layer_hdl                   */
    KARAOKE_CMD_SET_VID_WINDOW,                 /* ������Ƶ�������� pbuffer = (__rect_t *)                      */
    
    KARAOKE_CMD_

} __karaoke_cmd_set_t;


typedef enum tag_KaraokeStatus          /* the status os cedar                  */
{
    KARAOKE_STAT_PLAY=0,                  /* play                                 */
    KARAOKE_STAT_PAUSE,                   //���״ָ̬music��¼�Ƶ���ͣ���������ʱmicrophone˵�������ǿ���ͨ�����䴫��.
    KARAOKE_STAT_STOP,                    /* stop                                 */
    KARAOKE_STAT_
} KaraokeStatus;


typedef enum tag_AudioChanOutputMode
{
    AUDIO_CHANNEL_MODE_STEREO = 0,           /* ������ģʽ                       */
    AUDIO_CHANNEL_MODE_LEFT,                 /* ����ģʽ                         */
    AUDIO_CHANNEL_MODE_RIGHT,                /* ����ģʽ                         */

    AUDIO_CHANNEL_MODE_
} AudioChanOutputMode;

typedef enum tag_KaraokeEchoOputMode
{
    AUDIO_ECHO_MODE_0 = 0,              /*do nothing                       */
    AUDIO_ECHO_MODE_1,                 /*ģʽ1                         */
    AUDIO_ECHO_MODE_2,                   /* ģʽ2                     */
    AUDIO_ECHO_MODE_USER =  0xff,               /* �û�ģʽ           */

    AUDIO_ECHO_MODE_
} KaraokeEchoOutputMode;


typedef enum tag_KaraokeFeedbackMsg           /* ����ý�岥���м��������Ӧ�ó������Ϣ����               */
{
//    CEDAR_ERROR_NOP                 =   0,  /* ����Ϣ                                                   */
//
    //cedar feed back error type
    KARAOKE_ERROR_OPEN_FILE_FAIL      = -20,  /* �򿪵�ǰý���ļ�ʧ��                                     */
    KARAOKE_ERROR_FILE_FMT_UNKNOWN    = -19,           /* �޷�ʶ��ǰý���ļ���ʽ                                 */
    KARAOKE_ERROR_FILE_NOT_SUPPORT    = -18,           /* ��ǰý���ļ���֧��                                       */
//    CEDAR_ERROR_FILE_DATA_ERROR,            /* ý���ļ����ݴ��޷�����                                 */
    KARAOKE_ERROR_NO_FILE_INPUT       = -16,              /* δ����Ҫ���ŵ�ý���ļ�                                   */
//    CEDAR_ERROR_READ_FILE_FAIL,             /* ��ȡ�ļ�����ʧ��                                         */
//
    KARAOKE_ERROR_START_PLG_FAIL      = -40,  /* �������ʧ��                                             */
//    CEDAR_ERROR_ADEC_ERROR,                 /* ��Ƶ�������������                                     */
//    CEDAR_ERROR_VDEC_ERROR,                 /* ��Ƶ�������������                                     */
//    CEDAR_ERROR_TDEC_ERROR,                 /* ��Ļ�������������                                     */
//
//    CEDAR_ERROR_PIC_SIZE_TOO_LARGE  = -60,  /* ͼƬ�ߴ�̫���޷�֧�ֽ���                               */
//    CEDAR_ERROR_REQ_MEMORY_FAIL,            /* �����ڴ�ʧ��                                             */
//
//    CEDAR_ERROR_UNKOWN               = -80,  /* δ֪����                                                 */
//

    KARAOKE_GINKGO_ERROR_CREATE_FILE_FAIL = -60,    //GINKGO_ERROR_CREATE_FILE_FAIL
    KARAOKE_GINKGO_ERROR_START_PLG_FAIL,            //GINKGO_ERROR_START_PLG_FAIL

    //cedar feed back status type
    KARAOKE_FEDBAK_NO_ERROR           =  1,   /* ����Ϣ                                                   */
//    CEDAR_FEDBAK_PLAY_END,                  /* ����ý���ļ�����                                         */

    KARAOKE_ERROR_

} KaraokeFeedbackMsg;

typedef struct tag_KaraokeEchoAudioInf
{
    __s32       uInGain;                
    __s32       uOutGain;       
    __s32       uNumDelay;             
    __s32       uDelay[16];         
    __s32       uDecay[16];          

} KaraokeEchoAudioInf;

typedef struct tag_KaraokeEchoEffectInf
{
    //__s32       uEchoEnable;
    //__s32       uEchoEffectEnable; 
    
    KaraokeEchoOutputMode   uEchoOutputMode;       
    KaraokeEchoAudioInf     uEchoAudioInf;
} KaraokeEchoEffectInf;


//typedef struct tag_CedarAudioInputInf   //��Ƶ����Ĳ���,����adc, ���ڲ���Ҫ, ����Ҫʱ����
//{
//    __u32       uChn;                   /* ¼��������ģʽ��������÷Ƿ��������defaultֵ,����Ϊ__ginkgo_audio_chn_t    */
//    __u32       uSampWidth;             /* ¼���������λ��������÷Ƿ��������defaultֵ  */
//    __u32       uAudioSampRate;         /* ��Ƶ�����Ĳ����ʣ�������÷Ƿ��������defaultֵ  */
//    __u32       uAudioSource;           /* ��ƵԴ��������÷Ƿ��������defaultֵ, GINKGO_AUDIO_SRC_MIC */
//    __u32       uVolume;                /* ¼��������ֵ��������÷Ƿ��������defaultֵ      */
//    __u32       uAbsFmt;                /* ��Ƶ�������ͣ�if(uAbsFmt == GINKGO_ABS_TYPE_NONE), ���������������Ƶ����                             */
//    __u32       uAudioBps;              /* ��Ƶ���������ʣ�������÷Ƿ��������defaultֵ    */
//    __u32       reserved[12];
//} CedarAudioInputInf;
//__ginkgo_audio_inf_t

#endif  /* _MOD_KARAOKE_H_ */


