/**
 * Copyright (c), 20015-2016  QuantumCTek. All rights reserved.
 * @file        : QRNG.h
 * @project     : QRNGSDK
 * @author      : John
 * @date        : 2016-04-20
 * @version     : 1.0.1
 * @brief       :
 */
#ifndef __QRNG_H_H
#define __QRNG_H_H

#if defined(_MSC_VER)
#define __export __declspec(dllexport)
#else
#include <unistd.h>
#define __export __attribute__((visibility("default")))
#endif

#if _MSC_VER
#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
#undef ssize_t
#ifdef _WIN64
typedef __int64			 ssize_t;
#else
typedef int				 ssize_t;
#endif
#endif
#endif

typedef int BOOL;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct _QRNG_network
{
	unsigned char local_addr[6];
	unsigned char target_addr[6];
    unsigned short user_protocol;
    unsigned short data_protocol;
};

typedef int QRNG_error;

#define QRNG_SUCCESS 0         /*成功*/
#define QRNG_FAILED -1         /*失败*/
#define QRNG_ERROR -100        /*错误*/
#define QRNG_WAITING -2        /*请求中*/
#define QRNG_TIMEOUT -3        /*请求超时*/
#define QRNG_UNKNOWN -4        /*未知错误*/
#define QRNG_UNINITIALIZED -5  /*上下文未初始化错误*/
#define QRNG_UNCONNECTED -6    /*接口未连接错误*/
#define QRNG_PARAM_ERROR -11   /*参数错误*/
#define QRNG_SEND_ERROR -21    /*指令发送错误*/

typedef struct _QRNG_context*       QRNG_context;
typedef struct _QRNG_event*         QRNG_event;
typedef void                        (*QRNG_event_callback)(QRNG_event);
typedef struct _QRNG_network*       QRNG_network;

QRNG_context __export QRNG_init();
void         __export QRNG_release(QRNG_context context);
void	     __export QRNG_reopen(QRNG_context context);

QRNG_error __export QRNG_set_user_address(QRNG_context context, const char *adapter, const char *address, unsigned short  protocol);
QRNG_error __export QRNG_set_data_address(QRNG_context context, const char *adapter, const char *address, unsigned short  protocol);
QRNG_error __export QRNG_set_debug_address(QRNG_context context, const char *device);

QRNG_error __export QRNG_data_output_port_control(QRNG_context context, BOOL eGbe, BOOL lGbe, QRNG_event *event, QRNG_event_callback cb);
QRNG_error __export QRNG_random_data_output_control(QRNG_context context, BOOL output, const char *key, QRNG_event *event, QRNG_event_callback cb);
QRNG_error __export QRNG_device_reset(QRNG_context context, QRNG_event *event, QRNG_event_callback cb);

ssize_t    __export QRNG_data_receive(QRNG_context context, char *buffer, size_t length);
ssize_t    __export QRNG_data_receive_compat(QRNG_context context, char *buffer, size_t bufLen, size_t length);

QRNG_error __export QRNGD_data_output_port_control(QRNG_context context, BOOL eGBe, BOOL lGbe, QRNG_event *event, QRNG_event_callback cb);
QRNG_error __export QRNGD_random_data_output_control(QRNG_context context, BOOL output, const char *key, QRNG_event *event, QRNG_event_callback cb);

QRNG_error __export QRNGD_set_mac_address(QRNG_context context, const char *local_addr,  const char *target_addr, QRNG_event *event, QRNG_event_callback cb);
QRNG_error __export QRNGD_set_protocol_number(QRNG_context context, unsigned short user_protocol, unsigned short data_protocol, QRNG_event *event, QRNG_event_callback cb);
QRNG_error __export QRNGD_device_reset(QRNG_context context, QRNG_event *event, QRNG_event_callback cb);
QRNG_error __export QRNGD_set_key(QRNG_context context, const char *key, QRNG_event *event, QRNG_event_callback cb);

QRNG_error __export QRNGD_get_network_info(QRNG_context context, QRNG_network *network, QRNG_event *event, QRNG_event_callback cb);

QRNG_error __export QRNG_hardware_reset(QRNG_context context);
QRNG_error __export QRNGD_hardware_reset(QRNG_context context);

QRNG_error __export QRNG_wait(QRNG_event event);
QRNG_error __export QRNG_status(QRNG_event event);
void       __export QRNG_release_event(QRNG_event event);

#ifdef __cplusplus
};
#endif

#endif //__QRNG_H_H
