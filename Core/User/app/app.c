/*
 * app.c
 *
 *  Created on: Aug 9, 2024
 *      Author: Steven
 */

#include "app.h"
#include "cmsis_os.h"
#include "usart.h"
#include "fdcan.h"
#include "../../vendor_generated/can_tools/ucr_01.h"
#include <stdlib.h>

osThreadId_t gpsTaskHandle;
const osThreadAttr_t gpsTask_attributes = {
        .name = "gpsTask",
        .stack_size = 256 * 4,
        .priority = (osPriority_t) osPriorityLow,
};

osSemaphoreId_t gpsSemaphoreHandle;
const osSemaphoreAttr_t gpsSemaphore_attributes = {
  .name = "gpsSemaphore"
};

//uint8_t gpsData[UARTBUFFERLENGTH];
uint8_t canTxData[UCR_01_GPS_BEST_POS_LENGTH];
extern uint8_t temp[];
extern uint8_t FinalData[];
//double latitude = 0;
//double longitude = 0;
//double altitude = 0;
//double stdLatitude = 0;
//double stdLongitude = 0;
//double stdAltitude = 0;

struct ucr_01_gps_best_pos_t gpsData;


void RTOS_Init(void){
    gpsTaskHandle = osThreadNew(StartGPS, NULL, &gpsTask_attributes);

    gpsSemaphoreHandle = osSemaphoreNew(1, 0, &gpsSemaphore_attributes);
}

void StartGPS(void *argument){
    HAL_FDCAN_Start(&hfdcan1);
    unsigned char start[] = "log bestposa ontime 0.1\r\n";
    HAL_UART_Transmit (&huart1, start, sizeof(start), 10);
    HAL_UART_Receive_IT(&huart1, temp, 1);
    FDCAN_TxHeaderTypeDef TxHeader;
    TxHeader.Identifier = UCR_01_GPS_BEST_POS_FRAME_ID;
    TxHeader.IdType = FDCAN_STANDARD_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_48;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_ON;
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;
    osDelay(10000);
    while(1){
        osSemaphoreAcquire(gpsSemaphoreHandle, osWaitForever);
        char tempMsg[UARTBUFFERLENGTH];
        sprintf(tempMsg, strtok(FinalData, "#"));
        if(memcmp("BESTPOSA", tempMsg, 8) == 0){
    //      sprintf(gpsData, tempMsg);
            char *token = strtok(tempMsg, ",");
            int index = 0;
            while(token != NULL){
                token = strtok(NULL, ",");
                switch(index){
                    case 3:
                        // time status
                        break;
                    case 8:
                        //separate by ; and get sol stat
                        break;
                    case 9:
                        // pos type
                        break;
                    case 10:
                        gpsData.latitude++;
                        break;
                    case 11:
                        gpsData.longitude = atof(token);
                        break;
                    case 12:
                        gpsData.altitude = atof(token);
                        break;
                    case 15:
                        gpsData.std_latitude = atof(token);
                        break;
                    case 16:
                        gpsData.std_longitude = atof(token);
                        break;
                    case 17:
                        gpsData.std_altitude = atof(token);
                        break;
                    case 18:
                        // base station id
                        break;
                }
                index ++;
                if(index == 18){
                    break;
                }

            }
            ucr_01_gps_best_pos_pack(canTxData, &gpsData, UCR_01_GPS_BEST_POS_LENGTH);
            if(HAL_OK != HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, canTxData)){

            }
        }
    }
}
