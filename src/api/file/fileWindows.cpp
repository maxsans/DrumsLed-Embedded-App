/**
 * @file fileWindows.cpp
 * @author Cyprien Ménard
 * @date 2025-08-19
 */

#include "file.hpp"
#include "tools/containers/binary/crc32.hpp"
#include <vector>
#include <windows.h>

File::File() : m_handle(INVALID_HANDLE_VALUE), m_isOpen(false)
{
}

File::File(const std::string &filepath)
    : m_handle(INVALID_HANDLE_VALUE), m_filepath(filepath), m_isOpen(false)
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
    DWORD access = 0;
    DWORD creation = 0;

    switch (mode)
    {
        case FileMode::READ:
            access = GENERIC_READ;
            creation = OPEN_EXISTING;
            break;
        case FileMode::WRITE:
            access = GENERIC_WRITE;
            creation = CREATE_ALWAYS;
            break;
        case FileMode::APPEND:
            access = GENERIC_WRITE;
            creation = OPEN_ALWAYS;
            break;
        case FileMode::READ_WRITE:
            access = GENERIC_READ | GENERIC_WRITE;
            creation = OPEN_ALWAYS;
            break;
    }

    m_handle = CreateFileA(filepath.c_str(),
                           access,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                           nullptr,
                           creation,
                           FILE_ATTRIBUTE_NORMAL,
                           nullptr);

    m_isOpen = (m_handle != INVALID_HANDLE_VALUE);

    if (m_isOpen && mode == FileMode::APPEND)
    {
        SetFilePointer(static_cast<HANDLE>(m_handle), 0, nullptr, FILE_END);
    }

    return m_isOpen;
}

bool File::close()
{
    if (!m_isOpen)
    {
        return true;
    }

    BOOL result = CloseHandle(static_cast<HANDLE>(m_handle));
    if (result)
    {
        m_handle = INVALID_HANDLE_VALUE;
        m_isOpen = false;
        return true;
    }
    return false;
}

int64_t File::read(void *buffer, size_t size)
{
    if (!m_isOpen)
    {
        return -1;
    }

    DWORD bytesRead = 0;
    if (ReadFile(static_cast<HANDLE>(m_handle),
                 buffer,
                 static_cast<DWORD>(size),
                 &bytesRead,
                 nullptr))
    {
        return static_cast<int64_t>(bytesRead);
    }
    return -1;
}

int64_t File::write(const void *buffer, size_t size)
{
    if (!m_isOpen)
    {
        return -1;
    }

    DWORD bytesWritten = 0;
    if (WriteFile(static_cast<HANDLE>(m_handle),
                  buffer,
                  static_cast<DWORD>(size),
                  &bytesWritten,
                  nullptr))
    {
        return static_cast<int64_t>(bytesWritten);
    }
    return -1;
}

int64_t File::seek(int64_t offset, SeekOrigin origin)
{
    if (!m_isOpen)
    {
        return -1;
    }

    DWORD moveMethod;
    switch (origin)
    {
        case SeekOrigin::BEGIN:
            moveMethod = FILE_BEGIN;
            break;
        case SeekOrigin::CURRENT:
            moveMethod = FILE_CURRENT;
            break;
        case SeekOrigin::END:
            moveMethod = FILE_END;
            break;
    }

    LARGE_INTEGER li;
    li.QuadPart = offset;
    li.LowPart = SetFilePointer(
        static_cast<HANDLE>(m_handle), li.LowPart, &li.HighPart, moveMethod);

    if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
    {
        return -1;
    }

    return li.QuadPart;
}

int64_t File::tell()
{
    return seek(0, SeekOrigin::CURRENT);
}

int64_t File::size()
{
    if (!m_isOpen)
    {
        return -1;
    }

    LARGE_INTEGER fileSize;
    if (GetFileSizeEx(static_cast<HANDLE>(m_handle), &fileSize))
    {
        return fileSize.QuadPart;
    }
    return -1;
}

bool File::isOpen() const
{
    return m_isOpen;
}

bool File::exists(const std::string &filepath)
{
    DWORD attrs = GetFileAttributesA(filepath.c_str());
    return (attrs != INVALID_FILE_ATTRIBUTES);
}

bool File::remove(const std::string &filepath)
{
    return (DeleteFileA(filepath.c_str()) != 0);
}

bool File::createDirectory(const std::string &dirpath)
{
    return (CreateDirectoryA(dirpath.c_str(), nullptr) != 0
            || GetLastError() == ERROR_ALREADY_EXISTS);
}

uint32_t File::crc32(size_t bufferSize)
{
    if (!m_isOpen)
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
