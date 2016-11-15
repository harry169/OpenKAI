#include "_AutoPilot.h"

namespace kai
{

_AutoPilot::_AutoPilot()
{
	m_pAM = NULL;
	m_nAction = 0;
	m_pAA = NULL;

}

_AutoPilot::~_AutoPilot()
{
}

bool _AutoPilot::init(Config* pConfig)
{
	CHECK_F(!this->_ThreadBase::init(pConfig));
	pConfig->m_pInst = this;

	//create action instance
	Config* pCC = pConfig->o("action");
	CHECK_T(pCC->empty());
	Config** pItr = pCC->getChildItr();

	int i = 0;
	while (pItr[i])
	{
		Config* pAction = pItr[i];
		i++;

		bool bInst = false;
		F_INFO(pAction->v("bInst", &bInst));
		if (!bInst)continue;
		if (m_nAction >= N_ACTION)LOG(FATAL);

		ActionBase** pA = &m_pAction[m_nAction];
		m_nAction++;

		if (pAction->m_class == "RC_visualFollow")
		{
			*pA = new RC_visualFollow();
			F_ERROR_F(((RC_visualFollow* )(*pA))->RC_visualFollow::init(pAction));
			m_pAA = *pA;
		}
		else if (pAction->m_class == "APMcopter_landing")
		{
			*pA = new APMcopter_landing();
			F_ERROR_F(((APMcopter_landing* )(*pA))->APMcopter_landing::init(pAction));
		}
		else if (pAction->m_class == "HM_base")
		{
			*pA = new HM_base();
			F_ERROR_F(((HM_base* )(*pA))->HM_base::init(pAction));
		}
		else if (pAction->m_class == "HM_follow")
		{
			*pA = new HM_follow();
			F_ERROR_F(((HM_follow* )(*pA))->HM_follow::init(pAction));
		}
		else
		{
			LOG(INFO)<<"Unknown action class"+pAction->m_class;
		}
	}

	return true;
}

bool _AutoPilot::link(void)
{
	NULL_F(m_pConfig);

	int i;
	for(i=0;i<m_nAction;i++)
	{
		ActionBase* pA = m_pAction[i];

		if (*pA->getClass() == "RC_visualFollow")
		{
			F_ERROR_F(((RC_visualFollow*)pA)->RC_visualFollow::link());
		}
		else if (*pA->getClass() == "APMcopter_landing")
		{
			F_ERROR_F(((APMcopter_landing*)pA)->APMcopter_landing::link());
		}
		else if (*pA->getClass() == "HM_base")
		{
			F_ERROR_F(((HM_base*)pA)->HM_base::link());
		}
		else if (*pA->getClass() == "HM_follow")
		{
			F_ERROR_F(((HM_follow*)pA)->HM_follow::link());
		}
	}

	string iName="";
	F_INFO(m_pConfig->v("_Automaton", &iName));
	m_pAM = (_Automaton*) (m_pConfig->root()->getChildInstByName(&iName));

	return true;
}

bool _AutoPilot::start(void)
{
	//Start thread
	m_bThreadON = true;
	int retCode = pthread_create(&m_threadID, 0, getUpdateThread, this);
	if (retCode != 0)
	{
		LOG(ERROR)<<retCode;
		m_bThreadON = false;
		return false;
	}

	return true;
}

void _AutoPilot::update(void)
{
	while (m_bThreadON)
	{
		this->autoFPSfrom();

		//TODO:Execute actions based on Automaton State

		int i;
		for(i=0;i<m_nAction;i++)
		{
			ActionBase* pA = m_pAction[i];

			if(*pA->getClass() == "HM_follow")
			{
				((HM_follow*)pA)->HM_follow::update();

				m_pAA = pA;
			}
		}

		this->autoFPSto();
	}

}

bool _AutoPilot::draw(Frame* pFrame, vInt4* pTextPos)
{
	NULL_F(pFrame);

	Mat* pMat = pFrame->getCMat();

	putText(*pMat, "AutoPilot FPS: " + i2str(getFrameRate()),
			cv::Point(pTextPos->m_x, pTextPos->m_y), FONT_HERSHEY_SIMPLEX, 0.5,
			Scalar(0, 255, 0), 1);

	pTextPos->m_y += pTextPos->m_w;

	CHECK_T(!m_pAA);

	string* className = m_pAA->getClass();
	if(*className == "VisualFollow")
	{
		((RC_visualFollow*)m_pAA)->draw(pFrame, pTextPos);
	}
	else if(*className == "HM_follow")
	{
		((HM_follow*)m_pAA)->draw(pFrame, pTextPos);
	}


	return true;
}

void _AutoPilot::onMouse(MOUSE* pMouse)
{
	NULL_(m_pAA);
	NULL_(pMouse);
	CHECK_(*(m_pAA->getClass())!="RC_visualFollow");

	string* className = m_pAA->getClass();
	if(*className == "RC_visualFollow")
	{
		((RC_visualFollow*)m_pAA)->onMouse(pMouse);
	}
}


}
