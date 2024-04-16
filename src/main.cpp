#include <iostream>
#include "config/RegisterConfig.h"
#include "config/config.h"
#include "config/deviceparams.h"
#include "devices/deviceinter.h"
#include "devices/exam1.h"
#include "devices/utils.h"
#include "global.h"
#include "qcoreapplication.h"
#include "qmap.h"
#include "serialbus/modbusrtu.h"
#include "services/uploadservice.h"
#include "logger.h"
#include "serialbus/middleware.h"
#include <ostream>
#include <signal.h>
#include <vector>

// 信号处理函数
void signalHandler(int signal);

void quit();

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, quit);

    signal(SIGINT, signalHandler);
    //初始化日志实例
    Logger::init();
    //注册采集设备相关类到元对象系统中
    cfg::ClassRegisterInit();
    //配置上传服务端
    UploadServiceInstance::getInstance();
    //读取串口配置 并进行初始化 打开串口 开始读取
    auto config = cfg::readConfig();
    auto deviceParam = cfg::readDeviceParams();
    //开始的设备
    ModbusRtuManager m;
    if (config.getVersion() == 1) {
        m.addGlobalPreHandler(middleware::setModbusLogger);
        m.addGlobalEndHandler(middleware::setModbusLoggerEnd);
        if (deviceParam.getVersion() == 1) {
            auto params = deviceParam.getParams();
            for (int i = 0; i < params.size(); i++) {
                QMap<int, std::string> t;
                if (params[i].getProtocol()=="modbus") {
                    auto param = params[i];
                    for (const auto &[k, v]: param.getParams()) {
                        t[Utils::checkHex(k)] = v;
                    }
                    m.setSizeAndName(param.getPortname(), param.getAddress(), param.getParams().size(), t);
                }
            }
        }
        m.init();
        //...
        // 有多少的功能管理类都在这进行初始化,配置文件没有的部分会不执行其相关函数
    }
    return a.exec();
}

void quit() {
    std::cout << "waiting quit..." << std::endl;
    //删掉所有的logger记录器
    spdlog::get("global")->flush();
    spdlog::shutdown();
    spdlog::drop_all();
}

//在使用ctrl+C的时候主动发出quit信号
void signalHandler(int signal) {
    if (signal == SIGINT) {
        qDebug() << "Ctrl+C detected, performing cleanup operations...";
        QCoreApplication::quit(); // 退出应用程序
    }
}
