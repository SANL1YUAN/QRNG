/**
 * Copyright (c), 20015-2016  QuantumCTek. All rights reserved.
 * @file        : QRNG_receive.cpp
 * @project     : QRNGSDK
 * @author      : John 
 * @date        : 2016-04-28
 * @version     : 1.0.1
 * @brief       :
 */

#include <iostream>
#include <string>
#include <cstring>
#include "QRNG.h"

#if defined(_WIN32)
#include <Windows.h>
#include <iphlpapi.h>
#endif

using namespace std;

QRNG_context init_menu();

int main(int argc, char *argv[])
{
    QRNG_context context = init_menu();
    char buffer[4096];
    if(context) {
        cout << "接收文件将被保存在QRNG_receive.data文件中" << endl;
        FILE *file = fopen("QRNG_receive.data", "wb");
        while(true){
			memset(buffer, 0, 4096);
            ssize_t  ret = QRNG_data_receive(context, buffer, 4096);
            if(ret > 0){
                fwrite(buffer, 1, ret, file);
            }
			else {
				//printf("Read failed, reopen.\n");
				QRNG_reopen(context);
			}
        }
    }
    return 0;
}

QRNG_context init_menu()
{
    QRNG_context context = QRNG_init();
		

	string adapterName;
    string mac;
    unsigned short data_protocol;		
		
	cout << "选择接口类型：" << std::endl;
	cout << "1. USB" << std::endl;
	cout << "2. NET" << std::endl;
	cout << "输入:" << std::endl;

	int portType;
	cin >> portType;
	if (portType == 2) {
#if defined(_WIN32)
		std::string adapterNames[24];
		ULONG size = 0;
		PIP_ADAPTER_INFO adapterInfo = NULL;
		ULONG temp = GetAdaptersInfo(adapterInfo, &size);
		adapterInfo = (PIP_ADAPTER_INFO)malloc(size);
		temp = GetAdaptersInfo(adapterInfo, &size);
		int i = 0;
		while (adapterInfo) {
			adapterNames[i++] = adapterInfo->AdapterName;
			cout << i << ". " << adapterInfo->Description << " " << adapterInfo->AdapterName << endl;
			adapterInfo = adapterInfo->Next;
		}

		do{
			cout << "选取一个网络适配器：";
			cin >> i;
		} while (adapterNames[i - 1].empty());
		adapterName = adapterNames[i - 1];
#else
		cout << "输入网络适配器名称：";
		cin >> adapterName;
#endif

		cout << "输入MAC地址：";
		cin >> mac;

		cout << "输协议号（数据协议）:";
		cin >> data_protocol;
	}
	else if (portType == 1) {
		adapterName = "ttyusb";
		cout << "输入设备VID：";
		cin >> mac;

		cout << "输入设备PID：";
		cin >> data_protocol;
	}
	else {
		cout << "不支持的设备类型。" << std::endl;
		QRNG_release(context);
		return NULL;
	}

	if (QRNG_set_data_address(context, adapterName.c_str(), mac.c_str(), data_protocol) != QRNG_SUCCESS){
        cout << "打开失败" << endl;
		QRNG_release(context);
		return NULL;
    }

    return context;
}
