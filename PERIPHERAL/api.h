#ifndef __API_H__
#define __API_H__
extern u8 NB_Just_Send_Flag;
extern u8 COMMUNICATION_MODULE_STATE;
u8 COMMUNICATION_MODULE_REDO_REGISTER(void);//通讯模块注册直至成功  返回值为 SUCCESS 和  ERROR
u8 COMMUNICATION_MODULE_REDO_SENDPACKET(void);//通讯模块发数据包直至成功 返回值为 SUCCESS 和  ERROR
u8 COMMUNICATION_MODULE_Start(void);
u8 COMMUNICATION_MODULE_CLOSE(void);



#endif // __API_H__
