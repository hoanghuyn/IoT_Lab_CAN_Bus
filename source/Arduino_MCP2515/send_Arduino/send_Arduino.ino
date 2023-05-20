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
#include <SPI.h>          //Library for using SPI Communication 
#include <mcp2515.h>      //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
struct can_frame canMsg; // Khởi tạo biến cấu trúc cho CAN frame
MCP2515 mcp2515(10);     // chọn chân chip select là pin 10
char led_no = 0;             // dữ liệu gửi

/**
 * @brief Hàm thiết lập các thông số ban đầu.
 */
void setup()
{
  while (!Serial)
    ;
  Serial.begin(9600); // Begins Serial Communication at 9600 baudrate
  SPI.begin();        // Begins SPI communication

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); // Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();                   // Sets CAN at normal mode
}

/**
 * @brief Chương trình kiểm tra xem có dữ liệu được nhận thành công từ CAN bus hay không,
 * nếu có thì đọc dữ liệu và in ra Serial Terminal, nếu không thì in "Chua nhan"
 */
void loop()
{ 
  Serial.println("start");
  canMsg.can_id  = 0x65D; // Thiết lập CAN ID là 0x65D
  canMsg.can_dlc = 1      // Chiều dài frame dữ liệu là 1 byte

  canMsg.data[0] = ++led_no;
  if(led_no == 4)
  {
    led_no = 0;
  }  

  int error_check;  // biến lưu trạng thái trả về của hàm sendMessage
  // Đợi đến khi có dữ liệu nhận được và không lỗi
  do
  {
    error_check =  mcp2515.sendMessage(&canMsg);
  }
  while(error_check == (MCP2515::ERROR_FAILTX) || error_check == (MCP2515::ERROR_ALLTXBUSY));     //Sends the CAN message

  delay(1000);
  
  // In dữ liệu nhận được ra Serial Monitor
  for(int i = 0; i < canMsg.can_dlc; i++)
  {
    Serial.println(canMsg.data[i]);
  }
}
