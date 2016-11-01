/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    iprtrmib.h

Abstract:
    This file contains:
        o Definitions of the MIB_XX structures passed to and from the IP Router Manager
            to query and set MIB variables handled by the IP Router Manager
        o The #defines for the MIB variables IDs  handled by the IP Router Manager
            and made accessible by the MprAdminMIBXXX APIs
        o The Routing PID of the IP Router Manager (as mentioned in ipinfoid.h)

--*/

#ifndef __ROUTING_IPRTRMIB_H__
#define __ROUTING_IPRTRMIB_H__

#if _MSC_VER > 1000
#pragma once
#endif

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4200)
#pragma warning(disable:4201)
#pragma warning(disable:4214)

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Included to get the value of MAX_INTERFACE_NAME_LEN                      //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <mprapi.h>

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// Included to get the necessary constants                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#include <ipifcons.h>

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This is the Id for IP Router Manager.  The Router Manager handles        //
// MIB-II, Forwarding MIB and some enterprise specific information.         //
// Calls made with any other ID are passed on to the corresponding protocol //
// For example, an MprAdminMIBXXX call with a protocol ID of PID_IP and    //
// a routing Id of 0xD will be sent to the IP Router Manager and then       //
// forwarded to OSPF                                                        //
// This lives in the same number space as the protocol Ids of RIP, OSPF     //
// etc, so any change made to it should be done keeping this in mind        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#define IPRTRMGR_PID 10000

#ifndef ANY_SIZE

#define ANY_SIZE 1

#endif

#define TCPIP_OWNING_MODULE_SIZE 16

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// The following #defines are the Ids of the MIB variables made accessible  //
// to the user via MprAdminMIBXXX Apis.  It will be noticed that these are  //
// not the same as RFC 1213, since the MprAdminMIBXXX APIs work on rows and //
// groups instead of scalar variables                                       //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#define IF_NUMBER           0
#define IF_TABLE            (IF_NUMBER          + 1)
#define IF_ROW              (IF_TABLE           + 1)
#define IP_STATS            (IF_ROW             + 1)
#define IP_ADDRTABLE        (IP_STATS           + 1)
#define IP_ADDRROW          (IP_ADDRTABLE       + 1)
#define IP_FORWARDNUMBER    (IP_ADDRROW         + 1)
#define IP_FORWARDTABLE     (IP_FORWARDNUMBER   + 1)
#define IP_FORWARDROW       (IP_FORWARDTABLE    + 1)
#define IP_NETTABLE         (IP_FORWARDROW      + 1)
#define IP_NETROW           (IP_NETTABLE        + 1)
#define ICMP_STATS          (IP_NETROW          + 1)
#define TCP_STATS           (ICMP_STATS         + 1)
#define TCP_TABLE           (TCP_STATS          + 1)
#define TCP_ROW             (TCP_TABLE          + 1)
#define UDP_STATS           (TCP_ROW            + 1)
#define UDP_TABLE           (UDP_STATS          + 1)
#define UDP_ROW             (UDP_TABLE          + 1)
#define MCAST_MFE           (UDP_ROW            + 1)
#define MCAST_MFE_STATS     (MCAST_MFE          + 1)
#define BEST_IF             (MCAST_MFE_STATS    + 1)
#define BEST_ROUTE          (BEST_IF            + 1)
#define PROXY_ARP           (BEST_ROUTE         + 1)
#define MCAST_IF_ENTRY      (PROXY_ARP          + 1)
#define MCAST_GLOBAL        (MCAST_IF_ENTRY     + 1)
#define IF_STATUS           (MCAST_GLOBAL       + 1)
#define MCAST_BOUNDARY      (IF_STATUS          + 1)
#define MCAST_SCOPE         (MCAST_BOUNDARY     + 1)
#define DEST_MATCHING       (MCAST_SCOPE        + 1)
#define DEST_LONGER         (DEST_MATCHING      + 1)
#define DEST_SHORTER        (DEST_LONGER        + 1)
#define ROUTE_MATCHING      (DEST_SHORTER       + 1)
#define ROUTE_LONGER        (ROUTE_MATCHING     + 1)
#define ROUTE_SHORTER       (ROUTE_LONGER       + 1)
#define ROUTE_STATE         (ROUTE_SHORTER      + 1)
#define MCAST_MFE_STATS_EX  (ROUTE_STATE        + 1)
#define IP6_STATS           (MCAST_MFE_STATS_EX + 1)
#define UDP6_STATS          (IP6_STATS          + 1)
#define TCP6_STATS          (UDP6_STATS         + 1)

#define NUMBER_OF_EXPORTED_VARIABLES    (TCP6_STATS + 1)


//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// MIB_OPAQUE_QUERY is the structure filled in by the user to identify a    //
// MIB variable                                                             //
//                                                                          //
//  dwVarId     ID of MIB Variable (One of the Ids #defined above)          //
//  dwVarIndex  Variable sized array containing the indices needed to       //
//              identify a variable. NOTE: Unlike SNMP we dont require that //
//              a scalar variable be indexed by 0                           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

typedef struct _MIB_OPAQUE_QUERY
{
    DWORD  dwVarId;
    DWORD  rgdwVarIndex[ANY_SIZE];
}MIB_OPAQUE_QUERY, *PMIB_OPAQUE_QUERY;

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// The following are the structures which are filled in and returned to the //
// user when a query is made, OR  are filled in BY THE USER when a set is   //
// done                                                                     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

typedef struct _MIB_IFNUMBER
{
    DWORD    dwValue;
} MIB_IFNUMBER, *PMIB_IFNUMBER;


#define MAXLEN_IFDESCR 256
#define MAXLEN_PHYSADDR 8

typedef struct _MIB_IFROW
{
    WCHAR   wszName[MAX_INTERFACE_NAME_LEN];
    DWORD    dwIndex;
    DWORD    dwType;
    DWORD    dwMtu;
    DWORD    dwSpeed;
    DWORD    dwPhysAddrLen;
    BYTE    bPhysAddr[MAXLEN_PHYSADDR];
    DWORD    dwAdminStatus;
    DWORD    dwOperStatus;
    DWORD    dwLastChange;
    DWORD    dwInOctets;
    DWORD    dwInUcastPkts;
    DWORD    dwInNUcastPkts;
    DWORD    dwInDiscards;
    DWORD    dwInErrors;
    DWORD    dwInUnknownProtos;
    DWORD    dwOutOctets;
    DWORD    dwOutUcastPkts;
    DWORD    dwOutNUcastPkts;
    DWORD    dwOutDiscards;
    DWORD    dwOutErrors;
    DWORD    dwOutQLen;
    DWORD    dwDescrLen;
    BYTE    bDescr[MAXLEN_IFDESCR];
} MIB_IFROW,*PMIB_IFROW;

typedef struct _MIB_IFTABLE
{
    DWORD     dwNumEntries;
    MIB_IFROW table[ANY_SIZE];
} MIB_IFTABLE, *PMIB_IFTABLE;

#define SIZEOF_IFTABLE(X) (FIELD_OFFSET(MIB_IFTABLE,table[0]) + ((X) * sizeof(MIB_IFROW)) + ALIGN_SIZE)

typedef struct _MIBICMPSTATS
{
    DWORD        dwMsgs;
    DWORD        dwErrors;
    DWORD        dwDestUnreachs;
    DWORD        dwTimeExcds;
    DWORD        dwParmProbs;
    DWORD        dwSrcQuenchs;
    DWORD        dwRedirects;
    DWORD        dwEchos;
    DWORD        dwEchoReps;
    DWORD        dwTimestamps;
    DWORD        dwTimestampReps;
    DWORD        dwAddrMasks;
    DWORD        dwAddrMaskReps;
} MIBICMPSTATS, *PMIBICMPSTATS;

typedef    struct _MIBICMPINFO
{
    MIBICMPSTATS    icmpInStats;
    MIBICMPSTATS    icmpOutStats;
} MIBICMPINFO;

typedef struct _MIB_ICMP
{
    MIBICMPINFO stats;
} MIB_ICMP,*PMIB_ICMP;

//
// ICMPv6 Type Values from RFC 2292.
//
typedef enum {
    ICMP6_DST_UNREACH          =   1,
    ICMP6_PACKET_TOO_BIG       =   2,
    ICMP6_TIME_EXCEEDED        =   3,
    ICMP6_PARAM_PROB           =   4,
    ICMP6_ECHO_REQUEST         = 128,
    ICMP6_ECHO_REPLY           = 129,
    ICMP6_MEMBERSHIP_QUERY     = 130,
    ICMP6_MEMBERSHIP_REPORT    = 131,
    ICMP6_MEMBERSHIP_REDUCTION = 132,
    ND_ROUTER_SOLICIT          = 133,
    ND_ROUTER_ADVERT           = 134,
    ND_NEIGHBOR_SOLICIT        = 135,
    ND_NEIGHBOR_ADVERT         = 136,
    ND_REDIRECT                = 137,
} ICMP6_TYPE, *PICMP6_TYPE;

//
// There are no RFC-specified defines for ICMPv4 message types, so we try to
// use the ICMP6 values from RFC 2292 modified to be prefixed with ICMP4.
//
typedef enum {
    ICMP4_ECHO_REPLY        =  0, // Echo Reply
    ICMP4_DST_UNREACH       =  3, // Destination Unreachable
    ICMP4_SOURCE_QUENCH     =  4, // Source Quench
    ICMP4_REDIRECT          =  5, // Redirect
    ICMP4_ECHO_REQUEST      =  8, // Echo Request
    ICMP4_ROUTER_ADVERT     =  9, // Router Advertisement
    ICMP4_ROUTER_SOLICIT    = 10, // Router Solicitation
    ICMP4_TIME_EXCEEDED     = 11, // Time Exceeded
    ICMP4_PARAM_PROB        = 12, // Parameter Problem
    ICMP4_TIMESTAMP_REQUEST = 13, // Timestamp Request
    ICMP4_TIMESTAMP_REPLY   = 14, // Timestamp Reply
    ICMP4_MASK_REQUEST      = 17, // Address Mask Request
    ICMP4_MASK_REPLY        = 18, // Address Mask Reply
} ICMP4_TYPE, *PICMP4_TYPE;

typedef struct _MIBICMPSTATS_EX
{
    DWORD       dwMsgs;
    DWORD       dwErrors;
    DWORD       rgdwTypeCount[256];
} MIBICMPSTATS_EX, *PMIBICMPSTATS_EX;

typedef struct _MIB_ICMP_EX
{
    MIBICMPSTATS_EX icmpInStats;
    MIBICMPSTATS_EX icmpOutStats;
} MIB_ICMP_EX,*PMIB_ICMP_EX;

typedef struct _MIB_UDPSTATS
{
    DWORD       dwInDatagrams;
    DWORD       dwNoPorts;
    DWORD       dwInErrors;
    DWORD       dwOutDatagrams;
    DWORD       dwNumAddrs;
} MIB_UDPSTATS,*PMIB_UDPSTATS;

typedef struct _MIB_UDPROW
{
    DWORD       dwLocalAddr;
    DWORD       dwLocalPort;
} MIB_UDPROW, *PMIB_UDPROW;

typedef MIB_UDPROW MIB_UDPROW_BASIC, *PMIB_UDPROW_BASIC;

typedef struct _MIB_UDPROW_OWNER_PID
{
    DWORD           dwLocalAddr;
    DWORD           dwLocalPort;
    DWORD           dwOwningPid;
} MIB_UDPROW_OWNER_PID, *PMIB_UDPROW_OWNER_PID;

typedef struct _MIB_UDPROW_OWNER_MODULE
{
    DWORD           dwLocalAddr;
    DWORD           dwLocalPort;
    DWORD           dwOwningPid;
    LARGE_INTEGER   liCreateTimestamp;
    union {
        struct {
            DWORD   SpecificPortBind : 1;
        };
        DWORD       dwFlags;
    };
    ULONGLONG       OwningModuleInfo[TCPIP_OWNING_MODULE_SIZE];
} MIB_UDPROW_OWNER_MODULE, *PMIB_UDPROW_OWNER_MODULE;

typedef struct _MIB_UDP6ROW_OWNER_PID
{
    UCHAR           ucLocalAddr[16];
    DWORD           dwLocalScopeId;
    DWORD           dwLocalPort;
    DWORD           dwOwningPid;
} MIB_UDP6ROW_OWNER_PID, *PMIB_UDP6ROW_OWNER_PID;

typedef struct _MIB_UDP6ROW_OWNER_MODULE
{
    UCHAR           ucLocalAddr[16];
    DWORD           dwLocalScopeId;
    DWORD           dwLocalPort;
    DWORD           dwOwningPid;
    LARGE_INTEGER   liCreateTimestamp;
    union {
        struct {
            DWORD   SpecificPortBind : 1;
        };
        DWORD       dwFlags;
    };
    ULONGLONG       OwningModuleInfo[TCPIP_OWNING_MODULE_SIZE];
} MIB_UDP6ROW_OWNER_MODULE, *PMIB_UDP6ROW_OWNER_MODULE;

typedef struct _MIB_UDPTABLE
{
    DWORD       dwNumEntries;
    MIB_UDPROW  table[ANY_SIZE];
} MIB_UDPTABLE, *PMIB_UDPTABLE;

typedef MIB_UDPTABLE MIB_UDPTABLE_BASIC, *PMIB_UDPTABLE_BASIC;

#define SIZEOF_UDPTABLE(X) (FIELD_OFFSET(MIB_UDPTABLE, table[0]) + ((X) * sizeof(MIB_UDPROW)) + ALIGN_SIZE)

#define SIZEOF_UDPTABLE_BASIC(X) SIZEOF_UDPTABLE(X)

typedef struct _MIB_UDPTABLE_OWNER_PID
{
    DWORD                   dwNumEntries;
    MIB_UDPROW_OWNER_PID    table[ANY_SIZE];
} MIB_UDPTABLE_OWNER_PID, *PMIB_UDPTABLE_OWNER_PID;

#define SIZEOF_UDPTABLE_OWNER_PID(X) (FIELD_OFFSET(MIB_UDPTABLE_OWNER_PID, table[0]) + ((X) * sizeof(MIB_UDPROW_OWNER_PID)) + ALIGN_SIZE)

typedef struct _MIB_UDPTABLE_OWNER_MODULE
{
    DWORD                   dwNumEntries;
    MIB_UDPROW_OWNER_MODULE table[ANY_SIZE];
} MIB_UDPTABLE_OWNER_MODULE, *PMIB_UDPTABLE_OWNER_MODULE;

#define SIZEOF_UDPTABLE_OWNER_MODULE(X) (FIELD_OFFSET(MIB_UDPTABLE_OWNER_MODULE, table[0]) + ((X) * sizeof(MIB_UDPROW_OWNER_MODULE)) + ALIGN_SIZE)

typedef struct _MIB_UDP6TABLE_OWNER_PID
{
    DWORD                   dwNumEntries;
    MIB_UDP6ROW_OWNER_PID   table[ANY_SIZE];
} MIB_UDP6TABLE_OWNER_PID, *PMIB_UDP6TABLE_OWNER_PID;

#define SIZEOF_UDP6TABLE_OWNER_PID(X) (FIELD_OFFSET(MIB_UDP6TABLE_OWNER_PID, table[0]) + ((X) * sizeof(MIB_UDP6ROW_OWNER_PID)) + ALIGN_SIZE)

typedef struct _MIB_UDP6TABLE_OWNER_MODULE
{
    DWORD                    dwNumEntries;
    MIB_UDP6ROW_OWNER_MODULE table[ANY_SIZE];
} MIB_UDP6TABLE_OWNER_MODULE, *PMIB_UDP6TABLE_OWNER_MODULE;

#define SIZEOF_UDP6TABLE_OWNER_MODULE(X) (FIELD_OFFSET(MIB_UDP6TABLE_OWNER_MODULE, table[0]) + ((X) * sizeof(MIB_UDP6ROW_OWNER_MODULE)) + ALIGN_SIZE)

typedef struct _MIB_TCPSTATS
{
    DWORD       dwRtoAlgorithm;
    DWORD       dwRtoMin;
    DWORD       dwRtoMax;
    DWORD       dwMaxConn;
    DWORD       dwActiveOpens;
    DWORD       dwPassiveOpens;
    DWORD       dwAttemptFails;
    DWORD       dwEstabResets;
    DWORD       dwCurrEstab;
    DWORD       dwInSegs;
    DWORD       dwOutSegs;
    DWORD       dwRetransSegs;
    DWORD       dwInErrs;
    DWORD       dwOutRsts;
    DWORD       dwNumConns;
} MIB_TCPSTATS, *PMIB_TCPSTATS;

#define MIB_TCP_RTO_OTHER       1
#define MIB_TCP_RTO_CONSTANT    2
#define MIB_TCP_RTO_RSRE        3
#define MIB_TCP_RTO_VANJ        4

#define MIB_TCP_MAXCONN_DYNAMIC (DWORD)-1

typedef struct _MIB_TCPROW
{
    DWORD       dwState;
    DWORD       dwLocalAddr;
    DWORD       dwLocalPort;
    DWORD       dwRemoteAddr;
    DWORD       dwRemotePort;
} MIB_TCPROW, *PMIB_TCPROW;

typedef MIB_TCPROW MIB_TCPROW_BASIC, *PMIB_TCPROW_BASIC;

typedef struct _MIB_TCPROW_OWNER_PID
{
    DWORD       dwState;
    DWORD       dwLocalAddr;
    DWORD       dwLocalPort;
    DWORD       dwRemoteAddr;
    DWORD       dwRemotePort;
    DWORD       dwOwningPid;
} MIB_TCPROW_OWNER_PID, *PMIB_TCPROW_OWNER_PID;


typedef struct _MIB_TCPROW_OWNER_MODULE
{
    DWORD           dwState;
    DWORD           dwLocalAddr;
    DWORD           dwLocalPort;
    DWORD           dwRemoteAddr;
    DWORD           dwRemotePort;
    DWORD           dwOwningPid;
    LARGE_INTEGER   liCreateTimestamp;
    ULONGLONG       OwningModuleInfo[TCPIP_OWNING_MODULE_SIZE];
} MIB_TCPROW_OWNER_MODULE, *PMIB_TCPROW_OWNER_MODULE;

typedef struct _MIB_TCP6ROW_OWNER_PID
{
    UCHAR           ucLocalAddr[16];
    DWORD           dwLocalScopeId;
    DWORD           dwLocalPort;
    UCHAR           ucRemoteAddr[16];
    DWORD           dwRemoteScopeId;
    DWORD           dwRemotePort;
    DWORD           dwState;
    DWORD           dwOwningPid;
} MIB_TCP6ROW_OWNER_PID, *PMIB_TCP6ROW_OWNER_PID;

typedef struct _MIB_TCP6ROW_OWNER_MODULE
{
    UCHAR           ucLocalAddr[16];
    DWORD           dwLocalScopeId;
    DWORD           dwLocalPort;
    UCHAR           ucRemoteAddr[16];
    DWORD           dwRemoteScopeId;
    DWORD           dwRemotePort;
    DWORD           dwState;
    DWORD           dwOwningPid;
    LARGE_INTEGER   liCreateTimestamp;
    ULONGLONG       OwningModuleInfo[TCPIP_OWNING_MODULE_SIZE];
} MIB_TCP6ROW_OWNER_MODULE, *PMIB_TCP6ROW_OWNER_MODULE;

    
#define MIB_TCP_STATE_CLOSED            1
#define MIB_TCP_STATE_LISTEN            2
#define MIB_TCP_STATE_SYN_SENT          3
#define MIB_TCP_STATE_SYN_RCVD          4
#define MIB_TCP_STATE_ESTAB             5
#define MIB_TCP_STATE_FIN_WAIT1         6
#define MIB_TCP_STATE_FIN_WAIT2         7
#define MIB_TCP_STATE_CLOSE_WAIT        8
#define MIB_TCP_STATE_CLOSING           9
#define MIB_TCP_STATE_LAST_ACK         10
#define MIB_TCP_STATE_TIME_WAIT        11
#define MIB_TCP_STATE_DELETE_TCB       12

typedef struct _MIB_TCPTABLE
{
    DWORD      dwNumEntries;
    MIB_TCPROW table[ANY_SIZE];
} MIB_TCPTABLE, *PMIB_TCPTABLE;

typedef MIB_TCPTABLE MIB_TCPTABLE_BASIC, *PMIB_TCPTABLE_BASIC;

#define SIZEOF_TCPTABLE(X) (FIELD_OFFSET(MIB_TCPTABLE,table[0]) + ((X) * sizeof(MIB_TCPROW)) + ALIGN_SIZE)

#define SIZEOF_TCPTABLE_BASIC(X) SIZEOF_TCPTABLE(X)

typedef struct _MIB_TCPTABLE_OWNER_PID
{
    DWORD                dwNumEntries;
    MIB_TCPROW_OWNER_PID table[ANY_SIZE];
} MIB_TCPTABLE_OWNER_PID, *PMIB_TCPTABLE_OWNER_PID;

#define SIZEOF_TCPTABLE_OWNER_PID(X) (FIELD_OFFSET(MIB_TCPTABLE_OWNER_PID, table[0]) + ((X) * sizeof(MIB_TCPROW_OWNER_PID)) + ALIGN_SIZE)

typedef struct _MIB_TCPTABLE_OWNER_MODULE
{
    DWORD                   dwNumEntries;
    MIB_TCPROW_OWNER_MODULE table[ANY_SIZE];
} MIB_TCPTABLE_OWNER_MODULE, *PMIB_TCPTABLE_OWNER_MODULE;

#define SIZEOF_TCPTABLE_OWNER_MODULE(X) (FIELD_OFFSET(MIB_TCPTABLE_OWNER_MODULE, table[0]) + ((X) * sizeof(MIB_TCPROW_OWNER_MODULE)) + ALIGN_SIZE)

typedef struct _MIB_TCP6TABLE_OWNER_PID
{
    DWORD                   dwNumEntries;
    MIB_TCP6ROW_OWNER_PID   table[ANY_SIZE];
} MIB_TCP6TABLE_OWNER_PID, *PMIB_TCP6TABLE_OWNER_PID;

#define SIZEOF_TCP6TABLE_OWNER_PID(X) (FIELD_OFFSET(MIB_TCP6TABLE_OWNER_PID, table[0]) + ((X) * sizeof(MIB_TCP6ROW_OWNER_PID)) + ALIGN_SIZE)

typedef struct _MIB_TCP6TABLE_OWNER_MODULE
{
    DWORD                       dwNumEntries;
    MIB_TCP6ROW_OWNER_MODULE    table[ANY_SIZE];
} MIB_TCP6TABLE_OWNER_MODULE, *PMIB_TCP6TABLE_OWNER_MODULE;

#define SIZEOF_TCP6TABLE_OWNER_MODULE(X) (FIELD_OFFSET(MIB_TCP6TABLE_OWNER_MODULE, table[0]) + ((X) * sizeof(MIB_TCP6ROW_OWNER_PID)) + ALIGN_SIZE)

//
// Flags that describe the health of TCPIP with respect to security/DOS.
//
#define MIB_SECURITY_TCP_SYN_ATTACK 0x00000001

#define MIB_USE_CURRENT_TTL         ((DWORD)-1)
#define MIB_USE_CURRENT_FORWARDING  ((DWORD)-1)

typedef struct _MIB_IPSTATS
{
    DWORD        dwForwarding;
    DWORD        dwDefaultTTL;
    DWORD        dwInReceives;
    DWORD        dwInHdrErrors;
    DWORD        dwInAddrErrors;
    DWORD        dwForwDatagrams;
    DWORD        dwInUnknownProtos;
    DWORD        dwInDiscards;
    DWORD        dwInDelivers;
    DWORD        dwOutRequests;
    DWORD        dwRoutingDiscards;
    DWORD        dwOutDiscards;
    DWORD        dwOutNoRoutes;
    DWORD        dwReasmTimeout;
    DWORD        dwReasmReqds;
    DWORD        dwReasmOks;
    DWORD        dwReasmFails;
    DWORD        dwFragOks;
    DWORD        dwFragFails;
    DWORD        dwFragCreates;
    DWORD        dwNumIf;
    DWORD        dwNumAddr;
    DWORD        dwNumRoutes;
} MIB_IPSTATS, *PMIB_IPSTATS;

#define    MIB_IP_FORWARDING               1
#define    MIB_IP_NOT_FORWARDING           2


// Note: These addr types have dependency on ipdef.h

#define MIB_IPADDR_PRIMARY      0x0001   // Primary ipaddr
#define MIB_IPADDR_DYNAMIC      0x0004   // Dynamic ipaddr
#define MIB_IPADDR_DISCONNECTED 0x0008   // Address is on disconnected interface
#define MIB_IPADDR_DELETED      0x0040   // Address being deleted
#define MIB_IPADDR_TRANSIENT    0x0080   // Transient address


typedef struct _MIB_IPADDRROW
{
    DWORD        dwAddr;
    DWORD        dwIndex;
    DWORD        dwMask;
    DWORD        dwBCastAddr;
    DWORD        dwReasmSize;
    unsigned short    unused1;
    unsigned short    wType;
} MIB_IPADDRROW, *PMIB_IPADDRROW;

typedef struct _MIB_IPADDRTABLE
{
    DWORD         dwNumEntries;
    MIB_IPADDRROW table[ANY_SIZE];
} MIB_IPADDRTABLE, *PMIB_IPADDRTABLE;


#define SIZEOF_IPADDRTABLE(X) (FIELD_OFFSET(MIB_IPADDRTABLE,table[0]) + ((X) * sizeof(MIB_IPADDRROW)) + ALIGN_SIZE)


typedef struct _MIB_IPFORWARDNUMBER
{
    DWORD      dwValue;
}MIB_IPFORWARDNUMBER,*PMIB_IPFORWARDNUMBER;

typedef struct _MIB_IPFORWARDROW
{
    DWORD        dwForwardDest;
    DWORD        dwForwardMask;
    DWORD        dwForwardPolicy;
    DWORD        dwForwardNextHop;
    DWORD        dwForwardIfIndex;
    DWORD        dwForwardType;
    DWORD        dwForwardProto;
    DWORD        dwForwardAge;
    DWORD       dwForwardNextHopAS;
    DWORD        dwForwardMetric1;
    DWORD        dwForwardMetric2;
    DWORD        dwForwardMetric3;
    DWORD        dwForwardMetric4;
    DWORD        dwForwardMetric5;
}MIB_IPFORWARDROW, *PMIB_IPFORWARDROW;

#define    MIB_IPROUTE_TYPE_OTHER        1
#define    MIB_IPROUTE_TYPE_INVALID    2
#define    MIB_IPROUTE_TYPE_DIRECT        3
#define    MIB_IPROUTE_TYPE_INDIRECT    4

#define    MIB_IPROUTE_METRIC_UNUSED    (DWORD)-1

//
// THESE MUST MATCH the ids in routprot.h
//

#define MIB_IPPROTO_OTHER                1
#define MIB_IPPROTO_LOCAL                2
#define MIB_IPPROTO_NETMGMT                3
#define MIB_IPPROTO_ICMP                4
#define MIB_IPPROTO_EGP                    5
#define MIB_IPPROTO_GGP                    6
#define MIB_IPPROTO_HELLO                7
#define MIB_IPPROTO_RIP                    8
#define MIB_IPPROTO_IS_IS                9
#define MIB_IPPROTO_ES_IS                10
#define MIB_IPPROTO_CISCO                11
#define MIB_IPPROTO_BBN                    12
#define MIB_IPPROTO_OSPF                13
#define MIB_IPPROTO_BGP                    14

#define MIB_IPPROTO_NT_AUTOSTATIC       10002
#define MIB_IPPROTO_NT_STATIC           10006
#define MIB_IPPROTO_NT_STATIC_NON_DOD   10007

typedef struct _MIB_IPFORWARDTABLE
{
    DWORD               dwNumEntries;
    MIB_IPFORWARDROW    table[ANY_SIZE];
}MIB_IPFORWARDTABLE, *PMIB_IPFORWARDTABLE;




#define SIZEOF_IPFORWARDTABLE(X) (FIELD_OFFSET(MIB_IPFORWARDTABLE,table[0]) + ((X) * sizeof(MIB_IPFORWARDROW)) + ALIGN_SIZE)


typedef struct _MIB_IPNETROW
{
    DWORD        dwIndex;
    DWORD        dwPhysAddrLen;
    BYTE        bPhysAddr[MAXLEN_PHYSADDR];
    DWORD        dwAddr;
    DWORD        dwType;
} MIB_IPNETROW, *PMIB_IPNETROW;

#define    MIB_IPNET_TYPE_OTHER        1
#define    MIB_IPNET_TYPE_INVALID        2
#define    MIB_IPNET_TYPE_DYNAMIC        3
#define    MIB_IPNET_TYPE_STATIC        4

typedef struct _MIB_IPNETTABLE
{
    DWORD             dwNumEntries;
    MIB_IPNETROW      table[ANY_SIZE];
} MIB_IPNETTABLE, *PMIB_IPNETTABLE;

#define SIZEOF_IPNETTABLE(X) (FIELD_OFFSET(MIB_IPNETTABLE, table[0]) + ((X) * sizeof(MIB_IPNETROW)) + ALIGN_SIZE)

typedef struct _MIB_IPMCAST_OIF
{
    DWORD   dwOutIfIndex;
    DWORD   dwNextHopAddr;
    DWORD   dwReserved;
    DWORD   dwReserved1;
}MIB_IPMCAST_OIF, *PMIB_IPMCAST_OIF;

typedef struct _MIB_IPMCAST_MFE
{
    DWORD   dwGroup;
    DWORD   dwSource;
    DWORD   dwSrcMask;
    DWORD   dwUpStrmNgbr;
    DWORD   dwInIfIndex;
    DWORD   dwInIfProtocol;
    DWORD   dwRouteProtocol;
    DWORD   dwRouteNetwork;
    DWORD   dwRouteMask;
    ULONG   ulUpTime;
    ULONG   ulExpiryTime;
    ULONG   ulTimeOut;
    ULONG   ulNumOutIf;
    DWORD   fFlags;
    DWORD   dwReserved;
    MIB_IPMCAST_OIF rgmioOutInfo[ANY_SIZE];
}MIB_IPMCAST_MFE, *PMIB_IPMCAST_MFE;

typedef struct _MIB_MFE_TABLE
{
    DWORD           dwNumEntries;
    MIB_IPMCAST_MFE table[ANY_SIZE];
}MIB_MFE_TABLE, *PMIB_MFE_TABLE;


#define SIZEOF_BASIC_MIB_MFE          \
    (ULONG)(FIELD_OFFSET(MIB_IPMCAST_MFE, rgmioOutInfo[0]))

#define SIZEOF_MIB_MFE(X)             \
    (SIZEOF_BASIC_MIB_MFE + ((X) * sizeof(MIB_IPMCAST_OIF)))


typedef struct _MIB_IPMCAST_OIF_STATS
{
    DWORD   dwOutIfIndex;
    DWORD   dwNextHopAddr;
    DWORD   dwDialContext;
    ULONG   ulTtlTooLow;
    ULONG   ulFragNeeded;
    ULONG   ulOutPackets;
    ULONG   ulOutDiscards;
}MIB_IPMCAST_OIF_STATS, *PMIB_IPMCAST_OIF_STATS;

typedef struct _MIB_IPMCAST_MFE_STATS
{
    DWORD   dwGroup;
    DWORD   dwSource;
    DWORD   dwSrcMask;
    DWORD   dwUpStrmNgbr;
    DWORD   dwInIfIndex;
    DWORD   dwInIfProtocol;
    DWORD   dwRouteProtocol;
    DWORD   dwRouteNetwork;
    DWORD   dwRouteMask;
    ULONG   ulUpTime;
    ULONG   ulExpiryTime;
    ULONG   ulNumOutIf;
    ULONG   ulInPkts;
    ULONG   ulInOctets;
    ULONG   ulPktsDifferentIf;
    ULONG   ulQueueOverflow;

    MIB_IPMCAST_OIF_STATS   rgmiosOutStats[ANY_SIZE];
}MIB_IPMCAST_MFE_STATS, *PMIB_IPMCAST_MFE_STATS;

typedef struct _MIB_MFE_STATS_TABLE
{
    DWORD       dwNumEntries;
    MIB_IPMCAST_MFE_STATS   table[ANY_SIZE];
}MIB_MFE_STATS_TABLE, *PMIB_MFE_STATS_TABLE;

#define SIZEOF_BASIC_MIB_MFE_STATS    \
    (ULONG)(FIELD_OFFSET(MIB_IPMCAST_MFE_STATS, rgmiosOutStats[0]))

#define SIZEOF_MIB_MFE_STATS(X)       \
    (SIZEOF_BASIC_MIB_MFE_STATS + ((X) * sizeof(MIB_IPMCAST_OIF_STATS)))


typedef struct _MIB_IPMCAST_MFE_STATS_EX
{
    DWORD   dwGroup;
    DWORD   dwSource;
    DWORD   dwSrcMask;
    DWORD   dwUpStrmNgbr;
    DWORD   dwInIfIndex;
    DWORD   dwInIfProtocol;
    DWORD   dwRouteProtocol;
    DWORD   dwRouteNetwork;
    DWORD   dwRouteMask;
    ULONG   ulUpTime;
    ULONG   ulExpiryTime;
    ULONG   ulNumOutIf;
    ULONG   ulInPkts;
    ULONG   ulInOctets;
    ULONG   ulPktsDifferentIf;
    ULONG   ulQueueOverflow;
    ULONG   ulUninitMfe;
    ULONG   ulNegativeMfe;
    ULONG   ulInDiscards;
    ULONG   ulInHdrErrors;
    ULONG   ulTotalOutPackets;

    MIB_IPMCAST_OIF_STATS   rgmiosOutStats[ANY_SIZE];
}MIB_IPMCAST_MFE_STATS_EX, *PMIB_IPMCAST_MFE_STATS_EX;

typedef struct _MIB_MFE_STATS_TABLE_EX
{
    DWORD       dwNumEntries;
    MIB_IPMCAST_MFE_STATS_EX   table[ANY_SIZE];
}MIB_MFE_STATS_TABLE_EX, *PMIB_MFE_STATS_TABLE_EX;

#define SIZEOF_BASIC_MIB_MFE_STATS_EX    \
    (ULONG)(FIELD_OFFSET(MIB_IPMCAST_MFE_STATS_EX, rgmiosOutStats[0]))

#define SIZEOF_MIB_MFE_STATS_EX(X)       \
    (SIZEOF_BASIC_MIB_MFE_STATS_EX + ((X) * sizeof(MIB_IPMCAST_OIF_STATS)))


typedef struct _MIB_IPMCAST_GLOBAL {
    DWORD   dwEnable;
}MIB_IPMCAST_GLOBAL, *PMIB_IPMCAST_GLOBAL;

typedef struct _MIB_IPMCAST_IF_ENTRY
{
    DWORD   dwIfIndex;
    DWORD   dwTtl;
    DWORD   dwProtocol;
    DWORD   dwRateLimit;
    ULONG   ulInMcastOctets;
    ULONG   ulOutMcastOctets;
}MIB_IPMCAST_IF_ENTRY, *PMIB_IPMCAST_IF_ENTRY;

typedef struct _MIB_IPMCAST_IF_TABLE
{
    DWORD       dwNumEntries;
    MIB_IPMCAST_IF_ENTRY   table[ANY_SIZE];
}MIB_IPMCAST_IF_TABLE, *PMIB_IPMCAST_IF_TABLE;

#define SIZEOF_MCAST_IF_TABLE(X) (FIELD_OFFSET(MIB_IPMCAST_IF_TABLE,table[0]) + ((X) * sizeof(MIB_IPMCAST_IF_ENTRY)) + ALIGN_SIZE)

typedef struct _MIB_IPMCAST_BOUNDARY
{
    DWORD   dwIfIndex;
    DWORD   dwGroupAddress;
    DWORD   dwGroupMask;
    DWORD   dwStatus;
}MIB_IPMCAST_BOUNDARY, *PMIB_IPMCAST_BOUNDARY;

typedef struct _MIB_IPMCAST_BOUNDARY_TABLE
{
    DWORD       dwNumEntries;
    MIB_IPMCAST_BOUNDARY   table[ANY_SIZE];
}MIB_IPMCAST_BOUNDARY_TABLE, *PMIB_IPMCAST_BOUNDARY_TABLE;

#define SIZEOF_BOUNDARY_TABLE(X) (FIELD_OFFSET(MIB_IPMCAST_BOUNDARY_TABLE,table[0]) + ((X) * sizeof(MIB_IPMCAST_BOUNDARY)) + ALIGN_SIZE)

typedef struct {
    DWORD    dwGroupAddress;
    DWORD    dwGroupMask;
} MIB_BOUNDARYROW, *PMIB_BOUNDARYROW;

// Structure matching what goes in the registry in a block of type
// IP_MCAST_LIMIT_INFO.  This contains the fields of
// MIB_IPMCAST_IF_ENTRY which are configurable.

typedef struct {
    DWORD    dwTtl;
    DWORD    dwRateLimit;
} MIB_MCAST_LIMIT_ROW, *PMIB_MCAST_LIMIT_ROW;

#define MAX_SCOPE_NAME_LEN 255

//
// Scope names are unicode.  SNMP and MZAP use UTF-8 encoding.
//

#define SN_UNICODE
typedef WCHAR   SN_CHAR;
typedef SN_CHAR SCOPE_NAME_BUFFER[MAX_SCOPE_NAME_LEN+1], *SCOPE_NAME;

typedef struct _MIB_IPMCAST_SCOPE
{
    DWORD             dwGroupAddress;
    DWORD             dwGroupMask;
    SCOPE_NAME_BUFFER snNameBuffer;
    DWORD             dwStatus;
}MIB_IPMCAST_SCOPE, *PMIB_IPMCAST_SCOPE;

typedef struct _MIB_IPDESTROW
{
#ifdef __cplusplus
    MIB_IPFORWARDROW  ForwardRow;
#else
    MIB_IPFORWARDROW;
#endif

    DWORD             dwForwardPreference;
    DWORD             dwForwardViewSet;
}MIB_IPDESTROW, *PMIB_IPDESTROW;

typedef struct _MIB_IPDESTTABLE
{
    DWORD             dwNumEntries;
    MIB_IPDESTROW     table[ANY_SIZE];
}MIB_IPDESTTABLE, *PMIB_IPDESTTABLE;

typedef struct _MIB_BEST_IF
{
    DWORD       dwDestAddr;
    DWORD       dwIfIndex;
}MIB_BEST_IF, *PMIB_BEST_IF;

typedef struct _MIB_PROXYARP
{
    DWORD       dwAddress;
    DWORD       dwMask;
    DWORD       dwIfIndex;
}MIB_PROXYARP, *PMIB_PROXYARP;

typedef struct _MIB_IFSTATUS
{
    DWORD       dwIfIndex;
    DWORD       dwAdminStatus;
    DWORD       dwOperationalStatus;
    BOOL        bMHbeatActive;
    BOOL        bMHbeatAlive;
}MIB_IFSTATUS, *PMIB_IFSTATUS;

typedef struct _MIB_ROUTESTATE
{
    BOOL        bRoutesSetToStack;

}MIB_ROUTESTATE, *PMIB_ROUTESTATE;

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// All the info passed to (SET/CREATE) and from (GET/GETNEXT/GETFIRST)      //
// IP Router Manager is encapsulated in the following "discriminated"       //
// union.  To pass, say MIB_IFROW, use the following code                   //
//                                                                          //
//  PMIB_OPAQUE_INFO    pInfo;                                              //
//  PMIB_IFROW          pIfRow;                                             //
//  DWORD rgdwBuff[(MAX_MIB_OFFSET + sizeof(MIB_IFROW))/sizeof(DWORD) + 1]; //
//                                                                          //
//  pInfo   = (PMIB_OPAQUE_INFO)rgdwBuffer;                                 //
//  pIfRow  = (MIB_IFROW *)(pInfo->rgbyData);                               //
//                                                                          //
//  This can also be accomplished by using the following macro              //
//                                                                          //
//  DEFINE_MIB_BUFFER(pInfo,MIB_IFROW, pIfRow);                             //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


typedef struct _MIB_OPAQUE_INFO
{
    DWORD  dwId;

    union
    {
        ULONGLONG   ullAlign;
        BYTE        rgbyData[1];
    };

}MIB_OPAQUE_INFO, *PMIB_OPAQUE_INFO;

#define MAX_MIB_OFFSET      8

#define MIB_INFO_SIZE(S)                \
    (MAX_MIB_OFFSET + sizeof(S))

#define MIB_INFO_SIZE_IN_DWORDS(S)      \
    ((MIB_INFO_SIZE(S))/sizeof(DWORD) + 1)

#define DEFINE_MIB_BUFFER(X,Y,Z)                                        \
    DWORD        __rgdwBuff[MIB_INFO_SIZE_IN_DWORDS(Y)]; \
    PMIB_OPAQUE_INFO    X = (PMIB_OPAQUE_INFO)__rgdwBuff;               \
    Y *                 Z = (Y *)(X->rgbyData)


#define CAST_MIB_INFO(X,Y,Z)    Z = (Y)(X->rgbyData)

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4200)
#pragma warning(default:4201)
#pragma warning(default:4214)
#endif

#endif //__ROUTING_IPRTRMIB_H__

