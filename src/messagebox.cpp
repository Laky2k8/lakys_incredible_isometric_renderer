#ifdef _WIN32
#include <windows.h>

void ShowErrorMessageBox(const char* title, const char* message) {
    MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR);
}
#elif defined(__linux__)
#include <cstdlib>

void ShowErrorMessageBox(const char* title, const char* message) {
    std::string cmd = "zenity --error --title=\"";
    cmd += title;
    cmd += "\" --text=\"";
    cmd += message;
    cmd += "\" 2>/dev/null || echo \"";
    cmd += message;
    cmd += "\"";
    system(cmd.c_str());
}
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <cstdlib>

void ShowErrorMessageBox(const char* title, const char* message)
{
    std::string cmd = "osascript -e 'display alert \"";
    cmd += title;
    cmd += "\" message \"";
    cmd += message;
    cmd += "\"'";
    system(cmd.c_str());
}

#endif