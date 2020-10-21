
SET(_assimp_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/includes")
SET(_assimp_LIBS_SEARCH_DIRS
"/usr/lib"
"${CMAKE_SOURCE_DIR}/lib")
# check environment variable
SET(_assimp_ENV_ROOT_DIR "$ENV{ASSIMP_ROOT_DIR}")
IF(NOT ASSIMP_ROOT_DIR AND _assimp_ENV_ROOT_DIR)
	SET(ASSIMP_ROOT_DIR "${_assimp_ENV_ROOT_DIR}")
ENDIF(NOT ASSIMP_ROOT_DIR AND _assimp_ENV_ROOT_DIR)
# put user specified location at beginning of search
IF(ASSIMP_ROOT_DIR)
	SET(_assimp_HEADER_SEARCH_DIRS "${ASSIMP_ROOT_DIR}"
	"${ASSIMP_ROOT_DIR}/include"
	${_assimp_HEADER_SEARCH_DIRS})
ENDIF(ASSIMP_ROOT_DIR)
# locate header
FIND_PATH(ASSIMP_INCLUDE_DIR "assimp/"
PATHS ${_assimp_HEADER_SEARCH_DIRS})
INCLUDE(FindPackageHandleStandardArgs)
FIND_PATH(ASSIMP_LIB_DIR "libassimp.so" PATHS ${_assimp_LIBS_SEARCH_DIRS})
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Assimp DEFAULT_MSG
ASSIMP_INCLUDE_DIR ASSIMP_LIB_DIR)
IF(ASSIMP_FOUND)
	SET(ASSIMP_INCLUDE_DIRS "${ASSIMP_INCLUDE_DIR}")
	SET(ASSIMP_LIB "${ASSIMP_LIB_DIR}/libassimp.so")
	MESSAGE(STATUS "ASSIMP_INCLUDE_DIR = ${ASSIMP_INCLUDE_DIR}")
	MESSAGE(STATUS "ASSIMP_LIB = ${ASSIMP_LIB}")
ENDIF(ASSIMP_FOUND)