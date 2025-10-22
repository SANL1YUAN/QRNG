#include <iostream>
#include <string>

#include <QRNG.h>
#include <thread.h>
#include <serial.h>

#if defined(_WIN32)
#include <Windows.h>
#include <iphlpapi.h>
#endif

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC 1
#include <crtdbg.h>
#endif
using namespace std;

void test_callback();
void test_serial();
void test_QRNG();
int main(int argc, char *argv[]) 
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_LEAK_CHECK_DF);
#endif
//	test_callback();
//	test_serial();
	test_QRNG();
#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif
    return 0;
}

static void callback(QRNG_event event)
{
    cout << "event: " << QRNG_status(event) << endl;
	QRNG_release_event(event);
}

void test_callback()
{
	QRNG_context context = QRNG_init();

#if defined(_WIN32)
	if (QRNG_set_debug_address(context, "COM3"))
#else
	if (QRNG_set_debug_address(context, "/dev/ttyUSB0"))
#endif 
	{
		cout << "打开串口失败" << endl;
		QRNG_release(context);
		return;
	}

	for (int i = 0; i < 10000; ++i) {
		QRNG_event event = NULL;
		cout << "Send a command" << endl; 
		QRNGD_data_output_port_control(context, true, false, &event, callback);
	}
    Thread::msleep(1000);
    QRNG_release(context);
}

void test_serial()
{
	char buffer[4096];
	Serial serial("COM3");
	serial.setup(115200);
	serial.open();
	while (1){
		size_t len = serial.receive(buffer, 4095);
		printf("recieve a message: %llu\n", (unsigned long long)len);
	}
}

int print_menu()
{
	int input;
	cout << "0、退出" << endl;
    cout << "1、数据输出接口控制" << endl;
    cout << "2、随机数输出控制" << endl;
    cout << "3、设备复位" << endl;
    cout << "4、接收数据" << endl;

    cout << "5、数据输出接口控制（DEBUG）" << endl;
    cout << "6、随机数输出控制（DEBUG）" << endl;
    cout << "7、设备复位（DEBUG）" << endl;
    cout << "8、MAC地址设置（DEBUG）" << endl;
    cout << "9、网络协议号设置（DEBUG）" << endl;
    cout << "10、特征码设置（DEBUG）" << endl;
    cout << "11、网络信息查询（DEBUG）" << endl;
    cout << "12、硬件复位" << endl;
	cout << "选择：";
    cin >> input;
    return input;
}

QRNG_context init_menu()
{
    string debug;
    string mac;
    string adapterName;
	unsigned short control_protocol; // , data_protocol;
    QRNG_context context = QRNG_init();

    cout << "输入调试接口设备名称：" ;
    cin >> debug;

	if (QRNG_set_debug_address(context, debug.c_str())) {
		cout << "打开串口失败" << endl;
	}

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

		cout << "输协议号（控制协议）:";
		cin >> control_protocol;

	}
	else if(portType == 1) { //NOTE 暂不支持使用USB发送控制指令
		adapterName = "ttyusb";
		cout << "输入设备VID：";
		cin >> mac;

		cout << "输入设备PID：";
		cin >> control_protocol;
	}
	else {
		cout << "不支持的设备类型。" << std::endl;
		QRNG_release(context); 
		return NULL;
	}

	if (QRNG_set_user_address(context, adapterName.c_str(), mac.c_str(), control_protocol) != QRNG_SUCCESS){
		cout << "打开失败" << endl;
		QRNG_release(context);
		return NULL;
	}

    return context;
}
void data_output_port_control(QRNG_context context, bool debug)
{
    BOOL eGe, lGe;
	string input;
	QRNG_event event = NULL;

    cout << "使用GE电口（y/N）：";
    cin >> input;
    if(input == "N" || input == "n"){
        eGe = FALSE;
    }
    else{
        eGe = TRUE;
    }

    cout << "使用GE广口（y/N）：";
    cin >> input;
    if(input == "N" || input == "n"){
		lGe = FALSE;
    }
    else{
		lGe = TRUE;
    }

    if(debug){
		QRNGD_data_output_port_control(context, eGe, lGe, &event, NULL);
    }
    else{
		QRNG_data_output_port_control(context, eGe, lGe, &event, NULL);
    }
    if(QRNG_wait(event) != QRNG_SUCCESS) {
        cout << "失败" << endl;
    }
    else {
        cout << "成功" << endl;
    }
    QRNG_release_event(event);

}

void data_output_control(QRNG_context context, bool debug)
{
	BOOL output;
	int input;
	QRNG_event event = NULL;

	cout << "1、打开随机数输出" << endl;
	cout << "2、关闭随机数输出" << endl;
	cout << "输入：";

	cin >> input;
	if (input == 1) {
		output = TRUE;
	}
	else {
		output = FALSE;
	}

	string keyCode;
	cout << "输入特征码：";
	cin >> keyCode;
	
	if (keyCode.length() != 6) {
		cout << "特征码错误" << endl;
		return;
	}

	if (debug){
		QRNGD_random_data_output_control(context, output, keyCode.c_str(), &event, NULL);
	}
	else {
		QRNG_random_data_output_control(context, output, keyCode.c_str(), &event, NULL);
	}

	if (QRNG_wait(event) != QRNG_SUCCESS) {
		cout << "失败" << endl;
	}
	else {
		cout << "成功" << endl;
	}
	QRNG_release_event(event);
}

void device_reset(QRNG_context context, bool debug)
{
	QRNG_event event = NULL;

	if (debug){
		QRNGD_device_reset(context, &event, NULL);
	}
	else {
		QRNG_device_reset(context, &event, NULL);
	}

	if (QRNG_wait(event) != QRNG_SUCCESS) {
		cout << "失败" << endl;
	}
	else {
		cout << "成功" << endl;
	}
	QRNG_release_event(event);
}

void set_mac_address(QRNG_context context)
{
	QRNG_event event = NULL;
	string local;
	string target;

	cout << "输入设备MAC地址：";
	cin >> local;

	cout << "输入接收端MAC地址：";
	cin >> target;


	QRNGD_set_mac_address(context, local.c_str(), target.c_str(), &event, NULL);

	if (QRNG_wait(event) != QRNG_SUCCESS) {
		cout << "失败" << endl;
	}
	else {
		cout << "成功" << endl;
	}
	QRNG_release_event(event);
}

void set_protocol(QRNG_context context)
{
	QRNG_event event = NULL;
	unsigned short control;
	unsigned short data;

	cout << "输入控制协议号：";
	cin >> control;

	cout << "输入数据协议号：";
	cin >> data;


	QRNGD_set_protocol_number(context, control, data, &event, NULL);

	if (QRNG_wait(event) != QRNG_SUCCESS) {
		cout << "失败" << endl;
	}
	else {
		cout << "成功" << endl;
	}
	QRNG_release_event(event);
}

void set_key(QRNG_context context)
{
	string key;
	cout << "输入特征码：";
	cin >> key;
	if (key.length() == 6) {
		if (QRNGD_set_key(context, key.c_str(), NULL, NULL)) {
			cout << "失败" << endl;
		}
		else {
			cout << "成功" << endl;
		}
	}
}

void network_info_query(QRNG_context context)
{
	QRNG_event event = NULL;

	QRNG_network info = NULL;
	if (QRNGD_get_network_info(context, &info, &event, NULL) == QRNG_SUCCESS) {
		if (QRNG_wait(event) != QRNG_SUCCESS) {
			cout << "失败" << endl;
		}
		else {
			cout << "成功" << endl;
			unsigned char *address = info->target_addr;
			printf("TARGET: %02X-%02X-%02X-%02X-%02X-%02X \n", 
				(int)address[0],address[1],address[2],address[3],address[4],address[5]);
			address = info->local_addr;
			printf("LOCAL: %02X-%02X-%02X-%02X-%02X-%02X \n",
				(int)address[0],address[1],address[2],address[3],address[4],address[5]);

			printf("CONTROL: %04X\n",info->user_protocol);
			printf("DATA: %04X\n",info->data_protocol);
		}
	}
	QRNG_release_event(event);
}

void test_QRNG()
{
	QRNG_context context = init_menu();

    int command;

    do{
        command = print_menu();
        switch (command){
            case 1:
                data_output_port_control(context, false);
                break;
			case 2:
				data_output_control(context, false);
				break;
            case 3:
				device_reset(context, false);
				break;
            case 4:
				//todo
				break;
			case 5:
				data_output_port_control(context, true);
				break;
			case 6:
				data_output_control(context, true);
				break;
			case 7:
				device_reset(context, true);
				break;
            case 8:
				set_mac_address(context);
				break;
            case 9:
				set_protocol(context);
				break;
            case 10:
				set_key(context);
				break;
            case 11:
				network_info_query(context);
				break;
            case 12:
				QRNGD_hardware_reset(context);
                break;
            default:
                break;
        }
    } while (command != 0);

    QRNG_release(context);
}