//////////////////////////////////////////////////////////
// LoRa Image Transmission
//
// Description : Transmit a PPM Image by a LoRa modem
//
// Author : Lionel Isoz
// Version 11 août 2019
//////////////////////////////////////////////////////////
#define PACKET_INDEX_SIZE   250 // real max = 251
#define IMAGE_NAME          1   // argv
#define NAME_RX_FILE        "ImageRx.ppm"

// Packet Index - encapsulation order - use : packet[NAME]
#define NUMBER_L            0   //  L : Left  ( 2 uint8_t = uint16_t )
#define NUMBER_R            1   //  R : Right
#define SENT_NBR            2   //   change for each packet
#define RECEIVED            3   //
#define LENGTH              4   //
#define COLUMNS_NBR_L       5   ///
#define COLUMNS_NBR_R       6   ///  constant for an image
#define LINES_NBR_L         7   ///
#define LINES_NBR_R         8   ///
#define FIRST_IMG_INDEX     9   //// IMAGE part
#define LAST_IMG_INDEX      248 ////  => 80 pixel 3x80 = 240
#define BYTE_PER_PACKET     LAST_IMG_INDEX - FIRST_IMG_INDEX + 1

//////////////////////////////////////////////////////////
// LoRa PHY configuration
#define FREQUENCY           868.0   /* MHz */
#define TX_POWER            23   /* Tx power is from +5 to +23 dBm */
// config in lib/radiohead/RH_RF95.h at line 437
#define LORA_MODE           1   // Choose between 1, 2, 3 or 4
//  1 <=> MEDIUM_RANGE      2 <=> FAST_SHORT_RANGE
//  3 <=> SLOW_LONG_RANGE   4 <=> MAX_RANGE
enum {MEDIUM_RANGE = 1, FAST_SHORT_RANGE, SLOW_LONG_RANGE, MAX_RANGE};

#define RECEPTION_SLEEP_TIME 1  // sleep some number of microseconds
