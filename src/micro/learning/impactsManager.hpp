#ifndef __IMPACTS_MANAGER_HPP__
#define __IMPACTS_MANAGER_HPP__

#include "impact.hpp"
#include "micro/micro.hpp"

#include <vector>

class ImpactsManager
{
    private:
    /**
     * @brief Vector of all the micros
     * @note The index of the micro in the vector is the index of the micro in the impacts vector
     */
    std::vector<Micro *> m_micros_table;

    /**
     * @brief Vector 2D of impacts of all the micros on all the micros
     * The index of the first vector is the index of the micro that impacts
     * The index of the second vector is the index of the micro that is impacted
     * Diagonal is the impact of the micro on itself so it is always 1
     */
    std::vector<std::vector<Impact *> *> m_impacts;

    /**
     * @brief Return the matrix rank of an link between two micros
     *
     * @param impactorMicro Micro that impacts
     * @param impactedMicro Micro that is impacted
     * @return uint32_t Rank of the link
     */
    uint32_t impactRank(uint32_t impactorMicro, uint32_t impactedMicro);

    public:
    ImpactsManager();

    /**
     * @brief Get a micro from it index
     *
     * @param index Index of the micro
     * @return Micro* The micro
     */
    Micro *getMicro(uint32_t index);

    /**
     * @brief Get the index of a micro from the micro
     *
     * @param micro The micro
     * @return uint32_t The index of the micro
     */
    uint32_t getMicroIndex(Micro *micro);

    /**
     * @brief Add a micro to the impacts manager
     *
     * @param micro The micro to add
     */
    void addMicro(Micro *micro);

    /**
     * @brief Returns the real impact of a micro on an other micro
     *
     * @param impactorMicro The micro that impacts
     * @param impactedMicro The micro that is impacted
     * @return The real impact of the micro
     */
    float getRealImpact(Micro *impactorMicro, Micro *impactedMicro);

    /**
     * @brief Returns the artifical impact of a micro on an other micro
     *
     * @param impactorMicro The micro that impacts
     * @param impactedMicro The micro that is impacted
     * @return The artificial impact of the micro
     */
    float getArtImpact(Micro *impactorMicro, Micro *impactedMicro);

    /**
     * @brief Set the real impact of a micro on an other micro
     *
     * @param impactorMicro The micro that impacts
     * @param impactedMicro The micro that is impacted
     * @param impact The real impact of the micro
     */
    void setRealImpact(Micro *impactorMicro,
                       Micro *impactedMicro,
                       float impact);

    /**
     * @brief Set the impacts of a micro on an other micro
     *
     * @param impactorMicro The micro that impacts
     * @param impactedMicro The micro that is impacted
     * @param impact The impact of the micro
     */
    void setImpact(Micro *impactorMicro, Micro *impactedMicro, Impact impact);

    /**
     * @brief Function that calculates the artificial impacts
     * of all the micros on all the micros from the real impacts
     */
    void calculateArtImpacts();

    /**
     * @brief Set the micro value of a micro
     * @note the value will be corrected by the impacts
     *
     * @param micro The micro
     * @param microValue The value of the micro (not corrected)
     */
    void setMicroValue(Micro *micro, uint8_t microValue);

    /**
     * @brief Get the number of micros
     *
     * @return The number of micros
     */
    uint32_t getMicroCount();
};

#endif
