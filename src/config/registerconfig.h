#ifndef REGISTERCONFIG_H
#define REGISTERCONFIG_H

#include <QMetaType>

#include "../devices/exam1.h"
#include "../devices/common.h"
namespace cfg {
inline void ClassRegisterInit() {
    qRegisterMetaType<Exam1>();
    qRegisterMetaType<Common>();
    //...
}
}

#endif // REGISTERCONFIG_H
