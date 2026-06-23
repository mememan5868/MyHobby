/*
 * ISS_POWER.cpp
 *
 *  Created on: Apr 6, 2026
 *      Author: koiwa
 */
#include <cstdint>
#include "ISS_POWER.h"

ISS_POWER::ISS_POWER(){
	 _isEmgSwitch = true;
	_voltage3S = 0.0;
	_voltage6S = 0.0;
	_recieveDelayMs = 0;
}

bool ISS_POWER::isActive(){
	return _recieveDelayMs < TIMEOVER;
}

void ISS_POWER::receive(uint32_t canID, uint8_t *rxData){
	if(canID == 0x80){
		if(rxData[0] == 0xC3){
			_isEmgSwitch = true;
		}else{
			_isEmgSwitch = false;
		}

		_voltage3S = (double)((int16_t)((rxData[1] << 8) | rxData[2]) / (double)1000);
		_voltage6S = (double)((int16_t)((rxData[3] << 8) | rxData[4]) / (double)1000);

		_recieveDelayMs = 0;
	}
}


void ISS_POWER::recieveDelayUpdate(double dtms){
	if(_recieveDelayMs < 10000)
		_recieveDelayMs += (int)dtms;

	if(!isActive()){
		_isEmgSwitch = true;

		_voltage3S = 0.0;
		_voltage6S = 0.0;
	}
}

bool ISS_POWER::getIsEmgSwitch(){
	return _isEmgSwitch;
}

double ISS_POWER::getVoltage3S(){
	return _voltage3S;
}
double ISS_POWER::getVoltage6s(){
	return _voltage6S;
}
