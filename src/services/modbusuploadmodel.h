//
// Created by Administrator on 2024/4/14.
//

#ifndef MONITOR_MODBUSUPLOADMODEL_H
#define MONITOR_MODBUSUPLOADMODEL_H

#include <map>
#include <string>
#pragma once

#include <optional>
#include <stdexcept>
#include <regex>

class ModbusUploadModel {
public:
    ModbusUploadModel() = default;
    virtual ~ModbusUploadModel() = default;

private:
    int64_t timestamp;
    std::string deviceId;
    std::map<std::string,std::string> results;

public:
    const int64_t & getTimestamp() const { return timestamp; }
    int64_t & getMutableTimestamp() { return timestamp; }
    void setTimestamp(const int64_t & value) { this->timestamp = value; }

    const std::string & getDeviceId() const { return deviceId; }
    std::string & getMutableDeviceId() { return deviceId; }
    void setDeviceId(const std::string & value) { this->deviceId = value; }

    const std::map<std::string,std::string> & getResults() const { return results; }
    std::map<std::string,std::string> & getMutableResults() { return results; }
    void setResults(const std::map<std::string,std::string> & value) { this->results = value; }
};

namespace model {
using nlohmann::json;
void from_json(const json & j, ModbusUploadModel & x);
void to_json(json & j, const ModbusUploadModel & x);

inline void from_json(const json & j, ModbusUploadModel& x) {
        x.setResults(j.at("results").get<std::map<std::string,std::string>>());
        x.setDeviceId(j.at("device_id").get<std::string>());
        x.setTimestamp(j.at("timestamp").get<int>());
}

inline void to_json(json & j, const ModbusUploadModel & x) {
    j = json::object();
    j["results"]= x.getResults();
    j["device_id"] = x.getDeviceId();
    j["timestamp"]=x.getTimestamp();
}
}




#endif //MONITOR_MODBUSUPLOADMODEL_H
