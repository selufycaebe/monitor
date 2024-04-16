#include <QModbusReply>
#include <QModbusDevice>
#include <optional>

#include "exam1.h"
#include "utils.h"
#include "../logger.h"

Exam1::Exam1() :DeviceInterface("Exam1")
{
}

// void Exam1::sendRequest(std::shared_ptr<ModbusRtuContext> context)
// {
//     //发送请求 处理回复
//     auto client = context->client;
//     int address = Utils::checkHex(context->requestParam.getAddress());
//     auto* reply = client->sendRawRequest(context->request,address);
//     std::string deviceName = context->requestParam.getDeviceName();
//     if(reply==nullptr) {
//         Logger::logger->error(deviceName + "reply指针为空");
//         return;
//     }
//     QObject::connect(reply,&QModbusReply::finished,this,[context, this](){
//         onReadyRead(context);
//         },Qt::SingleShotConnection);

//     QObject::connect(reply,&QModbusReply::errorOccurred,this,[deviceName,this](QModbusDevice::Error e){
//         onError(deviceName,e);
//     },Qt::SingleShotConnection);
// }

std::optional<ModbusModel> Exam1::onReadyRead(std::shared_ptr<ModbusRtuContext> context)
{
    auto logger = Logger::logger;
    QModbusReply *reply=qobject_cast<QModbusReply *>(sender());
    if(!reply) return std::nullopt;
    auto unit = reply->result();
    if(reply->error()!= QModbusDevice::NoError) {
        logger->error( reply->errorString().toStdString());
        return std::nullopt;
    }
    auto results = reply->result().values();
    int address = Utils::checkHex(context->requestParam.getStartAddress());
    //double result= -1;

    //处理数据
    switch(address) {
    case 0:
    {
        uint32_t value =  Utils::getResult(Utils::DataType::UINT32Big,{results.at(0),results.at(1)});
        //logger->info(value); //65538
        m_results[address] = value;
    }
    break;
    case 2:
    {
        uint16_t value =  Utils::getResult(Utils::DataType::UINT16,{results.at(0)});
        m_results[address] = value;
    }
    break;

    case 4:
    {
        double value =  Utils::getResult(Utils::DataType::DOUBLEBig,{results.at(0),results.at(1),results.at(2),results.at(3)});
        m_results[address] = value;
    }
    break;

    case 8:
    {
        float value =  Utils::getResult(Utils::DataType::FLOATBig,{results.at(0),results.at(1)});
        m_results[address] = value;
    }
    break;
    }

    if(m_size==-1) {
        logger->error("deviceAddress:{} size dont set",context->requestParam.getAddress());
        return std::nullopt;
    }
    if(m_results.size() != m_size) return std::nullopt;

    //从机地址: 01-0002:145.5
    //logger->info("address-startAddress:result:: "+context->requestParam.getAddress()+"-"+context->requestParam.getStartAddress()+":"+std::to_string(result));
    logger->info("deviceAddress:{}",context->requestParam.getAddress());
    for(const auto & i:m_results) {
        logger->info("startAddress:{} , value:{}",i.first,i.second);
    }
    ModbusModel model;
    model.setDeviceName(context->requestParam.getDeviceName());
    model.setValues(m_results);
    model.setVersion(1);
    model.setTimestamp(Utils::getCurrentTimeStamp());
    model.setNameAddress(m_NameOfAddress.toStdMap());
    reply->deleteLater();
    m_results.clear();
    return model;
}
