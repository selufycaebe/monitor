#ifndef MODBUSMODEL_H
#define MODBUSMODEL_H

#include "qdatetime.h"
#include <map>
#include <nlohmann/json.hpp>
#include <QMetaType>
#include <stdint.h>
#include <string>

class ModbusModel {
public:
    ModbusModel() = default;
    virtual ~ModbusModel() = default;
public:
    const std::string &getDeviceName() const {
        return deviceName;
    }

    void setDeviceName(const std::string &deviceName) {
        ModbusModel::deviceName = deviceName;
    }

    const std::map<int, double> &getValues() const {
        return values;
    }

    void setValues(const std::map<int, double> &values) {
        ModbusModel::values = values;
    }

    const std::map<int,std::string> &getNameAddress() const {
        return nameAddress;
    }

    void setNameAddress(const std::map<int,std::string> &nameAddress) {
        ModbusModel::nameAddress = nameAddress;
    }

    int64_t getVersion() const {
        return version;
    }

    void setVersion(int64_t version) {
        ModbusModel::version = version;
    }

    int64_t getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(int64_t timestamp) {
        ModbusModel::timestamp = timestamp;
    }

private:
    //std::string object;
    //double value;
    std::string deviceName;
    std::map<int, double> values;
    std::map<int,std::string> nameAddress;
    int64_t version;
    int64_t timestamp;
};
Q_DECLARE_METATYPE(ModbusModel)
#endif // MODBUSMODEL_H
