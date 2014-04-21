#include "grBaseTypes.h"
using namespace std;
using namespace Graphonichk;

string get_ext (const string& st) {
    size_t pos = st.rfind('.');
    if (pos <= 0) return "";
    return st.substr(pos+1, std::string::npos);
}


#ifdef WIN32
HINSTANCE System::hInstance = nullptr;
uint16_t System::countProcessors = 1;
void System::init() {
	SYSTEM_INFO sysInfo;
	System::hInstance = GetModuleHandle(NULL);
	GetSystemInfo(&sysInfo);
	System::countProcessors = sysInfo.dwNumberOfProcessors;
	printf("count proc %i\n", System::countProcessors);
	Windows::regFirstWin();
	FileLoad::init();
	Font::init();
};
#endif