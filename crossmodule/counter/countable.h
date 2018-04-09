#pragma once

#include "counter.h"

namespace cross
{


struct instance_countable
{
	instance_countable(instance_counter& pCnt):
		m_pCnt(&pCnt)
	{
		m_pCnt->m_cnt++;
	}

	~instance_countable()
	{
		m_pCnt->m_cnt--;
	}

private:
	instance_counter* m_pCnt;
};


}
