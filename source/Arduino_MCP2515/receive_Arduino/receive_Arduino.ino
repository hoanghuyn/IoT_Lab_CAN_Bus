/*******************************************************************************
 * File name: receive_Arduino
 *
 * Description: Đây là source code của nốt Nhận trong bài mô phỏng truyền nhận tín
 * hiệu CAN giữa 2 Node dùng Arduino - MCP2515.
 *
 * Node gửi sẽ gửi một số từ 1-4 sang Node nhận, Node nhận sẽ đọc và in dữ liệu đã
 * nhận ra màn hình.
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
#include <SPI.h>              //Library for using SPI Communication
#include <mcp2515.h>          //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
struct can_frame canMsg;  // Khởi tạo biến cấu trúc cho CAN frame
MCP2515 mcp2515(10);  // chọn chân chip select là pin 10
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @brief Hàm thiết lập các thông số ban đầu.
 */
void setup()
{
  while (!Serial);
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate
  SPI.begin();                       //Begins SPI communication
 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();                   //Sets CAN at normal mode
}

/**
 * @brief Chương trình kiểm tra xem có dữ liệu được nhận thành công từ CAN bus hay không,
 * nếu có thì đọc dữ liệu và in ra Serial Monitor, nếu không thì in "Chua nhan"
 */
void loop()
{
  // Nếu message đến được nhận và quá trình nhận không lỗi
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) 
  {
    Serial.println("Received data");
    for(int i = 0; i < canMsg.can_dlc; i++)
    {
      Serial.println(canMsg.data[i]);
    }
    delay(1000);
  }
  else
  {
    Serial.println("Chua nhan");
  } 
}