#ifndef __INTER_MSG_EXAMPLE_HPP__
#define __INTER_MSG_EXAMPLE_HPP__

#include "network/interCom/interMsg/interMsg.hpp"

class InterMsgExample : public InterMsg
{
    private:
    static constexpr const SendType m_sendType = SendType::UnicastWithControl;

    struct Data
    {
        int exampleData;
    } m_data;

    public:
    /**
     * @brief Create an InterMsgExample with a specific example data.
     * @param exampleData The example data to be included in the message.
     */
    InterMsgExample(Client client, int exampleData);

    /**
     * @brief Create an InterMsgExample from raw data.
     * @param rawData The raw data to be used for the message.
     * @param size The size of the raw data.
     */
    InterMsgExample(Client client, char *rawData, uint32_t size);

    /**
     * @brief Get the example data from the message.
     * @return The example data.
     */
    int getExampleData() const;

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
};

#endif // __INTER_MSG_EXAMPLE_HPP__
