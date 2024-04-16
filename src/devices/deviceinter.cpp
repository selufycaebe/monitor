#include "deviceinter.h"
#include "qmap.h"
#include "utils.h"
#include "../logger.h"
#include <iostream>
DeviceInterface::DeviceInterface(const std::string&name)
    : deviceName{name} {}

void DeviceInterface::sendRequest(std::shared_ptr<ModbusRtuContext> context)
{

    auto logger = Logger::logger;
    //发送请求 处理回复
    auto client = context->client;
    int address = Utils::checkHex(context->requestParam.getAddress());
    auto* reply = client->sendRawRequest(context->request,address);

    std::string deviceName = context->requestParam.getDeviceName();
    if(reply==nullptr) {
        logger->error(deviceName + "reply指针为空");
        return;
    }
    QObject::connect(reply,&QModbusReply::errorOccurred,this,[context,deviceName,this](QModbusDevice::Error e){
            //处理错误
            //加上下面的信号 可以解决在设备没有连接成功的情况下,error的报错内容是 xxxx [common] [error] xxx
            //否则就是正常的 [global] [error] xxx
            emit startProcessingData(context);
            onError(deviceName,e);
            emit endProcessingData(context);
        },Qt::SingleShotConnection);
    QObject::connect(reply,&QModbusReply::finished,this,[context, this](){
            emit startProcessingData(context);
            auto md = onReadyRead(context);
            emit endProcessingData(context);
            if(!md.has_value()) return;
            ModbusModel m = md.value();
            //setNameOfAddress(m);
            emit upload(m);
        },Qt::SingleShotConnection);
}

void DeviceInterface::onError(const std::string& deviceName,QModbusDevice::Error e) {
    QModbusReply *reply=qobject_cast<QModbusReply *>(sender());
    Logger::logger->error("{}:{}",deviceName,reply->errorString().toStdString());
    reply->deleteLater();
}

void DeviceInterface::setSize(int size) {
    m_size = size;
}

void DeviceInterface::setNameOfAddress(const QMap<int, std::string> &names) {
    m_NameOfAddress = names;
}
