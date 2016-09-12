#include <dlfcn.h>
#include <string>

#include "display3r/SharedLibrary.hpp"
#include "display3r/Exception.hpp"

using display3r::SharedLibrary;
using display3r::SharedLibraryFlag;

SharedLibrary::SharedLibrary(
    std::string name, SharedLibraryFlag flag /* = LOCAL*/)
{
    if (name.substr(0, 3) != "lib")
        name.insert(0, "./lib");
    auto s = name.find_last_of(".");
    if (s == std::string::npos || name.substr(s) != ".so")
        name += ".so";

    m_handle = dlopen(name.c_str(), RTLD_NOW |
                      (flag == GLOBAL ? RTLD_GLOBAL : RTLD_LOCAL));
    if (m_handle == NULL)
        throw exception(dlerror());
}

void *SharedLibrary::symbol(char const *sym)
{
    void *s = dlsym(m_handle, sym);
    if (s == NULL)
        throw exception(dlerror());
    return s;
}

SharedLibrary::~SharedLibrary()
{
    if (m_handle != NULL)
        dlclose(m_handle);
    m_handle = NULL;
}

void SharedLibrary::Close()
{
    if (m_handle != NULL)
        dlclose(m_handle);
    m_handle = NULL;
}
