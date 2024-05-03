#include "deviceinter.h"

#include <QMap>
#include <QMetaObject>
#include <QModbusDevice>
#include <QMetaEnum>
#include <QEventLoop>
#include <QCoreApplication>

#include "fmt/core.h"
#include "qmodbusclient.h"
#include "qmodbusdevice.h"
#include "qmodbusrtuserialclient.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qobjectdefs.h"
#include "spdlog/common.h"

#include "utils.h"
#include "../logger.h"

DeviceInterface::DeviceInterface(const std::string &name)
        : deviceName{name} {}

void DeviceInterface::sendRequest(QSharedPointer<ModbusRtuContext> context) {
    auto logger = Logger::logger;
    //发送请求 处理回复
    auto client = context->client;
    int address = Utils::checkHex(context->requestParam.getAddress());
    auto *reply = client->sendRawRequest(context->request, address);
    std::string deviceName = context->requestParam.getDeviceName();
    if(reply && !reply->isFinished()) {
        QObject::connect(reply, &QModbusReply::errorOccurred,
                         this, [reply, client, context, this](QModbusDevice::Error e) mutable {
                    //处理错误
                    //加上下面的信号 可以解决在设备没有连接成功的情况下,error的报错内容是 xxxx [common] [error] xxx
                    //否则就是正常的 [global] [error] xxx
                    reply->deleteLater();
                    reply->disconnect();
                    auto logger = Logger::logger;
                    QMetaEnum me = QMetaEnum::fromType<QModbusDevice::Error>();
                    printCustom(spdlog::level::warn, me.key(e), context);
                    emit handleError(context);
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
                    client->disconnect(SIGNAL(timeoutChanged(int)),0,0);
#endif
        });
        connect(reply, &QModbusReply::finished,
                         this, [client,reply, context, this]()  {
                    if (!reply) return;
                    emit startProcessingData(context);
                    auto md = onReadyRead(context);
                    reply->deleteLater();
                    if (!md.has_value()) return;
                    context->isOnline = true;
                    emit endProcessingData(context);
                    ModbusModel m = md.value();
                    emit upload(m);
#if (QT_VERSION <= QT_VERSION_CHECK(6,2,0))
                    client->disconnect(SIGNAL(timeoutChanged(int)),0,0);
#endif
                });
    } else {
        delete reply;
        reply = nullptr;
    }
}


void DeviceInterface::setSize(int size) {
    m_size = size;
}

void DeviceInterface::setNameOfAddress(const QMap<int, std::string> &names) {
    m_NameOfAddress = names;
}


void DeviceInterface::printCustom(spdlog::level::level_enum level, const std::string &information,
                                  QSharedPointer<ModbusRtuContext> context) const {
    auto logger = Logger::logger;
    std::string pattern = fmt::format("[%Y-%m-%d %H:%M:%S.%e] [{}] [modbus] [{}] [%^%l%$] %v",
                                      context->portName, context->requestParam.getDeviceName());
    logger->set_pattern(pattern);
    logger->log(level, information);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [global] [%^%l%$] %v");
}
