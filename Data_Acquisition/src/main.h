#ifndef SOCKET_CLIENT_H_
#define SOCKET_CLIENT_H_

#define REC_SOCKET_BUF_SIZE		100

/* Relation between of samples of voltage per current. */
#define N_SAMPLES_CURRENT		100
#define N_SAMPLES_VOLTAGE		1

#define PRINT_TIMEOUT			100000			/* microseconds */

/*	0 ...       12			*/
/*	AEM current: 448.26 uA	*/
/*	AEM voltage: 3.3051		*/

const char GetCurrent[13] = "aem current\r\n";
const char GetVoltage[13] = "aem voltage\r\n";

typedef enum
{
	IDLE			= 0	,
	CALC_AVERAGE		,
} MachineState_t;

typedef enum
{
	CURRENT_VALUE	,
	VOLTAGE_VALUE	,
} ValueType_t;

uint8	InitSocketClient(void);
void	Send_SocketData(char data);
bool	Receive_SocketData(SOCKET sHandler, uint8 *BufRec);
bool	Get_Value(ValueType_t type, uint32 *ptrCurrent, uint32 *ptrVoltage);
void	ParseCurrent(uint8 *RawData, uint32 *OutData);
void	ParseVoltage(uint8 *RawData, uint32 *OutData);
uint32	GetCurrTime(void);
void	Delay_us(uint32 value);

#endif
