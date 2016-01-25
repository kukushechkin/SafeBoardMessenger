if (WIN32)
  kl_cm_binary_delivery(
    openssl_static
    BUILD_PATH "\\avp.ru\global\Builds2\Externals\openssl\1.0.1p-static\1.0.1.5001"
  )
else()
  kl_cm_binary_delivery(
    openssl_static
    BUILD_PATH "\\avp.ru\global\Builds2\Externals\openssl\1.0.1p-static\1.0.1.5001"
  )
  kl_cm_binary_delivery(
    openssl_shared
    BUILD_PATH "\\avp.ru\global\Builds2\Externals\openssl\1.0.1p-shared\1.0.1.5503"
  )
endif()