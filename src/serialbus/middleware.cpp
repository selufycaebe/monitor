#include "middleware.h"
#include "../logger.h"
#include "modbusrtucontext.h"
#include <iostream>
void middleware::setModbusLogger(std::shared_ptr<ModbusRtuContext> context)
{
    auto logger = Logger::logger;
    std::string pattern = fmt::format("[%Y-%m-%d %H:%M:%S.%e] [{}] [modbus] [{}] [%^%l%$] %v",
                                      context->portName,context->requestParam.getDeviceName());
    //logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [modbus] [%^%l%$] %v");
    logger->set_pattern(pattern);
}

void middleware::setModbusLoggerEnd(std::shared_ptr<ModbusRtuContext> context)
{
    auto logger = Logger::logger;
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [global] [%^%l%$] %v");
}

void middleware::isOnline(std::shared_ptr<ModbusRtuContext> context) {
    if(context->isOnline) {
        Logger::logger->info("{}:{} isOnline",context->portName,context->requestParam.getDeviceName());
    }else{
        Logger::logger->info("{}:{} offline",context->portName,context->requestParam.getDeviceName());
    }
}
