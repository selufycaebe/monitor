#include <QNetworkRequest>

#include "uploadservice.h"

UploadService::UploadService() {}

void UploadService::uploadNetwork1(const ModbusModel &model)
{
    //案例1
    QNetworkRequest request;
    QUrl url{"http://hqkserver.ml"};
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    m_manager.get(request);
    qDebug()<<"uploading..."<<model.getValue();
}

UploadService* UploadServiceInstance::getInstance()
{
    if(m_service==nullptr) {
        m_service = new UploadService();
    }
    return m_service;
}
