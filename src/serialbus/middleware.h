#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <memory>
class ModbusRtuContext;
namespace middleware {
//把global标签改为 对应协议的标签
void setModbusLogger(std::shared_ptr<ModbusRtuContext> context);
void setModbusLoggerEnd(std::shared_ptr<ModbusRtuContext> context);
void isOnline(std::shared_ptr<ModbusRtuContext> context);
}




#endif // MIDDLEWARE_H
