#include "learning.hpp"
#include "tools/logStream/logStream.hpp"
#include "modules/moduleManager.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"

#include <assert.h>

/**
 * @brief Maximum number of measures to record
 */
#define NB_MESURES_MAX 200

/**
 * @brief Time between each measure
 * @note Unit : ms
 */
#define TIME_BETWEEN_MEASURES 20

/**
 * @brief Threshold to consider that a micro has recorded something
 */
#define RECORD_LIMIT 10

/**
 * @brief Normalization value for the correction
 * @note The maximum of each micro on his learning must be normalized at this value
 */
#define CORRECTION_NORMALIZATION 180

/**
 * @brief Margin to add to the threshold
 */
#define THRESHOLD_MARGIN 0

/**
 * @brief Colors of the leds when a module is in learning or not
 */
#define LEARNING_COLOR RgbColor(255, 255, 255)
#define NOT_LEARNING_COLOR RgbColor(0, 0, 0)

int32_t Learning::m_MicroInRecord = -1;
periodicCallsMs *Learning::m_recordPeriodicCall;
std::vector<RecordSlot *> Learning::m_microRecordSlots;

void Learning::init()
{
    // Initialize the periodic call
    m_recordPeriodicCall = new periodicCallsMs(TIME_BETWEEN_MEASURES, recordCallback, NULL);
    // At start, no micro is in record
    m_recordPeriodicCall->enable(false);
}

bool Learning::isLearning()
{
    // Check if a learning process is running
    return m_recordPeriodicCall->isEnabled();
}

void Learning::startLearning()
{
    // Disable the addition of new modules
    // ModuleManager::enableNewModules(false);

    uint32_t l_nbMicros = 0; //ModuleManager::getImpactsManager()->getMicroCount();
    if (l_nbMicros > 0)
    {
        LogStream::cout << "Start learning" << LogStream::endl;
        // Start the learning process on the first micro
        // Add as much of micros as microsManager has in the vector of records
        for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
        {
            m_microRecordSlots.push_back(new RecordSlot(l_nbMicros));
        }
        // Start the learning on the first micro
        startLearning(0);
    }
    else
    {
        LogStream::cout << "No micros to learn" << LogStream::endl;
    }
}

void Learning::startLearning(int32_t microIndex)
{
    // Start the learning process on a specific micro
    assert(microIndex >= 0 && microIndex < m_microRecordSlots.size());
    m_MicroInRecord = microIndex;

    LogStream::cout << "Start learning on micro " << LogStream::endl;

    // Get the main micro of this learning
    Micro *l_mainMicro = 0; // ModuleManager::getImpactsManager()->getMicro(microIndex);

    // highlight the module that the micro is associated with if it has leds
    // and put black the others
    for(uint32_t l_moduleIndex = 0; l_moduleIndex < 0/*ModuleManager::getModuleCount()*/; l_moduleIndex++)
    {
        Module *l_module = 0;//ModuleManager::getModule(l_moduleIndex);
        if (l_module->getMicro() == l_mainMicro)
        {
            // The module is associated with the micro
            // Turn on the leds
            RgbLed *l_led = l_module->getRgbLed();
            if(l_led != nullptr)
            {
                l_led->setColor(COLOR_PRIORITY_LEARNING, LEARNING_COLOR);
            }
        }
        else
        {
            // The module is not associated with the micro
            // Turn off the leds
            RgbLed *l_led = l_module->getRgbLed();
            if(l_led != nullptr)
            {
                l_led->setColor(COLOR_PRIORITY_LEARNING, NOT_LEARNING_COLOR);
            }
        }
    }

    // Start the learning process
    m_recordPeriodicCall->enable(true);
}

void Learning::recordCallback(void* object)
{
    ((Learning*)object)->recordAllMic();
}

void Learning::recordAllMic()
{
    // Record all the micros only if at least one micro is record something
    bool l_recordSomething = false;
    ImpactsManager *l_impactsManager = nullptr; //ModuleManager::getImpactsManager();
    uint32_t l_nbMicros = 0; //l_impactsManager->getMicroCount();
    for(uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the micro
        Micro *l_micro = nullptr; //l_impactsManager->getMicro(l_microIndex);

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
        Record l_record(l_nbMicros);
        for(uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
        {

            // Get the micro
            Micro *l_micro = l_impactsManager->getMicro(l_microIndex);

            // Get the micro's value
            uint8_t l_value = l_micro->getMicroValue();

            // Add a record to the vector of records for each micro
            l_record.setValue(l_microIndex, l_value);

            LogStream::cout << l_value << "  ";
        }
        LogStream::cout << LogStream::endl;
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

void Learning::stopLearning()
{

    // If there are still micros to learn
    if (m_MicroInRecord < m_microRecordSlots.size() - 1)
    {
        // Start the learning process on the next micro
        startLearning(m_MicroInRecord + 1);
    }
    else
    {
        // Turn off all the leds
        for(uint32_t l_moduleIndex = 0; l_moduleIndex < 0/*ModuleManager::getModuleCount()*/; l_moduleIndex++)
        {
            Module *l_module = 0;//ModuleManager::getModule(l_moduleIndex);
            RgbLed *l_led = l_module->getRgbLed();
            if(l_led != nullptr)
            {
                l_led->releaseColor(COLOR_PRIORITY_LEARNING);
            }
        }
        // All the micros have been learned
        // Stop the learning process
        m_recordPeriodicCall->enable(false);
        LogStream::cout << LogStream::endl << "End of learning" << LogStream::endl;

        // Interpret the records
        calculateCorrection();      // Calculate the correction of each micro
        calculateRealImpacts();     // Calculate the real impacts of each micro
        // ModuleManager::getImpactsManager()->calculateArtImpacts();         // Calculate the artificial impacts
        calculateThreshold();        // Calculate the threshold of each micro

        // Print the results
        printResults();

        // Free the memory
        for (uint8_t l_microIndex = 0; l_microIndex < m_microRecordSlots.size(); l_microIndex++)
        {
            delete m_microRecordSlots[l_microIndex];
        }

        // Enable back the addition of new modules
        // ModuleManager::enableNewModules(true);
    }
}

void Learning::calculateCorrection()
{
    // Shearch the maximum of each micro on his learning for normalize the values
    // So that the maximum of this micro must be normalizet at CORRECTION_NORMALIZATION
    // CORRECTION_NORMALIZATION isn't 255 because we want to keep some margin


    ImpactsManager *l_impactsManager = 0; //ModuleManager::getImpactsManager();
    uint32_t l_nbMicros = 0; //l_impactsManager->getMicroCount();

    for (uint32_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the micro
        Micro *l_micro = l_impactsManager->getMicro(l_microIndex);

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
            LogStream::cout << "Error : The maximum of the micro " << l_microIndex << " is 0, seems to be impossible. Correction applied is 1.00" << LogStream::endl;
        }
        // Set the correction
        l_micro->setCorrection(l_correction);
    }
}

void Learning::calculateRealImpacts()
{
    // Calculate the real impact of each micro for all the records
    ImpactsManager *l_impactsManager = 0; //ModuleManager::getImpactsManager();
    uint32_t l_nbMicros = 0; //l_impactsManager->getMicroCount();
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Calculate the impacts of the micro on all the records
        m_microRecordSlots[l_microIndex]->calculateImpacts(l_impactsManager, l_microIndex);
    }
}

void Learning::calculateThreshold()
{
    // Calculate the threshold of each micro
    // Method :
    // Apply artificial impacts all records
    // Then shearch the maximum positive error
    // The threshold is the maximum positive error
    // First, reset all the thresholds to 0
    ImpactsManager *l_impactsManager = 0; //ModuleManager::getImpactsManager();
    uint32_t l_nbMicros = 0; //l_impactsManager->getMicroCount();
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the micro
        Micro *l_micro = l_impactsManager->getMicro(l_microIndex);

        // Reset the threshold
        l_micro->setThreshold(0);
    }
    for(uint32_t l_learningIndex = 0; l_learningIndex < l_nbMicros; l_learningIndex++)
    {
        for(uint32_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros; l_impactedMicro++)
        {
            if (l_impactedMicro != l_learningIndex)
            {
                // Get the micro
                Micro *l_micro = l_impactsManager->getMicro(l_impactedMicro);

                // Apply the artificial impact on each record
                for (uint32_t l_recordIndex = 0; l_recordIndex < m_microRecordSlots[l_learningIndex]->getSize(); l_recordIndex++)
                {
                    // Get the record
                    Record l_record = m_microRecordSlots[l_learningIndex]->getRecord(l_recordIndex);

                    // Get the value of the micro
                    int32_t l_value = l_record.getValue(l_impactedMicro);

                    // Apply the artificial impact
                    for (uint32_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros; l_impactorMicro++)
                    {
                        if (l_impactorMicro != l_impactedMicro)
                        {
                            // Get the artificial impact
                            float l_artImpact = l_impactsManager->getArtImpact(
                                    l_impactsManager->getMicro(l_impactorMicro),
                                    l_impactsManager->getMicro(l_impactedMicro));

                            // Get the value of the impactor micro
                            uint8_t l_impactorValue = l_record.getValue(l_impactorMicro);

                            // Apply the artificial impact
                            int32_t l_toSubstract = l_artImpact * l_impactorValue;
                            l_value -= l_toSubstract;
                        }
                    }

                    // Set the threshold if the value is greater
                    if (l_value + THRESHOLD_MARGIN > l_micro->getThreshold())
                    {
                        l_micro->setThreshold(l_value + THRESHOLD_MARGIN);
                    }
                }
            }
        }
    }
}

void Learning::printResults()
{
    // Display the corrections calculated
    ImpactsManager *l_impactsManager = 0; //ModuleManager::getImpactsManager();
    uint32_t l_nbMicros = 0; //l_impactsManager->getMicroCount();
    LogStream::cout << "\nCorrections :" << LogStream::endl;
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the correction
        float l_correction = l_impactsManager->getMicro(l_microIndex)->getCorrection();
        LogStream::cout << l_correction << "    ";
    }
    // Display the impacts calculated as 2 matrix (real and artificial)
    LogStream::cout << "\n\nReal impacts :" << LogStream::endl;
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros; l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros; l_impactedMicro++)
        {
            // Get the real impact
            float l_realImpact = l_impactsManager->getRealImpact(
                    l_impactsManager->getMicro(l_impactorMicro),
                    l_impactsManager->getMicro(l_impactedMicro));
            LogStream::cout << l_realImpact << "    ";
        }
        LogStream::cout << LogStream::endl;
    }
    LogStream::cout << "\nArtificial impacts :" << LogStream::endl;
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros; l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros; l_impactedMicro++)
        {
            // Get the artificial impact
            float l_artImpact = l_impactsManager->getArtImpact(
                    l_impactsManager->getMicro(l_impactorMicro),
                    l_impactsManager->getMicro(l_impactedMicro));
            LogStream::cout << l_artImpact << "    ";
        }
        LogStream::cout << "" << LogStream::endl;
    }
    LogStream::cout << "\nThresholds :" << LogStream::endl;
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the threshold
        uint8_t l_threshold = l_impactsManager->getMicro(l_microIndex)->getThreshold();
        LogStream::cout << l_threshold << "    ";
    }
    LogStream::cout << LogStream::endl;
}
