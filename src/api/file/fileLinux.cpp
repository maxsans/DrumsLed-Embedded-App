/**
 * @file fileLinux.cpp
 * @author Cyprien Ménard
 * @date 2025-08-19
 */

#include "file.hpp"
#include "tools/containers/binary/crc32.hpp"
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

File::File() : m_handle(nullptr), m_isOpen(false)
{
}

File::File(const std::string &filepath)
    : m_handle(nullptr), m_filepath(filepath), m_isOpen(false)
{
}

File::~File()
{
    if (m_isOpen)
    {
        close();
    }
}

bool File::open(const std::string &filepath, FileMode mode)
{
    if (m_isOpen)
    {
        close();
    }

    m_filepath = filepath;
    int flags = 0;

    switch (mode)
    {
        case FileMode::READ:
            flags = O_RDONLY;
            break;
        case FileMode::WRITE:
            flags = O_WRONLY | O_CREAT | O_TRUNC;
            break;
        case FileMode::APPEND:
            flags = O_WRONLY | O_CREAT | O_APPEND;
            break;
        case FileMode::READ_WRITE:
            flags = O_RDWR | O_CREAT;
            break;
    }

    int fd = ::open(filepath.c_str(), flags, 0644);
    if (fd != -1)
    {
        m_handle = new int(fd);
        m_isOpen = true;
    }
    else
    {
        m_handle = nullptr;
        m_isOpen = false;
    }
    return m_isOpen;
}

bool File::close()
{
    if (!m_isOpen || !m_handle)
    {
        return true;
    }

    int fd = *static_cast<int *>(m_handle);
    int result = ::close(fd);
    if (result == 0)
    {
        delete static_cast<int *>(m_handle);
        m_handle = nullptr;
        m_isOpen = false;
        return true;
    }
    return false;
}

int64_t File::read(void *buffer, size_t size)
{
    if (!m_isOpen || !m_handle)
    {
        return -1;
    }
    int fd = *static_cast<int *>(m_handle);
    return ::read(fd, buffer, size);
}

int64_t File::write(const void *buffer, size_t size)
{
    if (!m_isOpen || !m_handle)
    {
        return -1;
    }
    int fd = *static_cast<int *>(m_handle);
    return ::write(fd, buffer, size);
}

int64_t File::seek(int64_t offset, SeekOrigin origin)
{
    if (!m_isOpen || !m_handle)
    {
        return -1;
    }

    int whence;
    switch (origin)
    {
        case SeekOrigin::BEGIN:
            whence = SEEK_SET;
            break;
        case SeekOrigin::CURRENT:
            whence = SEEK_CUR;
            break;
        case SeekOrigin::END:
            whence = SEEK_END;
            break;
    }

    int fd = *static_cast<int *>(m_handle);
    return lseek(fd, offset, whence);
}

int64_t File::tell()
{
    if (!m_isOpen || !m_handle)
    {
        return -1;
    }
    int fd = *static_cast<int *>(m_handle);
    return lseek(fd, 0, SEEK_CUR);
}

int64_t File::size()
{
    if (!m_isOpen || !m_handle)
    {
        return -1;
    }

    int fd = *static_cast<int *>(m_handle);
    struct stat st;
    if (fstat(fd, &st) == 0)
    {
        return st.st_size;
    }
    return -1;
}

bool File::isOpen() const
{
    return m_isOpen;
}

bool File::exists(const std::string &filepath)
{
    struct stat st;
    return (stat(filepath.c_str(), &st) == 0);
}

bool File::remove(const std::string &filepath)
{
    return (unlink(filepath.c_str()) == 0);
}

bool File::createDirectory(const std::string &dirpath)
{
    return (mkdir(dirpath.c_str(), 0755) == 0 || errno == EEXIST);
}

uint32_t File::crc32(size_t bufferSize)
{
    if (!m_isOpen || !m_handle)
    {
        return 0;
    }

    // Save current position
    int64_t originalPos = tell();
    if (originalPos == -1)
    {
        return 0;
    }

    // Seek to beginning
    if (seek(0, SeekOrigin::BEGIN) == -1)
    {
        return 0;
    }

    std::vector<uint8_t> buffer(bufferSize);
    uint32_t crc = 0xFFFFFFFF;
    int64_t bytesRead;

    while ((bytesRead = read(buffer.data(), bufferSize)) > 0)
    {
        crc = CRC32::update(crc, buffer.data(), static_cast<size_t>(bytesRead));
    }

    // Restore original position
    seek(originalPos, SeekOrigin::BEGIN);

    return (bytesRead == -1) ? 0 : CRC32::finalize(crc);
}

std::string File::getExecutableDirectory()
{
    char path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);

    if (len == -1)
    {
        return "";
    }

    path[len] = '\0';
    char *dirPath = dirname(path);

    return std::string(dirPath);
}

std::string File::resolvePath(const std::string &relativePath)
{
    // If the path is already absolute, return it as-is
    if (!relativePath.empty() && relativePath[0] == '/')
    {
        return relativePath;
    }

    // Get executable directory
    std::string exeDir = getExecutableDirectory();
    if (exeDir.empty())
    {
        return relativePath; // Fallback to original path if we can't get exe dir
    }

    // Handle current directory reference
    std::string cleanPath = relativePath;
    if (cleanPath.substr(0, 2) == "./")
    {
        cleanPath = cleanPath.substr(2);
    }

    // Combine paths
    if (exeDir.back() != '/')
    {
        exeDir += '/';
    }

    return exeDir + cleanPath;
}
