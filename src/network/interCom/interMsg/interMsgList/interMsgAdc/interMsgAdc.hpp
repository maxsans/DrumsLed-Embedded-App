#ifndef __INTER_MSG_ADC_HPP__
#define __INTER_MSG_ADC_HPP__

#include "api/adc/adc.hpp"
#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgAdc : public InterMsg
{
    private:
    static constexpr const SendType m_sendType
        = SendType::UnicastWithoutControl;

    /**
     * @brief ADC value.
     */
    adc_measure_t m_adcValue;

    public:
    /**
     * @brief Create an InterMsgAdc with ADC channel data.
     * @param adcValues Array of ADC values.
     */
    InterMsgAdc(Client client, adc_measure_t adcValue);

    /**
     * @brief Constructor for InterMsg with a callback.
     * @note The others attributes are initialized to default values.
     * @note This constructor is protected to ensure that only derived classes can instantiate it.
     * @note This constructor is used in registered message handlers.
     */
    InterMsgAdc(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

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
