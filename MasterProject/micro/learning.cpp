#include "learning.h"

#include "ledManager.h"

#include <winsock2.h>
#include <stdio.h>
#include <assert.h>

#define NB_MESURES_MAX 200
#define TIME_BETWEEN_MEASURES 20 // ms

#define RECORD_LIMIT 1

learning::learning()
{
    // Constructor
    m_manager = NULL;
    m_inLearning = false;
    m_MicroInRecord = -1;
}

learning::learning(microManager *manager)
{
    m_manager = manager;
    // Add as much of micros as microsManager has in the vector of records
    for (uint8_t i = 0; i < m_manager->getMicroCount(); i++)
    {
        m_microRecords.push_back(std::vector<uint8_t>());
    }
}

learning::~learning()
{
    // Destructor
}

void learning::setManager(microManager *manager)
{
    // Set the manager
    m_manager = manager;
    // Add as much of micros as microsManager has in the vector of records
    for (uint8_t i = 0; i < m_manager->getMicroCount(); i++)
    {
        m_microRecords.push_back(std::vector<uint8_t>());
    }
}

void learning::startLearning()
{
    printf("Start learning\n");
    // Start the learning process on the first micro
    if (m_manager->getMicroCount() > 0)
    {
        startLearning(0);
    }
}

void learning::startLearning(int32_t microIndex)
{
    // Start the learning process on a specific micro
    assert(microIndex >= 0 && microIndex < m_manager->getMicroCount());
    m_MicroInRecord = microIndex;

    // If necessary, delete the previous records and free the memory
    for (uint8_t l_microIndex = 0; l_microIndex < m_manager->getMicroCount(); l_microIndex++)
    {
        m_microRecords[l_microIndex].clear();
    }

    printf("Start learning on micro %d\n", microIndex);

    // First, disable all of process that control the leds
    // TODO: Implement this function

    // Get the main micro of this learning
    micro *l_mainMicro = m_manager->getMicro(microIndex);

    // highlight the module that the micro is associated with if it has leds
    rgbLed *l_led = g_ledManager.getLed(l_mainMicro->getModule());
    if (l_led != NULL)
    {
        // The module has leds, highlight it
        l_led->setColor(255, 255, 255);
    }

    // Start the learning process
    m_inLearning = true;
}

void learning::process()
{
    // Function to process the learning
    // Must be called in the main loop

    // Check if we are in learning mode
    if (m_inLearning)
    {
        // Record all micros all TIME_BETWEEN_MEASURES ms
        static uint64_t l_lastRecordTime = GetTickCount64();
        if (GetTickCount64() - l_lastRecordTime > TIME_BETWEEN_MEASURES)
        {
            record();
            l_lastRecordTime = GetTickCount64();
        }
    }
}

void learning::record()
{
    // Record all the micros only if at least one micro is record something
    bool l_recordSomething = false;
    for(uint8_t l_microIndex = 0; l_microIndex < m_manager->getMicroCount(); l_microIndex++)
    {
        // Get the micro
        micro *l_micro = m_manager->getMicro(l_microIndex);

        // Get the micro's value
        uint8_t l_value = l_micro->getMicroValue();

        if (l_value > RECORD_LIMIT)
        {
            l_recordSomething = true;
            break;
        }
    }
    if (l_recordSomething)
    {
        // At least one micro has recorded something
        // Record all the micros
        for(uint8_t l_microIndex = 0; l_microIndex < m_manager->getMicroCount(); l_microIndex++)
        {
            // Get the micro
            micro *l_micro = m_manager->getMicro(l_microIndex);

            // Get the micro's value
            uint8_t l_value = l_micro->getMicroValue();

            // Add a record to the vector of records for each micro
            m_microRecords[l_microIndex].push_back(l_value);

            // Check if we have reached the maximum number of records
            if (m_microRecords[l_microIndex].size() >= NB_MESURES_MAX)
            {
                // Stop the learning process
                stopLearning();
                break;
            }
        }
    }
}

void learning::stopLearning()
{
    // Turn off all the leds
    for (uint16_t l_ledIndex = 0; l_ledIndex < g_ledManager.getLedCount(); l_ledIndex++)
    {
        g_ledManager.getLed(l_ledIndex)->setColor(0, 0, 0);
    }

    // Interpret the records
    calculate();

    // If there are still micros to learn
    if (m_MicroInRecord < m_manager->getMicroCount() - 1)
    {
        // Start the learning process on the next micro
        startLearning(m_MicroInRecord + 1);
    }
    else
    {
        // All the micros have been learned
        // Stop the learning process
        m_inLearning = false;
        printf("End of learning\n");

        // Calculate the artificial impacts
        m_manager->getImpactsManager()->calculateArtImpacts();

        // Display the impacts calculated as 2 matrix (real and artificial)
        printf("\nReal impacts :\n");
        for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_manager->getMicroCount(); l_impactorMicro++)
        {
            for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_manager->getMicroCount(); l_impactedMicro++)
            {
                // Get the real impact
                coeff l_realImpact = m_manager->getImpactsManager()->getRealImpact(l_impactorMicro, l_impactedMicro);
                printf("%.2f    ", l_realImpact.toFloat());
            }
            printf("\n");
        }
        printf("\nArtificial impacts :\n");
        for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_manager->getMicroCount(); l_impactorMicro++)
        {
            for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_manager->getMicroCount(); l_impactedMicro++)
            {
                // Get the artificial impact
                coeff l_artImpact = m_manager->getImpactsManager()->getArtImpact(l_impactorMicro, l_impactedMicro);
                printf("%.2f    ", l_artImpact.toFloat());
            }
            printf("\n");
        }
        printf("\n");
    }
}

void learning::calculate()
{
    // Calculate the real impacts of all micros on all micros
    // Method :
    // For each micro, calculate the sum of the records
    // Then, calculate the ratio of the sum of the records of the micro
    // with the sum of the records of all the other micros
    // This will give us the real impacts of all micros on all the other micros

    // For each micro, calculate the sum of the records
    std::vector<uint64_t> l_sums(m_manager->getMicroCount(), 0);
    for (uint8_t l_microIndex = 0; l_microIndex < m_manager->getMicroCount(); l_microIndex++)
    {
        // Calculate the sum of the records
        uint64_t l_sum = 0;
        for (uint8_t l_recordIndex = 0; l_recordIndex < m_microRecords[l_microIndex].size(); l_recordIndex++)
        {
            l_sum += m_microRecords[l_microIndex][l_recordIndex];
        }
        l_sums[l_microIndex] = l_sum;
    }

    // Fill the matrix of impacts
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_manager->getMicroCount(); l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_manager->getMicroCount(); l_impactedMicro++)
        {
            // Calculate the ratio of the sum of the records of the impacted micro
            // with the sum of the records of impactor micro
            coeff l_impact;
            if (l_sums[l_impactorMicro] != 0)
            {
                // Calculate the real impact
                l_impact.setFloat((float)l_sums[l_impactedMicro] / (float)l_sums[l_impactorMicro]);
            }
            else
            {
                // The sum of the impactor micro is 0
                // It seems to be an error but let's admit that the impact is 0
                l_impact.m_value = 0;
            }
            // Set the real impact
            m_manager->getImpactsManager()->setRealImpact(l_impactorMicro, l_impactedMicro, l_impact);
        }
    }
}
