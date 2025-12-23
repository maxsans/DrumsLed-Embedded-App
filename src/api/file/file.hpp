/**
 * @file file.hpp
 * @brief File API for managing files.
 * @author Cyprien Ménard
 * @date 2025-08-19
 */

#ifndef __FILE_HPP__
#define __FILE_HPP__

#include <cstdint>
#include <string>

/**
 * @brief File access modes
 */
enum class FileMode
{
    READ,
    WRITE,
    APPEND,
    READ_WRITE
};

/**
 * @brief File seek positions
 */
enum class SeekOrigin
{
    BEGIN,
    CURRENT,
    END
};

/**
 * @brief Cross-platform file management class
 */
class File
{
    private:
    void *
        m_handle; // Platform-specific handle (HANDLE on Windows, int* on Linux)
    std::string m_filepath;
    bool m_isOpen;

    public:
    /**
     * @brief Constructor
     */
    File();

    /**
     * @brief Constructor with filepath
     * @param filepath Path to the file
     */
    File(const std::string &filepath);

    /**
     * @brief Destructor
     */
    ~File();

    /**
     * @brief Open a file
     * @param filepath Path to the file
     * @param mode File access mode
     * @return true if successful, false otherwise
     */
    bool open(const std::string &filepath, FileMode mode);

    /**
     * @brief Close the file
     * @return true if successful, false otherwise
     */
    bool close();

    /**
     * @brief Read data from file
     * @param buffer Buffer to store read data
     * @param size Number of bytes to read
     * @return Number of bytes actually read, -1 on error
     */
    int64_t read(void *buffer, size_t size);

    /**
     * @brief Write data to file
     * @param buffer Data to write
     * @param size Number of bytes to write
     * @return Number of bytes actually written, -1 on error
     */
    int64_t write(const void *buffer, size_t size);

    /**
     * @brief Seek to a position in the file
     * @param offset Offset from origin
     * @param origin Seek origin
     * @return New file position, -1 on error
     */
    int64_t seek(int64_t offset, SeekOrigin origin);

    /**
     * @brief Get current file position
     * @return Current position, -1 on error
     */
    int64_t tell();

    /**
     * @brief Get file size
     * @return File size in bytes, -1 on error
     */
    int64_t size();

    /**
     * @brief Check if file is open
     * @return true if open, false otherwise
     */
    bool isOpen() const;

    /**
     * @brief Calculate CRC32 checksum of the file
     * @param bufferSize Buffer size for reading chunks (default: 64KB)
     * @return CRC32 checksum, 0 on error
     */
    uint32_t crc32(size_t bufferSize = 65536);

    /**
     * @brief Check if file exists
     * @param filepath Path to check
     * @return true if exists, false otherwise
     */
    static bool exists(const std::string &filepath);

    /**
     * @brief Delete a file
     * @param filepath Path to delete
     * @return true if successful, false otherwise
     */
    static bool remove(const std::string &filepath);

    /**
     * @brief Create a directory
     * @param dirpath Directory path to create
     * @return true if successful, false otherwise
     */
    static bool createDirectory(const std::string &dirpath);

    /**
     * @brief Get the directory path of the current executable
     * @return Path to the directory containing the executable, empty string on error
     */
    static std::string getExecutableDirectory();

    /**
     * @brief Resolve a relative path to be relative to the executable directory
     * @param relativePath Path to resolve (can be relative or absolute)
     * @return Absolute path resolved relative to executable directory
     */
    static std::string resolvePath(const std::string &relativePath);
};

#endif // __FILE_HPP__
