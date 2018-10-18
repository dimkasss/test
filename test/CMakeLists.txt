cmake_minimum_required (VERSION 2.10.0)

set (PROJECT test) 
project (${PROJECT}) 

# exe 
set(MAIN_PROGRAM detector)
set (SOURCES_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_PROGRAM}/src/detector.cpp)
set (INCLUDES_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/boost_1_66_0/)
set (LIBS_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/boost_1_66_0/lib64-msvc-14.0/)
INCLUDE_DIRECTORIES(${INCLUDES_MAIN})
LINK_DIRECTORIES(${LIBS_MAIN})

add_executable (${MAIN_PROGRAM} ${INCLUDES_MAIN} ${SOURCES_MAIN})
target_link_libraries(${MAIN_PROGRAM} optimized libboost_system-vc140-mt-x64-1_66.lib)
target_link_libraries(${MAIN_PROGRAM} optimized libboost_filesystem-vc140-mt-x64-1_66.lib)
target_link_libraries(${MAIN_PROGRAM} debug libboost_system-vc140-mt-gd-x64-1_66.lib)
target_link_libraries(${MAIN_PROGRAM} debug libboost_filesystem-vc140-mt-gd-x64-1_66.lib)

#dll
set (DLL_CODE face)
set (SOURCES_DLL ${CMAKE_CURRENT_SOURCE_DIR}/${DLL_CODE}/src/detectFace.cpp)
set (INCLUDES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${DLL_CODE}/include/exported.h)
set (INCLUDES_DLL ${CMAKE_CURRENT_SOURCE_DIR}/${DLL_CODE}/include/
				  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opencv/opencv_3.4.3_vc14/include/ )
set (LIBS_DLL ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/boost_1_66_0/lib64-msvc-14.0
			  ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opencv/opencv_3.4.3_vc14/lib)

INCLUDE_DIRECTORIES(${INCLUDES_DLL})
LINK_DIRECTORIES(${LIBS_DLL})

add_library(${DLL_CODE} SHARED ${INCLUDES_FILES} ${SOURCES_DLL})
target_link_libraries(${DLL_CODE} optimized libboost_system-vc140-mt-x64-1_66.lib)
target_link_libraries(${DLL_CODE} optimized libboost_filesystem-vc140-mt-x64-1_66.lib)
target_link_libraries(${DLL_CODE} optimized opencv_core343.lib)
target_link_libraries(${DLL_CODE} optimized opencv_imgcodecs343.lib)
target_link_libraries(${DLL_CODE} optimized opencv_highgui343.lib)
target_link_libraries(${DLL_CODE} optimized opencv_imgproc343.lib)
target_link_libraries(${DLL_CODE} optimized opencv_objdetect343.lib)
target_link_libraries(${DLL_CODE} debug libboost_system-vc140-mt-gd-x64-1_66.lib)
target_link_libraries(${DLL_CODE} debug libboost_filesystem-vc140-mt-gd-x64-1_66.lib)
target_link_libraries(${DLL_CODE} debug opencv_core343d.lib)
target_link_libraries(${DLL_CODE} debug opencv_imgcodecs343d.lib)
target_link_libraries(${DLL_CODE} debug opencv_highgui343d.lib)
target_link_libraries(${DLL_CODE} debug opencv_imgproc343d.lib)
target_link_libraries(${DLL_CODE} debug opencv_objdetect343d.lib)

#inctall

