SET(TF_CURRENT_BINARY_PATH_LIB "../lib")
SET(LIBRARY_OUTPUT_PATH ${TF_CURRENT_BINARY_PATH_LIB})

INCLUDE_DIRECTORIES(../include)

##update_begin


INCLUDE_DIRECTORIES(../../../../include)


SET (INCLUDE_LIST
	../include/iconv.h
)

SET (SRC_LIST
	../src/iconv.c
	../src/localcharset.c
	../src/relocatable.c
)

SET (RESOURCE_LIST
	../src/libiconv.rc
)



if (WIN32)
	source_group(include FILES ${INCLUDE_LIST})
	source_group(src FILES ${SRC_LIST})
	source_group(resource FILES ${RESOURCE_LIST})
endif()


ADD_LIBRARY(iconv
	${INCLUDE_LIST}
	${SRC_LIST}
	${RESOURCE_LIST}
)


##update_end

IF(CMAKE_SYSTEM MATCHES Windows)
  TARGET_LINK_LIBRARIES(vnet ws2_32)
ELSE()
  if(USE_32BITS)
    SET(CMAKE_C_FLAGS "-Wall -ggdb -pipe -march=i386 -mtune=i686")
    SET(CMAKE_CXX_FLAGS "-Wall -ggdb -pipe -march=i386 -mtune=i686")
  else()
    SET(CMAKE_C_FLAGS "-Wall -ggdb -pipe -march=x86-64 -mtune=i686")
    SET(CMAKE_CXX_FLAGS "-Wall -ggdb -pipe -march=x86-64 -mtune=i686")
  endif(USE_32BITS)
ENDIF(CMAKE_SYSTEM MATCHES Windows)


