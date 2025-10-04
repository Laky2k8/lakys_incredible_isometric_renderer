#pragma once

#ifdef _WIN32
    void ShowErrorMessageBox(const char* title, const char* message);
#elif defined(__linux__)
	void ShowErrorMessageBox(const char* title, const char* message);
#elif defined(__APPLE__)
	void ShowErrorMessageBox(const char* title, const char* message);
#else
    inline void ShowErrorMessageBox(const char* title, const char* message) {
        std::cerr << title << ": " << message << std::endl;
    }
#endif