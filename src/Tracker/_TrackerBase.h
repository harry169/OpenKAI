/*
 * _TrackerBase.h
 *
 *  Created on: Aug 28, 2018
 *      Author: yankai
 */

#ifndef OpenKAI_src_Tracker__TrackerBase_H_
#define OpenKAI_src_Tracker__TrackerBase_H_

#include "../Base/common.h"
#include "../Base/_ThreadBase.h"
#include "../Vision/_VisionBase.h"

namespace kai
{

class _TrackerBase: public _ThreadBase
{
public:
	_TrackerBase();
	virtual
	~_TrackerBase();

	bool init(void* pKiss);
	void update(void);
	virtual bool draw(void);
	virtual bool cli(int& iY);

	virtual void createTracker(void);
	virtual bool startTrack(vDouble4& bb);
	virtual void stopTrack(void);
	bool bTracking(void);
	vDouble4* getBB(void);

public:
	_VisionBase* m_pVision;
	Rect2d m_rBB;
	vDouble4 m_bb;
	double m_margin;

	Rect2f	m_newBB;
	int		m_iSet;
	int		m_iInit;

	string m_trackerType;
	uint64_t m_tStampBGR;
	bool	m_bTracking;
};

}
#endif
