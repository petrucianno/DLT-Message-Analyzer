#ifndef CCONSOLEVIEW_HPP
#define CCONSOLEVIEW_HPP

#include <QMap>

#include "QPlainTextEdit"
#include "QTabWidget"

namespace NDLTMessageAnalyzer
{
    namespace NConsole
    {
        enum class eMessageType
        {
            eMsg = 0,
            eWrn,
            eErr
        };

        struct tConsoleConfig
        {
            // size of log ring buffer, which is represente to the user. Measured in number of dedicated messages.
            unsigned int logSize = 1000;
            // Maximum size of single message. Measures in number of UTF-8 characters.
            unsigned int maxMsgSize = 1024;
            // consoleConfig - tab widget, which contains all tabs, including our console
            QTabWidget* pTabWidget = nullptr;
            // pConsoleTab - widget, which is a tab within pTabWidget
            QWidget* pConsoleTab = nullptr;
            // pConsoleTextEdit - the line edit, which is part of the pConsoleTab
            QPlainTextEdit* pConsoleTextEdit = nullptr;
        };

        /**
         * @brief The CConsoleCtrl class - this purely statis class is used to send messages from any Qt-based thread to a GUI thread and place them into provided
         * text edit.
         * Used to implement debug console within the DLTMessageAnalyzer plugin.
         */
        class CConsoleCtrl : public QObject
        {

            Q_OBJECT

        public:

            /**
             * @brief createInstance - creates singletone instance
             * In case if it already exists - will do nothing
             * Method is thread-safe.
             * Still it should be called ONLY!!! from the same QT-based thread, in which console config widgets are located.
             * @param consoleConfig - console configuration, which is used to create a signletone instance of console controller.
             */
            static void createInstance( const tConsoleConfig& consoleConfig );

            /**
             * @brief destroyInstance - destroys singletone instance, if it exists.
             * Otherwise does nothing.
             * Method is NOT thread-safe.
             * Should be called from the same thread, from which "createInstance" was previously called.
             */
            static void destroyInstance();

            /**
             * @brief isExist - tells, whether created instance exists
             * Method is thread-safe.
             * @return - true, if instance exist. False otherwise.
             */
            static bool isExist();

            /**
             * @brief sendMessage - sends a message to console, if created instance exists.
             * Otherwise does nothing.
             * Method is thread-safe.
             * @param message - message to be added to the log
             * @param messageType - message type to be considered.
             */
            static void sendMessage( const QString& message, eMessageType messageType = eMessageType::eMsg );

private slots:
            /**
             * @brief addMessage - when event form the client reaches thread, in which signletone instance is located -
             * sends a message to the corresponding text edit.
             * Depending on type of the message, highlights
             * @param message - string of the message, to be added to console
             * @param messageType - type of the message, to be applied for this message
             */
            void addMessage( const QString& message, eMessageType messageType = eMessageType::eMsg );

        private: // methods
            /**
             * @brief CConsoleCtrl - constructor
             * @param consoleConfig - console configuration, which is used to create a signletone instance of console controller.
             */
            CConsoleCtrl( const tConsoleConfig& consoleConfig );

        private: // fields
            tConsoleConfig mConsoleConfig;
            typedef QMap<eMessageType, unsigned int> tMessageCounters;
            tMessageCounters mMessageCounters;
            eMessageType mCountedMessageType;
        };
    }
}

/**
 * Sends debug message to console. Takes string as an argument.
 */
#define SEND_MSG(STRING) NDLTMessageAnalyzer::NConsole::CConsoleCtrl::sendMessage(STRING, NDLTMessageAnalyzer::NConsole::eMessageType::eMsg)

/**
 * Sends warning message to console. Takes string as an argument.
 */
#define SEND_WRN(STRING) NDLTMessageAnalyzer::NConsole::CConsoleCtrl::sendMessage(STRING, NDLTMessageAnalyzer::NConsole::eMessageType::eWrn)

/**
 * Sends error message to console. Takes string as an argument.
 */
#define SEND_ERR(STRING) NDLTMessageAnalyzer::NConsole::CConsoleCtrl::sendMessage(STRING, NDLTMessageAnalyzer::NConsole::eMessageType::eErr)

#endif // CCONSOLEVIEW_HPP
