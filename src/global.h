#ifndef GLOBAL_H
#define GLOBAL_H
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <QFile>
#include <QDebug>


//--------------------------------------------------json-------------------------------------------------------
#ifndef NLOHMANN_OPT_HELPER
#    define NLOHMANN_OPT_HELPER
namespace nlohmann
{
using nlohmann::json;
template <typename T>
struct adl_serializer<std::shared_ptr<T>> {
    static void to_json(json &j, const std::shared_ptr<T> &opt)
    {
        if (!opt)
            j = nullptr;
        else
            j = *opt;
    }

    static std::shared_ptr<T> from_json(const json &j)
    {
        if (j.is_null())
            return std::make_shared<T>();
        else
            return std::make_shared<T>(j.get<T>());
    }
};
template <typename T>
struct adl_serializer<std::optional<T>> {
    static void to_json(json &j, const std::optional<T> &opt)
    {
        if (!opt)
            j = nullptr;
        else
            j = *opt;
    }

    static std::optional<T> from_json(const json &j)
    {
        if (j.is_null())
            return std::make_optional<T>();
        else
            return std::make_optional<T>(j.get<T>());
    }
};
} // namespace nlohmann
#endif

namespace cfg {
#ifndef NLOHMANN_UNTYPED_Cfg_HELPER
#    define NLOHMANN_UNTYPED_Cfg_HELPER
using nlohmann::json;
inline json get_untyped(const json &j, const char *property)
{
    if (j.find(property) != j.end()) {
        return j.at(property).get<json>();
    }
    return json();
}

inline json get_untyped(const json &j, std::string property)
{
    return get_untyped(j, property.data());
}
#endif

#ifndef NLOHMANN_OPTIONAL_Cfg_HELPER
#    define NLOHMANN_OPTIONAL_Cfg_HELPER
template <typename T>
inline std::shared_ptr<T> get_heap_optional(const json &j, const char *property)
{
    auto it = j.find(property);
    if (it != j.end() && !it->is_null()) {
        return j.at(property).get<std::shared_ptr<T>>();
    }
    return std::shared_ptr<T>();
}

template <typename T>
inline std::shared_ptr<T> get_heap_optional(const json &j, std::string property)
{
    return get_heap_optional<T>(j, property.data());
}
template <typename T>
inline std::optional<T> get_stack_optional(const json &j, const char *property)
{
    auto it = j.find(property);
    if (it != j.end() && !it->is_null()) {
        return j.at(property).get<std::optional<T>>();
    }
    return std::optional<T>();
}

template <typename T>
inline std::optional<T> get_stack_optional(const json &j, std::string property)
{
    return get_stack_optional<T>(j, property.data());
}
#endif
}

namespace cfg {
class Utils final {
public:
    Utils() = default;
    virtual ~Utils() = default;
    static std::string readConfig(const QString &filename) {
        QFile file;
        file.setFileName(filename);
        std::string t;
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData;
            //简单解析
            while (!file.atEnd()) {
                auto lineText = file.readLine();
                if (lineText.isEmpty()) continue;
                //lineText = lineText.simplified();
                if (lineText.contains("//")) {
                    auto jsonWithoutAnnotation = lineText.first(lineText.indexOf("//"));
                    if (jsonWithoutAnnotation.isEmpty()) continue;
                    jsonData.append(jsonWithoutAnnotation);
                } else if (lineText.contains("#")) {
                    auto jsonWithoutAnnotation = lineText.first(lineText.indexOf("#"));
                    if (jsonWithoutAnnotation.isEmpty()) continue;
                    jsonData.append(jsonWithoutAnnotation);
                } else {
                    jsonData.append(lineText);
                }
            }
            t = jsonData.toStdString();
        } else {
            qDebug() << QString("打开%1失败").arg(filename);
            throw std::runtime_error(QString("打开%1失败").arg(filename).toStdString());
        }
        file.close();
        return t;
    }
};
}


//--------------------------------------------------json-------------------------------------------------------
enum IOType {
    ModbusRtu=0,
    //....

    None = 100
};

enum Os {
    Window=0,
    Linux
};

#endif // GLOBAL_H
