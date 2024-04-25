#ifndef DEVICEINTER_H
#define DEVICEINTER_H

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <QModbusDataUnit>
#include <QMap>
#include <spdlog/spdlog.h>
#include "../services/modbusmodel.h"
#include "../serialbus/modbusrtucontext.h"
#include "spdlog/common.h"

class DeviceInterface : public QObject {
Q_OBJECT
public:
    explicit DeviceInterface(const std::string &name);

    virtual void sendRequest(std::shared_ptr<ModbusRtuContext> context);

    virtual void setNameOfAddress(const QMap<int, std::string> &names);

    virtual void setSize(int size);

protected:
    void printCustom(spdlog::level::level_enum level,const std::string& information,std::shared_ptr<ModbusRtuContext> context) const;
protected slots:

    virtual void onError(std::shared_ptr<ModbusRtuContext> context, QModbusDevice::Error e);

    virtual std::optional<ModbusModel> onReadyRead(std::shared_ptr<ModbusRtuContext> context) = 0;

signals:

    void startProcessingData(std::shared_ptr<ModbusRtuContext> context);

    void endProcessingData(std::shared_ptr<ModbusRtuContext> context);

    void upload(const ModbusModel &model);

    void handleError(std::shared_ptr<ModbusRtuContext> context);
public:
    std::string deviceName;
protected:
    int m_size = -1;
    QMap<int, std::string> m_NameOfAddress;
    //01 03 00 00(寄存器的起始地址) 00 01 xx xx
    std::map<int, double> m_results; //寄存器的地址_值  //寄存器的地址从0开始算
};


#endif // DEVICEINTER_H
