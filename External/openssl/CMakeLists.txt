cmake_minimum_required(VERSION 2.6)
project(contrib-openssl C CXX)

#
set(BINLIB_NAME "openssl")
set(BINLIB_VERSION "1.0.1p")

#set(BINLIB_DIRNAME "${BINLIB_NAME}-${BINLIB_VERSION}")
#set(BINLIB_SRC_PKG "${BINLIB_NAME}-${BINLIB_VERSION}.tar.gz")

set(extra_opts "")
set(extra_env "")

if (NOT BUILD_REVISION)
    message(FATAL_ERROR "Build number is not specified, use -DBUILD_REVISION=<NUM> to set revision")
endif()

if (NOT LINK_TYPE)
    message(FATAL_ERROR "Linking type is not specified, use -DLINK_TYPE=<shared|static> to set libraries type")
elseif(LINK_TYPE STREQUAL "shared")
    list(APPEND extra_opts "shared")
elseif(LINK_TYPE STREQUAL "static")
    list(APPEND extra_opts "no-shared")
else()
    message(FATAL_ERROR "Invalid linking type '${LINK_TYPE}' is specified, use -DLINK_TYPE=<shared|static> to set libraries type")
endif()

set(BINLIB_PACKAGE "${BINLIB_NAME}-${LINK_TYPE}-${BINLIB_VERSION}-${BUILD_REVISION}.tar.gz")

message(STATUS "Name:      ${BINLIB_NAME}")
message(STATUS "Version:   ${BINLIB_VERSION}")
message(STATUS "Build:     ${BUILD_REVISION}")
message(STATUS "Link type: ${LINK_TYPE}")
message(STATUS "Unpacking ${BINLIB_SRC_PKG}")

if(IOS)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/ar-macho.sh ${CMAKE_BINARY_DIR}/ar-macho.sh
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${BINLIB_DIRNAME}"
  )
endif()

# The main target to drive.
#add_custom_target(make_all ALL
#	DEPENDS log_unzip_openssl log_configure_openssl log_build_openssl localinstall package)
add_custom_target(make_all ALL
	DEPENDS log_configure_openssl log_build_openssl localinstall package)


# A target for tracking unzipping operation.
#add_custom_target(log_unzip_openssl
#	DEPENDS "${CMAKE_BINARY_DIR}/log_unzip_openssl.txt")
#add_custom_command(
#	OUTPUT "${CMAKE_BINARY_DIR}/log_unzip_openssl.txt"
#    COMMAND tar -vzxf "${CMAKE_SOURCE_DIR}/${BINLIB_SRC_PKG}" 2>&1 | tee "${CMAKE_BINARY_DIR}/log_unzip_openssl.txt"\; exit $$PIPESTATUS
#    DEPENDS "${CMAKE_SOURCE_DIR}/${BINLIB_SRC_PKG}"
#    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
#)

### configure build options for targets
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (ANDROID)
        set(platform "android")
        list(APPEND extra_env "ANDROID_DEV=${ANDROID_SYSROOT}/usr")
		list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-DANDROID ${ANDROID_CXX_FLAGS}")
		if(ANDROID_ABI STREQUAL "x86")
		   set(platform "linux-generic32")
		elseif(ANDROID_ABI STREQUAL "x86_64")
		   set(platform "linux-generic64")
		endif()
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "armv5tejl")
		SET (platform "linux-generic32")
		LIST (APPEND extra_env "RANLIB=${KL_CM_TOOLCHAIN_ROOT}/bin/arm-linux-gnueabi-ranlib CC=${KL_CM_TOOLCHAIN_ROOT}/bin/arm-linux-gnueabi-gcc AR=${KL_CM_TOOLCHAIN_ROOT}/bin/arm-linux-gnueabi-ar LS=${KL_CM_TOOLCHAIN_ROOT}/bin/arm-linux-gnueabi-ld")
		LIST (APPEND extra_opts "-\"isysroot ${KL_CM_TOOLCHAIN_ROOT}/sysroot\" -fPIC")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
		SET (platform "linux-generic32")
		LIST (APPEND extra_env "RANLIB=${KL_CM_TOOLCHAIN_ROOT}/bin/i386-linux-gnueabi-ranlib CC=${KL_CM_TOOLCHAIN_ROOT}/bin/i386-linux-gnueabi-gcc AR=${KL_CM_TOOLCHAIN_ROOT}/bin/i386-linux-gnueabi-ar LS=${KL_CM_TOOLCHAIN_ROOT}/bin/i386-linux-gnueabi-ld")
		LIST (APPEND extra_opts "-\"isysroot ${KL_CM_TOOLCHAIN_ROOT}/sysroot\" -fPIC")
    ELSEIF(KL_CM_TOOLCHAIN_ID STREQUAL "i686-pc-linux-gnu_gcc4.2.3_glibc2.3.6_shared.stdc++")
        set(platform "linux-generic32")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-lgcc_s")
    ELSEIF(KL_CM_TOOLCHAIN_ID STREQUAL "i686-pc-linux-gnu_gcc4.2.3_glibc2.3.6_static.stdc++")
        set(platform "linux-generic32")
	    list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-lgcc_s")  
	ELSEIF(KL_CM_TOOLCHAIN_ID STREQUAL "i686-pc-linux-gnu_gcc4.2.3_glibc2.3.6")
        set(platform "linux-generic32")
	    list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-lgcc_s")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
        set(platform "linux-generic32")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-lgcc")
    elseif(KL_CM_ARCH AND KL_CM_ARCH STREQUAL "x86") # FIXME: remove after toolchains fix
        set(platform "linux-generic32")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-lgcc")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        #set(platform "linux-generic64")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        set(platform "linux-x86_64")
        list(APPEND extra_opts "-lgcc" "-fPIC")
    elseif(KL_CM_ARCH AND KL_CM_ARCH STREQUAL "x64") # FIXME: remove after toolchains fix
        #set(platform "linux-generic64")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        set(platform "linux-x86_64")
        list(APPEND extra_opts "-fPIC")
    else()
        message(FATAL_ERROR "Unsupported ${CMAKE_SYSTEM_NAME} arch '${CMAKE_SYSTEM_PROCESSOR}'")
    endif()

elseif (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
        set(platform "BSD-generic32")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
    elseif(KL_CM_ARCH AND KL_CM_ARCH STREQUAL "x86") # FIXME: remove after toolchains fix
        set(platform "BSD-generic32")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(platform "BSD-x86_64")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-fPIC")
    elseif(KL_CM_ARCH AND KL_CM_ARCH STREQUAL "x64") # FIXME: remove after toolchains fix
        set(platform "BSD-x86_64")
	list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-fPIC")
    else()
        message(FATAL_ERROR "Unsupported ${CMAKE_SYSTEM_NAME} arch '${CMAKE_SYSTEM_PROCESSOR}'")
    endif()

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if(IOS)
        set(platform "iphoneos-cross")
        list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")

        if (NOT DEFINED KL_IOS_DEVELOPER_ROOT)
            message(FATAL_ERROR "KL_IOS_DEVELOPER_ROOT is requred")
        endif()
        list(APPEND extra_env "CROSS_TOP=${KL_IOS_DEVELOPER_ROOT}")

        if (NOT DEFINED KL_IOS_PLATFORM)
            message(FATAL_ERROR "KL_IOS_PLATFORM is requred")
        endif()
        list(APPEND extra_env "CROSS_SDK=${KL_IOS_PLATFORM}${KL_SDK_VERSION}.sdk")

        list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "${CMAKE_C_FLAGS}")

        list(APPEND extra_env "LIPO=${XCODE_PATH}/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/lipo")
        list(APPEND extra_env "AR=${CMAKE_BINARY_DIR}/ar-macho.sh")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
        set(platform "darwin-i386-cc")
        list(APPEND extra_env "CC=${CMAKE_C_COMPILER}")
        list(APPEND extra_opts "-fPIC")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(platform "darwin64-x86_64-cc")
        list(APPEND extra_opts "-fPIC")
    else()
        message(FATAL_ERROR "Unsupported ${CMAKE_SYSTEM_NAME} arch '${CMAKE_SYSTEM_PROCESSOR}'")
    endif()
else()
    message(FATAL_ERROR "Unsupported system '${CMAKE_SYSTEM_NAME}'")
endif()

if (CMAKE_RANLIB)
    list(APPEND extra_env "RANLIB=${CMAKE_RANLIB}")
endif()

### ---
#extra_opts="$extra_opts no-idea no-mdc2 no-rc5"
### ---
set(pkg_install_dir "${CMAKE_BINARY_DIR}/build")
set(pkg_result_dir "${CMAKE_BINARY_DIR}/result")

list(APPEND extra_opts "no-asm")
list(APPEND extra_opts "--prefix=${pkg_install_dir}")
list(APPEND extra_opts "${platform}")

message(STATUS "Configuring ${BINLIB_DIRNAME}")
message(STATUS "... extra_env:  ${extra_env}")
message(STATUS "... extra_opts: ${extra_opts}")
message(STATUS "")

# A target for tracking generation of the OpenSSL configuration files.
add_custom_target(log_configure_openssl
	DEPENDS "${CMAKE_BINARY_DIR}/log_configure_openssl.txt")
add_custom_command(
	OUTPUT "${CMAKE_BINARY_DIR}/log_configure_openssl.txt"
	COMMAND test -e test/evptests.txt && chmod 644 test/evptests.txt || echo
	COMMAND test -e crypto/buildinf.h && chmod 644 crypto/buildinf.h || echo
	COMMAND test -e crypto/objects/obj_mac.num && chmod 644 crypto/objects/obj_mac.num || echo
	COMMAND test -e crypto/objects/obj_mac.h && chmod 644 crypto/objects/obj_mac.h || echo
	COMMAND test -e crypto/objects/obj_dat.h && chmod 644 crypto/objects/obj_dat.h || echo
	COMMAND test -e crypto/objects/obj_xref.h && chmod 644 crypto/objects/obj_xref.h || echo
	COMMAND chmod +x ./Configure
    COMMAND /usr/bin/env ${extra_env} ./Configure ${extra_opts} 2>&1 | tee "${CMAKE_BINARY_DIR}/log_configure_openssl.txt"\; exit $$PIPESTATUS
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if (APPLE AND NOT IOS)
	set(MAKE_TARGETS "build_libs" "build_apps" "libcrypto.pc" "libssl.pc" "openssl.pc")
elseif (IOS)
    set(MAKE_TARGETS "all")
else()
    set(MAKE_TARGETS "depend" "all")
endif()

# A target for tracking building of the library.
add_custom_target(log_build_openssl
	DEPENDS "${CMAKE_BINARY_DIR}/log_build_openssl.txt")
add_custom_command(
	OUTPUT "${CMAKE_BINARY_DIR}/log_build_openssl.txt"
	COMMAND /usr/bin/env ${extra_env} ${CMAKE_MAKE_PROGRAM} ${MAKE_TARGETS} 2>&1 | tee "${CMAKE_BINARY_DIR}/log_build_openssl.txt"\; exit $$PIPESTATUS
    DEPENDS log_configure_openssl
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Make ${BINLIB_DIRNAME}"
)


add_custom_target(localinstall
	DEPENDS "${CMAKE_BINARY_DIR}/log_install_openssl.txt")
add_custom_command(
	OUTPUT "${CMAKE_BINARY_DIR}/log_install_openssl.txt"
    COMMAND /usr/bin/env ${extra_env} ${CMAKE_MAKE_PROGRAM} install_sw 2>&1 | tee "${CMAKE_BINARY_DIR}/log_install_openssl.txt"\; exit $$PIPESTATUS
    DEPENDS log_build_openssl
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Install ${BINLIB_DIRNAME}"
)

if (APPLE AND NOT IOS AND LINK_TYPE STREQUAL "shared")

add_custom_command(TARGET localinstall POST_BUILD
	COMMAND chmod 755 ${pkg_install_dir}/lib/libssl.1.0.0.dylib
	COMMAND install_name_tool -id @loader_path/libssl.1.0.0.dylib ${pkg_install_dir}/lib/libssl.1.0.0.dylib
	COMMAND install_name_tool -change ${pkg_install_dir}/lib/libcrypto.1.0.0.dylib @loader_path/libcrypto.1.0.0.dylib ${pkg_install_dir}/lib/libssl.1.0.0.dylib
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${BINLIB_DIRNAME}"
	COMMENT "Fixing install name of libssl.1.0.0.dylib"
)

add_custom_command(TARGET localinstall POST_BUILD
    COMMAND chmod 755 ${pkg_install_dir}/lib/libcrypto.1.0.0.dylib
    COMMAND install_name_tool -id @loader_path/libcrypto.1.0.0.dylib ${pkg_install_dir}/lib/libcrypto.1.0.0.dylib
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${BINLIB_DIRNAME}"
	COMMENT "Fixing install name of libcrypto.1.0.0.dylib"
)

set (ENGINE_LIBS
	"lib4758cca.dylib"
	"libaep.dylib"
	"libatalla.dylib"
	"libcapi.dylib"
	"libchil.dylib"
	"libcswift.dylib"
	"libgmp.dylib"
	"libgost.dylib"
	"libnuron.dylib"
	"libpadlock.dylib"
	"libsureware.dylib"
	"libubsec.dylib"
)

foreach(ENGINE_LIB ${ENGINE_LIBS})
	add_custom_command(TARGET localinstall POST_BUILD
		COMMAND chmod 755 ${pkg_install_dir}/lib/engines/${ENGINE_LIB}
		COMMAND install_name_tool -id @loader_path/${ENGINE_LIB} ${pkg_install_dir}/lib/engines/${ENGINE_LIB}
		COMMAND install_name_tool -change ${pkg_install_dir}/lib/libcrypto.1.0.0.dylib @loader_path/libcrypto.1.0.0.dylib ${pkg_install_dir}/lib/engines/${ENGINE_LIB}
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${BINLIB_DIRNAME}"
		COMMENT "Fixing install name of ${ENGINE_LIB}")
endforeach()

endif()


add_custom_target(package
	DEPENDS "${BINLIB_PACKAGE}")
add_custom_command(
	OUTPUT "${BINLIB_PACKAGE}"
    rm -rf ${pkg_result_dir}
    COMMAND mkdir -p "${pkg_result_dir}/${BINLIB_NAME}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${pkg_install_dir}/lib ${pkg_result_dir}/${BINLIB_NAME}/lib
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${pkg_install_dir}/include ${pkg_result_dir}/${BINLIB_NAME}/include
	COMMAND tar -C "${pkg_result_dir}" -czf "${BINLIB_PACKAGE}" "${BINLIB_NAME}"
    DEPENDS localinstall
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Package to ${BINLIB_PACKAGE}"
)
