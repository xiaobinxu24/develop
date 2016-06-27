// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)


// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

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


//0        success
//-1       NULL input
//-2       Generral error
//-3       Head error
//-4       Result error

TEST(KFGenerateDataTest, Negative) {

	KFDataParseBind *npb = new KFDataParseBind();
	KFBindJudge bj = {"20F41BF69BB0"};
	EXPECT_TRUE(NULL != npb);

	EXPECT_EQ(0,npb->KFDataParseBindGenerateData(&bj));

	char str_for_md5[64] = "";
	char time_str[32] = {0};
	sprintf(time_str, "%ld", time(NULL));
	sprintf(str_for_md5, "bindJudge-0001-%s-09d08113-74b4-4ada-a04e-17358b6d0203", time_str);
	unsigned char output[16] = {0};
	char md5_result[32] = {0};
	kfmd5_ngx((unsigned char*)str_for_md5, strlen(str_for_md5), output);
	kfmd5_ngx_tostring(output, md5_result);
	char gen_str_expect[512] = {0};
	sprintf(gen_str_expect, "{\"MsgBody\":[{\"PARAMS\":{\"mac\":\"20F41BF69BB0\"}}],"
			"\"MsgHead\":{\"ServiceCode\":\"bindJudge\",\"SrcSysID\":\"0001\","
			"\"SrcSysSign\":\"%s\","
			"\"transactionID\":\"%s\"}}", md5_result, time_str);

	const char *generate_result = npb->KFDataParseBindGenerateData();
	char generate_result_format[512] = {0};
	clear_string(generate_result_format, generate_result);

	// expect the string  get string      compare them
	EXPECT_STREQ(gen_str_expect, generate_result_format);


	if(NULL !=npb ) {
		delete npb;
		npb = NULL;
	}
}



TEST(KFDataParseBindErrorTest, Negative) {

	KFDataParseBind *npb = new KFDataParseBind();
	KFBindJudge bj = {"20F41BF69BB0"};
	EXPECT_TRUE(NULL != npb);


	char  dataerror[512]="{\"errorCode\":\"004\",\"errorMsg\":\"请求异常\"}";
	EXPECT_EQ(-2,npb->KFDataParseResult(dataerror,sizeof(dataerror)));
	std::string error_code = npb->KFDataParseCmdErrorCode();
	std::string error_msg = npb->KFDataParseCmdErrorMsg();
	EXPECT_STREQ("004",error_code.c_str());
	EXPECT_STREQ("请求异常",error_msg.c_str());


	if(NULL !=npb ) {
		delete npb;
		npb = NULL;
	}
}


TEST(KFDataParseBindmd5Test, Negative) {

    KFDataParseBind *npb = new KFDataParseBind();
    KFBindJudge bj = {"20F41BF69BB0"};
    EXPECT_TRUE(NULL != npb);

    //  head   md5  is  error  then  return  -3  and  resultcode  is  null  and  resultmsg  is  null
    char  datamd[512]="{\"MsgHead\":{ \"ServiceCode\":\"bindJudge\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"2bad7c36f77f552c010e22d6af7f5586\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\", \"resultMsg\":\"N\" } } ";
    EXPECT_EQ(-3,npb->KFDataParseResult(datamd,sizeof(datamd)));

    if(NULL !=npb ) {
        delete npb;
        npb = NULL;
    }
}


TEST(KFDataParseBindCodeNULLTest, Negative) {

    KFDataParseBind *npb = new KFDataParseBind();
    KFBindJudge bj = {"20F41BF69BB0"};
    EXPECT_TRUE(NULL != npb);


    //  code  is null   then return -4  and resultcode  is  null  and  resultmsg  is  null
    char datacodemsgn[512]="{\"MsgHead\":{ \"ServiceCode\":\"bindJudge\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"1bad7c36f77f552c010e22d6af7f5586\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":, \"resultMsg\":\"N\" } } ";
    EXPECT_EQ(-4,npb->KFDataParseResult(datacodemsgn,sizeof(datacodemsgn)));

    if(NULL !=npb ) {
        delete npb;
        npb = NULL;
    }
}



TEST(KFDataParseBindMsgNULLTest, Negative) {

    KFDataParseBind *npb = new KFDataParseBind();
    KFBindJudge bj = {"20F41BF69BB0"};
    EXPECT_TRUE(NULL != npb);


    // code  is  success and  msg  is  null
    char datamsg[512]="{\"MsgHead\":{ \"ServiceCode\":\"bindJudge\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"1bad7c36f77f552c010e22d6af7f5586\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\", \"resultMsg\":} } ";
    EXPECT_EQ(0,npb->KFDataParseResult(datamsg,sizeof(datamsg)));
    std::string ret_code = npb->KFDataParseResultCode();
    EXPECT_STREQ("success",ret_code.c_str());

    if(NULL !=npb ) {
        delete npb;
        npb = NULL;
    }
}

TEST(KFDataParseBindMsgNTest, Negative) {

    KFDataParseBind *npb = new KFDataParseBind();
    KFBindJudge bj = {"20F41BF69BB0"};
    EXPECT_TRUE(NULL != npb);

    // code  is  success and  msg  is  N
    char datastrtest[512]="{\"MsgHead\":{ \"ServiceCode\":\"bindJudge\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"1bad7c36f77f552c010e22d6af7f5586\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\", \"resultMsg\":\"N\" } } ";
    EXPECT_EQ(0,npb->KFDataParseResult(datastrtest,sizeof(datastrtest)));
    std::string data_code = npb->KFDataParseResultCode();
    const char * data_msg = npb->BindResult();
    EXPECT_STREQ("success",data_code.c_str());
    EXPECT_STREQ("N",data_msg);
    if(NULL !=npb ) {
        delete npb;
        npb = NULL;
    }
}


TEST(KFDataParseBindMsgSTest, Negative) {

    KFDataParseBind *npb = new KFDataParseBind();
    KFBindJudge bj = {"20F41BF69BB0"};
    EXPECT_TRUE(NULL != npb);


    // code  is  success and  msg  is  S
    char dstr[512]="{\"MsgHead\":{ \"ServiceCode\":\"bindJudge\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"1bad7c36f77f552c010e22d6af7f5586\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\", \"resultMsg\":\"S\" } } ";
    EXPECT_EQ(0,npb->KFDataParseResult(dstr,sizeof(dstr)));
    std::string dstr_code = npb->KFDataParseResultCode();
    const char * dstr_msg = npb->BindResult();
    EXPECT_STREQ("success",dstr_code.c_str());
    EXPECT_STREQ("S",dstr_msg);

    if(NULL !=npb ) {
        delete npb;
        npb = NULL;
    }
}


TEST(KFDataParseBindMsgYTest, Negative) {

    KFDataParseBind *npb = new KFDataParseBind();
    KFBindJudge bj = {"20F41BF69BB0"};
    EXPECT_TRUE(NULL != npb);

    // code  is  success and  msg  is  Y
    char strtest[512]="{\"MsgHead\":{ \"ServiceCode\":\"bindJudge\", \"SrcSysID\":\"0001\", \"SrcSysSign\":\"1bad7c36f77f552c010e22d6af7f5586\", \"transactionID\":\"1448006795202\" },\"MsgBody\": { \"result\": { \"resultCode\":\"success\", \"resultMsg\":\"Y\" } } ";
    EXPECT_EQ(0,npb->KFDataParseResult(strtest,sizeof(strtest)));
    std::string code = npb->KFDataParseResultCode();
    const char * msg = npb->BindResult();;
    EXPECT_STREQ("success",code.c_str());
    EXPECT_STREQ("Y",msg);


    if(NULL !=npb ) {
        delete npb;
        npb = NULL;
    }
}

int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}

