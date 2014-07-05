#include "grMain.h"
#include <unistd.h>
using std::string;
using Graphonichk::System;

string get_ext (const string& st) {
    size_t pos = st.rfind('.');
    if (pos <= 0) return "";
    return st.substr(pos+1);
}


uint16_t System::countProcessors = 1;
#ifdef WIN32
HINSTANCE System::hInstance = nullptr;
void System::init() {
	SYSTEM_INFO sysInfo;
	System::hInstance = GetModuleHandle(NULL);
	GetSystemInfo(&sysInfo);
	System::countProcessors = sysInfo.dwNumberOfProcessors;
};
#else
void System::init() {
	System::countProcessors = sysconf(_SC_NPROCESSORS_ONLN);
};
#endif