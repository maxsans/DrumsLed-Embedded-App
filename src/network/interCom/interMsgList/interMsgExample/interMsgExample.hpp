#ifndef __INTER_MSG_EXAMPLE_HPP__
#define __INTER_MSG_EXAMPLE_HPP__

#include "network/interCom/interMsg/interMsg.hpp"
#include <functional>

class InterMsgExample : public InterMsg
{
    public:
        /**
         * @brief Type of callback to call when the message is received.
         * @param client The client that sent the message.
         * @param msg The message received.
         */
        using Callback = std::function<void(const Client &, InterMsgExample &)>;

    private:
        static constexpr const SendType m_sendType = SendType::UnicastWithControl;

        struct Data
        {
            int exampleData;
        } m_data;

        /**
         * @brief Callback to be called when the message is received.
         */
        static Callback m_callback;

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
        InterMsgExample(Client client, char *rawData, size_t size);

        /**
         * @brief Register a callback to be called when the message is received.
         * @param callback The callback to be registered.
         */
        static void registerCallback(Callback callback);

        /**
         * @brief Parse the message by calling the registered callback.
         */
        void parse();

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
