#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <memory>
#include <QSharedPointer>
class ModbusRtuContext;
namespace middleware {
//把global标签改为 对应协议的标签
void setModbusLogger(QSharedPointer<ModbusRtuContext> context);
void setModbusLoggerEnd(QSharedPointer<ModbusRtuContext> context);
void isOnline(QSharedPointer<ModbusRtuContext> context);
}




#endif // MIDDLEWARE_H
