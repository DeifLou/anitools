option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_BILATERALFILTER
    "Build the \"bilateral filter\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_BILATERALFILTER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.bilateralfilter
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.bilateralfilter
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.bilateralfilter
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.bilateralfilter
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.bilateralfilter
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.bilateralfilter)
endif()