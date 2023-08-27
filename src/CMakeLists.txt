include("${PROJECT_SOURCE_DIR}/cmake/extern-cmake-scripts/alglib-link.cmake")

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Concurrent PrintSupport REQUIRED)
#find_package(Qt5 REQUIRED COMPONENTS Concurrent)

# Qt files (ui, rc)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(API_HEADER_LIST_DIR "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}")

set(API_HEADER_LIST
    "${API_HEADER_LIST_DIR}/mainwindow.h"
    "${API_HEADER_LIST_DIR}/cubicspline.h"
    "${API_HEADER_LIST_DIR}/datastructs.h"
    "${API_HEADER_LIST_DIR}/interpolationwidget.h"
    "${API_HEADER_LIST_DIR}/interpolator.h"
    "${API_HEADER_LIST_DIR}/lagrangepoly.h"
    "${API_HEADER_LIST_DIR}/linearinterp.h"
    "${API_HEADER_LIST_DIR}/qcustomplot/qcustomplot.h"
    "${ALGLIB_HEADER_LIST}"

)

set(SOURCE_LIST
    "mainwindow.cpp"
    "mainwindow.ui"
    "interpolationwidget.cpp"
    "interpolationwidget.ui"
    "cubicspline.cpp"
    "lagrangepoly.cpp"
    "linearinterp.cpp"
    "qcustomplot/qcustomplot.cpp"
    "${ALGLIB_SOURCE_LIST}"

)

add_library(${PROJECT_NAME}
    ${SOURCE_LIST}
    ${API_HEADER_LIST}
)

message("qcustomplot ${PROJECT_SOURCE_DIR}/include/qcustomplot/")

add_library(drweb::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${ALGLIB_HEADERS_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/qcustomplot")
target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Widgets)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Concurrent Qt::PrintSupport)
