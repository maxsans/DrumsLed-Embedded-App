#ifndef __INTER_MSG_ADC_HPP__
#define __INTER_MSG_ADC_HPP__

#include "network/interCom/interMsg/interMsg.hpp"
#include "api/adc/adc.h"

class InterMsgAdc : public InterMsg
{
    private:
        static constexpr const SendType m_sendType = SendType::UnicastWithoutControl;

        struct Data
        {
            adc_measure_t m_adcValue;
        } m_data;

        /**
         * @see InterMsg::getPrivData
         */
        const char* getPrivData() const override;

    public:
        /**
         * @brief Create an InterMsgAdc with ADC channel data.
         * @param adcValues Array of ADC values.
         */
        InterMsgAdc(Client client, adc_measure_t adcValue);

        /**
         * @brief Create an InterMsgAdc from raw data.
         * @param rawData The raw data to be used for the message.
         * @param size The size of the raw data.
         */
        InterMsgAdc(Client client, char *rawData, uint32_t size);

        /**
         * @brief Get ADC value for a specific channel.
         * @return The ADC value.
         */
        adc_measure_t getAdcValue() const;

        /**
         * @brief Get a description of the message.
         */
        std::string toString() const override;
};

#endif // __INTER_MSG_ADC_HPP__
