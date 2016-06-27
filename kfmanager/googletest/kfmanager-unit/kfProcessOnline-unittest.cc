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
#include "kf_process_online_notify.h"
#include <rt_print.h>
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"



TEST(KFOnlineNotifyTest, Negative) {

	KFOnlineNotify on = {"20F41BF69BB0"};
	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);
	KFProcessOnlineNotify *onlineNotify = new KFProcessOnlineNotify();

	EXPECT_TRUE(NULL != onlineNotify);
	EXPECT_TRUE(NULL != net);
	EXPECT_TRUE(NULL != dpon);

   // 由于单独测试的话 服务器返回"MsgBody": {"result": {"resultCode":"FAIL-801","resultMsg":"【20F41BF69BB0】设备对应端口【0】已经释放，请重新获取"} ，所以比较数值为-4
   // 服务器正常返回结果应该是"MsgBody": {"result": {"resultCode":"success"},应该与0比较
   //  EXPECT_EQ(-4,onlineNotify->DoOnlineNotify(dpon,&on,net));
   int ret =  onlineNotify->DoOnlineNotify(dpon,&on,net);

	// resultCode  is  success
	EXPECT_EQ(0,ret);

	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}
	if(NULL != net){
		delete net;
		net = NULL;
	}

	if(NULL != onlineNotify){
		delete onlineNotify;
		onlineNotify = NULL;
	}

}


TEST(KFOnlineNotifyCodeTest, Negative) {

	KFOnlineNotify on = {"20F41BF69BB0"};
	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);
	KFProcessOnlineNotify *onlineNotify = new KFProcessOnlineNotify();

	EXPECT_TRUE(NULL != onlineNotify);
	EXPECT_TRUE(NULL != net);
	EXPECT_TRUE(NULL != dpon);

	// 由于单独测试的话 服务器返回"MsgBody": {"result": {"resultCode":"FAIL-801","resultMsg":"【20F41BF69BB0】设备对应端口【0】已经释放，请重新获取"} ，所以比较数值为-4
	// 服务器正常返回结果应该是"MsgBody": {"result": {"resultCode":"success"},应该与0比较
	//  EXPECT_EQ(-4,onlineNotify->DoOnlineNotify(dpon,&on,net));
	int ret =  onlineNotify->DoOnlineNotify(dpon,&on,net);

	// resultCode  is  FAIL-801
	EXPECT_EQ(-4,ret);

	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}
	if(NULL != net){
		delete net;
		net = NULL;
	}

	if(NULL != onlineNotify){
		delete onlineNotify;
		onlineNotify = NULL;
	}

}

TEST(KFOnlineNotifyInvalidHostTest, Negative) {

	KFOnlineNotify on = {"20F41BF69BB0"};
	KFDataParseOnlineNotify *dpon = new KFDataParseOnlineNotify();
	KFProcessOnlineNotify *onlineNotify = new KFProcessOnlineNotify();

	EXPECT_TRUE(NULL != onlineNotify);
	EXPECT_TRUE(NULL != dpon);


	//invalid  host
	KFNetwork *nw = new KFNetwork("api",8085, "unite/service", 5);
	EXPECT_TRUE(NULL != nw);
	EXPECT_EQ(-3,onlineNotify->DoOnlineNotify(dpon,&on,nw));


	if(NULL != dpon ) {
		delete dpon;
		dpon = NULL;
	}
	if(NULL != nw){

		delete  nw;
		nw = NULL;
	}

	if(NULL != onlineNotify){
		delete onlineNotify;
		onlineNotify = NULL;
	}

}



int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}

