set(TESTING_LIBRARY_NAME ${PROJECT_NAME})
project(
    "${TESTING_LIBRARY_NAME}-test"
    LANGUAGES CXX
)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

# Qt files (ui, rc)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

enable_testing(true)

add_executable(${PROJECT_NAME}
    "test_main.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Test)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE msd::${TESTING_LIBRARY_NAME})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
