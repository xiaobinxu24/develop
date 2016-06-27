project(kfmanager)

#add_definitions(-D_MAXOS)
add_definitions(-DUSE_ZLOG)

set(BASE_FILE
        kf_data_parse.cpp
        kf_data_parse.h
        kf_network.cpp
        kf_network.h
        kf_config.cpp
        kf_config.h
        kf_process.cpp
        kf_process.h
        kf_data_parse_queryip.cpp
        kf_data_parse_queryip.h
        kf_sys_operation.cpp
        kf_sys_operation.h
        kf_data_parse_pingupload.cpp
        kf_data_parse_pingupload.h
        kf_data_parse_traffic.cpp
        kf_data_parse_traffic.h
        kf_data_parse_onlinenotify.cpp
        kf_data_parse_onlinenotify.h
        kf_data_parse_bind.cpp
        kf_data_parse_bind.h
        kf_data_parse_error.cpp
        kf_data_parse_error.h
        kf_process_queryip.cpp
        kf_process_queryip.h
        kf_process_pingcheck.cpp
        kf_process_pingcheck.h
        kf_process_pingupload.cpp
        kf_process_pingupload.h
        kf_process_traffic.cpp
        kf_process_traffic.h
        kf_process_bind.cpp
        kf_process_bind.h
        kf_process_online_notify.cpp
        kf_process_online_notify.h
        kf_process_error.cpp
        kf_process_error.h
        kf_msg_data_parse.cpp
        kf_msg_data_parse.h
        checkNetworkAvailable.h
        checkNetworkAvailable.cpp
        )

set(MAIN_FILE
        main.cpp
        main.h)

add_library(kfmng STATIC ${BASE_FILE})

add_executable(kfmanager ${MAIN_FILE} ${BASE_FILE})

if(PC)
target_link_libraries(kfmanager kfnet kfmd5 kfjson kfcommon rtunixsocket lua5.1 dl)
else()
target_link_libraries(kfmanager kfnet kfmd5 kfjson kfcommon rtunixsocket lua dl)
endif()

#add_library(kfprogram STATIC)
#target_link_libraries(kfprogram kfcommon)


