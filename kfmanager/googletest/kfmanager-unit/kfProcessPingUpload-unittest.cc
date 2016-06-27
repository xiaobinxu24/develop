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
#include <rt_print.h>
#include "gtest/gtest.h"
#include "kf_process_pingupload.h"
#include "main.h"
#include "ngx_md5.h"




// result
// 0      success
// -1     Input error
// -2     GenerateData error
// -3     NetworkRequest != 200
//-4      DateParseResult error
TEST(DoPingUploadTest, Negative) {


	const char * id = "20F41BF69BB0";
	const char *wanip = "192.168.133.1";
	const char *resourceip= "123.133.23.201";

	KFDataParsePingUpload * pu = new KFDataParsePingUpload(id,wanip,resourceip);
	KFSSResultData ssData = {"aes-256-cfb","3eaac780-7c25-42bb-8735-26db3e385382","162.243.140.64",10090,1080};
	KFDataParseIPProperties ips = {"212.168.333.212"};
	KFPingCheckResult result = {3,2,1,22,10};

	KFProcessPingUpload * ppu = new KFProcessPingUpload();

	KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

	EXPECT_TRUE(NULL != ppu);
	EXPECT_TRUE(NULL != pu);
	EXPECT_TRUE(NULL != net);


	int ret = ppu->DoPingUpload(pu,net,&ssData,&ips,&result);

    //resultCode  is    FAIL-013
    EXPECT_EQ(-11,ret);

	if(  NULL != pu ){

		delete pu;
		pu  = NULL;
	}

	if(  NULL != ppu ){

		delete ppu;
		ppu = NULL;
	}


	if(  NULL != net ){

		delete net;
		net  = NULL;
	}

}




TEST(DoPingUploadSuccessTest, Negative) {


    const char * id = "20F41BF69BB0";
    const char *wanip = "192.168.133.1";
    const char *resourceip= "123.133.23.201";

    KFDataParsePingUpload * pu = new KFDataParsePingUpload(id,wanip,resourceip);
    KFSSResultData ssData = {"aes-256-cfb","3eaac780-7c25-42bb-8735-26db3e385382","162.243.140.64",10090,1080};
    KFDataParseIPProperties ips = {"212.168.333.212"};
    KFPingCheckResult result = {3,2,1,22,10};

    KFProcessPingUpload * ppu = new KFProcessPingUpload();

    KFNetwork *net = new KFNetwork("cloud.melinkr.cn",8085, "unite/service", 5);

    EXPECT_TRUE(NULL != ppu);
    EXPECT_TRUE(NULL != pu);
    EXPECT_TRUE(NULL != net);


    int ret = ppu->DoPingUpload(pu,net,&ssData,&ips,&result);
    //resultCode is  success
    EXPECT_EQ(0,ret);



    if(  NULL != pu ){

        delete pu;
        pu  = NULL;
    }

    if(  NULL != ppu ){

        delete ppu;
        ppu = NULL;
    }


    if(  NULL != net ){

        delete net;
        net  = NULL;
    }

}


//invalid  host
TEST(DoPingUploadInvalidHostTest, Negative) {



	const char * id = "20F41BF69BB0";
	const char *wanip = "192.168.133.1";
	const char *resourceip= "123.133.23.201";

	KFDataParsePingUpload * pu = new KFDataParsePingUpload(id,wanip,resourceip);
	KFSSResultData ssData = {"aes-256-cfb","3eaac780-7c25-42bb-8735-26db3e385382","162.243.140.64",10090,1080};
	KFDataParseIPProperties ips = {"212.168.333.212"};
	KFPingCheckResult result = {3,2,1,22,10};

	KFProcessPingUpload * ppu = new KFProcessPingUpload();

    KFNetwork *nw = new KFNetwork("api",8085, "unite/service", 5);


	EXPECT_TRUE(NULL != ppu);
	EXPECT_TRUE(NULL != pu);
	EXPECT_TRUE(NULL != nw);

	EXPECT_EQ(-3,ppu->DoPingUpload(pu,nw,&ssData,&ips,&result));

	if(  NULL != pu ){

		delete pu;
		pu  = NULL;
	}

	if(  NULL != ppu ){

		delete ppu;
		ppu = NULL;
	}


	if(  NULL != nw ){

		delete nw;
		nw  = NULL;
	}
}


int main(int argc,char *argv[])
{

	testing::InitGoogleTest(&argc,argv);

	return RUN_ALL_TESTS();
}

