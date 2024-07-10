#ifndef __IMPACTS_MANAGER_H__
#define __IMPACTS_MANAGER_H__

#include "impact.h"

#include <vector>

class impactsManager
{
    private:
        /**
         * @brief Vector 2D of impacts of all the micros on all the micros
         * The index of the first vector is the index of the micro that impacts
         * The index of the second vector is the index of the micro that is impacted
         * Indexs are the same as the index of the micro in the micros vector of the microManager
         * Diagonal is the impact of the micro on itself so it is always 1
         *
         */
        std::vector<std::vector<impact *>> m_impacts;
        /**
         * @brief Return the matrix rank of an link between two micros
         *
         * @param impactorMicro Micro that impacts
         * @param impactedMicro Micro that is impacted
         * @return uint32_t Rank of the link
         */
        uint32_t impactRank(uint32_t impactorMicro, uint32_t impactedMicro);

    public:
        impactsManager();

        /**
         * @brief Returns the number of micros
         *
         * @return The number of micros
         */
        uint32_t getMicroCount();

        /**
         * @brief Reserves memory for the impacts when a new micro is added
         *
         */

        void addMicro();
        /**
         * @brief Returns the real impact of a micro on an other micro
         *
         * @param impactorMicro The index of micro that impacts
         * @param impactedMicro The index of micro that is impacted
         * @return The real impact of the micro
         */
        coeff getRealImpact(uint32_t impactorMicro, uint32_t impactedMicro);

        /**
         * @brief Returns the artifical impact of a micro on an other micro
         *
         * @param impactorMicro The index of micro that impacts
         * @param impactedMicro The index of micro that is impacted
         * @return The artificial impact of the micro
         */
        coeff getArtImpact(uint32_t impactorMicro, uint32_t impactedMicro);

        /**
         * @brief Return the impacts of a micro on an other micro
         *
         * @param impactorMicro The index of micro that impacts
         * @param impactedMicro The index of micro that is impacted
         *
         * @return The impact of the micro
         */
        impact *getImpact(uint32_t impactorMicro, uint32_t impactedMicro);

        /**
         * @brief Set the real impact of a micro on an other micro
         *
         * @param impactorMicro The index of micro that impacts
         * @param impactedMicro The index of micro that is impacted
         * @param impact The real impact of the micro
         */
        void setRealImpact(uint32_t impactorMicro, uint32_t impactedMicro, coeff impact);

        /**
         * @brief Set the impacts of a micro on an other micro
         *
         * @param impactorMicro The index of micro that impacts
         * @param impactedMicro The index of micro that is impacted
         * @param impact The impact of the micro
         */
        void setImpact(uint32_t impactorMicro, uint32_t impactedMicro, impact impact);

        /**
         * @brief Function that calculates the artificial impacts
         * of all the micros on all the micros from the real impacts
         *
         */
        void calculateArtImpacts();
};

#endif
