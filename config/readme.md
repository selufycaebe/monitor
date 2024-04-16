## 字段含义
- **version**:`int`  **默认值为1**,代表当前config配置的版本
- **loglevel**:`string` warn,代表打印的等级,低于当前值不打印
- **printlevel**:`string` info,同上
  - >   可选列表:  由低到高 \
        trace \
        debug \
        info \
        warn \
        err \
        critical \
        off  (关闭)  
- **server**:{} 
  - **os**:`int` 0,  0:window 1:linux 
  - **typeid**:`int` 0, 0:ModbusRtu  其他还没有
  - **port_name**:`string` "COM2",  端口名称
  - **baud_rate**:`int` 9600, 波特率
  - **data_bits**:`int` 8,    数据位
  - **stop_bits**:`int` 1,    停止位
  - **parity**:`int` 0,       奇偶校验
    - >输入数字,前面是数字对应的检验解释
       NoParity     0 \
       EvenParity   2 \
       OddParity    3 \
       SpaceParity  4 \
       MarkParity   5 
  - **flow_control**:`int` 0, 流控制
    - > NoFlowControl    0 \
        HardwareControl  1 \
        SoftwareControl  2 
  - **interval_ms**:`int` 5000,   每次间隔多少ms把所有的请求发送出去
  - **request_paras**:[] 请求参数 
    - **[description]**:`string` uint32big类型, 对当前列表参数的一个简单描述(**可空,可以没有这个字段**)
    - **data_type**:`string` uint32big, 当前请求的数据 需要按照哪种数据类型进行读取
    - **class_name**:`string` Exam1, 处理这个请求数据的类(C++代码中的类名)
    - **device_name**:`string` exam1, 设备的名称 设备名称+类名称 可以用来区分同一种设备但是有多个可以采集的情况
    - **address**:`string` 01, 请求设备的地址
    - **func_code**:`string` 03, 请求的功能码
    - **start_address**:`string` 0000, 请求的起始地址
    - **count_or_data**:`string` 0002 请求的数据,或者当请求的功能码是写入时,需要

