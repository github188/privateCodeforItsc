#ifndef __CALCULATE_H_
#define __CALCULATE_H_

#include "its.h"

typedef struct _PassTimeInfo	//����ʱ����Ϣ
{
	UInt16 greenTime;			//�̵�ʱ��
	UInt8 greenBlinkTime;		//����ʱ��
	UInt8 yellowTime;			//�Ƶ�ʱ��
	UInt8 redYellowTime;		//���ʱ��
	UInt8 allRedTime;			//ȫ��ʱ��
} PassTimeInfo;

typedef struct rfidTagBits
{//��������tag�����128��
	UINT64 nLow;
	UINT64 nHigh;
}RFIDTagBits;

typedef struct _PhaseTimeInfo
{
	UInt16 splitTime;			//���ű�ʱ��
	PassTimeInfo passTimeInfo;	//����ʱ����Ϣ
    UInt8 pedestrianPassTime;	//���˷���ʱ��
    UInt8 pedestrianClearTime;	//�������ʱ��
	UInt8 pedAutoRequestFlag;	//�����Զ������־��0:û����������,1:����������
	UInt64 vehicleDetectorBits;	//��λ��Ӧ�ĳ�������
	UINT16 pedestrianDetectorBits; //��λ��Ӧ�����˼������
	UINT32 busDetectorBits[4];     //��������Ӧ�ļ������
	UINT32 busPrioPassTime;     //�����������̵�ʣ��ʱ�䲻�������õ����ȷ���ʱ��
	UINT32 scarDetectorBits[4];    //���ڳ�����Ӧ�������bitλ
	UINT32 scarPassTime;        //���ڳ�������ʱ��
	UInt8 unitExtendGreen;		//��λ�ӳ���
	Int8 maxExtendGreen;		//�������ӳ����̵�ʱ��
	Int8 maxExtendGreen2;      //����Ӧ�����������ӳ����̵�ʱ��
	INT8 minShortGreen;        //��С�������̵��̵�ʱ��
	UInt32 motorChannelBits;	//��λ��Ӧ�Ļ�����ͨ����bit0-bit31�ֱ����ͨ��1-32
	UInt32 pedChannelBits;		//��λ��Ӧ������ͨ����bit0-bit31�ֱ����ͨ��1-32
	UInt8 laneLevel;			//��λ����Ӧͨ������Ϊ����1���򸨵�0
} PhaseTimeInfo;

typedef struct
{
	UInt16 phaseBits;			//������λ�����ĸ��λ��bit0-bit15�ֱ������λ1-16
	UInt32 motorChannelBits;	//������λ��Ӧ�Ļ�����ͨ����bit0-bit31�ֱ����ͨ��1-32
	UInt32 pedChannelBits;		//������λ��Ӧ������ͨ����bit0-bit31�ֱ����ͨ��1-32
} FollowPhaseInfo;

typedef struct _StageInfo
{
	PassTimeInfo passTimeInfo;					//����ʱ����Ϣ
	UInt16 runTime;								//�׶�����ʱ��
	Boolean isBarrierStart;						//�׶��Ƿ���������ʼ
	Boolean isBarrierEnd;						//�׶��Ƿ������Ͻ���
	UInt8 includeNum;							//�׶ΰ�����λ�ĸ���
	UInt8 includePhases[MAX_PHASE_NUM];	//�׶ΰ�����λ
} StageInfo;

typedef struct _CalInfo
{
	UInt8 timeIntervalId;					//ʱ�α���
	UInt8 actionId;							//������
	UInt8 schemeId;							//������
	UInt8 splitId;							//���űȺ�
	UInt8 phaseTurnId;						//�����
	UInt8 phaseTableId;						//��λ����
	UInt8 channelTableId;					//ͨ������
	UInt16 cycleTime;						//����ʱ��
	UInt16 inductiveCoordinateCycleTime;	//��ӦЭ����������ʱ��
	
	UInt8 transitionCycle;					//��������
	UInt8 coordinatePhaseId;				//Э����λ
	UInt16 isCoordinatePhase;				//�Ƿ���Э����λ,ÿbit����һ����λ,0:����,1:��
	UInt16 phaseOffset;						//��λ��
	int timeGapSec;							//��ǰʱ����ʱ����ʼʱ����λΪs
	
	UInt8 collectCycle;						//�ɼ�����
	UInt8 checkTime;						//���ʱ��
	UINT16 singlespotCheckTime;             //�����Ż��ļ��ʱ��
	UINT64 vehDetectorState;                //������״̬
	
	UINT8 pedestrianRequest;               //2: ���˹��ֶ�������; 1 : ���˹���һ������
	UInt16 isIgnorePhase;					//�Ƿ��Ǻ�����λ,ÿbit����һ����λ,0:����,1:��
	
	PhaseTimeInfo phaseTimes[MAX_PHASE_NUM];	//��λʱ����ص���Ϣ
	FollowPhaseInfo followPhaseInfos[MAX_FOLLOWPHASE_NUM];	//������λ�������Ϣ
	
	UInt8 maxStageNum;						//���׶κ�
	StageInfo stageInfos[MAX_STAGE_NUM];	//�׶������Ϣ
	
	UInt8 includeNums[MAX_PHASE_NUM];						//ÿ����λ�������Ľ׶κŸ���
	UInt8 phaseIncludeStage[MAX_PHASE_NUM][MAX_STAGE_NUM];	//��λ�������Ľ׶κ�
} CalInfo;


#endif