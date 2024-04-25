#include <iostream>
#include "config/registerconfig.h"
#include "config/config.h"
#include "config/deviceparams.h"
#include "devices/utils.h"
#include "qcoreapplication.h"
#include "qmap.h"
#include "serialbus/modbusrtu.h"
#include "services/uploadservice.h"
#include "logger.h"
#include "serialbus/middleware.h"
#include <ostream>
#include <signal.h>

#ifdef DEBUG_MODE
#include <vld.h>
#endif

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
    //...
    if (config.getVersion() == 1) {
        //添加logger的一些提示记录
        m.addGlobalPreHandler(middleware::setModbusLogger);
        m.addGlobalEndHandler(middleware::setModbusLoggerEnd);
        m.addGlobalEndHandler(middleware::isOnline);
        m.addErrorHandler(middleware::isOnline);
        if (deviceParam.getVersion() == 1) {
            auto params = deviceParam.getParams();
            //把各地址对应的值的含义读取出来,用于上传的时候进行标注
            for (size_t i = 0; i < params.size(); i++) {
                QMap<int, std::string> t; // <地址,代表的含义>
                if (params[i].getProtocol()=="modbus") {
                    auto param = params[i];
                    for (const auto &[k, v]: param.getParams()) {
                        t[Utils::checkHex(k)] = v;
                    }
                    //端口,设备地址01等,这个设备一共读取多少个寄存器(16bit),<地址,代表的含义>
                    m.setSizeAndName(param.getPortname(), param.getAddress(), param.getParams().size(), t);
                }
            }
        }
        //进行打开串口,打开定时器等配置
        m.init();
        //... 其它的如can管理类,mqtt等.(都没有实现)
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
    delete UploadServiceInstance::getInstance();
}

//在使用ctrl+C的时候主动发出quit信号
void signalHandler(int signal) {
    if (signal == SIGINT) {
        qDebug() << "Ctrl+C detected, performing cleanup operations...";
        QCoreApplication::quit(); // 退出应用程序
    }
}
