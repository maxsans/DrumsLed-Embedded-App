#include "session.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/term/term.hpp"
#include "tools/async/async.hpp"

Session::Session(bool active)
    : m_active(active),
      m_animationManager(),
      m_moduleManager(active),
      m_learning(nullptr)
{
    // Register some terminal commands
    Term::registerCommand(TermCommand(
        TermAction([this](const TermParameters&)
        {
            this->startLearning();
        }),
        "start_learning",
        "Starts the learning process."
    ));

    Term::registerCommand(TermCommand(
        TermAction([this](const TermParameters&)
        {
            this->stopLearning();
        }),
        "stop_learning",
        "Stops the learning process."
    ));
}

Session::~Session()
{
    // Delete the learning session
    if (m_learning != nullptr)
    {
        delete m_learning;
        m_learning = nullptr;
    }
}

void Session::setActive(bool active)
{
    m_active = active;
}

bool Session::isActive() const
{
    return m_active;
}

void Session::learningDoneCallback(void *object)
{
    // Cast the object to Session
    Session *session = static_cast<Session *>(object);
    // Process the learning done
    session->processLearningDone();
}

void Session::processLearningDone()
{
    // Use Async because the callback is called from Learning class itself
    Async::registerAsync([this]()
    {
        LogStream::cout << "Learning session is done." << LogStream::endl;
        // Delete the learning session
        if (m_learning != nullptr)
        {
            delete m_learning;
            m_learning = nullptr;
        }
    });
}

void Session::startLearning()
{
    // Check if the session is active
    if (!m_active)
    {
        LogStream::cout << "Session is not active. Cannot start learning." << LogStream::endl;
        return;
    }
    // Check if a learning session is already active
    if (m_learning != nullptr)
    {
        LogStream::cout << "A learning session is already active." << LogStream::endl;
        return;
    }
    // Create a new learning session
    m_learning = new Learning(&m_moduleManager, Session::learningDoneCallback, this);
    m_learning->startLearning();
}

void Session::stopLearning()
{
    // Check if a learning session is active
    if (m_learning == nullptr)
    {
        LogStream::cout << "No learning session is active." << LogStream::endl;
        return;
    }
    // Stop the learning session
    m_learning->stopLearning();
    delete m_learning;
    m_learning = nullptr;
}
