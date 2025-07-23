#include "term.hpp"

#include "tools/logStream/logStream.hpp"

ApiTerminal *Term::m_nativeTerminal;
std::vector<TermCommand> Term::m_commands;
std::string Term::m_stringBuffer;

void Term::init()
{
    m_nativeTerminal = new ApiTerminal([](char c) { Term::onCharReceived(c); });

    // Register the default "help" command
    TermCommand helpCmd(
        TermAction([](const TermParameters&) {
            LogStream::cout << "Commandes disponibles :" << LogStream::endl;
            for (const auto& cmd : m_commands) {
                LogStream::cout << "  " << cmd.getKeyword() << " - " << cmd.getDescription() << LogStream::endl;
            }
        }),
        "help",
        "Affiche la liste des commandes disponibles."
    );
    registerCommand(helpCmd);

    // Register the default "example" command with parameters
    TermCommand exampleCmd(
        TermAction([](const TermParameters& params) {
            LogStream::cout << "Commande 'example' exécutée avec paramètres : ";
            for (const auto& param : params.getParameters()) {
                LogStream::cout << "'" <<param.getValue() << "' ";
            }
            LogStream::cout << LogStream::endl;
        }),
        "example",
        "Exemple de commande avec paramètres. Usage: example <param1> <param2> ..."
    );
    registerCommand(exampleCmd);
}

void Term::registerCommand(const TermCommand& command)
{
    // Check if the command already exists
    TermCommand* existingCommand = findCommand(command.getKeyword());
    if (existingCommand)
    {
        // If it exists, replace it
        *existingCommand = command;
        // Log a warning about the replacement
        LogStream::cout << "Warning: Command '" << command.getKeyword()
                        << "' already exists. Replacing with new definition." << LogStream::endl;
    }
    else
    {
        // Otherwise, add the new command to the list
        m_commands.push_back(command);
    }
}

void Term::unregisterCommand(const std::string& keyword)
{
    TermCommand* existingCommand = findCommand(keyword);
    if (existingCommand)
    {
        // Remove the command from the list
        m_commands.erase(std::remove(m_commands.begin(), m_commands.end(), *existingCommand),
                         m_commands.end());
        LogStream::cout << "Command '" << keyword << "' unregistered successfully." << LogStream::endl;
    }
    else
    {
        LogStream::cout << "Warning: Command '" << keyword << "' not found for unregistration." << LogStream::endl;
    }
}

void Term::onCharReceived(char c)
{
    // Affiche le caractère tapé (sauf retour arrière et retour à la ligne)
    if (c == '\r' || c == '\n')
    {
        // Affiche un retour à la ligne
        LogStream::cout << LogStream::endl;
        // ...traitement de la commande...
        onEndOfLineReceived();
    }
    else if (c == 127 || c == '\b')
    {
        // Efface le dernier caractère à l'écran si le buffer n'est pas vide
        if (!m_stringBuffer.empty())
        {
            // Efface visuellement le caractère
            LogStream::cout << "\b \b";
            m_stringBuffer.pop_back();
        }
    }
    else if (c == '\t')
    {
        // TAB key for self-completion
        autoCompleteBuffer();
    }
    else
    {
        // Affiche le caractère et ajoute au buffer
        LogStream::cout << c;
        // Ajoute le caractère au buffer
        m_stringBuffer += c;
    }
}

void Term::onEndOfLineReceived()
{
    // Get the keyword from the string buffer
    // The keyword is separated by a space with the first parameter
    // Or by a ";"
    // Or by any separator if any parameter is present
    std::string keyword;
    size_t spacePos = m_stringBuffer.find_first_of(" ;");
    if (spacePos != std::string::npos)
    {
        keyword = m_stringBuffer.substr(0, spacePos);
    }
    else
    {
        keyword = m_stringBuffer; // No space found, use the whole buffer as keyword
    }

    // Find the command by its keyword
    TermCommand* command = findCommand(keyword);
    if (command)
    {
        // Get the parameters from the string buffer
        std::string parameters;
        if (spacePos != std::string::npos)
        {
            parameters = m_stringBuffer.substr(spacePos + 1);
        }
        TermParameters params(parameters);
        // Execute the command with the parameters
        command->execute(params);
    }
    else
    {
        // If the command is not found, log an error
        LogStream::cout << "Error: Command '" << keyword << "' not found." << LogStream::endl;
    }

    // Reset the string buffer after processing the command
    m_stringBuffer.clear();
}

void Term::autoCompleteBuffer()
{
    // We only complete the keyword (before space or separator)
    size_t spacePos = m_stringBuffer.find_first_of(" ;");
    std::string partial = (spacePos == std::string::npos)
        ? m_stringBuffer
        : m_stringBuffer.substr(0, spacePos);

    // Looking for all the orders that start with the prefix
    std::vector<std::string> matches;
    for (const auto& cmd : m_commands)
    {
        if (cmd.getKeyword().find(partial) == 0)
            matches.push_back(cmd.getKeyword());
    }

    if (matches.empty())
    {
        // Nothing to complete
        return;
    }
    else if (matches.size() == 1)
    {
        // Unique completion: replaces the keyword in the buffer
        std::string completion = matches[0];
        if (completion.length() > partial.length())
        {
            // Erases the current text on the screen
            for (size_t i = 0; i < partial.length(); ++i)
                LogStream::cout << "\b \b";
            // Displays the completion
            LogStream::cout << completion;
            // Updates the buffer
            if (spacePos == std::string::npos)
                m_stringBuffer = completion;
            else
                m_stringBuffer.replace(0, spacePos, completion);
        }
    }
    else
    {
        // Several possibilities: displays the list
        LogStream::cout << LogStream::endl;
        for (const auto& match : matches)
            LogStream::cout << match << "  ";
        LogStream::cout << LogStream::endl;
        // Restructs the current buffer
        LogStream::cout << m_stringBuffer;
    }
}

TermCommand* Term::findCommand(const std::string& keyword)
{
    for (auto& command : m_commands)
    {
        if (command.getKeyword() == keyword)
        {
            return &command; // Return a pointer to the found command
        }
    }
    return nullptr; // Command not found
}
