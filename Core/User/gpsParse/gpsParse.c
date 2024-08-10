/*
 * gpsParse.c
 *
 *  Created on: Aug 9, 2024
 *      Author: Steven
 */

#include "gpsParse.h"
#include "usart.h"
#include "app.h"
#include "cmsis_os.h"

uint8_t pos = 0;
uint8_t temp[2];
uint8_t RxData[UARTBUFFERLENGTH];
uint8_t FinalData[UARTBUFFERLENGTH];
extern uint8_t gpsData[UARTBUFFERLENGTH];
extern osSemaphoreId_t gpsSemaphoreHandle;

//void start_bestpos(void)
//{
//  static uint8_t bestpos_cmd[] = "LOG BESTPOSB ontime 1\r\n";
//
//  HAL_UART_Transmit(&huart1, bestpos_cmd, sizeof(bestpos_cmd), 1000);
//
////  HAL_UARTEx_ReceiveToIdle_IT(&huart1, gps_rx_buffer, BESTPOS_BUFFER_SIZE);
////  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
//  return;
//}

//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
//    gpsData[Size] = '\0';
//    gpsData[0] = '\0';
//    HAL_UARTEx_ReceiveToIdle_IT(&huart1, gpsData, UARTBUFFERLENGTH);
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(temp[0] == '\n'){
        memcpy(FinalData, RxData, pos);
        pos = 0;
        osSemaphoreRelease(gpsSemaphoreHandle);
    }else{
        memcpy(RxData + pos, temp, 1);
        pos++;
    }
    HAL_UART_Receive_IT(&huart1, temp, 1);
}

