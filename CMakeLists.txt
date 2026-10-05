cmake_minimum_required(VERSION 3.16.0)
list(APPEND EXTRA_COMPONENT_DIRS
	"${CMAKE_CURRENT_LIST_DIR}/lib/esp_adapter"
	"${CMAKE_CURRENT_LIST_DIR}/lib/fan"
	"${CMAKE_CURRENT_LIST_DIR}/lib/app"
	"${CMAKE_CURRENT_LIST_DIR}/lib/adapter"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(VentSimulationEspIdf)
