#pragma once
#include <string>
#include <WinSock2.h>
#pragma once

using namespace std;

inline string get_error_msg_text(int code)
{
	string msg_text;
	switch (code) {
    case WSAEINTR: msg_text = "WSAEINTR"; break;
    case WSAEACCES: msg_text = "WSAEACCES"; break;
    case WSAEFAULT: msg_text = "WSAEFAULT"; break;
    case WSAEINVAL: msg_text = "WSAEINVAL"; break;
    case WSAEMFILE: msg_text = "WSAEMFILE"; break;
    case WSAEWOULDBLOCK: msg_text = "WSAEWOULDBLOCK"; break;
    case WSAEINPROGRESS: msg_text = "WSAEINPROGRESS"; break;
    case WSAEALREADY: msg_text = "WSAEALREADY"; break;
    case WSAENOTSOCK: msg_text = "WSAENOTSOCK"; break;
    case WSAEDESTADDRREQ: msg_text = "WSAEDESTADDRREQ"; break;
    case WSAEMSGSIZE: msg_text = "WSAEMSGSIZE"; break;
    case WSAEPROTOTYPE: msg_text = "WSAEPROTOTYPE"; break;
    case WSAENOPROTOOPT: msg_text = "WSAENOPROTOOPT"; break;
    case WSAEPROTONOSUPPORT: msg_text = "WSAEPROTONOSUPPORT"; break;
    case WSAESOCKTNOSUPPORT: msg_text = "WSAESOCKTNOSUPPORT"; break;
    case WSAEOPNOTSUPP: msg_text = "WSAEOPNOTSUPP"; break;
    case WSAEPFNOSUPPORT: msg_text = "WSAEPFNOSUPPORT"; break;
    case WSAEAFNOSUPPORT: msg_text = "WSAEAFNOSUPPORT"; break;
    case WSAEADDRINUSE: msg_text = "WSAEADDRINUSE"; break;
    case WSAEADDRNOTAVAIL: msg_text = "WSAEADDRNOTAVAIL"; break;
    case WSAENETDOWN: msg_text = "WSAENETDOWN"; break;
    case WSAENETUNREACH: msg_text = "WSAENETUNREACH"; break;
    case WSAENETRESET: msg_text = "WSAENETRESET"; break;
    case WSAECONNABORTED: msg_text = "WSAECONNABORTED"; break;
    case WSAECONNRESET: msg_text = "WSAECONNRESET"; break;
    case WSAENOBUFS: msg_text = "WSAENOBUFS"; break;
    case WSAEISCONN: msg_text = "WSAEISCONN"; break;
    case WSAENOTCONN: msg_text = "WSAENOTCONN"; break;
    case WSAESHUTDOWN: msg_text = "WSAESHUTDOWN"; break;
    case WSAETIMEDOUT: msg_text = "WSAETIMEDOUT"; break;
    case WSAECONNREFUSED: msg_text = "WSAECONNREFUSED"; break;
    case WSAEHOSTDOWN: msg_text = "WSAEHOSTDOWN"; break;
    case WSAEHOSTUNREACH: msg_text = "WSAEHOSTUNREACH"; break;
    case WSAEPROCLIM: msg_text = "WSAEPROCLIM"; break;
    case WSASYSNOTREADY: msg_text = "WSASYSNOTREADY"; break;
    case WSAVERNOTSUPPORTED: msg_text = "WSAVERNOTSUPPORTED"; break;
    case WSANOTINITIALISED: msg_text = "WSANOTINITIALISED"; break;
    case WSAEDISCON: msg_text = "WSAEDISCON"; break;
    case WSATYPE_NOT_FOUND: msg_text = "WSATYPE_NOT_FOUND"; break;
    case WSAHOST_NOT_FOUND: msg_text = "WSAHOST_NOT_FOUND"; break;
    case WSATRY_AGAIN: msg_text = "WSATRY_AGAIN"; break;
    case WSANO_RECOVERY: msg_text = "WSANO_RECOVERY"; break;
    case WSANO_DATA: msg_text = "WSANO_DATA"; break;
    case WSA_INVALID_HANDLE: msg_text = "WSA_INVALID_HANDLE"; break;
    case WSA_INVALID_PARAMETER: msg_text = "WSA_INVALID_PARAMETER"; break;
    case WSA_IO_INCOMPLETE: msg_text = "WSA_IO_INCOMPLETE"; break;
    case WSA_IO_PENDING: msg_text = "WSA_IO_PENDING"; break;
    case WSA_NOT_ENOUGH_MEMORY: msg_text = "WSA_NOT_ENOUGH_MEMORY"; break;
    case WSA_OPERATION_ABORTED: msg_text = "WSA_OPERATION_ABORTED"; break;
    case WSASYSCALLFAILURE: msg_text = "WSASYSCALLFAILURE"; break;
    default: msg_text = "ERROR "; break;
	}

	return msg_text;
}

inline auto set_error_msg_text(const string msg_text, const int code) -> string
{
	return msg_text + get_error_msg_text(code) + "\n\n";
}