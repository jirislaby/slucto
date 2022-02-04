set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Sql Widgets REQUIRED)

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

target_link_libraries(slucto Qt5::Core Qt5::Gui Qt5::Sql Qt5::Widgets)

install(TARGETS slucto
	RUNTIME
)
