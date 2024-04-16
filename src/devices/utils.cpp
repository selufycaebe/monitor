#include "utils.h"
#include "../logger.h"
#include "qdatetime.h"
#include <QTimeZone>
std::map<std::string, Utils::DataType> Utils::dataType = {
        {"bit", DataType::BIT},
        {"char", DataType::CHAR},
        {"uint16", DataType::UINT16},
        {"int16", DataType::INT16},
        {"uint32big", DataType::UINT32Big},
        {"uint32little", DataType::UINT32Little},
        {"uint32big_s", DataType::UINT32Big_S},
        {"uint32little_s", DataType::UINT32Little_S},
        {"int32big", DataType::INT32Big},
        {"int32little", DataType::INT32Little},
        {"int32big_s", DataType::INT32Big_S},
        {"int32little_s", DataType::INT32Little_S},
        {"uint64big", DataType::UINT64Big},
        {"uint64little", DataType::UINT64Little},
        {"uint64big_s", DataType::UINT64Big_S},
        {"uint64little_s", DataType::UINT64Little_S},
        {"int64big", DataType::INT64Big},
        {"int64little", DataType::INT64Little},
        {"int64big_s", DataType::INT64Big_S},
        {"int64little_s", DataType::INT64Little_S},
        {"floatbig", DataType::FLOATBig},
        {"floatlittle", DataType::FLOATLittle},
        {"floatbig_s", DataType::FLOATBig_S},
        {"floatlittle_s", DataType::FLOATLittle_S},
        {"doublebig", DataType::DOUBLEBig},
        {"doublelittle", DataType::DOUBLELittle},
        {"doublebig_s", DataType::DOUBLEBig_S},
        {"doublelittle_s", DataType::DOUBLELittle_S}
};


Utils::Utils()
{
}

double Utils::getResult(DataType type,const QList<quint16>& values,int bitOrCharIndex) {
    if(values.isEmpty()) {
        Logger::logger->error("getResult values数组为空");
        throw std::runtime_error("getResult values数组为空");
    }

    double result = -1;

    switch(type) {
    case DataType::BIT: //读取一位
        result= (double)((values.at(0)>>bitOrCharIndex) & 1 );
        break;
    case DataType::CHAR: //读取一个字节
        if(bitOrCharIndex == 0) {
            result = (double) (values.at(0) & 0x0FF);
        }
        if(bitOrCharIndex == 1) {
            result = (double) ((values.at(0)>>8)&0xFF);
        }
        break;
        //不管是大端还是小端 计算机地址的存储和读取顺序都是从 低地址向高地址存储,现代计算机都是小端模式,嵌入式很多大端
    case DataType::UINT16:  //D1 D0 大端就是假设0xABCD 地址n存AB 地址n+1 存CD  低地址存高位AB 高地址存低位
        result= values.at(0);
        break;
    case DataType::INT16:  //D1 D0 大端就是假设0xABCD 地址n存AB 地址n+1 存CD  低地址存高位AB 高地址存低位
        result= static_cast<int16_t>(values.at(0));
        break;
        //--------------------------------------------------------------------
    case DataType::UINT32Big: //D3,D2,D1,D0
    {
        quint32 iBuf = (values.at(0) << 16) + values.at(1);
        result = (double) iBuf;
    }
    break;
    case DataType::UINT32Little://D1,D0,D3,D2  32位
    {
        quint32 iBuf = (values[1] << 16) + values[0];
        result = (double) iBuf;
    }
    break;
    case DataType::UINT32Big_S: //D2,D3,D0,D1
    {
        quint16 hs = (values.at(0) >> 8) + (values.at(0)<<8);
        quint16 ls = (values.at(1) >> 8) + (values.at(1)<<8);
        quint32 iBuf = (hs << 16) + ls;
        result = (double) iBuf;
    }
    break;
    case DataType::UINT32Little_S://D0,D1,D2,D3  32位
    {
        quint16 hs = (values.at(1) >> 8) + (values.at(1)<<8) ;
        quint16 ls = (values.at(0) >> 8) + (values.at(0)<<8) ;
        quint32 iBuf = (hs << 16) + ls;
        result = (double) iBuf;
    }
    break;
        //--------------------------------------------------------------------
    case DataType::INT32Big: //D3,D2,D1,D0
    {
        quint32 iBuf = (values.at(0) << 16) + values.at(1);
        result = static_cast<int16_t>(iBuf);
    }
    break;
    case DataType::INT32Little://D1,D0,D3,D2  32位
    {
        quint32 iBuf = (values[1] << 16) + values[0];
        result = static_cast<int16_t>(iBuf);
    }
    break;
    case DataType::INT32Big_S: //D2,D3,D0,D1
    {
        quint16 hs = (values.at(0) >> 8) + (values.at(0)<<8);
        quint16 ls = (values.at(1) >> 8) + (values.at(1)<<8);
        quint32 iBuf = (hs << 16) + ls;
        result = static_cast<int16_t>(iBuf);
    }
    break;
    case DataType::INT32Little_S://D0,D1,D2,D3  32位
    {
        quint16 hs = (values.at(1) >> 8) + (values.at(1)<<8) ;
        quint16 ls = (values.at(0) >> 8) + (values.at(0)<<8) ;
        quint32 iBuf = (hs << 16) + ls;
        result = static_cast<int16_t>(iBuf);
    }
    break;
        //--------------------------------------------------------------------
    case DataType::UINT64Big: //D7 D6 D5 D4 D3 D2 D1 D0
    {
        quint32 iBufh = (values.at(0)<<16) + values.at(1);
        quint32 iBufl = (values.at(2)<<16) + values.at(3);
        result = double((uint64_t)iBufh<<32) + iBufl;
    }
    break;
    case DataType::UINT64Little: //D1 D0 D3 D2 D5 D4 D7 D6
    {
        quint32 iBufh = (values.at(3)<<16) + values.at(2);
        quint32 iBufl = (values.at(1)<<16) + values.at(0);
        result = double((uint64_t)iBufh<<32) + iBufl;
    }
    break;
    case DataType::UINT64Big_S: //D6 D7 D4 D5 D2 D3 D0 D1
    {
        quint16 hs1 = (values.at(0) >> 8) + (values.at(0)<<8) ; //D6 D7
        quint16 hs2 = (values.at(1) >> 8) + (values.at(1)<<8) ; // D4 D5
        quint16 ls1 = (values.at(2) >> 8) + (values.at(2)<<8) ; //D2 D3
        quint16 ls2 = (values.at(3) >> 8) + (values.at(3)<<8) ; //D0 D1
        quint32 iBufh = (hs1<<16) + hs2;
        quint32 iBufl = (ls1<<16) + ls2;
        result = double(((uint64_t)iBufh<<32) + iBufl);
    }
    break;
    case DataType::UINT64Little_S: //D0 D1 D2 D3 D4 D5 D6 D7
    {
        quint16 hs1 = (values.at(3) >> 8) + (values.at(3)<<8) ; //D0 D1
        quint16 hs2 = (values.at(2) >> 8) + (values.at(2)<<8) ; //D2 D3
        quint16 ls1 = (values.at(1) >> 8) + (values.at(1)<<8) ; //D4 D5
        quint16 ls2 = (values.at(0) >> 8) + (values.at(0)<<8) ; //D6 D7
        quint32 iBufh = (hs1<<16) + hs2;
        quint32 iBufl = (ls1<<16) + ls2;
        result = double((uint64_t)iBufh<<32) + iBufl;
    }
    break;
        //--------------------------------------------------------------------
    case DataType::INT64Big: //D7 D6 D5 D4 D3 D2 D1 D0
    {
        quint32 iBufh = (values.at(0)<<16) + values.at(1);
        quint32 iBufl = (values.at(2)<<16) + values.at(3);
        result = static_cast<int64_t>(((uint64_t)iBufh<<32) + iBufl);
    }
    break;
    case DataType::INT64Little: //D1 D0 D3 D2 D5 D4 D7 D6
    {
        quint32 iBufh = (values.at(3)<<16) + values.at(2);
        quint32 iBufl = (values.at(1)<<16) + values.at(0);
        result = static_cast<int64_t>(((uint64_t)iBufh<<32) + iBufl);
    }
    break;
    case DataType::INT64Big_S: //D6 D7 D4 D5 D2 D3 D0 D1
    {
        quint16 hs1 = (values.at(0) >> 8) + (values.at(0)<<8) ; //D6 D7
        quint16 hs2 = (values.at(1) >> 8) + (values.at(1)<<8) ; // D4 D5
        quint16 ls1 = (values.at(2) >> 8) + (values.at(2)<<8) ; //D2 D3
        quint16 ls2 = (values.at(3) >> 8) + (values.at(3)<<8) ; //D0 D1
        quint32 iBufh = (hs1<<16) + hs2;
        quint32 iBufl = (ls1<<16) + ls2;
        result = static_cast<int64_t>(((uint64_t)iBufh<<32) + iBufl);
    }
    break;
    case DataType::INT64Little_S: //D0 D1 D2 D3 D4 D5 D6 D7
    {
        quint16 hs1 = (values.at(3) >> 8) + (values.at(3)<<8) ; //D0 D1
        quint16 hs2 = (values.at(2) >> 8) + (values.at(2)<<8) ; //D2 D3
        quint16 ls1 = (values.at(1) >> 8) + (values.at(1)<<8) ; //D4 D5
        quint16 ls2 = (values.at(0) >> 8) + (values.at(0)<<8) ; //D6 D7
        quint32 iBufh = (hs1<<16) + hs2;
        quint32 iBufl = (ls1<<16) + ls2;
        result = static_cast<int64_t>(((uint64_t)iBufh<<32) + iBufl);
    }
    break;
        //--------------------------------------------------------------------
    case DataType::FLOATBig:  //32位  //D3,D2,D1,D0
        // 将32位整数的位模式解释为浮点数
        {
            float r = 0;
            uint32_t iBuf = (values[0] << 16) + values[1];
            std::memcpy(&r, &iBuf, sizeof(r));
            result = r;
        }
        break;
    case DataType::FLOATLittle: //D1,D0,D3,D2  32位
    {
        float r = 0;
        quint32 iBuf = (values[1] << 16) + values[0];
        std::memcpy(&r, &iBuf, sizeof(r));
        result = r;
    }
    break;

    case DataType::FLOATBig_S:  //32位  D2,D3,D0,D1
        // 将32位整数的位模式解释为浮点数
        {
            float r = 0;
            quint16 hs = (values.at(0) >> 8) + (values.at(0)<<8);
            quint16 ls = (values.at(1) >> 8) + (values.at(1)<<8);
            quint32 iBuf = (hs << 16) + ls;
            std::memcpy(&r, &iBuf, sizeof(r));
            result = r;
        }
        break;
    case DataType::FLOATLittle_S: //D0,D1,D2,D3  32位
    {
        float r = 0;
        quint16 hs = (values.at(1) >> 8) + (values.at(1)<<8) ;
        quint16 ls = (values.at(0) >> 8) + (values.at(0)<<8) ;
        quint32 iBuf = (hs << 16) + ls;
        std::memcpy(&r, &iBuf, sizeof(r));
        result = r;
    }
    break;
        //--------------------------------------------------------------------
    case DataType::DOUBLEBig: //D7 D6 D5 D4 D3 D2 D1 D0
    {
        quint32 iBufh = (values.at(0)<<16) + values.at(1);
        quint32 iBufl = (values.at(2)<<16) + values.at(3);
        quint64 buf = ((quint64)iBufh<<32) + iBufl;
        //result = int64ToDouble(buf);
        std::memcpy(&result,&buf,sizeof(result));
    }
    break;
    case DataType::DOUBLELittle: //D1 D0 D3 D2 D5 D4 D7 D6
    {
        quint32 iBufh = (values.at(3)<<16) + values.at(2);
        quint32 iBufl = (values.at(1)<<16) + values.at(0);
        quint64 buf = ((quint64)iBufh<<32) + iBufl;
        std::memcpy(&result,&buf,sizeof(result));
    }
    break;
    case DataType::DOUBLEBig_S: //D6 D7 D4 D5 D2 D3 D0 D1
    {
        quint16 hs1 = (values.at(0) >> 8) + (values.at(0)<<8) ; //D6 D7
        quint16 hs2 = (values.at(1) >> 8) + (values.at(1)<<8); // D4 D5
        quint16 ls1 = (values.at(2) >> 8) + (values.at(2)<<8) ; //D2 D3
        quint16 ls2 = (values.at(3) >> 8) + (values.at(3)<<8) ; //D0 D1
        quint32 iBufh = (hs1<<16) + hs2;
        quint32 iBufl = (ls1<<16) + ls2;
        quint64 buf = ((quint64)iBufh<<32) + iBufl;
        std::memcpy(&result,&buf,sizeof(result));
    }
    break;
    case DataType::DOUBLELittle_S: //D0 D1 D2 D3 D4 D5 D6 D7
    {
        quint16 hs1 = (values.at(3) >> 8) + (values.at(3)<<8) ; //D0 D1
        quint16 hs2 = (values.at(2) >> 8) + (values.at(2)<<8) ; //D2 D3
        quint16 ls1 = (values.at(1) >> 8) + (values.at(1)<<8) ; //D4 D5
        quint16 ls2 = (values.at(0) >> 8) + (values.at(0)<<8) ; //D6 D7
        quint32 iBufh = (hs1<<16) + hs2;
        quint32 iBufl = (ls1<<16) + ls2;
        quint64 buf = ((quint64)iBufh<<32) + iBufl;
        std::memcpy(&result,&buf,sizeof(result));
    }
    break;
    default:
        Logger::logger->error("Modbus DataType error,result={}",-1);
        break;
    }
    return result;
}

//检查是否为16进制
int Utils::checkHex(const std::string& input)
{
    if(input.substr(0,2)=="0x")
        return std::stoi(input,nullptr,16);
    return std::stoi(input);
}

int Utils::getCurrentTimeStamp()  {
    return QDateTime::currentSecsSinceEpoch();
}
