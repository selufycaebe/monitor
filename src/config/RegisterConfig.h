#ifndef REGISTERCONFIG_H
#define REGISTERCONFIG_H

#include <QMetaType>

#include "../devices/exam1.h"
namespace Cfg {
inline void ClassRegisterInit() {
    qRegisterMetaType<Exam1>();
    //...
}
}

#endif // REGISTERCONFIG_H
