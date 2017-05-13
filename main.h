/*
 * main.h
 *
 *  Created on: May 13, 2017
 *      Author: tstern
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "board.h"
#include "PCA9685.h"

void main_setup(void);
void main_loop(void);

extern PCA9685 pca9685;

#endif /* MAIN_H_ */
