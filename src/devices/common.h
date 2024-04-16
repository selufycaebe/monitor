//
// Created by Administrator on 2024/3/26.
//

#ifndef MONITOR_COMMON_H
#define MONITOR_COMMON_H


#include "deviceinter.h"
#include <string>

class Common : public DeviceInterface {
public:
    explicit Common();
protected slots:
    std::optional<ModbusModel>  onReadyRead(std::shared_ptr<ModbusRtuContext> context) override;
};


#endif //MONITOR_COMMON_H
