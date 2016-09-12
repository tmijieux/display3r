#ifndef DISPLAY3R_SHAREDLIBRARY_H
#define DISPLAY3R_SHAREDLIBRARY_H

namespace display3r {

enum SharedLibraryFlag {
    LOCAL,
    GLOBAL,
};

class SharedLibrary{
public:
    SharedLibrary(std::string name, SharedLibraryFlag flag = LOCAL);
    ~SharedLibrary();

    void Close();
    void *symbol(char const *);
private:
    void *m_handle;
};


};
#endif // DISPLAY3R_SHAREDLIBRARY_H
