#ifndef MODBUSMODEL_H
#define MODBUSMODEL_H

#include <nlohmann/json.hpp>
#include <QMetaType>
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

class ModbusModel {
public:
    ModbusModel() = default;
    virtual ~ModbusModel() = default;

private:
    std::string deviceName;
    std::string object;
    double value;
    int64_t version;

public:
    const std::string & getDeviceName() const { return deviceName; }
    std::string & getMutableDeviceName() { return deviceName; }
    void setDeviceName(const std::string & value) { this->deviceName = value; }

    const std::string & getObject() const { return object; }
    std::string & getMutableObject() { return object; }
    void setObject(const std::string & value) { this->object = value; }

    const double & getValue() const { return value; }
    double & getMutableValue() { return value; }
    void setValue( double value) { this->value = value; }

    const int64_t & getVersion() const { return version; }
    int64_t & getMutableVersion() { return version; }
    void setVersion( int64_t value) { this->version = value; }
};
Q_DECLARE_METATYPE(ModbusModel)

void from_json(const json & j, ModbusModel & x);
void to_json(json & j, const ModbusModel & x);

inline void from_json(const json & j, ModbusModel& x) {
    x.setDeviceName(j.at("deviceName").get<std::string>());
    x.setObject(j.at("object").get<std::string>());
    x.setValue(j.at("value").get<double>());
    x.setVersion(j.at("version").get<int64_t>());
}

inline void to_json(json & j, const ModbusModel & x) {
    j = json::object();
    j["deviceName"] = x.getDeviceName();
    j["object"] = x.getObject();
    j["value"] = x.getValue();
    j["version"] = x.getVersion();
}
#endif // MODBUSMODEL_H
