/*
 * board.h
 *
 *  Created on: May 13, 2017
 *      Author: tstern
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <Arduino.h>
#include "syslog.h"

#define SERIAL_BAUD (115200)
#define SerialUSB Serial

#define PCA9685_ADDR  (0x55)

#define PIN_PCA9685_nOE  (7)

#define FW_VERSION "0.01"

#endif /* BOARD_H_ */
