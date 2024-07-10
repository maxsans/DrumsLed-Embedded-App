#include "terminal.h"

#include <conio.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <cstring>

using namespace std;

session *terminal::m_currentSession = nullptr;
char terminal::m_buffer[TERMINAL_BUFFER_SIZE];
int32_t terminal::m_bufferIndex = 0;

terminal::terminal()
{
}

terminal::~terminal()
{
}

void terminal::setCurrentSession(session *currentSession)
{
    m_currentSession = currentSession;
}

void terminal::parseBuffer()
{
    // Parse the command
    if (strcmp(m_buffer, "test") == 0)
    {
        printf("Test command\n");
    }
    else if (strcmp(m_buffer, "learn") == 0)
    {
        m_currentSession->getLearning()->startLearning();
    }
    else if (strcmp(m_buffer, "stop learn") == 0)
    {
        m_currentSession->getLearning()->stopLearning();
    }

    // Add more commands here
    /*
    else if (strcmp(m_buffer, "command") == 0)
    {
        // Do something
    }
    */

    else
    {
        printf("Unknown command : \"%s\"\n", m_buffer+1);
    }
}

void terminal::process()
{
    // Check if a character was entered
    if (_kbhit())
    {
        // Make sure we don't overflow the buffer (this is a programming error if it happens)
        assert(m_bufferIndex < TERMINAL_BUFFER_SIZE);
        // Get the character and store it in the buffer
        m_buffer[m_bufferIndex] = _getch();
        printf("%c", m_buffer[m_bufferIndex]);

        // Check if the character is a newline
        if (m_buffer[m_bufferIndex] == 13)
        {
            printf("\n");
            // Null terminate the string
            m_buffer[m_bufferIndex] = '\0';
            // Process the command
            parseBuffer();
            // Reset the buffer
            m_bufferIndex = 0;
        }
        else
        {
            // Increment the buffer index
            m_bufferIndex++;
        }
    }
}
