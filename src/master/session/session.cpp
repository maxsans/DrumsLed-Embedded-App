#include "session.hpp"
#include "tools/logStream/logStream.hpp"

Session::Session(bool active)
    : m_active(active),
      m_animationManager(),
      m_moduleManager(),
      m_learning(nullptr)
{
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
    LogStream::cout << "Learning session is done." << LogStream::endl;
    // Delete the learning session
    if (m_learning != nullptr)
    {
        delete m_learning;
        m_learning = nullptr;
    }
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
    m_learning = new Learning(&m_moduleManager, Session::learningDoneCallback);
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
