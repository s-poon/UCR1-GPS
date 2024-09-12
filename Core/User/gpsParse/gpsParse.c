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
#include <stdbool.h>

uint8_t pos = 0;
uint8_t temp[2];
uint8_t RxData[UARTBUFFERLENGTH];
uint8_t messageLength;
uint8_t headerLength;
uint16_t imuRatePVAMessageID = 1778;
uint16_t recievedMessageID;
double latitude = 0;
double longitude = 0;
double height = 0;

char FinalData[UARTBUFFERLENGTH];
extern uint8_t gpsData[UARTBUFFERLENGTH];
extern osSemaphoreId_t gpsSemaphoreHandle;
typedef enum RecieveState{
    Header,
    Message
}RecieveState_t;
RecieveState_t CurrentState = Header; 

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
        memcpy(&FinalData, RxData, pos);
        pos = 0;
        osSemaphoreRelease(gpsSemaphoreHandle);
    }else{
        memcpy(RxData + pos, temp, 1);
        pos++;
    }
    if(pos >= UARTBUFFERLENGTH){
        pos = 0;
    }
    HAL_UART_Receive_IT(huart, temp, 1);
}

void HAL_UART_RxCpltCallback1(UART_HandleTypeDef *huart){
    switch(CurrentState){
        case Header:
            DecodeHeader();
            break;
        case Message:

            break;
    }
    osSemaphoreRelease(gpsSemaphoreHandle);
    HAL_UART_Receive_IT(huart, temp, 1);
}

void IMURATEPVA(void){
    for(int i = 0; i < sizeof(double); i++){
        ((uint8_t*) & latitude)[i] = temp[0];
    }
}

void DecodeHeader(void){
    if(pos == 3){
        // set header length
        headerLength = temp[0];
    }else if(pos == 4){
        // Set Header Length
        recievedMessageID = temp[0];
    }else if(pos == 5){
        recievedMessageID += temp[0] << 8;
    }else if(pos == 8){
        messageLength = temp[0];
    }else if(pos == 9){
        messageLength += temp[0] << 8; 
    }else if(pos == headerLength){
        CurrentState = Message;
        pos = 0;
        return;
    }
    pos ++;
}
