#include "learning.hpp"
#include "modules/moduleManager.hpp"
#include "tools/logStream/logStream.hpp"
#include "tools/timeTools/periodicCallsMs.hpp"

#include <assert.h>

const timeMs Learning::m_timeBetweenMeasures = 20;
const RgbColor Learning::m_learningColor = RgbColor(255, 255, 255);
const RgbColor Learning::m_notLearningColor = RgbColor(0, 0, 0);

Learning::Learning(ModuleManager *moduleManager,
                   void (*learningDoneCallback)(void *object),
                   void *obj)
    : m_moduleManager(moduleManager),
      m_recordPeriodicCall(m_timeBetweenMeasures, recordCallback, this),
      m_learningDoneCallback(learningDoneCallback),
      m_learningDoneCallbackObject(obj)
{
    // Initialize the micro record slots
    m_microRecordSlots.clear();
    m_MicroInRecord = -1;
    // At start, no micro is in record
    m_recordPeriodicCall.enable(false);
    // Add all the micros of the module manager to the impacts manager
    uint32_t l_nbModule = m_moduleManager->getModuleCount();
    for (uint32_t l_moduleIndex = 0; l_moduleIndex < l_nbModule;
         l_moduleIndex++)
    {
        Module *l_module = m_moduleManager->getModule(l_moduleIndex);
        if (l_module != nullptr)
        {
            Micro *l_micro = l_module->getMicro();
            if (l_micro != nullptr)
            {
                m_moduleManager->getImpactsManager()->addMicro(l_micro);
            }
        }
    }
}

bool Learning::isLearning()
{
    // Check if a learning process is running
    return m_recordPeriodicCall.isEnabled();
}

void Learning::startLearning()
{
    // Check if the module manager is enabled
    if (!m_moduleManager->isActive())
    {
        LogStream::cout
            << "Module manager is not enabled, cannot start learning"
            << LogStream::endl;
        if (m_learningDoneCallback != nullptr)
        {
            m_learningDoneCallback(m_learningDoneCallbackObject);
        }
        return;
    }

    uint32_t l_nbMicros = m_moduleManager->getImpactsManager()->getMicroCount();
    if (l_nbMicros > 0)
    {
        LogStream::cout << "Start learning" << LogStream::endl;
        // Disable the addition of new modules by disabling the module manager
        m_moduleManager->enable(false);
        // Start the learning process on the first micro
        // Add as much of micros as microsManager has in the vector of records
        for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros;
             l_microIndex++)
        {
            m_microRecordSlots.push_back(new RecordSlot(l_nbMicros));
        }
        // Start the learning on the first micro
        startLearning(0);
    }
    else
    {
        LogStream::cout << "No micros to learn" << LogStream::endl;
        // Call the learning done callback if it is set
        if (m_learningDoneCallback != nullptr)
        {
            m_learningDoneCallback(m_learningDoneCallbackObject);
        }
    }
}

void Learning::startLearning(int32_t microIndex)
{
    // Start the learning process on a specific micro
    assert(microIndex >= 0 && microIndex < m_microRecordSlots.size());
    m_MicroInRecord = microIndex;

    LogStream::cout << "Start learning on micro " << LogStream::endl;

    // Get the main micro of this learning
    Micro *l_mainMicro
        = m_moduleManager->getImpactsManager()->getMicro(microIndex);

    // highlight the module that the micro is associated with if it has leds
    // and put black the others
    for (uint32_t l_moduleIndex = 0;
         l_moduleIndex < m_moduleManager->getModuleCount();
         l_moduleIndex++)
    {
        Module *l_module = m_moduleManager->getModule(l_moduleIndex);
        if (l_module->getMicro() == l_mainMicro)
        {
            // The module is associated with the micro
            // Turn on the leds
            RgbLed *l_led = l_module->getRgbLed();
            if (l_led != nullptr)
            {
                l_led->setColor(COLOR_PRIORITY_LEARNING, m_learningColor);
            }
        }
        else
        {
            // The module is not associated with the micro
            // Turn off the leds
            RgbLed *l_led = l_module->getRgbLed();
            if (l_led != nullptr)
            {
                l_led->setColor(COLOR_PRIORITY_LEARNING, m_notLearningColor);
            }
        }
    }

    // Start the learning process
    m_recordPeriodicCall.enable(true);
}

void Learning::recordCallback(void *object)
{
    ((Learning *)object)->recordAllMic();
}

void Learning::recordAllMic()
{
    // Record all the micros only if at least one micro is record something
    bool l_recordSomething = false;
    uint32_t l_nbMicros = m_moduleManager->getImpactsManager()->getMicroCount();
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the micro
        Micro *l_micro
            = m_moduleManager->getImpactsManager()->getMicro(l_microIndex);

        // Get the micro's value
        uint8_t l_value = l_micro->getMicroValue();

        if (l_value > m_recordLimit)
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
        for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros;
             l_microIndex++)
        {

            // Get the micro
            Micro *l_micro
                = m_moduleManager->getImpactsManager()->getMicro(l_microIndex);

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
        if (m_microRecordSlots[m_MicroInRecord]->getSize() >= m_nbMesuresMax)
        {
            // Stop the learning process
            stopLearning();
        }
    }
}

bool Learning::stopLearning()
{
    // Check if a learning process is running
    if (!isLearning())
    {
        LogStream::cout << "No learning process is running" << LogStream::endl;
        // Call the learning done callback if it is set
        if (m_learningDoneCallback != nullptr)
        {
            m_learningDoneCallback(m_learningDoneCallbackObject);
        }
        return true;
    }
    // If there are still micros to learn
    if (m_MicroInRecord < m_microRecordSlots.size() - 1)
    {
        // Start the learning process on the next micro
        startLearning(m_MicroInRecord + 1);
        return false;
    }
    else
    {
        // Turn off all the leds
        for (uint32_t l_moduleIndex = 0;
             l_moduleIndex < m_moduleManager->getModuleCount();
             l_moduleIndex++)
        {
            Module *l_module = m_moduleManager->getModule(l_moduleIndex);
            RgbLed *l_led = l_module->getRgbLed();
            if (l_led != nullptr)
            {
                l_led->releaseColor(COLOR_PRIORITY_LEARNING);
            }
        }
        // All the micros have been learned
        // Stop the learning process
        m_recordPeriodicCall.enable(false);
        LogStream::cout << LogStream::endl
                        << "End of learning" << LogStream::endl;

        // Interpret the records
        calculateCorrection();  // Calculate the correction of each micro
        calculateRealImpacts(); // Calculate the real impacts of each micro
        m_moduleManager->getImpactsManager()
            ->calculateArtImpacts(); // Calculate the artificial impacts
        calculateThreshold();        // Calculate the threshold of each micro

        // Print the results
        printResults();

        // Free the memory
        for (uint8_t l_microIndex = 0; l_microIndex < m_microRecordSlots.size();
             l_microIndex++)
        {
            delete m_microRecordSlots[l_microIndex];
        }

        // Enable back the addition of new modules
        m_moduleManager->enable(true);

        // Call the learning done callback if it is set
        if (m_learningDoneCallback != nullptr)
        {
            m_learningDoneCallback(m_learningDoneCallbackObject);
        }
        return true;
    }
}

void Learning::calculateCorrection()
{
    // Shearch the maximum of each micro on his learning for normalize the values
    // So that the maximum of this micro must be normalizet at CORRECTION_NORMALIZATION
    // CORRECTION_NORMALIZATION isn't 255 because we want to keep some margin

    uint32_t l_nbMicros = m_moduleManager->getImpactsManager()->getMicroCount();

    for (uint32_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the micro
        Micro *l_micro
            = m_moduleManager->getImpactsManager()->getMicro(l_microIndex);

        // Get the maximum of the micro
        uint8_t l_max = 0;
        for (uint8_t l_recordIndex = 0;
             l_recordIndex < m_microRecordSlots[l_microIndex]->getSize();
             l_recordIndex++)
        {
            uint8_t l_newValue = m_microRecordSlots[l_microIndex]
                                     ->getRecord(l_recordIndex)
                                     .getValue(l_microIndex);
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
            l_correction = (float)m_correctionNormalization / l_max;
        }
        else
        {
            // The maximum is 0
            // It seems to be an error but let's admit that the correction is 1
            l_correction = 1;
            LogStream::cout
                << "Error : The maximum of the micro " << l_microIndex
                << " is 0, seems to be impossible. Correction applied is 1.00"
                << LogStream::endl;
        }
        // Set the correction
        l_micro->setCorrection(l_correction);
    }
}

void Learning::calculateRealImpacts()
{
    // Calculate the real impact of each micro for all the records
    uint32_t l_nbMicros = m_moduleManager->getImpactsManager()->getMicroCount();
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Calculate the impacts of the micro on all the records
        m_microRecordSlots[l_microIndex]->calculateImpacts(
            m_moduleManager->getImpactsManager(), l_microIndex);
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
    uint32_t l_nbMicros = m_moduleManager->getImpactsManager()->getMicroCount();
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the micro
        Micro *l_micro
            = m_moduleManager->getImpactsManager()->getMicro(l_microIndex);

        // Reset the threshold
        l_micro->setThreshold(0);
    }
    for (uint32_t l_learningIndex = 0; l_learningIndex < l_nbMicros;
         l_learningIndex++)
    {
        for (uint32_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros;
             l_impactedMicro++)
        {
            if (l_impactedMicro != l_learningIndex)
            {
                // Get the micro
                Micro *l_micro = m_moduleManager->getImpactsManager()->getMicro(
                    l_impactedMicro);

                // Apply the artificial impact on each record
                for (uint32_t l_recordIndex = 0;
                     l_recordIndex
                     < m_microRecordSlots[l_learningIndex]->getSize();
                     l_recordIndex++)
                {
                    // Get the record
                    Record l_record
                        = m_microRecordSlots[l_learningIndex]->getRecord(
                            l_recordIndex);

                    // Get the value of the micro
                    int32_t l_value = l_record.getValue(l_impactedMicro);

                    // Apply the artificial impact
                    for (uint32_t l_impactorMicro = 0;
                         l_impactorMicro < l_nbMicros;
                         l_impactorMicro++)
                    {
                        if (l_impactorMicro != l_impactedMicro)
                        {
                            // Get the artificial impact
                            float l_artImpact
                                = m_moduleManager->getImpactsManager()
                                      ->getArtImpact(
                                          m_moduleManager->getImpactsManager()
                                              ->getMicro(l_impactorMicro),
                                          m_moduleManager->getImpactsManager()
                                              ->getMicro(l_impactedMicro));

                            // Get the value of the impactor micro
                            uint8_t l_impactorValue
                                = l_record.getValue(l_impactorMicro);

                            // Apply the artificial impact
                            int32_t l_toSubstract
                                = l_artImpact * l_impactorValue;
                            l_value -= l_toSubstract;
                        }
                    }

                    // Set the threshold if the value is greater
                    if (l_value + m_thresholdMargin > l_micro->getThreshold())
                    {
                        l_micro->setThreshold(l_value + m_thresholdMargin);
                    }
                }
            }
        }
    }
}

void Learning::printResults()
{
    // Display the corrections calculated
    uint32_t l_nbMicros = m_moduleManager->getImpactsManager()->getMicroCount();
    LogStream::cout << "\nCorrections :" << LogStream::endl;
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the correction
        float l_correction = m_moduleManager->getImpactsManager()
                                 ->getMicro(l_microIndex)
                                 ->getCorrection();
        LogStream::cout << l_correction << "    ";
    }
    // Display the impacts calculated as 2 matrix (real and artificial)
    LogStream::cout << "\n\nReal impacts :" << LogStream::endl;
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros;
         l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros;
             l_impactedMicro++)
        {
            // Get the real impact
            float l_realImpact
                = m_moduleManager->getImpactsManager()->getRealImpact(
                    m_moduleManager->getImpactsManager()->getMicro(
                        l_impactorMicro),
                    m_moduleManager->getImpactsManager()->getMicro(
                        l_impactedMicro));
            LogStream::cout << l_realImpact << "    ";
        }
        LogStream::cout << LogStream::endl;
    }
    LogStream::cout << "\nArtificial impacts :" << LogStream::endl;
    for (uint8_t l_impactorMicro = 0; l_impactorMicro < l_nbMicros;
         l_impactorMicro++)
    {
        for (uint8_t l_impactedMicro = 0; l_impactedMicro < l_nbMicros;
             l_impactedMicro++)
        {
            // Get the artificial impact
            float l_artImpact
                = m_moduleManager->getImpactsManager()->getArtImpact(
                    m_moduleManager->getImpactsManager()->getMicro(
                        l_impactorMicro),
                    m_moduleManager->getImpactsManager()->getMicro(
                        l_impactedMicro));
            LogStream::cout << l_artImpact << "    ";
        }
        LogStream::cout << "" << LogStream::endl;
    }
    LogStream::cout << "\nThresholds :" << LogStream::endl;
    for (uint8_t l_microIndex = 0; l_microIndex < l_nbMicros; l_microIndex++)
    {
        // Get the threshold
        uint8_t l_threshold = m_moduleManager->getImpactsManager()
                                  ->getMicro(l_microIndex)
                                  ->getThreshold();
        LogStream::cout << l_threshold << "    ";
    }
    LogStream::cout << LogStream::endl;
}
