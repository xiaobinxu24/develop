//
// Created by v on 16-3-15.
// Author: jmdvirus@roamter.com
//

#include "json/json.h"

#include <iostream>

using namespace std;


int main() {
    Json::Value j;
    j["test1"] = "1234";
    j["value"] = 8;

    j["atest"]["num"] = 9;

    int cnts = j.size();
    cout << " cnts: " << cnts << endl;

    string out = j.toStyledString();
    cout << " out : " << out << endl;

    Json::Reader r;
    Json::Value root;
    r.parse(out, root, 0);
    cout << "out : " << root["tet1"] << endl;
//    Json::Value v = root.Value("test2");
//    cout << "ooo: " << v << endl;
    if (root["test2"] == Json::nullValue) {
        cout << "reuslt: null " << endl;
    }

    return 0;
}

