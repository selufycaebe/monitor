#ifndef CONFIG_H
#define CONFIG_H
//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//     DeviceParam data = nlohmann::json::parse(jsonString);
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include "../global.h"
#include <QIODevice>
#include <string>

namespace cfg {
using nlohmann::json;

class RequestPara {
public:
    RequestPara() = default;

    virtual ~RequestPara() = default;

private:
    std::optional<std::string> description;
    std::string className;
    std::string deviceName;
    std::string address;
    std::string funcCode;
    std::string startAddress;
    std::string countOrData;
    std::string dataType;

public:
    std::optional<std::string> getDescription() const { return description; }

    void setDescription(std::optional<std::string> value) { this->description = value; }

    const std::string &getClassName() const { return className; }

    std::string &getMutableClassName() { return className; }

    void setClassName(const std::string &value) { this->className = value; }

    const std::string &getDeviceName() const { return deviceName; }

    std::string &getMutableDeviceName() { return deviceName; }

    void setDeviceName(const std::string &value) { this->deviceName = value; }

    const std::string &getAddress() const { return address; }

    std::string &getMutableAddress() { return address; }

    void setAddress(const std::string &value) { this->address = value; }

    const std::string &getFuncCode() const { return funcCode; }

    std::string &getMutableFuncCode() { return funcCode; }

    void setFuncCode(const std::string &value) { this->funcCode = value; }

    const std::string &getStartAddress() const { return startAddress; }

    std::string &getMutableStartAddress() { return startAddress; }

    void setStartAddress(const std::string &value) { this->startAddress = value; }

    const std::string &getCountOrData() const { return countOrData; }

    std::string &getMutableCountOrData() { return countOrData; }

    void setCountOrData(const std::string &value) { this->countOrData = value; }

    const std::string &getDataType() const { return dataType; }

    std::string &getMutableDataType() { return dataType; }

    void setDataType(const std::string &value) { this->dataType = value; }
};

class Server {
public:
    Server() = default;

    virtual ~Server() = default;

private:
    int64_t os;
    int64_t serverTypeid;
    std::string portName;
    int64_t baudRate;
    int64_t dataBits;
    int64_t stopBits;
    int64_t parity;
    int64_t flowControl;
    int64_t intervalMs;
    std::vector<RequestPara> requestParas;

public:
    const int64_t &getOs() const { return os; }

    int64_t &getMutableOs() { return os; }

    void setOs(const int64_t &value) { this->os = value; }

    const int64_t &getServerTypeid() const { return serverTypeid; }

    int64_t &getMutableServerTypeid() { return serverTypeid; }

    void setServerTypeid(const int64_t &value) { this->serverTypeid = value; }

    const std::string &getPortName() const { return portName; }

    std::string &getMutablePortName() { return portName; }

    void setPortName(const std::string &value) { this->portName = value; }

    const int64_t &getBaudRate() const { return baudRate; }

    int64_t &getMutableBaudRate() { return baudRate; }

    void setBaudRate(const int64_t &value) { this->baudRate = value; }

    const int64_t &getDataBits() const { return dataBits; }

    int64_t &getMutableDataBits() { return dataBits; }

    void setDataBits(const int64_t &value) { this->dataBits = value; }

    const int64_t &getStopBits() const { return stopBits; }

    int64_t &getMutableStopBits() { return stopBits; }

    void setStopBits(const int64_t &value) { this->stopBits = value; }

    const int64_t &getParity() const { return parity; }

    int64_t &getMutableParity() { return parity; }

    void setParity(const int64_t &value) { this->parity = value; }

    const int64_t &getFlowControl() const { return flowControl; }

    int64_t &getMutableFlowControl() { return flowControl; }

    void setFlowControl(const int64_t &value) { this->flowControl = value; }

    const int64_t &getIntervalMs() const { return intervalMs; }

    int64_t &getMutableIntervalMs() { return intervalMs; }

    void setIntervalMs(const int64_t &value) { this->intervalMs = value; }

    const std::vector<RequestPara> &getRequestParas() const { return requestParas; }

    std::vector<RequestPara> &getMutableRequestParas() { return requestParas; }

    void setRequestParas(const std::vector<RequestPara> &value) { this->requestParas = value; }
};

class Config {
public:
    Config() = default;

    virtual ~Config() = default;

private:
    int64_t version = -1;
    std::string loglevel = "warn";
    std::string printlevel = "info";
    std::vector<Server> server;

public:
    const int64_t &getVersion() const { return version; }

    int64_t &getMutableVersion() { return version; }

    void setVersion(const int64_t &value) { this->version = value; }

    const std::string &getLogLevel() const { return loglevel; }

    std::string &getMutableLogLevel() { return loglevel; }

    void setLoglevel(const std::string &value) { this->loglevel = value; }

    const std::string &getPrintLevel() const { return printlevel; }

    std::string &getMutablePrintLevel() { return printlevel; }

    void setPrintlevel(const std::string &value) { this->printlevel = value; }

    const std::vector<Server> &getServer() const { return server; }

    std::vector<Server> &getMutableServer() { return server; }

    void setServer(const std::vector<Server> &value) { this->server = value; }
};
} // namespace cfg

namespace cfg {
void from_json(const json &j, RequestPara &x);

void to_json(json &j, const RequestPara &x);

void from_json(const json &j, Server &x);

void to_json(json &j, const Server &x);

void from_json(const json &j, Config &x);

void to_json(json &j, const Config &x);

inline void from_json(const json &j, RequestPara &x) {
    x.setDescription(get_stack_optional<std::string>(j, "description"));
    x.setClassName(j.at("class_name").get<std::string>());
    x.setDeviceName(j.at("device_name").get<std::string>());
    x.setAddress(j.at("address").get<std::string>());
    x.setFuncCode(j.at("func_code").get<std::string>());
    x.setStartAddress(j.at("start_address").get<std::string>());
    x.setCountOrData(j.at("count_or_data").get<std::string>());
    x.setDataType(j.at("data_type").get<std::string>());
}

inline void to_json(json &j, const RequestPara &x) {
    j = json::object();
    if (x.getDescription()) {
        j["description"] = x.getDescription();
    }
    j["class_name"] = x.getClassName();
    j["device_name"] = x.getDeviceName();
    j["address"] = x.getAddress();
    j["func_code"] = x.getFuncCode();
    j["start_address"] = x.getStartAddress();
    j["count_or_data"] = x.getCountOrData();
    j["data_type"] = x.getDataType();
}

inline void from_json(const json &j, Server &x) {
    x.setOs(j.at("os").get<int64_t>());
    x.setServerTypeid(j.at("typeid").get<int64_t>());
    x.setPortName(j.at("port_name").get<std::string>());
    x.setBaudRate(j.at("baud_rate").get<int64_t>());
    x.setDataBits(j.at("data_bits").get<int64_t>());
    x.setStopBits(j.at("stop_bits").get<int64_t>());
    x.setParity(j.at("parity").get<int64_t>());
    x.setFlowControl(j.at("flow_control").get<int64_t>());
    x.setIntervalMs(j.at("interval_ms").get<int64_t>());
    x.setRequestParas(j.at("request_paras").get<std::vector<RequestPara>>());
}

inline void to_json(json &j, const Server &x) {
    j = json::object();
    j["os"] = x.getOs();
    j["typeid"] = x.getServerTypeid();
    j["port_name"] = x.getPortName();
    j["baud_rate"] = x.getBaudRate();
    j["data_bits"] = x.getDataBits();
    j["stop_bits"] = x.getStopBits();
    j["parity"] = x.getParity();
    j["flow_control"] = x.getFlowControl();
    j["interval_ms"] = x.getIntervalMs();
    j["request_paras"] = x.getRequestParas();
}

inline void from_json(const json &j, Config &x) {
    x.setVersion(j.at("version").get<int64_t>());
    x.setLoglevel(j.at("loglevel").get<std::string>());
    x.setPrintlevel(j.at("printlevel").get<std::string>());
    x.setServer(j.at("server").get<std::vector<Server>>());
}

inline void to_json(json &j, const Config &x) {
    j = json::object();
    j["version"] = x.getVersion();
    j["loglevel"] = x.getLogLevel();
    j["printlevel"] = x.getPrintLevel();
    j["server"] = x.getServer();
}
} // namespace cfg

namespace cfg {
static Config readConfig() {
    using namespace std;
    static Config config;
    if (config.getVersion() == -1) {
        Config tconfig;
        //auto bytes= file.readAll();
        auto stdString = cfg::Utils::readConfig("config/config.json");
        try {
            nlohmann::json json = nlohmann::json::parse(stdString);
            from_json(json, tconfig);
        } catch (nlohmann::json::exception &e) {
            qDebug() << "config.json字段有误:" + QString{e.what()};
        }
        if (tconfig.getVersion() == -1) {
            qDebug() << "读取config.json失败,请检查配置文档";
            throw runtime_error("读取config.json失败,请检查配置文档");
        }
        config = tconfig;
    }
    return config;
}
}


#endif // CONFIG_H
