#ifndef __API_H__
#define __API_H__
extern u8 NB_Just_Send_Flag;
extern u8 COMMUNICATION_MODULE_STATE;
u8 COMMUNICATION_MODULE_REDO_REGISTER(void);//ͨѶģ��ע��ֱ���ɹ�  ����ֵΪ SUCCESS ��  ERROR
u8 COMMUNICATION_MODULE_REDO_SENDPACKET(void);//ͨѶģ�鷢���ݰ�ֱ���ɹ� ����ֵΪ SUCCESS ��  ERROR
u8 COMMUNICATION_MODULE_Start(void);
u8 COMMUNICATION_MODULE_CLOSE(void);



#endif // __API_H__
