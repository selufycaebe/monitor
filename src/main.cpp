#include <QCoreApplication>

#include "config/RegisterConfig.h"
#include "serialbus/modbusrtu.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //初始化日志实例
    Logger::init();
    //注册采集设备相关类到元对象系统中
    Cfg::ClassRegisterInit();
    //读取串口配置 并进行初始化 打开串口 开始读取
    ModbusRtuManager m;
    m.init();

    return a.exec();
}
