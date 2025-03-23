cmake_minimum_required(VERSION 3.30)

project(hex-launcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(QT_QML_GENERATE_QMLLS_INI ON)


find_package(Qt6 6.8 REQUIRED COMPONENTS Quick Widgets)
add_subdirectory(extern/qmicroz)

qt_standard_project_setup()
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(apphex-launcher
    src/main.cpp
)

qt_add_qml_module(apphex-launcher
    URI hex_launcher
    VERSION 1.0

    QML_FILES
        ui/Main.qml
        ui/components/GamePresetDelegate.qml
        ui/components/CustomDropDown.qml
        ui/components/CButton.qml
        ui/components/ErrorPopUp.qml
        ui/components/AddPresetPopUp.qml
        ui/components/Sidebar.qml

        ui/views/Presets.qml
        ui/views/Games.qml
        ui/views/Settings.qml

    RESOURCES    
        ui/icons/Presets.svg
        ui/icons/Menu.svg
        ui/icons/Settings.svg
        ui/icons/Games.svg



    SOURCES
        src/presetparser.h src/presetparser.cpp
        src/gamepreset.h src/gamepreset.cpp
        src/optionmodel.h src/optionmodel.cpp
        src/gamepresetmodel.h src/gamepresetmodel.cpp
        src/globalmodels.h src/globalmodels.cpp
        src/optionitem.h src/optionitem.cpp
        src/RRUpdater/updater.cpp
        src/RRUpdater/updater.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.

target_link_libraries(apphex-launcher
    PRIVATE Qt6::Quick Qt6::Widgets qmicroz
)

target_include_directories(apphex-launcher PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RRUpdater)

include(GNUInstallDirs)
install(TARGETS apphex-launcher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
