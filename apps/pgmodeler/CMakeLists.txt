pgm_add_executable(pgmodeler
    src/main.cpp
    src/pgmodelerapp.cpp src/pgmodelerapp.h)

pgm_set_default_inc_libs(${PGM_TARGET})
pgm_install_executable(${PGM_TARGET} OFF)
