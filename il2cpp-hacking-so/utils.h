#pragma once

#include <jni.h>
#include <unistd.h>
#include <cstdlib>

typedef unsigned long DWORD;

class Utils {
public:
    static DWORD findLibrary(const char* library) {
        FILE* fp;
        unsigned long addr = 0;
        char* pch;
        char filename[32];
        char buffer[1024];

        snprintf(filename, sizeof(filename), "/proc/self/maps");

        fp = fopen(filename, "r");

        if (fp != NULL) {
            while (fgets(buffer, sizeof(buffer), fp)) {
                if (strstr(buffer, library)) {
                    addr = (DWORD)strtoul(buffer, NULL, 16);
                    if (addr == 0x8000)
                        addr = 0;
                    break;
                }
            }
            fclose(fp);
        }
        return addr;
    }

    static DWORD getAbsoluteAddress(const char* libraryName, DWORD relativeAddr) {
        DWORD libBase;
        if (libBase == 0)
            libBase = findLibrary(libraryName);
        if (libBase != 0)
            return (reinterpret_cast<DWORD>(libBase + relativeAddr));
        else
            return 0;
    }

    static bool isLibraryLoaded(const char* libraryName) {
        char line[512] = { 0 };
        FILE* fp = fopen("/proc/self/maps", "rt");
        if (fp != NULL) {
            while (fgets(line, sizeof(line), fp)) {
                if (strstr(line, libraryName))
                    return true;
            }
            fclose(fp);
        }
        return false;
    }
};