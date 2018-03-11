#ifndef OpenKAI_src_Autopilot_Controller_HM_marker_H_
#define OpenKAI_src_Autopilot_Controller_HM_marker_H_

#include "../../../Base/common.h"
#include "../../../Detector/_DetectorBase.h"
#include "../../ActionBase.h"
#include "HM_base.h"

namespace kai
{

class HM_marker: public ActionBase
{
public:
	HM_marker();
	~HM_marker();

	bool init(void* pKiss);
	bool link(void);
	void update(void);
	bool draw(void);

private:
	_DetectorBase*	m_pDet;
	HM_base* m_pHM;

	int		m_iMarkerClass;
	double	m_minSize;
	int		m_rpmSteer;
	OBJECT	m_obj;

};

}

#endif

