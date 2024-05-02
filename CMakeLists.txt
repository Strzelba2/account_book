cmake_minimum_required(VERSION 3.16)

project(ROZLICZENIE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMake version: ${CMAKE_VERSION}")

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_LIBRARIES "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD")

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/qrcodegen)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd/libssl.lib")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd/libcrypto.lib")
else()
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libssl.lib")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib")
endif()

message("OPENSSL_INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR}")
message("OPENSSL_CRYPTO_LIBRARY: ${OPENSSL_CRYPTO_LIBRARY}")
message("OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY}")

find_package(Qt6 6.2 COMPONENTS Quick Sql Qml REQUIRED)

set(SOURCES
    main.cpp
    user.cpp
    databasemanager.cpp
    viewservice.cpp
    book.cpp
    sessionmanager.cpp
    bookmodel.cpp
    usbuser.cpp
    totpmanager.cpp
    qrcodegen/qrcodegen.cpp
    loginstate.cpp
    subcontractor.cpp
    subcontractormodel.cpp
    )

set(HEADERS
    user.hpp
    databasemanager.hpp
    viewservice.hpp
    book.hpp
    sessionmanager.hpp
    bookmodel.hpp
    usbuser.hpp
    totpmanager.hpp
    qrcodegen/qrcodegen.hpp
    loginstate.hpp
    subcontractor.hpp
    subcontractormodel.hpp
    )

qt_add_resources(SOURCES qml.qrc)

qt_add_executable(appROZLICZENIE
    main.cpp
    ${SOURCES}
    ${HEADERS}
)

qt_add_qml_module(appROZLICZENIE
    URI ROZLICZENIE
    VERSION 1.0
    QML_FILES main.qml
    QML_FILES Register.qml
    QML_FILES Company.qml
    QML_FILES Login.qml
    QML_FILES LoginAdmin.qml
    QML_FILES Pin.qml
    QML_FILES QRCode.qml
    QML_FILES Totp.qml
    QML_FILES PopupWindow.qml
    QML_FILES LoadWindow.qml
    QML_FILES Subcontractor.qml
    QML_FILES Book.qml
    QML_FILES AddBooks.qml
    QML_FILES SubcontractorList.qml
    QML_FILES SubcontractorWindow.qml
)

set_target_properties(appROZLICZENIE PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appROZLICZENIE
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(appROZLICZENIE
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Qml
    PRIVATE ${OPENSSL_SSL_LIBRARY}
    PRIVATE ${OPENSSL_CRYPTO_LIBRARY}
    )
