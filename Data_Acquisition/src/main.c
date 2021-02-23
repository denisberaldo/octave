#include "Types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <io.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include <sys/time.h>
#include <time.h>

#include "main.h"

/*
 * Acquisition data program
 *
 * This program performs acquisition data from Silicon Labs Wireless Kit.
 * Open a socket to connect the kit via Telnet port.
 * Commands is listed on UG181 (Board_User_Guide.pdf).
 *
 * All sampled data is saved in specific files.
 * Power and Energy is calculated over each acquired sample.
 *
 */

static SOCKET SocketHandler;

int main(int argc, char *argv[])
{
	FILE *fcurr, *fenergy, *fvoltage, *ftime;

	static MachineState_t	Machine = IDLE;
	static uint32			SamplesCount = N_SAMPLES_CURRENT;

	static uint32			Current_nA = 0;
	static uint32			Voltage_mA = 0;
	static uint32			Power_uW = 0;
	static uint32			TotalEnergy_nJ = 0;

	static uint32			StartTime;
	static uint32			PrintTimeout;
	static uint32			LastTime = 0;
	uint32					LastCurrTime = 0;
	uint32					CurrTime;
	uint32					DiffTime;

	ftime		= fopen("C:\\Denis\\Workspace_eclipse\\Data_Acquisition\\data_time.txt","w+");
	fcurr		= fopen("C:\\Denis\\Workspace_eclipse\\Data_Acquisition\\data_current.txt","w+");
	fenergy		= fopen("C:\\Denis\\Workspace_eclipse\\Data_Acquisition\\data_energy.txt","w+");
	fvoltage	= fopen("C:\\Denis\\Workspace_eclipse\\Data_Acquisition\\data_voltage.txt","w+");

	if ((ftime == NULL)		||
		(fcurr == NULL)		||
		(fenergy == NULL)	||
		(fvoltage == NULL))
	{
	   printf("Error!");
	   exit(1);
	}

	printf("\nReady\n");

    if (InitSocketClient() == TRUE)
    {
    	StartTime = GetCurrTime();
    	PrintTimeout = StartTime + PRINT_TIMEOUT;

		while (1)
		{
			switch (Machine)
			{
				case IDLE:
				{
					if (SamplesCount >= N_SAMPLES_CURRENT)
					{
						SamplesCount = 0;
						//Sleep(1);
						//Delay_us(1000);
						if (Get_Value(VOLTAGE_VALUE, &Current_nA, &Voltage_mA) == true)
						{
							// ???
						}
					}
					else
					{
						SamplesCount++;
						//Sleep(1);
						//Delay_us(1000);
						if (Get_Value(CURRENT_VALUE, &Current_nA, &Voltage_mA) == true)
						{
							Machine = CALC_AVERAGE;
						}
					}
				}
				break;

				case CALC_AVERAGE:
				{
					CurrTime = GetCurrTime();

					/* Windows fix
					 * Sometimes Windows returns the same microseconds value.
					 * So we put a virtual increment (500us).
					 */
					if (CurrTime == LastCurrTime)
					{	LastCurrTime += 500;		}
					else
					{	LastCurrTime = CurrTime;	}

					/* Calculate window time - converts us to ms. */
					DiffTime = (CurrTime/1000) - LastTime;
					LastTime = (CurrTime/1000);

					/* Fix negative logarithmic. */
					if (Current_nA == 0u)
					{	Current_nA = 1;	}

					Power_uW = (uint32)(((UINT64)Current_nA * (UINT64)Voltage_mA)/(UINT64)1000);
					TotalEnergy_nJ += (uint32)(((UINT64)Power_uW * (UINT64)DiffTime)/(UINT64)1000);

					fprintf(fcurr, "%d\n", Current_nA);
					fprintf(fenergy, "%d\n", TotalEnergy_nJ);
					//fprintf(fvoltage, "%d\n", Voltage_mA);
					fprintf(ftime, "%d\n", (LastCurrTime - StartTime));

					if (CurrTime >= PrintTimeout)
					{
						PrintTimeout = CurrTime + PRINT_TIMEOUT;
						printf("%d\t%d\t%d\t%d\t%d\n", Current_nA, Voltage_mA, Power_uW, (CurrTime - StartTime), TotalEnergy_nJ);
						//fprintf(fcurr,"%d\n", Current_nA);
						//fprintf(fenergy, "%d\n", TotalEnergy_nJ);
						fflush(stdout);
					}

					Machine = IDLE;
				}
				break;
			}
		}
    }

    fclose(ftime);
    fclose(fcurr);
    fclose(fenergy);
    fclose(fvoltage);

	(void)closesocket(SocketHandler);
	(void)WSACleanup();

	return EXIT_SUCCESS;
}

bool Get_Value(ValueType_t type, uint32 *ptrCurrent, uint32 *ptrVoltage)
{
	bool	ret = false;
	uint8	RecBuffer[REC_SOCKET_BUF_SIZE];

	if (type == CURRENT_VALUE)
	{
		(void)send(SocketHandler, &GetCurrent[0], sizeof(GetCurrent), 0);
	}
	else if (type == VOLTAGE_VALUE)
	{
		(void)send(SocketHandler, &GetVoltage[0], sizeof(GetVoltage), 0);
	}

	(void)memset(&RecBuffer[0], 0x00, sizeof(RecBuffer));

	if (Receive_SocketData(SocketHandler, &RecBuffer[0]) == true)
	{
		if (strncmp("AEM current:", (const char *)&RecBuffer[0], 12u) == 0)
		{
			ParseCurrent(&RecBuffer[12], ptrCurrent);
			ret = true;
		}
		else if (strncmp("AEM voltage:", (const char *)&RecBuffer[0], 12u) == 0)
		{
			ParseVoltage(&RecBuffer[12], ptrVoltage);
			ret = true;
		}
	}
	return ret;
}

void ParseCurrent(uint8 *RawData, uint32 *OutData)
{
	uint8 i;
	uint32 tempData;
	uint8 tempNum[10] = {[0 ... 9] = 0};
	uint8 tempUnit[10] = {[0 ... 9] = 0};
	uint8 cNum = 0;
	uint8 cUnit = 0;

	/* 448.26 uA	*/
	/* 1.31 uA		*/
	/* 10.11 mA		*/

	for (i = 0; i < 11; i++)
	{
		if ((*(RawData + i) >= '0')	&&
			(*(RawData + i) <= '9'))
		{
			tempNum[cNum++] = *(RawData + i);
		}

		if ((*(RawData + i) >= 'A')	&&
			(*(RawData + i) <= 'z'))
		{
			tempUnit[cUnit++] = *(RawData + i);
		}

		if (*(RawData + i) == 'A')
		{
			break;
		}
	}

	tempData = atoi((const char *)&tempNum[0]);

	if (strncmp("uA", (const char *)&tempUnit[0], 2u) == 0)
	{
		*OutData = tempData * 10;
	}
	else if (strncmp("mA", (const char *)&tempUnit[0], 2u) == 0)
	{
		*OutData = tempData * 10000;
	}
}

void Delay_us(uint32 value)
{
	static	uint32 i;

	for (i=0; i<0xF; i++) {}
}

void ParseVoltage(uint8 *RawData, uint32 *OutData)
{
	/* AEM voltage: 3.3027	*/

	uint8 i;
	uint32 tempData;
	uint8 tempNum[10] = {[0 ... 9] = 0};
	uint8 cNum = 0;

	for (i = 0; i < 7; i++)
	{
		if ((*(RawData + i) >= '0')	&&
			(*(RawData + i) <= '9'))
		{
			tempNum[cNum++] = *(RawData + i);
		}
	}

	tempData = atoi((const char *)&tempNum[0]);
	*OutData = tempData / 10;
}

uint32 GetCurrTime(void)
{
//	SYSTEMTIME time;
//
//	GetSystemTime(&time);
//
//	return ((((time.wDay * 86400) + (time.wHour * 3600) + (time.wMinute * 60) + time.wSecond) * 1000) + time.wMilliseconds);
//
	/*	1496409380	*/
	/*	GMT: Friday, 2 June 2017 - 13:16:20	*/

	static struct timeval TimeVal;
	gettimeofday(&TimeVal, NULL);

	return (((TimeVal.tv_sec - 1496409380UL) * 1000000) + TimeVal.tv_usec);
}

bool Receive_SocketData(SOCKET sHandler, uint8 *BufRec)
{
	int				socketRet;
	static uint8	tempData[100];
	bool			ret = false;

	socketRet = recv(SocketHandler, (char *)&tempData[0], 100, 0);

	if (socketRet != SOCKET_ERROR)
	{
		(void)memcpy(BufRec, &tempData[0], 100);
		ret = true;
	}
	else
	{
		printf("#");
		fflush(stdout);
	}

	return ret;
}

uint8 InitSocketClient(void)
{
	uint8		ret = FALSE;
	WSADATA		wsa;
	SOCKADDR_IN	ServerAddr;

	int  RetCode;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
		printf("Failed. Error Code : %d",WSAGetLastError());
		fflush(stdout);
    }
    else
    {
		// Create a new socket to make a client connection.
		SocketHandler = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if(SocketHandler == INVALID_SOCKET)
		{
			WSACleanup();
			return -1;
		}
		else
		{
			printf("Client: socket() is OK!\n");
			fflush(stdout);

			// IPv4
			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_port = htons(4902);
			ServerAddr.sin_addr.s_addr = inet_addr("10.0.0.103");

			// Make a connection to the server with socket SocketHandler.
			RetCode = connect(SocketHandler, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr));

			if (RetCode != 0)
			{
				printf("Client: connect() failed! Error code: %d\n", WSAGetLastError());
				fflush(stdout);

				closesocket(SocketHandler);
				WSACleanup();
				return -1;
			}
			else
			{
				// If iMode!=0, non-blocking mode is enabled.
				//u_long iMode = 1;
				//ioctlsocket(SocketHandler, FIONBIO, &iMode);

				printf("Client: connect() is OK, got connected...\n");
				fflush(stdout);

				ret = TRUE;
			}
		}
    }

	return ret;
}
