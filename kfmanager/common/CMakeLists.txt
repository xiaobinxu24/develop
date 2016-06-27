
project(kfcommon)

set(SOURCE_COMMON_FILE
        bvpu_utils.c
        bvpu_utils.h
        jk_common.h
        jk_conn_tcp.c
        jk_conn_tcp.h
        jk_list.c
        jk_list.h
        rt_print.c
        rt_print.h
        rt_server_tcp.c
        rt_server_tcp.h
        XN_Queue.h)

set(BASE_FILES
        ${SOURCE_COMMON_FILE} jk_conn_udp.c jk_conn_udp.h)

add_library(kfcommon STATIC ${BASE_FILES})
target_link_libraries(kfcommon m libzlog)

link_directories(${CMAKE_BINARY_DIR}/common)

add_subdirectory(zlog)
add_subdirectory(unixsocket)