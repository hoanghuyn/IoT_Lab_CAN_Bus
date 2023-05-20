/*******************************************************************************
 * File name: node_1
 *
 * Description: Đây là source code của Node_1 trong bài mô phỏng truyền nhận tín 
 * hiệu CAN giữa 2 Node.
 * 
 * Node_2 sẽ gửi "hellocan" sang Node_1, Node_1 nhận được và convert thành chữ viết
 * hoa "HELLOCAN" và gửi lại sang Node_1
 *
 * Author: Nguyen Vuong Quoc Bao, Nguyen Huy Hoang
 *
 * Last Changed By:  $Author: hoanghuyn $
 * Revision:         $Revision: 1.0$
 * Last Changed:     $Date: $May 20, 2023
 * 
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <ESP32CAN.h>
#include <CAN_config.h>
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
CAN_device_t CAN_cfg; // Biến cấu trúc chứa các thông số cấu hình cho CAN
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @brief Hàm thiết lập các thông số ban đầu.
 */
void setup() {
    Serial.begin(115200); // Khởi tạo Serial với tốc độ baud 115200 bps
    Serial.println("iotsharing.com CAN demo");
    
    /* Thiết lập các chân cho CAN và tốc độ */
    CAN_cfg.speed=CAN_SPEED_500KBPS;  // Thiết lập tốc độ giao tiếp CAN bus là 500 kbps
    CAN_cfg.tx_pin_id = GPIO_NUM_5;   // GPIO5 (G4/D4) làm chân truyền từ Controller (MCU) -> Transceiver
    CAN_cfg.rx_pin_id = GPIO_NUM_4;   // GPIO4 (G4/D4) làm chân nhận từ Transceiver -> Controller (MCU)
    
    /* Tạo một hàng đợi để nhận dữ liệu CAN */
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    
    ESP32Can.CANInit();               // Khởi tạo module CAN
}

void loop() 
{
    CAN_frame_t rx_frame;   // Khởi tạo biến cấu trúc khung tin nhận
    
    // Nhận khung tin CAN tiếp theo từ hàng đợi, nếu có tin đang trong hàng đợi
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE)
    {
      if(rx_frame.FIR.B.FF==CAN_frame_std)  // Nếu khung tin là STANDARD
      {
        char buff[100];
        sprintf(buff, "New standard frame");
        Serial.println(buff);
      }
      else // Nếu khung tin là EXTENDED
      {
        char buff[100];
        sprintf(buff, "New extended frame");
        Serial.println(buff);
      }

      if(rx_frame.FIR.B.RTR==CAN_RTR)   // Nếu khung tin đến là RTR (Remote Transmission Request)
      {
        char buff[100];
        sprintf(buff, " RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        Serial.println(buff);
      }
      else  // Nếu khung tin đến là Data Frame
      {
        char buff[100];
        sprintf(buff, " from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        Serial.println(buff);
        /* Nếu dữ liệu là chữ cái viết thường thì chuyển thành viết hoa và gửi lại cho Node_2 */
        for(int i = 0; i < 8; i++)
        {
          if(rx_frame.data.u8[i] >= 'a' && rx_frame.data.u8[i] <= 'z')
          {
            rx_frame.data.u8[i] = rx_frame.data.u8[i] - 32;
          }
        }
      }
      // Gửi gói tin trở lại ra CAN bus cho Node_2
      ESP32Can.CANWriteFrame(&rx_frame);
    }
}
