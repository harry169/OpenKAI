/*
 * _Lightware_SF40.h
 *
 *  Created on: Nov 4, 2016
 *      Author: yankai
 */

#ifndef AI__Lightware_SF40_H_
#define AI__Lightware_SF40_H_

#include "../Base/common.h"
#include "../Base/_ThreadBase.h"
#include "../Navigation/_Universe.h"
#include "../IO/SerialPort.h"
#include "../Algorithm/Filter.h"

#define DEG_AROUND 360.0
#define DIV_AROUND 2500
#define CR '\x0d'
#define LF '\x0a'


namespace kai
{

using std::string;
using namespace std;
using namespace cv;

class _Lightware_SF40: public _ThreadBase
{
public:
	_Lightware_SF40();
	~_Lightware_SF40();

	bool init(Config* pConfig);
	bool link(void);
	void close(void);
	bool draw(Frame* pFrame, iVec4* pTextPos);

	std::vector<vector<float> > detect(Frame* pFrame);
	bool start(void);

private:
	void updateLidar(void);

	void reqMap(void);
	void write(void);
	bool read(void);

	void update(void);
	static void* getUpdateThread(void* This)
	{
		((_Lightware_SF40*) This)->update();
		return NULL;
	}

public:
	_Universe* m_pUniverse;
	string m_sportName;
	SerialPort* m_pSerialPort;
	int m_baudRate;

	double	m_offsetAngle;
	double	m_nDiv;
	double	m_dAngle;
	double	m_pDist[DIV_AROUND];
	double	m_minDist;
	double	m_maxDist;
	int		m_mwlX;
	int		m_mwlY;
	Filter* m_pX;
	Filter* m_pY;

	uint64_t m_lastReq;

	string	m_strRecv;

	double	m_showScale;






};

}

#endif
