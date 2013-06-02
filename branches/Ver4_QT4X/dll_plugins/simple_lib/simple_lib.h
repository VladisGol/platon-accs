#ifndef SIMPLE_LIB_H
#define SIMPLE_LIB_H

#include "simple_lib_global.h"
#include <QMessageBox>

class SIMPLE_LIBSHARED_EXPORT Simple_lib {
public:
    Simple_lib();
};
extern "C" int AnProc(const QString& instring);

#endif // SIMPLE_LIB_H
