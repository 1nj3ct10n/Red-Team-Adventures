#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

// Color constants for console output
const int DEFAULT_COLOR = 7;  // Default console color (white)
const int RED_COLOR = 12;     // Red color for analyzed files
const int GREEN_COLOR = 10;   // Green color for files with RWX sections

// Function to set console text color
void setConsoleColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

std::vector<std::string> getFilesInDirectory(const std::string& directory)
{
    std::vector<std::string> files;

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((directory + "\\*").c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string fileName = findData.cFileName;
                files.push_back(directory + "\\" + fileName);
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);
    }

    return files;
}

bool hasRWXSection(const std::string& filePath)
{
    HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    HANDLE hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hFileMapping == NULL)
    {
        CloseHandle(hFile);
        std::cerr << "Failed to create file mapping: " << filePath << std::endl;
        return false;
    }

    LPVOID baseAddress = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
    if (baseAddress == NULL)
    {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        std::cerr << "Failed to map view of file: " << filePath << std::endl;
        return false;
    }

    PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(baseAddress);
    PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<char*>(baseAddress) + dosHeader->e_lfanew);

    PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);
    for (WORD i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i, ++sectionHeader)
    {
        DWORD characteristics = sectionHeader->Characteristics;
        if ((characteristics & IMAGE_SCN_MEM_WRITE) && (characteristics & IMAGE_SCN_MEM_EXECUTE))
        {
            std::cout << "RWX section found in file: " << filePath << std::endl;
            UnmapViewOfFile(baseAddress);
            CloseHandle(hFileMapping);
            CloseHandle(hFile);
            return true;
        }
    }

    UnmapViewOfFile(baseAddress);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);
    return false;
}

int main()
{
    std::string directoryPath;
    std::cout << "Enter the directory path to search for files: ";
    std::getline(std::cin, directoryPath);

    std::vector<std::string> files = getFilesInDirectory(directoryPath);
    if (files.empty())
    {
        std::cout << "No files found in the directory." << std::endl;
        return 0;
    }

    bool foundRWXSection = false;
    for (const std::string& file : files)
    {
        setConsoleColor(RED_COLOR);
        std::cout << "Analyzing file: " << file << std::endl;
        setConsoleColor(DEFAULT_COLOR);

        if (hasRWXSection(file))
        {
            foundRWXSection = true;
            setConsoleColor(GREEN_COLOR);
            std::cout << "RWX section found in file: " << file << std::endl;
        }
    }

    if (!foundRWXSection)
        std::cout << "No files with RWX sections found." << std::endl;

    return 0;
}
