set(ALGLIB_HEADERS_DIR "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/alglib")
set(ALGLIB_SRC_DIR "alglib")

set(ALGLIB_HEADER_LIST
    "${ALGLIB_HEADERS_DIR}/interpolation.h"
    "${ALGLIB_HEADERS_DIR}/ap.h"
    "${ALGLIB_HEADERS_DIR}/alglibinternal.h"
    "${ALGLIB_HEADERS_DIR}/alglibmisc.h"
    "${ALGLIB_HEADERS_DIR}/linalg.h"
    "${ALGLIB_HEADERS_DIR}/solvers.h"
    "${ALGLIB_HEADERS_DIR}/optimization.h"
    "${ALGLIB_HEADERS_DIR}/specialfunctions.h"
    "${ALGLIB_HEADERS_DIR}/integration.h"
    "${ALGLIB_HEADERS_DIR}/stdafx.h"

)

set(ALGLIB_SOURCE_LIST
    "${ALGLIB_SRC_DIR}/interpolation.cpp"
    "${ALGLIB_SRC_DIR}/ap.cpp"
    "${ALGLIB_SRC_DIR}/alglibinternal.cpp"
    "${ALGLIB_SRC_DIR}/alglibmisc.cpp"
    "${ALGLIB_SRC_DIR}/linalg.cpp"
    "${ALGLIB_SRC_DIR}/solvers.cpp"
    "${ALGLIB_SRC_DIR}/optimization.cpp"
    "${ALGLIB_SRC_DIR}/specialfunctions.cpp"
    "${ALGLIB_SRC_DIR}/integration.cpp"
)

