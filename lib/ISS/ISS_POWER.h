/*
 * ISS_POWER.h
 *
 *  Created on: Apr 6, 2026
 *      Author: koiwa
 */
#ifndef LIB_ISS_POWER_H_
#define LIB_ISS_POWER_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"//マイコン機種ごとに異なる


#define TIMEOVER 100

class ISS_POWER{
public:
	ISS_POWER();

	bool isActive();

	void receive(uint32_t canID, uint8_t *rxData);

	void recieveDelayUpdate(double dtms);

	bool getIsEmgSwitch();

	double getVoltage3S();
	double getVoltage6s();
private:
	bool _isEmgSwitch;

	double _voltage3S;
	double _voltage6S;

	int _recieveDelayMs; //何ミリ秒前がデータの最終更新か
};

#endif /* LIB_ISS_POWER_H_ */

