#include <windows.h>

int main() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    char* target = "C:\\Users\\Public\\";
    char* filename = "out.txt";
    char fullPath[MAX_PATH];
    lstrcpy(fullPath, target);
    lstrcat(fullPath, filename);

    HANDLE hFile = CreateFileA(
        fullPath,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_HIDDEN,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        const char* msg = "Reverse me.";
        DWORD written;
        WriteFile(hFile, msg, lstrlen(msg), &written, NULL);
        CloseHandle(hFile);
    }

    return 0;
}
