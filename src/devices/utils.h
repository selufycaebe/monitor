#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils
{
public:
    enum class DataType {
        BIT,
        CHAR,
        UINT16,
        INT16,
        UINT32Big,
        UINT32Little,
        UINT32Big_S,
        UINT32Little_S,
        INT32Big,
        INT32Little,
        INT32Big_S,
        INT32Little_S,
        UINT64Big,
        UINT64Little,
        UINT64Big_S,
        UINT64Little_S,
        INT64Big,
        INT64Little,
        INT64Big_S,
        INT64Little_S,
        FLOATBig,
        FLOATLittle,
        FLOATBig_S,
        FLOATLittle_S,
        DOUBLEBig,
        DOUBLELittle,
        DOUBLEBig_S,
        DOUBLELittle_S,
    };
    Utils();
    static double getResult(DataType type,const QList<quint16>& values,int bitOrCharIndex = 0);
    static int checkHex(const std::string& input);
};

#endif // UTILS_H
