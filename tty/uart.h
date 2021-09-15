#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

	/*********************
 *      INCLUDES
 *********************/
	typedef struct 
	{
		unsigned char dev_addr;
		unsigned char dev_func;
		unsigned char reg_addr[2];
		unsigned char reg_data[2];
		unsigned char crc[2];
	}W_REG_A_FORMAT;
	
	typedef struct 
	{
		unsigned char dev_addr;
		unsigned char dev_func;
		unsigned char reg_addr_start[2];
		unsigned char reg_addr_num[2];
		unsigned char reg_data_len; //reg_addr_num * 2
		unsigned char *reg_pdata;	//data -> 2 * byte
		unsigned char crc[2];
	}W_REG_S_FORMAT;
	
	typedef struct 
	{
		unsigned char dev_addr;
		unsigned char dev_func;
		unsigned char reg_addr_start[2];
		unsigned char reg_addr_num[2];
		unsigned char crc[2];
	}R_REG_S_FORMAT;
	
	typedef struct 
	{
		unsigned char dev_addr;
		unsigned char dev_func;
		unsigned char reg_addr[2];
		unsigned char reg_data[2];
		unsigned char crc[2];
	}DEV_ACK_A_FORMAT;

	typedef struct 
	{
		unsigned char dev_addr;
		unsigned char dev_func;
		unsigned char reg_data_len; //reg_addr_num * 2
		unsigned char *reg_pdata;	//data -> 2 * byte
		unsigned char crc[2];
	}DEV_ACK_S_FORMAT;
	//通讯过程中，所有数据都是高字节在前，低字节在后，CRC 校验则是低字节在前高字节在后
	//接收数据的数据长度

	void uart_send_data(const char *sdata, unsigned char len);
	void uart_test2();
	uint16_t CRC16(uint8_t *DataP, uint8_t Len);

#ifdef __cplusplus
} /* extern "C" */
#endif