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
void System::init() {
	System::hInstance = GetModuleHandle(NULL);
	Windows::regFirstWin();
	FileLoad::init();
	Font::init();
}
#endif