m4_define([tss2tab_major_version], [0])
m4_define([tss2tab_minor_version], [1])
m4_define([tss2tab_version_string], [tss2tab_major_version.tss2tab_minor_version])

AC_INIT([tss2tab], [tss2tab_version_string])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
LT_INIT()
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_FILES([Makefile])
AX_PTHREAD([], [AC_MSG_ERROR([requires pthread])])
AC_ARG_ENABLE([unit],
              [AS_HELP_STRING([--enable-unit],
                   [build cmocka unit tests (default is no)])],
              [enable_unit=$enableval],
              [enable_unit=no])
AS_IF([test "x$enable_unit" != xno],
      [PKG_CHECK_MODULES([CMOCKA],
                         [cmocka],
                         [AC_DEFINE([HAVE_CMOCKA],
                                    [1])])])
AM_CONDITIONAL([UNIT], [test "x$enable_unit" != xno])
PKG_CHECK_MODULES([DBUS], [dbus-1])
PKG_CHECK_MODULES([GIO], [gio-unix-2.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0])
PKG_CHECK_MODULES([SAPI],[sapi])
AC_ARG_VAR([GDBUS_CODEGEN],[The gdbus-codegen executable.])
AC_PATH_PROG([GDBUS_CODEGEN], [`$PKG_CONFIG --variable=gdbus_codegen gio-2.0`])
if test -z "$GDBUS_CODEGEN"; then
    AC_MSG_ERROR([*** gdbus-codegen is required to build tss2-tabd])
fi
# disable helgrind and drd, they hate GAsyncQueue
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_DFLT([helgrind], [off])
AX_VALGRIND_DFLT([drd], [off])
AX_VALGRIND_CHECK
# function from the gnu.org docs
AC_DEFUN([MY_ARG_WITH],
         [AC_ARG_WITH(m4_translit([[$1]], [_], [-]),
                      [AS_HELP_STRING([--with-m4_translit([$1], [_], [-])],
                                      [use $1 (default is $2)])],
                      [use_[]$1=$withval],
                      [use_[]$1=$2])
         ])
#
# TCTIs
#
MY_ARG_WITH([tcti_device], [yes])
AS_IF([test "x$use_tcti_device" != xno],
      [PKG_CHECK_MODULES([TCTI_DEVICE],
                         [tcti-device],
                         [AC_DEFINE([HAVE_TCTI_DEVICE],
                                    [1])])])
AM_CONDITIONAL([TCTI_DEVICE], [test "x$use_tcti_device" != xno])
MY_ARG_WITH([tcti_socket], [yes])
AS_IF([test "x$use_tcti_socket" != xno],
      [PKG_CHECK_MODULES([TCTI_SOCKET],
                         [tcti-socket],
                         [AC_DEFINE([HAVE_TCTI_SOCKET],
                                    [1])])])
AM_CONDITIONAL([TCTI_SOCKET], [test "x$use_tcti_socket" != xno])
AS_IF([test "x$use_tcti_device" == xno -a \
            "x$use_tcti_socket" == xno],
      [AC_MSG_ERROR([At least one TCTI must be enabled.])])

#
# systemd
#
AC_ARG_WITH([systemdsystemunitdir],
            AS_HELP_STRING([--with-systemdsystemunitdir=DIR],
                           [Directory for systemd service files]),
            [],
            [with_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)])
AS_IF([test "x$with_systemdsystemunitdir" != xno],
      [AC_SUBST([systemdsystemunitdir],
      [$with_systemdsystemunitdir])])
AM_CONDITIONAL(HAVE_SYSTEMD, [test -n "$with_systemdsystemunitdir" -a "x$with_systemdsystemunitdir" != xno ])

#
# dbus
#
AC_ARG_WITH([dbuspolicydir],
            [AS_HELP_STRING([--with-dbuspolicydir=DIR],[D-Bus policy directory])],
            [],
            [with_dbuspolicydir=${sysconfdir}/dbus-1/system.d])
AX_NORMALIZE_PATH([with_dbuspolicydir])
AC_SUBST([dbuspolicydir], [$with_dbuspolicydir])
#
# udev
#
AC_ARG_WITH([udevrulesdir],
            [AS_HELP_STRING([--with-udevrulesdir=DIR],[udev rules directory])],
            [],
            [with_udevrulesdir=${libdir}/udev/rules.d])
AX_NORMALIZE_PATH([with_udevrulesdir])
AC_SUBST([udevrulesdir], [$with_udevrulesdir])

# preprocessor / compiler / linker flags
#   these macros are defined in m4/flags.m4
AX_ADD_COMPILER_FLAG([-Wall])
AX_ADD_COMPILER_FLAG([-Werror])
AX_ADD_COMPILER_FLAG([-std=gnu99])
AX_ADD_COMPILER_FLAG([-Wformat])
AX_ADD_COMPILER_FLAG([-Wformat-security])
AX_ADD_COMPILER_FLAG([-Wno-missing-braces])
AX_ADD_COMPILER_FLAG([-fdata-sections])
AX_ADD_COMPILER_FLAG([-ffunction-sections])
AX_ADD_COMPILER_FLAG([-fstack-protector-all])
AX_ADD_COMPILER_FLAG([-fpic])
AX_ADD_COMPILER_FLAG([-fPIC])
AX_ADD_PREPROC_FLAG([-D_GNU_SOURCE])
AX_ADD_PREPROC_FLAG([-D_FORTIFY_SOURCE=2])
AX_ADD_PREPROC_FLAG([-U_FORTIFY_SOURCE])
AX_ADD_LINK_FLAG([-Wl,--gc-sections])
AX_ADD_LINK_FLAG([-Wl,--no-undefined])
AX_ADD_LINK_FLAG([-Wl,-z,noexecstack])
AX_ADD_LINK_FLAG([-Wl,-z,now])
AX_ADD_LINK_FLAG([-Wl,-z,relro])

AC_OUTPUT
