//
// Created by Administrator on 2024/4/11.
//

#ifndef MONITOR_DEVICEPARAMS_H
#define MONITOR_DEVICEPARAMS_H


#include "config.h"
#pragma once

#include <nlohmann/json.hpp>

#include <optional>
#include <stdexcept>
#include <regex>

#include "../global.h"
#include "config.h"

namespace cfg {
using nlohmann::json;

#ifndef NLOHMANN_UNTYPED_Cfg_HELPER
#define NLOHMANN_UNTYPED_Cfg_HELPER
inline json get_untyped(const json & j, const char * property) {
    if (j.find(property) != j.end()) {
        return j.at(property).get<json>();
    }
    return json();
}

inline json get_untyped(const json & j, std::string property) {
    return get_untyped(j, property.data());
}
#endif

class Param {
public:
    Param() = default;
    virtual ~Param() = default;

private:
    std::string portname;
    std::string protocol;
    std::string deviceName;
    int64_t address;
    std::map<std::string, std::string> params;

public:
    const std::string & getPortname() const { return portname; }
    std::string & getMutablePortname() { return portname; }
    void setPortname(const std::string & value) { this->portname = value; }

    const std::string & getProtocol() const { return protocol; }
    std::string & getMutableProtocol() { return protocol; }
    void setProtocol(const std::string & value) { this->protocol = value; }

    const std::string & getDeviceName() const { return deviceName; }
    std::string & getMutableDeviceName() { return deviceName; }
    void setDeviceName(const std::string & value) { this->deviceName = value; }

    const int64_t & getAddress() const { return address; }
    int64_t & getMutableAddress() { return address; }
    void setAddress(const int64_t & value) { this->address = value; }

    const std::map<std::string, std::string> & getParams() const { return params; }
    std::map<std::string, std::string> & getMutableParams() { return params; }
    void setParams(const std::map<std::string, std::string> & value) { this->params = value; }
};

class DeviceParam {
public:
    DeviceParam() = default;
    virtual ~DeviceParam() = default;

private:
    int64_t version=-1;
    std::vector<Param> params;

public:
    const int64_t & getVersion() const { return version; }
    int64_t & getMutableVersion() { return version; }
    void setVersion(const int64_t & value) { this->version = value; }

    const std::vector<Param> & getParams() const { return params; }
    std::vector<Param> & getMutableParams() { return params; }
    void setParams(const std::vector<Param> & value) { this->params = value; }
};
}

namespace cfg {
void from_json(const json & j, Param & x);
void to_json(json & j, const Param & x);

void from_json(const json & j, DeviceParam & x);
void to_json(json & j, const DeviceParam & x);

inline void from_json(const json & j, Param& x) {
    x.setPortname(j.at("portname").get<std::string>());
    x.setProtocol(j.at("protocol").get<std::string>());
    x.setDeviceName(j.at("device_name").get<std::string>());
    x.setAddress(j.at("address").get<int64_t>());
    x.setParams(j.at("params").get<std::map<std::string, std::string>>());
}

inline void to_json(json & j, const Param & x) {
    j = json::object();
    j["portname"] = x.getPortname();
    j["protocol"] = x.getProtocol();
    j["device_name"] = x.getDeviceName();
    j["address"] = x.getAddress();
    j["params"] = x.getParams();
}

inline void from_json(const json & j, DeviceParam& x) {
    x.setVersion(j.at("version").get<int64_t>());
    x.setParams(j.at("params").get<std::vector<Param>>());
}

inline void to_json(json & j, const DeviceParam & x) {
    j = json::object();
    j["version"] = x.getVersion();
    j["params"] = x.getParams();
}
}

namespace cfg {
static DeviceParam readDeviceParams(){
    using namespace std;
    static DeviceParam param;
    if (param.getVersion() == -1) {
        DeviceParam tparam;
        //auto bytes= file.readAll();
        auto stdString = cfg::Utils::readConfig("config/deviceparams.json");
        try {
            nlohmann::json json = nlohmann::json::parse(stdString);
            from_json(json, tparam);
        } catch (nlohmann::json::exception &e) {
            qDebug() << "deviceparams.json字段有误:" + QString{e.what()};
        }
        if (tparam.getVersion() == -1) {
            qDebug() << "读取deviceparams.json失败,请检查配置文档";
            throw runtime_error("读取deviceparams.json失败,请检查配置文档");
        }
        param = tparam;
    }
    return param;
}
}




#endif //MONITOR_DEVICEPARAMS_H
