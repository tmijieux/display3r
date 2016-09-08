#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

#ifndef __GNUC__
#define noexcept
#endif

namespace display3r {

class exception : public std::exception {
public:
    exception(std::string const &s): _s(s) {}
    const char *what() const noexcept override { return _s.c_str(); }
private:
    std::string _s;
};

};
#endif //EXCEPTION_H
