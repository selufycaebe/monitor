//
// Created by Administrator on 2024/3/26.
//

#include "common.h"
#include <QList>
#include <QModbusReply>
#include <QObject>
#include <optional>
#include <string>
#include "qdatetime.h"
#include "qlist.h"
#include "utils.h"
#include "../logger.h"

Common::Common() :DeviceInterface("Common") {

}

// 有符号数16位
std::optional<ModbusModel> Common::onReadyRead(std::shared_ptr<ModbusRtuContext> context) {
    auto logger = Logger::logger;
    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply) return std::nullopt;
    auto unit = reply->result();
    if (reply->error() != QModbusDevice::NoError) {
        logger->error(reply->errorString().toStdString());
        return std::nullopt;
    }
    auto values = reply->result().values();
    int startAddress = Utils::checkHex(context->requestParam.getStartAddress());

    // 只能采集连续相同类型的值
    // 多个值
    for (int i = 0; i < values.size();) {
        Utils::DataType type = Utils::dataType[context->requestParam.getDataType()];
        QList<quint16> value;
        double result = -1;
        if (type >= Utils::DataType::BIT && type <= Utils::DataType::INT16) {
            if(i>values.size()) break;
            value.append(values.at(i));
            result = Utils::getResult(type, value);
            m_results[startAddress + i] = result; // 0+0=0  0+1=1 ...
            i++;
        } else if (type >= Utils::DataType::UINT32Big && type <= Utils::DataType::FLOATLittle_S) {
            if(i+1>values.size()) break;
            value.append(values.at(i));
            value.append(values.at(i+1));
            result = Utils::getResult(type, value);
            m_results[startAddress + i] = result; // 0+0=0  0+1=1 ...
            i+=2;
        }else if(type >= Utils::DataType::UINT64Big && type <=Utils::DataType::DOUBLELittle_S) {
            if(i+3>values.size()) break;
            value.append(values.at(i));
            value.append(values.at(i+1));
            value.append(values.at(i+2));
            value.append(values.at(i+3));
            result = Utils::getResult(type, value);
            m_results[startAddress + i] = result; // 0+0=0  0+1=1 ...
            i+=4;
        }

    }
    reply->deleteLater();
    if (m_results.size() != m_size) return std::nullopt;
    //需要上传的数据
    ModbusModel model;
    model.setDeviceName(context->requestParam.getDeviceName());
    model.setTimestamp(Utils::getCurrentTimeStamp());
    model.setValues(m_results);
    model.setVersion(1);
    model.setNameAddress(m_NameOfAddress.toStdMap());
    m_results.clear();
    return model;
}
