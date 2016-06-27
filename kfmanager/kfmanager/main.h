//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#ifndef KFROUTER_MAIN_H
#define KFROUTER_MAIN_H

// Error code and msg
#define KF_STR_ERRORCODE    "errorCode"
#define KF_STR_ERRORMSG     "errorMsg"
//errorreport
#define KF_STR_WARINGCODE   "warningCode"
#define KF_STR_WARNINGENTITY "warningEntity"
#define KF_STR_WARNINGMSG   "warningMsg"
#define KF_STR_WARNINGLEVEL "warningLevel"

#define KF_STR_SUCCESS      "success"

#define KF_STR_RESULT       "result"
#define KF_STR_RESULTDATA   "resultData"

// Result code and msg
#define KF_STR_RESULTCODE   "resultCode"
#define KF_STR_RESULTMSG    "resultMsg"

#define KF_STR_PARAMS       "PARAMS"

#define KF_STR_MSGHEAD      "MsgHead"
#define KF_STR_MSGBODY      "MsgBody"
#define KF_STR_SERVICECODE  "serviceCode"
#define KF_STR_SRCSYSID     "SrcSysID"
#define KF_STR_SRCSYSSIGN   "SrcSysSign"
#define KF_STR_TRANSACTION  "transactionID"

#define KF_STR_MAC          "mac"
#define KF_STR_WANIP        "wanIp"
#define KF_STR_RESOURCEIP   "resourceIp"
#define KF_STR_PINGVAL      "pingVal"
#define KF_STR_PKGLOSSRATE  "pkgLossRate"

// config
#define KF_STR_CONFIG       "config"

// Query ip
#define KF_STR_QUERYIP      "queryAvailableServerIp"
#define KF_STR_SYSID        "0001"
#define KF_STR_BELONGGROUP  "belongGroup"
#define KF_STR_DISTANCE     "distance"
#define KF_STR_LATITUDE     "latitude"
#define KF_STR_LONGITUDE    "longitude"
#define KF_STR_SERVERIP     "serverIp"

// PingUpload
#define KF_STR_PINGUPLOAD   "uploadTestMsgAndGetServerPort"
#define KF_STR_LOCALPORT    "local_port"
#define KF_STR_METHOD       "method"
#define KF_STR_PASSWORD     "password"
#define KF_STR_SERVER       "server"
#define KF_STR_SERVERPORT   "server_port"

// Bind
#define KF_STR_BINDJUDGE "bindJudge"

//UploadTraffic
#define KF_STR_UPLOADROUTEMSG                "uploadRouteMsg"
#define KF_STR_STARTUPTIMESTAMP              "startupTimestamp"
#define KF_STR_REMAINRAM                     "remainedRam"
#define KF_STR_RESOURCEPORT                  "resourcePort"
#define KF_STR_CLIENTCONNNUM                 "clientConnNum"
#define KF_STR_REQUESTURLNUM                 "requestUrlNum"
#define KF_STR_TOTALTRAFFIC                  "totalTraffic"
#define KF_STR_TOTALFOREIGNTRAFFIC           "totalForeignTraffic"
#define KF_STR_PREVTRAFFIC                   "prevTraffic"
#define KF_STR_PREVTOTALFOREIGNTRAFFIC       "prevTotalForeignTraffic"
#define KF_STR_CIRCLETOTALTRAFFIC            "circleTotalTraffic"
#define KF_STR_CIRCLEFOREIGNTOTALTRAFFIC     "circleForeignTotalTraffic"


// OnlineNotify
#define KF_STR_ONLINENOTYFY "routerOnlineNotify"

// config KFRemoteServerInfo
#define KF_STR_SZHOST     "szHost"
#define KF_STR_IPORT      "iPort"
#define KF_STR_SZURL      "szUrl"


//shadowsocks
#define KF_STR_SZMETHOD      "method"
#define KF_STR_SZPASSWORD    "password"
#define KF_STR_SZSERVERIP    "server"
#define KF_STR_ISERVERPORT   "server_port"
#define KF_STR_ILOCALPORT    "local_port"
#define KF_STR_LOCALADDRESS  "local_address"


// ERROR code
#define KF_ERROR_OUTSIDE_CODE  "FAIL-901"

typedef struct {
    char         szIP[32];
    int          iPort;
    int          iBelongGroup;
    char         szDistance[16];
    double       fLongitude;
    double       fLatitude;
} KFDataParseIPProperties;

typedef struct {
    float        max;
    float        avg;
    float        min;
    float        pkgloss;
    float        iWeightNow;
} KFPingCheckResult;

// for shadowsocks to restart
typedef struct {
    char         szMethod[16];
    char         szPassword[128];
    char         szServerIp[32];
    int          iServerPort;
    int          iLocalPort;
} KFSSResultData;

// Traffic
typedef struct {
    char                szMac[16];
    char                szWanIP[32];
    char                wanip[64];
    char                dns[64];
    char                dns2[64];
    char                netmask[64];
    char                gateway[64];




    char                szStartupTime[128];
    long                iRemainRam;
    char                szResourceIP[32];
    int                 iResourcePort;
    int                 iClientConnNum;
    int                 iRequestUrlNum;

    long long               lTotalTraffic;//内外网总流量
    long long              lTotalForeignTraffic;//外网总流量
    long long               lPrevTraffic;//上一次流量
    long long               lPrevTotalForeignTraffic;//上一次外网流量
    long long               lCircleTotalTraffic;//周期内外网总流量
    long long               lCircleForeignTotalTraffic;//周期外网流量

} KFTrafficInfo;

typedef struct {
    char                szEntity[16]; // mac address
    char                szCode[16]; // FAIL-XXX
    char                szMsg[1024]; // max 1024
    char                szLevel[16]; // ERROR, INFO, WARNING
} KFErrorReport;

//OnlineNotify
typedef struct {
    char szMac[16];//device mac
} KFOnlineNotify;

//Bind
typedef struct {
    char szMac[16];//device mac
} KFBindJudge;

//GetMemInfo
typedef struct{
    long int total;//total memery
    long int free;//free memery
} KFGetMemInfoResult;

//GetWanInfo
typedef struct {
    char wanip[64];
    char dns1[64];
    char dns2[64];
    char netmask[64];
    char gateway[64];
} KFGetWanInfoResult;


typedef struct {
    int           version;
    char          sender[128];
    unsigned int  seq;
    char          cmd[128];
    char          args[1024];
} KFControlMsg;



#endif //KFROUTER_MAIN_H
