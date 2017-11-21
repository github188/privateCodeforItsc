/******************************************************************************

                  ��Ȩ���� (C), 2001-2015, ���ݺ����������ּ����ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : hikmsg.h
  �� �� ��   : ����
  ��    ��   : Jicky
  ��������   : 2014��11��29��
  ����޸�   :
  ��������   : hikmsg.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��11��29��
    ��    ��   : Jicky
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __HIKMSG_H__
#define __HIKMSG_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <netinet/in.h>

#include "hik.h"
#include "its.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

//#define MSGZERO 0	//�������͵���Ϣû������
#define MSGSIZE	128	//����������Ϣ���ݵĳ���

#define AHEAD_OF_TIME	2	//��һ��������ǰ6s��ʼ����

#define ONE_SECOND_NSECS		1000000000					//1s�����ns��ֵ
#define DELAY_TIME				250							//��ʱʱ�䣬��λ��ms����ÿDELAY_TIMME ms��ʱһ��
//ÿ����ʱ֮ǰ����usleep��ʱ��ʱ�䣬Ԥ��50ms����ȷ��ʱ
#define DELAY_TIME_USECS_BEFORE	((DELAY_TIME - 50) * 1000)	
#define DELAY_TIME_NSECS		(DELAY_TIME * 1000000)			//ÿ����ʱʱ�任���������
#define LOOP_TIMES_PER_SECOND	(ONE_SECOND_NSECS / DELAY_TIME_NSECS)	//ÿ������ѯ���͵����ֵ����
/*----------------------------------------------*
 * �����ض����Լ��ṹ�嶨��                     *
 *----------------------------------------------*/
typedef enum 
{
	MSG_NEW_CONFIG = 1,			//���µ������·���ͨ��ģ�鷢�͸����ݹ���ģ��
	
	MSG_CONTROL_TYPE,		//ͨ��ģ������ݲɼ�ģ�鷢�͸����Կ���ģ��Ŀ���������Ϣ
	
	MSG_FAULT_LOG,	//������־��Ϣ
	
	MSG_CONTROL_MODE,		//���Կ�����������λ����ģ��Ŀ��Ʒ�ʽ
	MSG_BEGIN_READ_DATA,	//����ģ�鷢�͸���λ����ģ�鿪ʼ��ȡ����
	
	MSG_START_CALCULATE_NEXT_CYCLE,	//��ʼ������һ����
	
	MSG_START_TIMER,		//��λ����ģ�鷢�͸���ʱ��ģ��
	MSG_CHANNEL_STATUS,		//ͨ������ģ�鷢�͸���λ����ģ���ͨ��״̬

	MSG_CHANNEL_CHECK,		//ͨ��ģ�鷢�͸���λ����ģ�����Ϣ
	
	MSG_RED_SIGNAL_CHECK,	//��λ����ģ�鷢�͸�����źż����ģ�飬ÿ��һ��

} MsgType;		//��Ϣ���ͱ��

typedef enum
{
	SYSTEM_MODE = 0,		//ϵͳ����ģʽ
	TURNOFF_LIGHTS_MODE = 1,	//�صƿ���ģʽ
	YELLOWBLINK_MODE = 2,		//��������ģʽ
	ALLRED_MODE = 3,			//ȫ�����ģʽ
	INDUCTIVE_COORDINATE_MODE = 4,	//��ӦЭ��ģʽ
	INDUCTIVE_MODE = 6,			//��Ӧ����ģʽ
	SINGLE_ADAPT_MODE = 7,          //����Ӧ����
	//����4���ǹ��������Ŀ��Ʒ�ʽ
	SINGLE_SPOT_OPTIMIZE = 8,		//�����Ż�
	PEDESTRIAN_REQ_MODE = 9,        //���˹��ְ�������
	BUS_PRIORITY_MODE = 10,      //��������
	MASTER_SLAVE_LINE_CONTROL = 11,	//�����߿�
	SYSTEM_OPTIMIZE = 12,			//ϵͳ�Ż�
	INTERPOSE_LINE_CONTROL = 13,	//��Ԥ�߿�
	
	MANUAL_MODE,			//�ֶ�����ģʽ
	STEP_MODE,				//��������ģʽ
} ControlMode;	//���Ʒ�ʽ


struct msgbuf
{
	long mtype;		//��Ϣ���ͣ������������ MsgType ������
	
	union
	{
		char mtext[MSGSIZE];	//����ģ�鷢�͸�������־ģ��Ĺ�����Ϣ
		char wFlag;		//������Ϣ�Ƿ�дflash�ı�־��0:��д�룬1:д��

		struct 
		{
			ControlType	controlType;	//���Կ���ģ����յĿ�������
			ControlMode mode;		//���Կ���ģ�鷢����λ����ģ��Ŀ��Ʒ�ʽ
			int stageNum;		//�����׶κţ���������ʱΪ0��
			UInt8 mSchemeId;	//��ʾ�ֶ�������
		} controlMsg;

		//��λ����ģ�鷢�͸�����ģ��
		struct
		{
			UInt8 schemeId;		//��ʼ�������һ������Ӧ��ʹ�õķ�����0:��ʾ���ձ���ϵͳ����
			time_t calTime;		//��ʼ������һ���ڵ�ʱ��
		} calMsg;

		UInt8 allChannels[MAX_CHANNEL_NUM];		//��λ����ģ�鷢�͸���λ����ģ���ͨ��״̬
		//ͨ��������ز���
		struct
		{
			UInt8 channelId;
			LightStatus status;
		} channelCheckParam;

		struct faultLogMsg
		{
#define READ_FAULT_LOG	1
#define WRITE_FAULT_LOG	2
#define CLEAR_FAULT_LOG	3
			UInt8 rwflag;
			FaultLogType type;
			time_t time;
			int value;
			int startLine;
			int lineNum;
			UploadFaultLogFunc func;	//���ع�����־�ĺ���ָ��
			char netArg[0];	//����������ع�����־ʱ��������ز�������С���ܳ���(MSGSIZE - sizeof(struct faultLogMsg))
		} faultLog;
	} mdata;
//������struct msgbuf�ڲ����ݳ�Ա��һЩ�궨��
#define msgText					mdata.mtext
#define msgwFlag				mdata.wFlag
#define	msgControlType			mdata.controlMsg.controlType
#define msgMode					mdata.controlMsg.mode
#define msgStageNum				mdata.controlMsg.stageNum
#define msgmSchemeId			mdata.controlMsg.mSchemeId
#define msgSchemeId	    		mdata.calMsg.schemeId
#define msgCalTime				mdata.calMsg.calTime
#define msgAllChannels			mdata.allChannels
#define msgChannelId			mdata.channelCheckParam.channelId
#define msgChannelStatus		mdata.channelCheckParam.status
#define msgFLrwflag				mdata.faultLog.rwflag
#define msgFLtype				mdata.faultLog.type
#define msgFLtime				mdata.faultLog.time
#define	msgFLvalue				mdata.faultLog.value
#define msgFLstartLine			mdata.faultLog.startLine
#define msgFLlineNum			mdata.faultLog.lineNum
#define msgFLfunc				mdata.faultLog.func
#define msgFLnetArg				mdata.faultLog.netArg
};

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __HIKMSG_H__ */
