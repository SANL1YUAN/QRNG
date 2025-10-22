# QRNG
软件架构
软件开发包初始化的主要工作是创建QRNG_context对象（核心结构体 ），并打开所需的通信接口 。通信接口分为三类 ：



用户接口：使用以太网链路层通信 。主要功能包括设备参数的设置和读取、随机数输出控制、设备复位控制等 。


调试接口：使用串口通信 。功能与用户接口类似，主要用于参数设置、读取和控制 。


数据接口：可以使用以太网链路层通信或USB口通信 。其主要功能是接收随机数 。

安装教程
本开发包支持Windows和Linux平台 。


Windows平台 


头文件：QRNG.h 

连接库文件：QRNG.lib 

运行库文件：QRNG.dll、platform.dll、business.dll 


Linux平台 


头文件：QRNG.h 

运行库文件：libqrng.so 

运行环境：推荐 CentOS 6.6 64位 

使用说明
初始化上下文 调用 QRNG_init() 函数初始化，创建 QRNG_context 对象 。如果失败，将返回 NULL 。


设置接口地址 根据通信方式，设置一个或多个接口地址：


数据接口：调用 QRNG_set_data_address() 。此接口用于设置以太网或USB通道以接收随机数 。




用户接口：调用 QRNG_set_user_address() 设置以太网用户接口 。




调试接口：调用 QRNG_set_debug_address() 设置串口调试接口 。



读取随机数 设置数据接口后 ，调用 QRNG_data_receive() 函数来读取随机数 。



参数 pbuf 是用于存放随机数的缓存 ，rlen 是期望读取的数据长度 。


返回值大于0时，表示实际读取到的随机数长度 。注意：实际获取的数据长度可能会小于请求的 rlen 长度 。


返回值小于0时，表示发生错误 。

释放上下文 使用完毕后，调用 QRNG_release() 释放 QRNG_context 对象 。
