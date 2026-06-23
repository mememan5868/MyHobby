/*
 * ISS_SBUS16.h
 *
 *  Created on: Apr 3, 2026
 *      Author: koiwa
 */

#ifndef LIB_ISS_SBUS16_H_
#define LIB_ISS_SBUS16_H_

#include "stdbool.h"
#include "stm32f4xx_hal.h"//マイコン機種ごとに異なる

/*
 * ISS_SBUS_Lib - Version 26.04.04
 * */

#define TIMEOVER 100

class ISS_SBUS16{
public:
	ISS_SBUS16();

	void receive(uint32_t canID, uint8_t *rxData);

	void recieveDelayUpdate(double dtms);

	bool isActive();
	bool getFailsafe();
	double getStick(int id);
	double getStick(int id, double cutoff);
	double getSlide(int id);
	double getVolume(int id);
	int8_t getSwitch(int id);

	enum StickID{
		LeftX = 0,
		LeftY = 1,
		RightX = 2,
		RightY = 3
	};

	enum SlideID{
		SlideL = 0,
		SlideR = 1
	};

	enum VolumeID{
		VoluemeL = 0,
		VoluemeR = 1
	};

	enum SwitchStatus{
		Upper = -1,
		Central = 0,
		Lower = 1
	};
private:
	double _stick[4];
	double _volume[2];
	double _slide[2];
	int8_t _switch[8];
	bool _failsafe;
	int8_t _swbit(uint8_t sw);
	int recieveDelayMs; //何ミリ秒前がデータの最終更新か
};

#endif /* LIB_ISS_SBUS16_H_ */
