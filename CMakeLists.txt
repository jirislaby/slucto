cmake_minimum_required(VERSION 3.12.0)

project(slucto VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)
if (USE_WEXTRA)
	add_compile_options(-Wextra -pedantic -Wno-unused-parameter)
endif()
if (USE_WERROR)
	add_compile_options(-Werror)
endif()

add_subdirectory(frontend)

install(PROGRAMS slucto_cli slucto_create
	TYPE BIN
)
install(FILES faktura/faktura.tex
	DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/
)
