# QRNG
Here is the English version:

QRNG
Introduction
This document is the software development kit (SDK) guide for the Quantum Random Number Generator (QRNG). It is intended to guide users in performing secondary development of the QRNG software using this SDK.

Software Architecture
The main task during the initialization of the SDK is to create a QRNG_context object, which is the core structure, and to open the necessary communication interfaces. There are three types of communication interfaces:


User Interface: Uses Ethernet link-layer communication. Its main functions include setting and reading device parameters, controlling random number output, and managing device resets.


Debug Interface: Uses serial port communication. Its functions are similar to the User Interface, primarily for setting, reading, and controlling parameters.


Data Interface: Can use either Ethernet link-layer communication or USB communication. Its main function is to receive random numbers.

Installation Tutorial
This SDK supports both Windows and Linux platforms.


Windows Platform 

Header File: QRNG.h 

Link Library File: QRNG.lib 

Runtime Library Files: QRNG.dll, platform.dll, business.dll 


Linux Platform 

Header File: QRNG.h 

Runtime Library File: libqrng.so 

Runtime Environment: CentOS 6.6 64-bit is recommended.

Usage Instructions
Initialize Context Call the QRNG_init() function to initialize and create the QRNG_context object. It will return NULL if initialization fails.




Set Interface Address Depending on the communication method, set one or more interface addresses:


Data Interface: Call QRNG_set_data_address(). This interface is used to set up the Ethernet or USB channel to receive random numbers.






User Interface: Call QRNG_set_user_address() to set up the Ethernet user interface.



Debug Interface: Call QRNG_set_debug_address() to set up the serial port debug interface.


Read Random Numbers After setting the data interface, call the QRNG_data_receive() function to read random numbers.


The pbuf parameter is the buffer to store the random numbers , and rlen is the expected length of data to read.


A return value greater than 0 indicates the actual length of the random numbers read. Note: The actual data length obtained may be less than the requested rlen length.


A return value less than 0 indicates an error.

Release Context When finished, call QRNG_release() to release the QRNG_context object.
