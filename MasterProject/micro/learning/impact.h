#ifndef __IMPACT_H__
#define __IMPACT_H__

#include "coeff.h"

#include <stdint.h>

class impact
{
    private:

    public:
        impact();
        coeff m_realImpact;
        coeff m_ArtImpact;
};

#endif
