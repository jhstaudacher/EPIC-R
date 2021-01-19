## Process this file with autoconf to produce a configure script.
##
## Configure.ac for EfficientPowerIndices
##
## Copyright (C) 2020 Linus Olsson
##
## Licensed under GNU GPL 2 or later

# The version set here will propagate to other files from here
## Process this file with autoconf to produce a configure script.
AC_INIT([EfficientPowerIndices], m4_esyscmd_s([awk '/^Version:/ {print $2}' DESCRIPTION]))

# Checks for common programs using default macros
#AC_PROG_CC

AC_CHECK_LIB(gmp, __gmpz_init, ,
  [AC_MSG_ERROR([GNU MP not found, see https://gmplib.org/])])

AC_SUBST(GMP_DIR, m4_esyscmd_s([find  / -name gmp.h 2>/dev/null -exec dirname {} \; |head -n 1]))

if test "${GMP_DIR}" == ""; then
    
    AC_MSG_ERROR([GMP sources not found, are GMP sources installed?])
fi



AC_OUTPUT(src/Makevars)
