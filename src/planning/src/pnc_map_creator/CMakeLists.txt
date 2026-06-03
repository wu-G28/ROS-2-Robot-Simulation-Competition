cmake_minimum_required(VERSION 3.22.1)
project(pnc_map_creator)

#设置依赖
set(DEPENDS
    "rclcpp"
	"geometry_msgs"
	"visualization_msgs"
	"base_msgs"
)

#添加动态库
add_library(pnc_map_creator SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/pnc_map_global/pnc_map_global.cpp
)

#添加节点
add_executable(pnc_map_server
	${CMAKE_CURRENT_SOURCE_DIR}/pnc_map_server.cpp
)

set(project_target
	pnc_map_creator
	pnc_map_server
)

#添加依赖
foreach(target ${project_target})
	ament_target_dependencies(${target}
	${DEPENDS}
)
endforeach()

target_link_libraries(pnc_map_creator
    config_reader
)

target_link_libraries(pnc_map_server
	pnc_map_creator
)

#安装动态库、节点和相关目录
install(TARGETS
	pnc_map_creator
	LIBRARY DESTINATION lib
)

install(TARGETS
	pnc_map_server
	DESTINATION lib/${PLANNING_PACKAGE_NAME}
)
