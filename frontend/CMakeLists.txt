set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_COMPONENTS Core Gui Sql Widgets)
find_package(Qt6 QUIET COMPONENTS ${QT_COMPONENTS})
if (NOT Qt6_FOUND)
	find_package(Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
endif()

add_compile_definitions(QT_NO_FOREACH QT_DEPRECATED_WARNINGS)

add_executable(slucto
	itemmodel.cpp
	itemmodel.h
	main.cpp
	newinvoice.cpp
	newinvoice.h
	newinvoice.ui
	picker.cpp
	picker.h
	picker.ui
	printinvoice.cpp
	printinvoice.h
	printinvoice.ui
)

add_compile_definitions(QT_DEPRECATED_WARNINGS)

target_link_libraries(slucto Qt::Core Qt::Gui Qt::Sql Qt::Widgets)

install(TARGETS slucto
	RUNTIME
)
