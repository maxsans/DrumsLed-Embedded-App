#include "learning.h"
#include "api/logs/logStream.h"
#include "modules/moduleManager.h"
#include "led/ledManager.h"
#include "tools/timeTools/periodicCallsMs.h"

#include <assert.h>

#define NB_MESURES_MAX 200
#define TIME_BETWEEN_MEASURES 20 // ms

#define RECORD_LIMIT 10

#define CORRECTION_NORMALIZATION 180

#define THRESHOLD_MARGIN 0

#define LEARNING_COLOR RgbColor(255, 255, 255)

learning::learning()
{
    // Constructor
    m_microManager = NULL;
    m_MicroInRecord = -1;
    m_recordPeriodicCall.setPeriod(TIME_BETWEEN_MEASURES);
    m_recordPeriodicCall.setCallback(recordCallback, this);
    m_recordPeriodicCall.enable(false); // At start, learning isn't started
}

learning::~learning()
{
    // Destructor
}

bool learning::isLearning()
{
    // Check if a learning process is running
    return m_recordPeriodicCall.isEnabled();
}

void learning::setSession(ModuleManager *moduleManager, MicroManager *microManager, LedManager *ledManager)
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
        LogStream() << "Start learning" << LogStream::endl;
        // Start the learning process on the first micro
        // Add as much of micros as microsManager has in the vector of records
        uint32_t l_nbMicros = m_microManager->getMicroCount();
        for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
        {
            m_microRecordSlots.push_back(new recordSlot(l_nbMicros));
        }
        // Start the learning on the first micro
        startLearning(0);
    }
    else
    {
        LogStream() << "No micros to learn" << LogStream::endl;
    }
}

void learning::startLearning(int32_t microIndex)
{
    // Start the learning process on a specific micro
    assert(microIndex >= 0 && microIndex < m_microRecordSlots.size());
    m_MicroInRecord = microIndex;

    LogStream() << "Start learning on micro " << LogStream::endl;

    // Get the main micro of this learning
    Micro *l_mainMicro = m_microManager->getMicro(microIndex);

    // highlight the module that the micro is associated with if it has leds
    // and put black the others
    for (uint16_t l_ledIndex = 0; l_ledIndex < m_ledManager->getLedCount(); l_ledIndex++)
    {
        RgbLed *l_led = m_ledManager->getLed(l_ledIndex);
        if (l_led->getModule() == l_mainMicro->getModule())
        {
            l_led->setColor(COLOR_PRIORITY_LEARNING, LEARNING_COLOR);
        }
        else if (l_led->getModule() != NULL) // Check if the module has leds
        {
            l_led->setColor(COLOR_PRIORITY_LEARNING, RgbColor(0, 0, 0));
        }
    }

    // Start the learning process
    m_recordPeriodicCall.enable(true);
}

void learning::recordCallback(void* object)
{
    ((learning*)object)->recordAllMic();
}

void learning::recordAllMic()
{
    // Record all the micros only if at least one micro is record something
    bool l_recordSomething = false;
    for(uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the micro
        Micro *l_micro = m_microManager->getMicro(l_microIndex);

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
        record l_record(m_microManager->getMicroCount());
        for(uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
        {

            // Get the micro
            Micro *l_micro = m_microManager->getMicro(l_microIndex);

            // Get the micro's value
            uint8_t l_value = l_micro->getMicroValue();

            // Add a record to the vector of records for each micro
            l_record.setValue(l_microIndex, l_value);

            LogStream() << l_value << "  ";
        }
        LogStream() << LogStream::endl;
        // Add the record to the record slot
        m_microRecordSlots[m_MicroInRecord]->addRecord(l_record);

        // Check if we have reached the maximum number of records
        if (m_microRecordSlots[m_MicroInRecord]->getSize() >= NB_MESURES_MAX)
        {
            // Stop the learning process
            stopLearning();
        }
    }
}

void learning::stopLearning()
{
    // Turn off all the leds
    for (uint16_t l_ledIndex = 0; l_ledIndex < m_ledManager->getLedCount(); l_ledIndex++)
    {
        m_ledManager->getLed(l_ledIndex)->releaseColor(COLOR_PRIORITY_LEARNING);
    }

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
        m_recordPeriodicCall.enable(false);
        LogStream() << LogStream::endl << "End of learning" << LogStream::endl;

        // Interpret the records
        calculateCorrection();
        calculateRealImpacts();
        m_microManager->getImpactsManager()->calculateArtImpacts();
        calculateThreshold();

        // Print the results
        printResults();

        // Enable all the process that have been disabled during the learning process

        // Free the memory
        for (uint8_t l_microIndex = 0; l_microIndex < m_microRecordSlots.size(); l_microIndex++)
        {
            delete m_microRecordSlots[l_microIndex];
        }

        // Enable back the addition of new modules
        m_moduleManager->enableNewModules(true);
    }
}

void learning::calculateCorrection()
{
    // Shearch the maximum of each micro on his learning for normalize the values
    // So that the maximum of this micro must be normalizet at CORRECTION_NORMALIZATION
    // CORRECTION_NORMALIZATION isn't 255 because we want to keep some margin

    for (uint32_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the micro
        Micro *l_micro = m_microManager->getMicro(l_microIndex);

        // Get the maximum of the micro
        uint8_t l_max = 0;
        for (uint8_t l_recordIndex = 0; l_recordIndex < m_microRecordSlots[l_microIndex]->getSize(); l_recordIndex++)
        {
            uint8_t l_newValue = m_microRecordSlots[l_microIndex]->getRecord(l_recordIndex).getValue(l_microIndex);
            if (l_newValue > l_max)
            {
                l_max = l_newValue;
            }
        }

        // Calculate the correction
        float l_correction;
        if (l_max != 0)
        {
            // Calculate the correction
            l_correction = (float)CORRECTION_NORMALIZATION / l_max;
        }
        else
        {
            // The maximum is 0
            // It seems to be an error but let's admit that the correction is 1
            l_correction = 1;
            LogStream() << "Error : The maximum of the micro " << l_microIndex << " is 0, seems to be impossible. Correction applied is 1.00" << LogStream::endl;
        }
        // Set the correction
        l_micro->setCorrection(l_correction);
    }
}

void learning::calculateRealImpacts()
{
    // Calculate the real impact of each micro for all the records
    for (uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Calculate the impacts of the micro on all the records
        m_microRecordSlots[l_microIndex]->calculateImpacts(m_microManager->getImpactsManager(), l_microIndex);
    }
}

void learning::calculateThreshold()
{
    // Calculate the threshold of each micro
    // Method :
    // Apply artificial impacts all records
    // Then shearch the maximum positive error
    // The threshold is the maximum positive error
    // First, reset all the thresholds to 0
    for (uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the micro
        Micro *l_micro = m_microManager->getMicro(l_microIndex);

        // Reset the threshold
        l_micro->setThreshold(0);
    }
    uint32_t l_nbMicros = m_microManager->getMicroCount();
    for(uint32_t l_learningIndex = 0; l_learningIndex < l_nbMicros; l_learningIndex++)
    {
        for(uint32_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros; l_impactedMicro++)
        {
            if (l_impactedMicro != l_learningIndex)
            {
                // Get the micro
                Micro *l_micro = m_microManager->getMicro(l_impactedMicro);

                // Apply the artificial impact on each record
                for (uint32_t l_recordIndex = 0; l_recordIndex < m_microRecordSlots[l_learningIndex]->getSize(); l_recordIndex++)
                {
                    // Get the record
                    record l_record = m_microRecordSlots[l_learningIndex]->getRecord(l_recordIndex);

                    // Get the value of the micro
                    int32_t l_value = l_record.getValue(l_impactedMicro);

                    // Apply the artificial impact
                    for (uint32_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros; l_impactorMicro++)
                    {
                        if (l_impactorMicro != l_impactedMicro)
                        {
                            // Get the artificial impact
                            float l_artImpact = m_microManager->getImpactsManager()->getArtImpact(l_impactorMicro, l_impactedMicro);

                            // Get the value of the impactor micro
                            uint8_t l_impactorValue = l_record.getValue(l_impactorMicro);

                            // Apply the artificial impact
                            int32_t l_toSubstract = l_artImpact * l_impactorValue;
                            l_value -= l_toSubstract;
                        }
                    }

                    // Set the threshold if the value is greater
                    if (l_value > l_micro->getThreshold())
                    {
                        l_micro->setThreshold(l_value + THRESHOLD_MARGIN);
                    }
                }
            }
        }
    }
}

void learning::printResults()
{
    // Display the corrections calculated
    LogStream() << "\nCorrections :" << LogStream::endl;
    for (uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the correction
        float l_correction = m_microManager->getMicro(l_microIndex)->getCorrection();
        LogStream() << l_correction << "    ";
    }
    // Display the impacts calculated as 2 matrix (real and artificial)
    LogStream() << "\n\nReal impacts :" << LogStream::endl;
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_microManager->getMicroCount(); l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_microManager->getMicroCount(); l_impactedMicro++)
        {
            // Get the real impact
            float l_realImpact = m_microManager->getImpactsManager()->getRealImpact(l_impactorMicro, l_impactedMicro);
            LogStream() << l_realImpact << "    ";
        }
        LogStream() << LogStream::endl;
    }
    LogStream() << "\nArtificial impacts :" << LogStream::endl;
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < m_microManager->getMicroCount(); l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < m_microManager->getMicroCount(); l_impactedMicro++)
        {
            // Get the artificial impact
            float l_artImpact = m_microManager->getImpactsManager()->getArtImpact(l_impactorMicro, l_impactedMicro);
            LogStream() << l_artImpact << "    ";
        }
        LogStream() << "" << LogStream::endl;
    }
    LogStream() << "\nThresholds :" << LogStream::endl;
    for (uint8_t l_microIndex = 0; l_microIndex < m_microManager->getMicroCount(); l_microIndex++)
    {
        // Get the threshold
        uint8_t l_threshold = m_microManager->getMicro(l_microIndex)->getThreshold();
        LogStream() << l_threshold << "    ";
    }
    LogStream() << LogStream::endl;
}
