#ifndef UPLOADSERVICE_H
#define UPLOADSERVICE_H

#include <QNetworkAccessManager>
#include "modbusmodel.h"

//单例
class UploadService;
class UploadServiceInstance{
public:
    UploadServiceInstance()=default;
    static UploadService *getInstance();
private:
    inline static UploadService *m_service=nullptr;
};



class UploadService
{
public:
    UploadService();
    Q_DISABLE_COPY(UploadService)
    void serialUploadNetwork(const ModbusModel& model);
private:
    QNetworkAccessManager m_manager;
};



#endif // UPLOADSERVICE_H
