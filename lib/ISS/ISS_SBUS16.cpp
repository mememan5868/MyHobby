/*
 * ISS_SBUS16.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: koiwa
 */

/*
 * ISS_SBUS_Lib - Version 26.04.04
 * */

#include "ISS_SBUS16.h"

/*
 * プロポの設定次第で変わる　(１例)
#define Ach 5
#define Bch 6
#define Cch 7
#define Dch 8
#define Ech 9
#define Fch 10
#define Gch 11
#define Hch 12
 */

ISS_SBUS16::ISS_SBUS16(){
	_failsafe = true;

	_stick[4] = {0};
	_volume[2] = {0};
	_slide[2] = {0};
	_switch[8] = {1};

	recieveDelayMs = 0;
}

int8_t ISS_SBUS16::_swbit(uint8_t sw){
	if(sw == 0b00000011){
		return ISS_SBUS16::Upper;
	}else if(sw == 0b00000010){
		return ISS_SBUS16::Central;
	}else{
		return ISS_SBUS16::Lower;
	}
}

void ISS_SBUS16::receive(uint32_t canID, uint8_t *rxData){
	if(canID == 0x50){ //Stick
		_stick[0] = ((int16_t)((rxData[0] << 8) | rxData[1]) / (double)0x7F);
		_stick[1] = ((int16_t)((rxData[2] << 8) | rxData[3]) / (double)0x7F);
		_stick[2] = ((int16_t)((rxData[4] << 8) | rxData[5]) / (double)0x7F);
		_stick[3] = ((int16_t)((rxData[6] << 8) | rxData[7]) / (double)0x7F);

		recieveDelayMs = 0;
	}else if(canID == 0x51){ //failsafe & switch & slide & volume
		//failsafe
		_failsafe = (rxData[0] == 0xC3);

		//Switch 1byte目
		_switch[0] = _swbit((rxData[1] >> 0) & 0x03);
		_switch[1] = _swbit((rxData[1] >> 2) & 0x03);
		_switch[2] = _swbit((rxData[1] >> 4) & 0x03);
		_switch[3] = _swbit((rxData[1] >> 6) & 0x03);

		//Switch 2byte目
		_switch[4] = _swbit((rxData[2] >> 0) & 0x03);
		_switch[5] = _swbit((rxData[2] >> 2) & 0x03);
		_switch[6] = _swbit((rxData[2] >> 4) & 0x03);
		_switch[7] = _swbit((rxData[2] >> 6) & 0x03);

		//volume
		_volume[0] = (int8_t)rxData[3] / (double)0x7F;
		_volume[1] = (int8_t)rxData[4] / (double)0x7F;

		//slide
		_slide[0] = (int8_t)rxData[5] / (double)0x7F;
		_slide[1] = (int8_t)rxData[6] / (double)0x7F;

		recieveDelayMs = 0;
	}
}

bool ISS_SBUS16::isActive(){
	return recieveDelayMs < TIMEOVER;
}

bool ISS_SBUS16::getFailsafe(){
	if(!isActive())
		return true;

	return _failsafe;
}

double ISS_SBUS16::getStick(int id){
	if(!isActive())
		return 0;

	if(id < 0 || 4 < id)
		return 0;

	return _stick[id];
}

double ISS_SBUS16::getStick(int id, double cutoff){
	if(!isActive())
		return 0;

	if(id < 0 || 4 < id)
		return 0;

	if(_stick[id] < cutoff && -1 * cutoff < _stick[id] )
		return 0;

	return _stick[id];
}

double ISS_SBUS16::getSlide(int id){
//	if(!isActive())
//		return 0;

	if(id < 0 || 2 < id)
		return 0;

	return _slide[id];
}

double ISS_SBUS16::getVolume(int id){
//	if(!isActive())
//		return 0;

	if(id < 0 || 2 < id)
		return 0;

	return _volume[id];
}

int8_t ISS_SBUS16::getSwitch(int id){
//	if(!isActive())
//		return 0;

	if(id < 5 || 12 < id)
		return 0;

	return _switch[id - 5];
}

void ISS_SBUS16::recieveDelayUpdate(double dtms){
	if(recieveDelayMs < 10000)
		recieveDelayMs += (int)dtms;

	if(!isActive()){
		_stick[0] = 0.0;
		_stick[1] = 0.0;
		_stick[2] = 0.0;
		_stick[3] = 0.0;
	}
}
