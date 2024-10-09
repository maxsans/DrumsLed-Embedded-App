#ifndef __MICRO_MANAGER_H__
#define __MICRO_MANAGER_H__

#include "micro.h"
#include "module.h"
#include "impactsManager.h"

#include <vector>

/**
 * @brief Class to manage the microphones.
 */
class microManager
{
    private:
        std::vector<micro *> m_micros;
        impactsManager m_impactsManager;

    public:
        microManager();
        ~microManager();
        /**
         * @brief Process the microphones.
         * @note This function must be called in the main loop.
         */
        void process();
        /**
         * @brief Add a microphone.
         *
         * @param m A pointer on the module to wich the microphone is attached.
         */
        void addMicro(module *m);
        /**
         * @brief Set the micro value.
         *
         * @param m A pointer on the module to wich the microphone is attached.
         * @param microValue The new micro value.
         */
        void setMicro(module *m, uint8_t microValue);
        /**
         * @brief Get the micro.
         *
         * @param index The index of the micro.
         * @return micro* A pointer on the micro.
         */
        micro *getMicro(int32_t index);
        /**
         * @brief Get the micro of a specific module.
         *
         * @param m A pointer on the module to wich the microphone is attached.
         * @return micro* A pointer on the micro.
         */
        micro *getMicro(module *m);
        /**
         * @brief Get the number of micros.
         *
         * @return uint32_t The number of micros.
         */
        uint32_t getMicroCount();
        /**
         * @brief Get the impacts manager.
         *
         * @return impactsManager* The impacts manager.
         */
        impactsManager *getImpactsManager();
};

#endif
