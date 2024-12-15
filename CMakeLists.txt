cmake_minimum_required(VERSION 3.16)

project(hex-launcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)
set(QT_QML_GENERATE_QMLLS_INI ON)

qt_add_executable(apphex-launcher
    main.cpp
)

qt_add_qml_module(apphex-launcher
    URI hex_launcher
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES components/GamePresetDelegate.qml

        SOURCES src/presetparser.h src/presetparser.cpp
        SOURCES src/gamepreset.h src/gamepreset.cpp
        SOURCES src/optionmodel.h src/optionmodel.cpp
        SOURCES src/gamepresetmodel.h src/gamepresetmodel.cpp
        SOURCES src/globalmodels.h src/globalmodels.cpp
        QML_FILES components/CButton.qml
        QML_FILES components/ErrorPopUp.qml
        QML_FILES components/AddPresetPopUp.qml
        SOURCES src/optionitem.h src/optionitem.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apphex-launcher PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apprr-launcher
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apphex-launcher
    PRIVATE Qt6::Quick Qt6::Widgets
)

target_include_directories(apphex-launcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(GNUInstallDirs)
install(TARGETS apphex-launcher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
