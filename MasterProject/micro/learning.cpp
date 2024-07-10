#include "learning.h"
#include "moduleManager.h"
#include "ledManager.h"

#include <winsock2.h>
#include <stdio.h>
#include <assert.h>

#define NB_MESURES_MAX 200
#define TIME_BETWEEN_MEASURES 20 // ms

#define RECORD_LIMIT 10

#define CORRECTION_NORMALIZATION 100

learning::learning()
{
    // Constructor
    m_microManager = NULL;
    m_inLearning = false;
    m_MicroInRecord = -1;
}

learning::~learning()
{
    // Destructor
}

void learning::setSession(moduleManager *moduleManager, microManager *microManager, ledManager *ledManager)
{
    // Set the session to learn
    m_moduleManager = moduleManager;
    m_microManager = microManager;
    m_ledManager = ledManager;
}

void learning::startLearning()
{
    // Disable the addition of new modules
    m_moduleManager->enableNewModules(false);
    if (m_microManager->getMicroCount() > 0)
    {
        printf("Start learning\n");
        // Start the learning process on the first micro
        // Add as much of micros as microsManager has in the vector of records
        uint32_t l_lastVectorSize = m_microRecords.size();
        uint32_t l_newVectorSize = m_microManager->getMicroCount();
        if (l_lastVectorSize < l_newVectorSize)
        {
            for (uint8_t i = l_lastVectorSize; i < l_newVectorSize; i++)
            {
                m_microRecords.push_back(std::vector<uint8_t>());
            }
        }
        else if (l_lastVectorSize > l_newVectorSize)
        {
            for (uint8_t i = l_newVectorSize; i < l_lastVectorSize; i++)
            {
                m_microRecords.pop_back();
            }
        }
        startLearning(0);
    }
    else
    {
        printf("No micros to learn\n");
    }
}

void learning::startLearning(int32_t microIndex)
{
    // Start the learning process on a specific micro
    assert(microIndex >= 0 && microIndex < m_microRecords.size());
    m_MicroInRecord = microIndex;

    // If necessary, delete the previous records and free the memory
    for (uint8_t l_microIndex = 0; l_microIndex < m_microRecords.size(); l_microIndex++)
    {
        m_microRecords[l_microIndex].clear();
    }

    printf("Start learning on micro %d\n", microIndex);

    // First, disable all of process that control the leds
    // TODO: Implement this function

    // Get the main micro of this learning
    micro *l_mainMicro = m_microManager->getMicro(microIndex);

    // highlight the module that the micro is associated with if it has leds
    rgbLed *l_led = m_ledManager->getLed(l_mainMicro->getModule());
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
    for(uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the micro
        micro *l_micro = m_microManager->getMicro(l_microIndex);

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
        for(uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
        {
            // Get the micro
            micro *l_micro = m_microManager->getMicro(l_microIndex);

            // Get the micro's value
            uint8_t l_value = l_micro->getMicroValue();

            // Add a record to the vector of records for each micro
            m_microRecords[l_microIndex].push_back(l_value);

            printf("%d  ",l_value);

            // Check if we have reached the maximum number of records
            if (m_microRecords[l_microIndex].size() >= NB_MESURES_MAX)
            {
                // Stop the learning process
                stopLearning();
                break;
            }
        }
        printf("\n");
    }
}

void learning::stopLearning()
{
    // Turn off all the leds
    for (uint16_t l_ledIndex = 0; l_ledIndex < m_ledManager->getLedCount(); l_ledIndex++)
    {
        m_ledManager->getLed(l_ledIndex)->setColor(0, 0, 0);
    }

    // Interpret the records
    calculateCorrection();
    calculateRealImpacts();

    // If there are still micros to learn
    if (m_MicroInRecord < m_microManager->getMicroCount() - 1)
    {
        // Start the learning process on the next micro
        startLearning(m_MicroInRecord + 1);
    }
    else
    {
        // All the micros have been learned
        // Stop the learning process
        m_inLearning = false;
        printf("\nEnd of learning\n");

        // Calculate the artificial impacts
        m_microManager->getImpactsManager()->calculateArtImpacts();

        // Print the results
        printResults();

        // Enable back the addition of new modules
        m_moduleManager->enableNewModules(true);
    }
}

void learning::calculateCorrection()
{
    // Shearch the maximum of each micro for normalize the values
    // So that the maximum of each micro is CORRECTION_NORMALIZATION
    // CORRECTION_NORMALIZATION isn't 255 because we want to keep some margin
    for (uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the micro
        micro *l_micro = m_microManager->getMicro(l_microIndex);

        // Get the maximum of the micro
        uint8_t l_max = 0;
        for (uint8_t l_recordIndex = 0; l_recordIndex < m_microRecords[l_microIndex].size(); l_recordIndex++)
        {
            if (m_microRecords[l_microIndex][l_recordIndex] > l_max)
            {
                l_max = m_microRecords[l_microIndex][l_recordIndex];
            }
        }

        // Calculate the correction
        coeff l_correction;
        if (l_max != 0)
        {
            // Calculate the correction
            l_correction.m_value = QUANTUM_COEFF * CORRECTION_NORMALIZATION / l_max;
        }
        else
        {
            // The maximum is 0
            // It seems to be an error but let's admit that the correction is 1
            l_correction.m_value = QUANTUM_COEFF;
            printf("Error : The maximum of the micro %d is 0, seems to be impossible. Correction applied is 1.00\n", l_microIndex);
        }
        // Set the correction
        l_micro->setCorrection(l_correction);
    }
}

void learning::calculateRealImpacts()
{
    // Calculate the real impacts of all micros on all micros
    // Method :
    // For each micro, calculate the sum of the records
    // Then, calculate the ratio of the sum of the records of the micro
    // with the sum of the records of all the other micros
    // This will give us the real impacts of all micros on all the other micros

    // For each micro, calculate the sum of the records
    std::vector<uint64_t> l_sums(m_microManager->getMicroCount(), 0);
    for (uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
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
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_microManager->getMicroCount(); l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_microManager->getMicroCount(); l_impactedMicro++)
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
            m_microManager->getImpactsManager()->setRealImpact(l_impactorMicro, l_impactedMicro, l_impact);
        }
    }
}

void learning::printResults()
{
    // Display the corrections calculated
    printf("\nCorrections :\n");
    for (uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the correction
        coeff l_correction = m_microManager->getMicro(l_microIndex)->getCorrection();
        printf("%.2f    ", l_correction.toFloat());
    }
    // Display the impacts calculated as 2 matrix (real and artificial)
    printf("\n\nReal impacts :\n");
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_microManager->getMicroCount(); l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_microManager->getMicroCount(); l_impactedMicro++)
        {
            // Get the real impact
            coeff l_realImpact = m_microManager->getImpactsManager()->getRealImpact(l_impactorMicro, l_impactedMicro);
            printf("%.2f    ", l_realImpact.toFloat());
        }
        printf("\n");
    }
    printf("\nArtificial impacts :\n");
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_microManager->getMicroCount(); l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_microManager->getMicroCount(); l_impactedMicro++)
        {
            // Get the artificial impact
            coeff l_artImpact = m_microManager->getImpactsManager()->getArtImpact(l_impactorMicro, l_impactedMicro);
            printf("%.2f    ", l_artImpact.toFloat());
        }
        printf("\n");
    }
    printf("\n");
}
