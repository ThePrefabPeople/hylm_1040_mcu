#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
#include "main.h"
#include "string.h"

#define TIME_100MS 10

uint8 cmd_buffer[CMD_MAX_SIZE];
static int32 test_value = 0;
static uint8 update_en = 0;
void UpdateUI(void);

//�������
void MonitorPro(void)
{
	qsize  size = 0;
	size = size;

	if(Var.BSPwork.Stu == SYSINIT) return;
//	UartInit(115200);
	/*��մ��ڽ��ջ�����*/


	//while(1)
	{
//		size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��        
//		if(size>0)//���յ�ָ��
//		{
//			ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
//		}		

		/****************************************************************************************************************
		�ر�ע��
		MCU��ҪƵ���򴮿����������ݣ����򴮿������ڲ��������������Ӷ��������ݶ�ʧ(��������С����׼��8K��������4.7K)
		1) һ������£�����MCU�򴮿����������ݵ����ڴ���100ms���Ϳ��Ա������ݶ�ʧ�����⣻
		2) �����Ȼ�����ݶ�ʧ�����⣬���жϴ�������BUSY���ţ�Ϊ��ʱ���ܷ������ݸ���������
		******************************************************************************************************************/

		//TODO: ����û�����
		//�����и���ʱ��ÿ100����ˢ��һ��
		update_en =1;
		if(update_en && 0 == Var.Monitor.UpCnt)
		{
			update_en = 0;
			Var.Monitor.UpCnt =2;			
			UpdateUI();
		}
	}
}

/*! 
 *  \brief  ��Ϣ�������̣��˴�һ�㲻��Ҫ����
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, uint16 size )
{
	uint8 cmd_type = msg->cmd_type;//ָ������
	//uint8 ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8 control_type = msg->control_type;//�ؼ�����
	uint16 screen_id = PTR2U16(&msg->screen_id);//����ID
	uint16 control_id = PTR2U16(&msg->control_id);//�ؼ�ID
	uint32 value = PTR2U32(msg->param);//��ֵ

	switch(cmd_type)
	{
	case NOTIFY_SCREEN://�л�����
		NotifyScreen(screen_id);
		break;
	case NOTIFY_TOUCH_PRESS:
	case NOTIFY_TOUCH_RELEASE:
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK:
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD:
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK:
		NotifyReadFlash(1,cmd_buffer+2,size-6);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD:
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_CONTROL:
		{
			switch(control_type)
			{
			case kCtrlButton: //��ť�ؼ�
				NotifyButton(screen_id,control_id,msg->param[1]);
				break;
			case kCtrlText://�ı��ؼ�
				NotifyText(screen_id,control_id,msg->param);
				break;
			case kCtrlProgress: //�������ؼ�
				NotifyProgress(screen_id,control_id,value);
				break;
			case kCtrlSlider: //�������ؼ�
				NotifySlider(screen_id,control_id,value);
				break;
			case kCtrlMeter: //�Ǳ�ؼ�
				NotifyMeter(screen_id,control_id,value);
				break;
			case kCtrlMenu://�˵��ؼ�
				NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
				break;
			case kCtrlSelector://ѡ��ؼ�
				NotifySelector(screen_id,control_id,msg->param[0]);
				break;
			case kCtrlRTC://����ʱ�ؼ�
				NotifyTimer(screen_id,control_id);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16 screen_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8 press,uint16 x,uint16 y)
{
	//TODO: ����û�����
}

//�ı��ؼ���ʾ����ֵ
void SetTextValueInt32(uint16 screen_id, uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%ld",value);
	SetTextValue(screen_id,control_id,(uchar *)buffer);
}

void SetTextValueStr(uint16 screen_id, uint16 control_id,uint8_t *str)
{
	//char buffer[12] = {0};
	//sprintf(buffer,"%ld",value);
	SetTextValue(screen_id,control_id,(uchar *)str);
}
//�ַ���ת����
int32 StringToInt32(uint8 *str)
{
	int32 v = 0;
	sscanf((char *)str,"%ld",&v);
	return v;
}

//���½���ؼ���ʾ
void UpdateUI()
{
	static u16 ct;
	static u8 s=0;
	u8 str[20]={0};
	u8 str2[20]={0};
	u16 sts=0;//�趨��ʱ��
	u16 sv=0;//�趨��ѹ
	u16 sc=0;//�趨����
	u16 cs=0;//ʵ��ʱ��
	u16 cv=0;//ʵ�ʵ�ѹ
	u16 cc=0;//ʵ�ʵ���
	u8	door=0;//�ſ�
	u8	vbit=0;//��ѹ
	ct = ct;
	
	//SetScreen(0);
	if(Var.Power.DoorFlag) door =0; else door=1;
	if(s==0)
	{
		//GraphChannelAdd(2,3,1,6000);
		//���ű�����Ҫ����ʱ�����
		//GraphSetViewport(2,3,0,100,0,100);
		s=1;
	}
	
	if(1 == Var.Out.Y06)
	{
		vbit =1;

		cc = Var.Power.RelCur; 
		cv = Var.Power.RelVo1; 
		cs = Var.BSPwork.Cursec;//ʵ��ʱ��
		
		sc =100; 
		sv =Var.Power.SetVoltage;  
		sts = Var.BSPwork.TotalSec;//�趨ʱ��
		
		if(0 == Var.Monitor.UpCnt2)
		{
			Var.Monitor.UpCnt2=2;

			str[0]=Var.Power.RelCur/10;
			str[1] =0;
			str2[0]=Var.Power.RelVo1/300;
			str2[1] =0;
			
			//�������
			GraphChannelDataAdd(2,3,1,str,1);
			GraphChannelDataAdd(2,3,0,str2,1);
		}
	}
	else 
	{
		vbit =0;
		cc =0; cv =0; cs =0;
		sc =0; sv =0; sts =0;
	}
	
	AnimationPlayFrame(0,15,door);//����ʱ
	AnimationPlayFrame(0,16,vbit);//����ָ��֡
	memset(str,0,20);
	sprintf((char *)str,"%d",cv);
	SetTextValue(0,1,(uchar *)str);
	//SetTextValueInt32(0,1,cv);//ʵ�ʵ�ѹ
	memset(str,0,20);
	sprintf((char *)str,"%.1f",cc/10.0);
	SetTextValue(0,2,(uchar *)str);
	//SetTextValueInt32(0,2,cc);//ʵ�ʵ���
	
	memset(str,0,20);
	sprintf((char *)str,"%d",cs);
	SetTextValue(0,3,(uchar *)str);
	//SetTextValueInt32(0,3,cs);//ʵ��ʱ��

	memset(str,0,20);
	sprintf((char *)str,"%d",sv);
	SetTextValue(0,4,(uchar *)str);
	//SetTextValueInt32(0,4,sv);//�趨��ѹ

	memset(str,0,20);
	sprintf((char *)str,"%d",sc);
	SetTextValue(0,5,(uchar *)str);
	//SetTextValueInt32(0,5,sc);//�趨����

	memset(str,0,20);
	sprintf((char *)str,"%d",sts);
	SetTextValue(0,6,(uchar *)str);
	//SetTextValueInt32(0,6,sts);//�趨ʱ��
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16 screen_id, uint16 control_id, uint8  state)
{
	//TODO: ����û�����
	if(screen_id==0&&control_id==5)//������ť
	{
		if(test_value<100)
			++test_value;
	}
	else if(screen_id==0&&control_id==6)//�ݼ���ť
	{
		if(test_value>0)
			--test_value;
	}

	
	//UpdateUI();
	update_en = 1;
}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str)
{
	//TODO: ����û�����
	test_value = StringToInt32(str);

	if(test_value>100)
		test_value = 100;
	else if(test_value<0)
		test_value = 0;

	//UpdateUI();
	update_en = 1;
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
	test_value = value;

	//UpdateUI();
	update_en = 1;
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8  item, uint8  state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(uint16 screen_id, uint16 control_id, uint8  item)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16 screen_id, uint16 control_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8 status,uint8 *_data,uint16 length)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8 status)
{
	//TODO: ����û�����
}
