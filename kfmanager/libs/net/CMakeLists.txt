project(kfnet)

#add_library(kflibs STATIC)
#target_link_libraries(kflibs kfjson)

set(BASE_HTTP_FILES
        http/ghttp.c
        http/ghttp.h
        http/ghttp_constants.h
        http/http_base64.c
        http/http_base64.h
        http/http_date.c
        http/http_date.h
        http/http_global.h
        http/http_hdrs.c
        http/http_hdrs.h
        http/http_req.c
        http/http_req.h
        http/http_resp.c
        http/http_resp.h
        http/http_trans.c
        http/http_trans.h
        http/http_uri.c
        http/http_uri.h
        http/KFHttp.cpp
        http/KFHttp.h)

set(DEMO_BASE_HTTP_FILES
        http/demo_http.cpp)

add_library(kfnet ${BASE_HTTP_FILES})

add_executable(demo_http ${DEMO_BASE_HTTP_FILES} ${BASE_HTTP_FILES})
target_link_libraries(demo_http kfcommon)
