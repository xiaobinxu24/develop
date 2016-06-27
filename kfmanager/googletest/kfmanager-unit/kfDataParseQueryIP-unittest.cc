#include "kf_data_parse_queryip.h"
#include <limits.h>
#include <kf_process_bind.h>
#include <rt_print.h>
#include "kf_data_parse_bind.h"
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"

int clear_string(char *save, const char *value)
{
    const char *p = value;
    char *n = save;
    for(;;) {
        if (*p == '\0') break;
        if (*p == '\n' || *p == '\t' || *p == ' ') {
            p++;
            continue;
        }
        (*n) = *p;
        n++;
        p++;
    }
    return 0;
}
//errAlways = true initDevice =  true
TEST(KFGenerateDataTest1, Negative) {

    KFDataParseQueryIP *qip = new KFDataParseQueryIP();

    qip->KFDataParseQueryIPString("34BDF966A8", true, true);
    char str_for_md5[64] = "";
    char time_str[32] = {0};
    sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "queryAvailableServerIp-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);
    char gen_str_expect[512] = {0};
    sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"34BDF966A8\",\"type\":\"1\",\"vipFlag\":\"T\"}}],"
    "\"MsgHead\":{\"ServiceCode\":\"queryAvailableServerIp\",\"SrcSysID\":\"0001\","
    "\"SrcSysSign\":\"%s\","
    "\"transactionID\":\"%s\"}}", md5_result, time_str);

    const char *generate_result = qip->KFDataParseDataString();
    char generate_result_format[512] = {0};
    clear_string(generate_result_format, generate_result);

    // expect the string  get string      compare them
    EXPECT_STREQ(gen_str_expect, generate_result_format);
        if(NULL !=qip ) {
        delete qip;
        qip = NULL;
        }
}
//errAlways = true initDevice =  false
TEST(KFGenerateDataTest2, Negative) {

    KFDataParseQueryIP *qip = new KFDataParseQueryIP();

    qip->KFDataParseQueryIPString("34BDF966A8", true, false);
    char str_for_md5[64] = "";
    char time_str[32] = {0};
    sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "queryAvailableServerIp-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);
    char gen_str_expect[512] = {0};
    sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"34BDF966A8\",\"type\":\"0\",\"vipFlag\":\"T\"}}],\"MsgHead\":{\"ServiceCode\":\"queryAvailableServerIp\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"%s\",\"transactionID\":\"%s\"}}", md5_result, time_str);

    const char *generate_result = qip->KFDataParseDataString();
    char generate_result_format[512] = {0};
    clear_string(generate_result_format, generate_result);

    // expect the string  get string      compare them
    EXPECT_STREQ(gen_str_expect, generate_result_format);
    if(NULL !=qip ) {
    delete qip;
    qip = NULL;
    }
}
//errAlways = false initDevice =  true
TEST(KFGenerateDataTest3, Negative) {

    KFDataParseQueryIP *qip = new KFDataParseQueryIP();

    qip->KFDataParseQueryIPString("34BDF966A8", false, true);
    char str_for_md5[64] = "";
    char time_str[32] = {0};
    sprintf(time_str, "%ld", time(NULL));
    sprintf(str_for_md5, "queryAvailableServerIp-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
    unsigned char output[16] = {0};
    char md5_result[32] = {0};
    kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
    kfmd5_ngx_tostring(output, md5_result);
    char gen_str_expect[512] = {0};
    sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"34BDF966A8\",\"type\":\"1\"}}],\"MsgHead\":{\"ServiceCode\":\"queryAvailableServerIp\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"%s\",\"transactionID\":\"%s\"}}", md5_result, time_str);

    const char *generate_result = qip->KFDataParseDataString();
    char generate_result_format[512] = {0};
    clear_string(generate_result_format, generate_result);

    // expect the string  get string      compare them
    EXPECT_STREQ(gen_str_expect, generate_result_format);
    if(NULL !=qip ) {
    delete qip;
    qip = NULL;
    }
}
//GeneralError -2
TEST(KFDataParseQueryIPErrorTest, Negative) {

    KFDataParseQueryIP *qip = new KFDataParseQueryIP();

    char  dataerror[512]="{\"errorCode\":\"004\",\"errorMsg\":\"请求异常\"}";
    EXPECT_EQ(-2,qip->KFDataParseQueryIPResult(dataerror,sizeof(dataerror)));
    std::string error_code = qip->KFDataParseCmdErrorCode();
    std::string error_msg = qip->KFDataParseCmdErrorMsg();
    EXPECT_STREQ("004",error_code.c_str());
    EXPECT_STREQ("请求异常",error_msg.c_str());


    if(NULL !=qip ) {
    delete qip;
qip = NULL;
    }
}
//md5error -3
TEST(KFDataParseQueryIPmd5Test, Negative) {

    KFDataParseQueryIP *qip = new KFDataParseQueryIP();

    //  head   md5  is  error  then  return  -3  and  resultcode  is  null  and  resultmsg  is  null
    char datamd[1024]="{\"MsgHead\":{\"ServiceCode\":\"queryAvailableServerIp\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"c7ac3d63c42246ca23ec669312b74f0\",\"transactionID\":\"1439610486203\"},\"MsgBody\":{\"result\":{\"resultCode\":\"success\",\"resultData\":[{\"belongGroup\":0,\"distance\":\"10116.47\",\"latitude\":37.7749,\"longitude\":-122.4194,\"serverIp\":\"107.170.235.119\"}]}}}";
    EXPECT_EQ(-3,qip->KFDataParseQueryIPResult(datamd,sizeof(datamd)));

    if(NULL !=qip ) {
    delete qip;
    qip = NULL;
    }
}

//code is null -4
TEST(KFDataParseQueryIPCodeNULLTest, Negative) {
    KFDataParseQueryIP *qip = new KFDataParseQueryIP();

    char datamd[1024]="{\"MsgHead\":{\"ServiceCode\":\"queryAvailableServerIp\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"6c7ac3d63c42246ca23ec669312b74f0\",\"transactionID\":\"1439610486203\"},\"MsgBody\":{\"result\":{\"resultCode\":\"\",\"resultData\":[{\"belongGroup\":0,\"distance\":\"10116.47\",\"latitude\":37.7749,\"longitude\":-122.4194,\"serverIp\":\"107.170.235.119\"}]}}}";
    EXPECT_EQ(-4,qip->KFDataParseQueryIPResult(datamd,sizeof(datamd)));

    if(NULL !=qip ) {
    delete qip;
    qip = NULL;
    }
}


//no error
TEST(KFDataParseSuccessTest, Negative) {
    KFDataParseQueryIP *qip = new KFDataParseQueryIP();
    char datamd[1024]="{\"MsgHead\":{\"ServiceCode\":\"queryAvailableServerIp\",\"SrcSysID\":\"0001\",\"SrcSysSign\":\"6c7ac3d63c42246ca23ec669312b74f0\",\"transactionID\":\"1439610486203\"},\"MsgBody\":{\"result\":{\"resultCode\":\"success\",\"resultData\":[{\"belongGroup\":0,\"distance\":\"10116.47\",\"latitude\":37.7749,\"longitude\":-122.4194,\"serverIp\":\"107.170.235.119\"}]}}}";
    EXPECT_EQ(0,qip->KFDataParseQueryIPResult(datamd,sizeof(datamd)));

    if(NULL !=qip ) {
    delete qip;
    qip = NULL;
    }
}



int main(int argc,char *argv[])
{
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}