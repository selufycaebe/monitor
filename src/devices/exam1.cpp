#include <QModbusReply>
#include <QModbusDevice>

#include "exam1.h"
#include "../serialbus/modbusrtucontext.h"
#include "utils.h"
#include "../logger.h"
Exam1::Exam1() : DeviceInterface("Exam1")
{
}

void Exam1::sendRequest(const ModbusRtuContext &unit)
{
    //发送请求 处理回复
    auto client = unit.client;
    int address = Utils::checkHex(unit.requestParam.getAddress());
    auto* reply = client->sendRawRequest(unit.request,address);
    if(reply==nullptr) {
        Logger::logger->error(unit.requestParam.getDeviceName() + "reply指针为空");
        return;
    }
    QObject::connect(reply,&QModbusReply::finished,this,[unit, this](){
        onReadyRead(unit);
    });
    QObject::connect(reply,&QModbusReply::errorOccurred,this,&Exam1::onError);
}

void Exam1::onReadyRead(const ModbusRtuContext &c)
{
    auto logger = Logger::logger;
    QModbusReply *reply=qobject_cast<QModbusReply *>(sender());
    auto unit = reply->result();
    if(reply->error()!= QModbusDevice::NoError) {
        logger->error( reply->errorString().toStdString());
        return;
    }
    auto results = reply->result().values();
    int address = Utils::checkHex(c.requestParam.getStartAddress());
    double result= -1;
    switch(address) {
    case 0:
    {
        uint32_t value =  Utils::getResult(Utils::DataType::UINT32Big,{results.at(0),results.at(1)});
        logger->info(value); //65538
        result = value;
    }
    break;
    case 2:
    {
        uint16_t value =  Utils::getResult(Utils::DataType::UINT16,{results.at(0)});
        logger->info(value);
        result = value;
    }
    break;

    case 4:
    {
        double value =  Utils::getResult(Utils::DataType::DOUBLEBig,{results.at(0),results.at(1),results.at(2),results.at(3)});
        logger->info(value);
        result = value;
    }
    break;

    case 8:
    {
        float value =  Utils::getResult(Utils::DataType::FLOATBig,{results.at(0),results.at(1)});
        logger->info(value);
        result = value;
    }
    break;
    }
    if(result == -1) return;
    ModbusModel model;
    model.setDeviceName(c.requestParam.getDeviceName());
    model.setObject("modbusrtu");
    model.setVersion(1);
    model.setValue(result);
    emit upload(model);
}



void Exam1::onError(QModbusDevice::Error e)
{
    QModbusReply *reply=qobject_cast<QModbusReply *>(sender());
    Logger::logger->error(reply->errorString().toStdString());
}


