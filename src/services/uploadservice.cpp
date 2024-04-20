#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <exception>
#include <iostream>
#include "fmt/core.h"
#include "nlohmann/json_fwd.hpp"
#include "qcoreapplication.h"
#include "qdatetime.h"
#include "qglobal.h"
#include "replymodel.h"
#include "../logger.h"
#include "uploadservice.h"
#include "modbusuploadmodel.h"

UploadService::UploadService() {
    //这个好像并不起作用,内存还是会一直增加 需要手动进行delete reply指针
    m_manager.setAutoDeleteReplies(true);
}

UploadService *UploadServiceInstance::getInstance() {
    if (m_service == nullptr) {
        m_service = new UploadService();
    }
    return m_service;
}

//-----------------------------------------------------------
void UploadService::serialUploadNetwork(const ModbusModel &model) {
    //案例1
    QNetworkRequest request;
    QUrl url{"http://192.168.31.128:15555"};
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /**
     *  device_name:"exam1",
     *  id_name:"温度",
     *  value:22.2
     *  timestamp:xlsx
     */
    ModbusUploadModel m;
    m.setTimestamp(QDateTime::currentSecsSinceEpoch());
    m.setDeviceId(model.getDeviceName());
    std::map<std::string, std::string> t;
    for (const auto &[k, v]: model.getNameAddress()) {
        try {
            t[v] = fmt::format("{}", model.getValues().at(k));
        } catch (std::exception &e) {
            Logger::logger->error(R"(config.json's "request_paras" and deviceparams.json's "params" do not match;
error.what:{})", e.what());
            qApp->quit();
        }
    }
    m.setResults(t);

    nlohmann::json j;
    model::to_json(j, m);
    auto body = j.dump();
    Logger::logger->info(body);
    auto reply = m_manager.post(request, QByteArray::fromStdString(body));
    std::string deviceName = model.getDeviceName();

    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, deviceName]() {
        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            return;
        }
        auto m = reply->readAll();
        if (m.isEmpty()) {
            reply->deleteLater();
            qDebug() << "empty reply";
            return;
        }
        ReplyModel replyModel;
        auto j = m.toStdString();
        std::cout << j << std::endl;
        Logger::logger->info("readData:{}", j);
        try {
            nlohmann::json json = j;
            model::from_json(json, replyModel);
        } catch (nlohmann::json::exception &e) {
            Logger::logger->info("json parse error:{}", e.what());
        }
        if (replyModel.getCode()== 200) {
            Logger::logger->info("{} upload successfully", deviceName);
        } else {
            Logger::logger->error("{} upload fail:{}", deviceName, replyModel.getMessage());
        }
        reply->deleteLater();
    }, Qt::QueuedConnection);
    QObject::connect(reply, &QNetworkReply::errorOccurred, reply, [reply, deviceName](QNetworkReply::NetworkError e) {
        Logger::logger->error("{}上传服务器超时...!code:{}", deviceName, std::to_string(e));
        reply->deleteLater();
    }, Qt::QueuedConnection);
}