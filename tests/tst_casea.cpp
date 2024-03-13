
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../src/devices/utils.h"

using namespace testing;

TEST(ModbusUtils, CaseBit)
{
    Utils u;
    auto r1 = u.getResult(Utils::DataType::BIT,{0x02},1); // 二进制 010
    auto r0 = u.getResult(Utils::DataType::BIT,{0x02},0);
    EXPECT_EQ(r1,1.0);
    EXPECT_EQ(r0,0);
}

TEST(ModbusUtils, CaseChar)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::CHAR,{0xFFCA},1); // 高字节FF
    auto l = u.getResult(Utils::DataType::CHAR,{0xFFCA},0); // 低字节CA
    EXPECT_EQ(h, 0xff);
    EXPECT_EQ(l, 0xca);
}

TEST(ModbusUtils, CaseUInt)
{
    Utils u;
    uint16_t h = u.getResult(Utils::DataType::UINT16,{0xFFCA,0xFFCB});//0xFFCA
    EXPECT_EQ(h, 0xFFCA);
}

TEST(ModbusUtils, CaseInt)
{
    Utils u;
    int16_t h = u.getResult(Utils::DataType::INT16,{0xFFCA,0xFFCB});//0xFFCA
    EXPECT_EQ(h, (int16_t)0xFFCA);
}

//----------------------UINT32----------------------------------
TEST(ModbusUtils, CaseUInt32Big)
{
    Utils u;
    uint32_t h= u.getResult(Utils::DataType::UINT32Big,{0xFFCA,0x1232});//0xFFCA1232
    EXPECT_EQ(h, 0xFFCA1232);
}

TEST(ModbusUtils, CaseUInt32Little)
{
    Utils u;
    uint32_t h = u.getResult(Utils::DataType::UINT32Little,{0xFFCA,0x1232});//0x1232FFCA
    EXPECT_EQ(h, 0x1232FFCA);
}

TEST(ModbusUtils, CaseUInt32Big_S)
{
    Utils u;
    uint32_t h = u.getResult(Utils::DataType::UINT32Big_S,{0xFFCA,0x1232});//0xCAFF3212
    EXPECT_EQ(h, 0xCAFF3212);
}

TEST(ModbusUtils, CaseUInt32Little_S)
{
    Utils u;
    uint32_t h = u.getResult(Utils::DataType::UINT32Little_S,{0xFFCA,0x1232});//0x3212CAFF
    EXPECT_EQ(h, 0x3212CAFF);
}
//----------------------INT32-----------------------------------
TEST(ModbusUtils, CaseInt32Big)
{
    Utils u;
    uint32_t h= u.getResult(Utils::DataType::UINT32Big,{0xFFCA,0x1232}); //0XFFCA1232
    EXPECT_EQ(h, (uint32_t)0xFFCA1232);
}

TEST(ModbusUtils, CaseInt32Little)
{
    Utils u;
    uint32_t h = u.getResult(Utils::DataType::UINT32Little,{0xFFCA,0x1232}); //0x1232FFCA
    EXPECT_EQ(h, (uint32_t)0x1232FFCA);
}

TEST(ModbusUtils, CaseInt32Big_S)
{
    Utils u;
    uint32_t h = u.getResult(Utils::DataType::UINT32Big_S,{0xFFCA,0x1232});//0xCAFF3212
    EXPECT_EQ(h, (uint32_t)0xCAFF3212);
}

TEST(ModbusUtils, CaseInt32Little_S)
{
    Utils u;
    uint32_t h = u.getResult(Utils::DataType::UINT32Little_S,{0xFFCA,0x1232}); //0x3212CAFF
    EXPECT_EQ(h, (uint32_t)0x3212CAFF);
}
//----------------------UINT64----------------------------------
TEST(ModbusUtils, CaseUINT64Big)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::UINT64Big,{0xFFCA,0x1232,0xCDEF,0x1234}); //0xFFCA1232CDEF1234
    EXPECT_EQ(h, 0xFFCA1232CDEF1234);
}

TEST(ModbusUtils, CaseUINT64Big_S)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::UINT64Big_S,{0xFFCA,0x1232,0xCDEF,0x1234});//0xCAFF3212EFCD3412
    EXPECT_EQ(h, 0xCAFF3212EFCD3412);
}
TEST(ModbusUtils, CaseUINT64Little)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::UINT64Little,{0xFFCA,0x1232,0xCDEF,0x1234});//0x1234CDEF1232FFCA
    EXPECT_EQ(h, 0x1234CDEF1232FFCA);
}
TEST(ModbusUtils, CaseUINT64Little_S)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::UINT64Little_S,{0xFFCA,0x1232,0xCDEF,0x1234});//0x3412EFCD3212CAFF
    EXPECT_EQ(h, 0x3412EFCD3212CAFF);
}
//----------------------INT64----------------------------------
TEST(ModbusUtils, CaseINT64Big)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::INT64Big,{0xFFCA,0x1232,0xCDEF,0x1234});//0xFFCA1232CDEF1234
    EXPECT_EQ(h, (int64_t)0xFFCA1232CDEF1234);
}

TEST(ModbusUtils, CaseINT64Big_S)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::INT64Big_S,{0xFFCA,0x1232,0xCDEF,0x1234});//0xCAFF3212EFCD3412
    EXPECT_EQ(h, (int64_t)0xCAFF3212EFCD3412);
}
TEST(ModbusUtils, CaseINT64Little)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::INT64Little,{0xFFCA,0x1232,0xCDEF,0x1234});//0x1234CDEF1232FFCA
    EXPECT_EQ(h, (int64_t)0x1234CDEF1232FFCA);
}
TEST(ModbusUtils, CaseINT64Little_S)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::INT64Little_S,{0xFFCA,0x1232,0xCDEF,0x1234});//0x3412EFCD3212CAFF
    EXPECT_EQ(h, (int64_t)0x3412EFCD3212CAFF);
}
//----------------------Float----------------------------------
TEST(ModbusUtils, CaseFLOATBig)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::FLOATBig,{0x471C,0x4A00}); //0x471C4A00
    float r=0;
    uint32_t e = 0x471C4A00;
    std::memcpy(&r,&e,sizeof(float));
    EXPECT_TRUE((h-r)< std::numeric_limits<float>::epsilon());
}
TEST(ModbusUtils, CaseFLOATBig_S)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::FLOATBig_S,{0x471C,0x4A00}); //0X1C47004A
    float r=0;
    uint32_t e = 0X1C47004A;
    std::memcpy(&r,&e,sizeof(float));
    EXPECT_TRUE((h-r)< std::numeric_limits<float>::epsilon());

}
TEST(ModbusUtils, CaseFLOATLittle)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::FLOATLittle,{0x471C,0x4A00}); //0X4A00471C
    float r=0;
    uint32_t e = 0X4A00471C;
    std::memcpy(&r,&e,sizeof(float));
    EXPECT_TRUE((h-r)< std::numeric_limits<float>::epsilon());
}
TEST(ModbusUtils, CaseFLOATLittle_S)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::FLOATLittle_S,{0x471C,0x4A00}); //0X004A1C47
    float r=0;
    uint32_t e = 0X004A1C47;
    std::memcpy(&r,&e,sizeof(float));
    EXPECT_TRUE((h-r)< std::numeric_limits<float>::epsilon());
}
//----------------------Double----------------------------------
TEST(ModbusUtils, CaseDoubleBig)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::DOUBLEBig,{0xFFCA,0x1232,0xCDEF,0x1234}); //0xFFCA1232CDEF1234
    double r=0;
    uint64_t e = 0xFFCA1232CDEF1234;
    std::memcpy(&r,&e,sizeof(double));
    EXPECT_TRUE((h-r)< std::numeric_limits<double>::epsilon());
}
TEST(ModbusUtils, CaseDoubleBig_S)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::DOUBLEBig_S,{0xFFCA,0x1232,0xCDEF,0x1234}); //0xCAFF3212EFCD3412
    double r=0;
    uint64_t e = 0xCAFF3212EFCD3412;
    std::memcpy(&r,&e,sizeof(double));
    EXPECT_TRUE((h-r)< std::numeric_limits<double>::epsilon());
}
TEST(ModbusUtils, CaseDOUBLELittle)
{
    Utils u;
    auto h = u.getResult(Utils::DataType::DOUBLELittle,{0xFFCA,0x1232,0xCDEF,0x1234}); //0x1234CDEF1232FFCA
    double r=0;
    uint64_t e = 0x1234CDEF1232FFCA;
    std::memcpy(&r,&e,sizeof(double));
    EXPECT_TRUE((h-r)< std::numeric_limits<double>::epsilon());
}

TEST(ModbusUtils, CaseDOUBLELittle_S)
{
    Utils u;
    double h = u.getResult(Utils::DataType::DOUBLELittle_S,{0xFFCA,0x1232,0xCDEF,0x1234}); //0x3412EFCD3212CAFF
    double r=0;
    uint64_t e = 0x3412EFCD3212CAFF;
    std::memcpy(&r,&e,sizeof(double));
    EXPECT_TRUE((h-r)< std::numeric_limits<double>::epsilon());
}

