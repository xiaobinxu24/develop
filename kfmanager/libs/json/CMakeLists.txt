
project(kfjson)

set(BASE_JSON_FILE
        json_reader.cpp
        json_value.cpp
        json_writer.cpp
        json_valueiterator.inl
        json_tool.h)

set(DEMO_BASE_JSON_FILE
        demo_json.cpp
        )

add_library(kfjson ${BASE_JSON_FILE})

add_executable(demo_json ${DEMO_BASE_JSON_FILE} ${BASE_JSON_FILE})
#target_link_libraries(demo_json json)
