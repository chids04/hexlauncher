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
        ui/components/CCombobox.qml
        ui/components/DownloadProgress.qml

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
        src/optionitem.h src/optionitem.cpp

        src/models/optionmodel.h src/models/optionmodel.cpp
        src/models/gamepresetmodel.h src/models/gamepresetmodel.cpp
        src/models/globalmodels.h src/models/globalmodels.cpp
        src/models/gamesmodel.h src/models/gamesmodel.cpp

        src/RRUpdater/updater.cpp
        src/RRUpdater/updater.h
        src/utils/common.h

        src/utils/unzipper.h


        
)

if(WIN32)
    add_custom_target(kill_apphex_launcher
        COMMAND taskkill /IM apphex-launcher.exe /F || exit 0
        COMMENT "Killing previous instance of apphex-launcher.exe"
    )
else()  # Linux/macOS
    add_custom_target(kill_apphex_launcher
        COMMAND killall -9 apphex-launcher || true
        COMMENT "Killing previous instance of apphex-launcher"
    )
endif()

# Make the apphex-launcher target depend on the kill target
add_dependencies(apphex-launcher kill_apphex_launcher)


target_link_libraries(apphex-launcher
    PRIVATE Qt6::Quick Qt6::Widgets qmicroz
)

target_include_directories(apphex-launcher PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/RRUpdater)

