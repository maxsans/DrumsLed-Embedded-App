#ifndef __MICRO_H__
#define __MICRO_H__

#include "module.h"

#include <stdint.h>

/**
 * @brief Class to represent a microphone.
 */
class micro
{
    private:

    protected:
        bool m_connected;
        uint8_t m_microValue;
        uint8_t m_microValueCorrected;
        module *m_module;
        float m_correction;
        uint8_t m_threshold;
        uint64_t m_lastSyncTime;
        void checkTime();

    public:
        micro(module *m);
        /**
         * @brief Process the microphone.
         * @note This function must be called in the main loop.
         */
        void process();
        /**
         * @brief After news over udp, sync the module to push back the timeout.
         */
        void sync();
        /**
         * @brief Check if the microphone is connected.
         *
         * @return bool True if connected.
         */
        bool isConnected();
        /**
         * @brief Get the Module object
         *
         * @return module* The module.
         */
        module *getModule();
        /**
         * @brief Set the correction of the microphone.
         *
         * @param correction The correction.
         */
        void setCorrection(float correction);
        /**
         * @brief Get the correction of the microphone.
         *
         * @return float The correction.
         */
        float getCorrection();
        /**
         * @brief Set the micro value.
         *
         * @param microValue The micro value.
         */
        void setMicroValue(uint8_t microValue);
        /**
         * @brief Set the micro value after correction.
         *
         * @param microValueCorrected The micro value corrected.
         */
        void setMicroValueCorrected(uint8_t microValueCorrected);
        /**
         * @brief Set the threshold of the microphone (after correction).
         *
         * @param threshold The threshold.
         */
        void setThreshold(uint8_t threshold);
        /**
         * @brief Get the micro value.
         *
         * @return uint8_t The micro value.
         */
        uint8_t getMicroValue();
        /**
         * @brief Get the micro value after correction.
         *
         * @return uint8_t The micro value corrected.
         */
        uint8_t getMicroValueCorrected();
        /**
         * @brief Get the threshold of the microphone (after correction).
         *
         * @return uint8_t The threshold.
         */
        uint8_t getThreshold();
        /**
         * @brief Check if the microphone is hit.
         *
         * @return bool True if hit.
         */
        bool isHit();
};

#endif
