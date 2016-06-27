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
#include "gtest/gtest.h"
#include "main.h"
#include "ngx_md5.h"


TEST(KFProcessBindCodeYTest, Negative) {

	KFBindJudge bj = {"20F41BF69BB0"};
	KFProcessBind *pb = new KFProcessBind();
	KFDataParseBind *dpb = new KFDataParseBind();

	KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

	EXPECT_TRUE(NULL != pb);
	EXPECT_TRUE(NULL != dpb);
	EXPECT_TRUE(NULL != net);

	//resultMsg is  Y
	int ret = pb->DoBind(dpb,&bj,net);
	EXPECT_EQ(1,ret);


	if(NULL !=pb ) {
		delete pb;
		pb = NULL;
	}
	if(NULL != net){
		delete net;
		net = NULL;
	}
	if(NULL != dpb ) {
		delete dpb;
		dpb = NULL;
	}


}






TEST(KFProcessBindCodeSTest, Negative) {

	KFBindJudge bj = {"20F41BF69BB0"};
	KFProcessBind *pb = new KFProcessBind();
	KFDataParseBind *dpb = new KFDataParseBind();

	KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

	EXPECT_TRUE(NULL != pb);
	EXPECT_TRUE(NULL != dpb);
	EXPECT_TRUE(NULL != net);

	int ret = pb->DoBind(dpb,&bj,net);
	//resultMsg is  S
	EXPECT_EQ(-5,ret);


	if(NULL !=pb ) {
		delete pb;
		pb = NULL;
	}
	if(NULL != net){
		delete net;
		net = NULL;
	}
	if(NULL != dpb ) {
		delete dpb;
		dpb = NULL;
	}


}
TEST(KFProcessBindCodeNTest, Negative) {

	KFBindJudge bj = {"20F41BF69BB0"};
	KFProcessBind *pb = new KFProcessBind();
	KFDataParseBind *dpb = new KFDataParseBind();

	KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

	EXPECT_TRUE(NULL != pb);
	EXPECT_TRUE(NULL != dpb);
	EXPECT_TRUE(NULL != net);

	int ret = pb->DoBind(dpb,&bj,net);
	//resultMsg is  N
	EXPECT_EQ(-6,ret);



	if(NULL !=pb ) {
		delete pb;
		pb = NULL;
	}
	if(NULL != net){
		delete net;
		net = NULL;
	}
	if(NULL != dpb ) {
		delete dpb;
		dpb = NULL;
	}


}




TEST(KFProcessBindInvalidHeadTest, Negative) {

    KFBindJudge bj = {"20F41BF69BB0"};
    KFProcessBind *pb = new KFProcessBind();
    KFDataParseBind *dpb = new KFDataParseBind();

    KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

    EXPECT_TRUE(NULL != pb);
    EXPECT_TRUE(NULL != dpb);
    EXPECT_TRUE(NULL != net);

    int ret = pb->DoBind(dpb,&bj,net);
    //head is error  or didn't give msg head
    EXPECT_EQ(-4,ret);



    if(NULL !=pb ) {
        delete pb;
        pb = NULL;
    }
    if(NULL != net){
        delete net;
        net = NULL;
    }
    if(NULL != dpb ) {
        delete dpb;
        dpb = NULL;
    }


}










TEST(KFProcessBindInvalidHostTest, Negative) {

	KFBindJudge bj = {"20F41BF69BB0"};
	KFProcessBind *pb = new KFProcessBind();
	KFDataParseBind *dpb = new KFDataParseBind();

	EXPECT_TRUE(NULL != pb);
	EXPECT_TRUE(NULL != dpb);

	// invalid host
	KFNetwork *nn = new KFNetwork("api",8085, "unite/service", 5);

	EXPECT_TRUE(NULL != nn);

	EXPECT_EQ(-3,pb->DoBind(dpb,&bj,nn));


	if(NULL !=pb ) {
		delete pb;
		pb = NULL;
	}
	if(NULL != dpb ) {
		delete dpb;
		dpb = NULL;
	}

	if(NULL != nn){
		delete nn;
		nn = NULL;
	}
}

int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}

