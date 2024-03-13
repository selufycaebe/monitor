cmake_minimum_required(VERSION 3.14)

project(monitor LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#------------------------------------------------------------------------
find_package(nlohmann_json CONFIG REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core SerialBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialBus)
find_package(spdlog CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
#------------------------------------------------------------------------
#设置可执行文件的源文件
set(HEADERS
    serialbus/modbusrtu.h serialbus/modbusrtu.cpp
    config/config.h
    logger.h logger.cpp
    enums.h
    serialbus/modbusrtucontext.h serialbus/modbusrtucontext.cpp
    devices/deviceinter.h
    devices/exam1.h devices/exam1.cpp
    devices/utils.h devices/utils.cpp
    config/registerconfig.h
    services/uploadservice.h services/uploadservice.cpp
    services/modbusmodel.h
)
#设置link库文件
set(LIBS
    Qt${QT_VERSION_MAJOR}::Core
    Qt6::SerialBus
    nlohmann_json::nlohmann_json
    spdlog::spdlog_header_only
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
#------------------------------------------------------------------------
#可执行程序
add_executable(monitor
    main.cpp
    ${HEADERS}
)

target_link_libraries (monitor
    PUBLIC
    ${LIBS}
)
#------------------------------------------------------------------------
#测试库
add_library(monitorlib STATIC ${HEADERS})
target_link_libraries(monitorlib
    PUBLIC
    ${LIBS}
)
#------------------------------------------------------------------------
configure_file(../config/config.json ${CMAKE_CURRENT_BINARY_DIR}/config/config.json COPYONLY)

include(GNUInstallDirs)
install(TARGETS monitor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
