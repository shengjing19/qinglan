cmake_minimum_required(VERSION 3.5)

project(ShengXiaoBooter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CURL_INCLUDE_DIR "D:/VisualStudio/Qt/CLion_ShengXiaoBooter/example/CURL/include")
set(CURL_LIBRARY "D:/VisualStudio/Qt/CLion_ShengXiaoBooter/example/CURL/lib/libcurl.lib")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(CURL REQUIRED)


file(GLOB_RECURSE CPP_FILES  *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND example_files ${filename})
endforeach(filepath)

set(PROJECT_SOURCES
        ${example_files}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ShengXiaoBooter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        main.cpp
        1.rc
        admin_manifest.rc
    )


    #遍历所有资源文件
    file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js *.ini *.rc)
    foreach(filepath ${RES_PATHS})
            string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
            list(APPEND resource_files ${filename})
    endforeach(filepath)

    qt_add_resources(${PROJECT_NAME} "ShengXiaoBooter"
        SRESOURCES PREFIX "/"
        FILES
        ${resource_files}
    )

endif()

include_directories(
    ../src/include
    ExamplePage
    ModelView
    #CURL
)

set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_BINARY_DIR})
# 查找 Qt6 并指定需要的组件
find_package(Qt6 COMPONENTS Sql REQUIRED)

target_link_libraries(ShengXiaoBooter PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    elawidgettools   #src库项目名
    #${CURL_LIBRARY}
    CURL::libcurl  # 添加这行
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ShengXiaoBooter)
endif()
set_target_properties(ShengXiaoBooter PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ShengXiaoBooter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ShengXiaoBooter)
endif()
