#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

#include "osc/OscReceivedElements.h"
#include "osc/OscPrintReceivedElements.h"
#include "ip/UdpSocket.h"
#include "ip/PacketListener.h"

#ifdef _DEBUG
#pragma comment(lib, "liboscpack_vc12x86D.lib")
#else
#pragma comment(lib, "liboscpack_vc12x86.lib")
#endif
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
