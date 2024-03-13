#ifndef UPLOADSERVICE_H
#define UPLOADSERVICE_H

#include <QNetworkAccessManager>
#include "modbusmodel.h"
class UploadService
{
public:
    UploadService();
    void uploadNetwork1(const ModbusModel &model);
private:
    QNetworkAccessManager m_manager;
};

class UploadServiceInstance{
public:
    UploadServiceInstance()=default;
    static UploadService *getInstance();
private:
    inline static UploadService *m_service=nullptr;
};
#endif // UPLOADSERVICE_H
