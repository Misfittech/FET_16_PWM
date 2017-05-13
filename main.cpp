/*
 * main.cpp
 *
 *  Created on: May 13, 2017
 *      Author: tstern
 */

#include "main.h"
#include "board.h"
#include "commands.h"


PCA9685 pca9685=PCA9685(PCA9685_ADDR);

void main_setup(void)
{
	// put your setup code here, to run once:
  SerialUSB.begin(SERIAL_BAUD);

  int to=20;
  //wait for USB serial port to come alive
   while (!SerialUSB)
   {
      to--;
      if (to == 0)
      {
   break;
      }
      delay(500);
   };     //wait for serial
   SysLogInit(NULL, LOG_DEBUG);
   SysLogDisable();

   pca9685.begin();
   pca9685.setPWMFreq(1500);
   SerialUSB.println("Power UP");
   commandsInit();


}

void main_loop(void)
{
	commandsProcess();
}

