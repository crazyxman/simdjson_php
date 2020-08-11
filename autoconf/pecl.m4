
yes() {
	true
}
no() {
	false
}
dnl
dnl PECL_INIT(name)
dnl
dnl Start configuring the PECL extension.
dnl
AC_DEFUN([PECL_INIT], [dnl
	m4_define([PECL_NAME],[$1])dnl
])dnl
dnl
dnl
dnl PECL_VAR(name)
dnl
AC_DEFUN([PECL_VAR], [dnl
AS_TR_CPP([PHP_]PECL_NAME[_$1])dnl
])dnl
dnl
dnl PECL_CACHE_VAR(name)
dnl
AC_DEFUN([PECL_CACHE_VAR], [dnl
AS_TR_SH([PECL_cv_$1])dnl
])dnl
dnl
dnl PECL_SAVE_VAR(name)
dnl
AC_DEFUN([PECL_SAVE_VAR], [dnl
AS_TR_SH([PECL_sv_$1])dnl
])dnl
dnl
dnl PECL_DEFINE(what, to[, desc])
dnl
AC_DEFUN([PECL_DEFINE], [dnl
	AC_DEFINE(PECL_VAR([$1]), ifelse([$2],,1,[$2]), ifelse([$3],,[ ],[$3]))
])dnl
dnl
dnl PECL_DEFINE_UQ(what, to[, desc])
dnl
AC_DEFUN([PECL_DEFINE_UQ], [dnl
	AC_DEFINE_UNQUOTED(PECL_VAR([$1]), [$2], ifelse([$3],,[ ],[$3]))
])dnl
dnl
dnl PECL_DEFINE_SH(what, to[, desc])
dnl
AC_DEFUN([PECL_DEFINE_SH], [dnl
	PECL_VAR([$1])=$2
	PECL_DEFINE_UQ([$1], [$2], [$3])
])
dnl
dnl PECL_DEFINE_FN(fn)
dnl
AC_DEFUN([PECL_DEFINE_FN], [
	AC_DEFINE(AS_TR_CPP([HAVE_$1]), [1], [ ])
])
dnl
dnl PECL_SAVE_ENV(var, ns)
dnl
AC_DEFUN([PECL_SAVE_ENV], [
	PECL_SAVE_VAR([$2_$1])=[$]$1
])
dnl
dnl PECL_RESTORE_ENV(var, ns)
dnl
AC_DEFUN([PECL_RESTORE_ENV], [
	$1=$PECL_SAVE_VAR([$2_$1])
])
dnl
dnl PECL_EVAL_LIBLINE(libline)
dnl
AC_DEFUN([PECL_EVAL_LIBLINE], [
	PECL_SAVE_ENV(ext_shared, pecl)
	ext_shared=no
	PHP_EVAL_LIBLINE([$1], _pecl_eval_libline_dummy)
	PECL_RESTORE_ENV(ext_shared, pecl)
])
dnl
dnl PECL_PROG_EGREP
dnl
dnl Checks for an egrep. Defines $EGREP.
dnl
AC_DEFUN([PECL_PROG_EGREP], [
	ifdef([AC_PROG_EGREP], [
		AC_PROG_EGREP
	], [
		AC_CHECK_PROG(EGREP, egrep, egrep)
	])
])
dnl
dnl PECL_PROG_AWK
dnl
dnl Checks for an awk. Defines $AWK.
dnl
AC_DEFUN([PECL_PROG_AWK], [
	ifdef([AC_PROG_AWK], [
		AC_PROG_AWK
	], [
		AC_CHECK_PROG(AWK, awk, awk)
	])
])
dnl
dnl PECL_PROG_SED
dnl
dnl Checks for the sed program. Defines $SED.
dnl
AC_DEFUN([PECL_PROG_SED], [
	ifdef([AC_PROG_SED], [
		AC_PROG_SED
	], [
		ifdef([LT_AC_PROG_SED], [
			LT_AC_PROG_SED
		], [
			AC_CHECK_PROG(SED, sed, sed)
		])
	])
])
dnl
dnl PECL_PROG_PKGCONFIG
dnl
dnl Checks for pkg-config program and defines $PKG_CONFIG (to false if not found).
dnl
AC_DEFUN([PECL_PROG_PKGCONFIG], [
	if test -z "$PKG_CONFIG"; then
		AC_PATH_PROG([PKG_CONFIG], [pkg-config], [false])
	fi
])
dnl
dnl PECL_HAVE_PHP_EXT(name[, code-if-yes[, code-if-not]])
dnl
dnl Check whether ext/$name is enabled in $PHP_EXECUTABLE (PECL build)
dnl or if $PHP_<NAME> is defined to anything else than "no" (in-tree build).
dnl Defines shell var PECL_VAR(HAVE_EXT_<NAME>) to true or false.
dnl
AC_DEFUN([PECL_HAVE_PHP_EXT], [
	AC_REQUIRE([PECL_PROG_EGREP])dnl
	AC_CACHE_CHECK([whether ext/$1 is enabled], PECL_CACHE_VAR([HAVE_EXT_$1]), [
		PECL_CACHE_VAR([HAVE_EXT_$1])=no
		if test -x "$PHP_EXECUTABLE"; then
			if $PHP_EXECUTABLE -m | $EGREP -q ^$1\$; then
				PECL_CACHE_VAR([HAVE_EXT_$1])=yes
			fi
		elif test -n "$AS_TR_CPP([PHP_$1])" && test "$AS_TR_CPP([PHP_$1])" != "no"; then
			PECL_CACHE_VAR([HAVE_EXT_$1])=yes
		fi
	])
	if $PECL_CACHE_VAR([HAVE_EXT_$1]); then
		PECL_VAR([HAVE_EXT_$1])=true
		PECL_DEFINE([HAVE_EXT_$1])
		$2
	else
		PECL_VAR([HAVE_EXT_$1])=false
		$3
	fi
])
dnl
dnl PECL_HAVE_PHP_EXT_HEADER(ext[, header])
dnl
dnl Check where to find a header for ext and add the found dir to $INCLUDES.
dnl If header is not specified php_<ext>.h is assumed.
dnl Defines shell var PHP_<PECL_NAME>_EXT_<EXT>_INCDIR to the found dir.
dnl Defines PHP_<PECL_NAME>_HAVE_<HEADER> to the found path.
dnl
AC_DEFUN([PECL_HAVE_PHP_EXT_HEADER], [dnl
	AC_REQUIRE([PECL_PROG_SED])dnl
	m4_define([EXT_HEADER], ifelse([$2],,php_$1.h,[$2]))dnl
	AC_CACHE_CHECK([for EXT_HEADER of ext/$1], PECL_CACHE_VAR([EXT_$1]_INCDIR), [
		for i in $(printf "%s" "$INCLUDES" | $SED -e's/-I//g') $abs_srcdir ../$1; do
			if test -d $i; then
				for j in $i/EXT_HEADER $i/ext/$1/EXT_HEADER; do
					if test -f $j; then
						PECL_CACHE_VAR([EXT_$1]_INCDIR)=$(dirname "$j")
						break
					fi
				done
			fi
		done
	])
	PECL_VAR([EXT_$1]_INCDIR)=$PECL_CACHE_VAR([EXT_$1]_INCDIR)
	PHP_ADD_INCLUDE([$PECL_VAR([EXT_$1]_INCDIR)])
	PECL_DEFINE_UQ([HAVE_]EXT_HEADER, "$PECL_VAR([EXT_$1]_INCDIR)/EXT_HEADER")
])
dnl
dnl PECL_HAVE_CONST(header, const[, type=int[, code-if-yes[, code-if-mno]]])
dnl
AC_DEFUN([PECL_HAVE_CONST], [dnl
	AC_REQUIRE([PECL_PROG_EGREP])dnl
	AC_CACHE_CHECK([for $2 in $1], PECL_CACHE_VAR([HAVE_$1_$2]), [
		AC_TRY_COMPILE([
			#include "$1"
		], [
			]ifelse([$3],,int,[$3])[ _c = $2;
			(void) _c;
		], [
			PECL_CACHE_VAR([HAVE_$1_$2])=yes
		], [
			PECL_CACHE_VAR([HAVE_$1_$2])=no
		])
	])
	if $PECL_CACHE_VAR([HAVE_$1_$2]); then
		PECL_DEFINE([HAVE_$2])
		$4
	else
		ifelse([$5],,:,[$5])
	fi
])
dnl
dnl _PECL_TR_VERSION(version)
dnl
AC_DEFUN([_PECL_TR_VERSION], [dnl
AC_REQUIRE([PECL_PROG_AWK])dnl
$(printf "%s" $1 | $AWK -F "[.]" '{print $[]1*1000000 + $[]2*10000 + $[]3*100 + $[]4}')
])
dnl
dnl PECL_CHECKED_VERSION(name)
dnl
dnl Shell var name of an already checked version.
dnl
AC_DEFUN([PECL_CHECKED_VERSION], [PECL_VAR([$1][_VERSION])])
dnl
dnl PECL_HAVE_VERSION(name, min-version[, code-if-yes[, code-if-not]])
dnl
dnl Perform a min-version check while in an PECL_CHECK_* block.
dnl Expands AC_MSG_ERROR when code-if-not is empty and the version check fails.
dnl
AC_DEFUN([PECL_HAVE_VERSION], [
	aversion=_PECL_TR_VERSION([$PECL_CHECKED_VERSION([$1])])
	mversion=_PECL_TR_VERSION([$2])
	AC_MSG_CHECKING([whether $1 version $PECL_CHECKED_VERSION([$1]) >= $2])
	if test -z "$aversion" || test "$aversion" -lt "$mversion"; then
		ifelse($4,,AC_MSG_ERROR([no]), [
			AC_MSG_RESULT([no])
			$4
		])
	else
		AC_MSG_RESULT([ok])
		$3
	fi
])
dnl
dnl PECL_CHECK_CUSTOM(name, path, header, lib, version)
dnl
AC_DEFUN([PECL_CHECK_CUSTOM], [
	PECL_SAVE_ENV([CPPFLAGS], [$1])
	PECL_SAVE_ENV([LDFLAGS], [$1])
	PECL_SAVE_ENV([LIBS], [$1])

	AC_MSG_CHECKING([for $1])
	AC_CACHE_VAL(PECL_CACHE_VAR([$1_prefix]), [
		for path in $2 /usr/local /usr /opt; do
			if test "$path" = "" || test "$path" = "yes" || test "$path" = "no"; then
				continue
			elif test -f "$path/include/$3"; then
				PECL_CACHE_VAR([$1_prefix])="$path"
				break
			fi
		done
	])
	if test -n "$PECL_CACHE_VAR([$1_prefix])"; then
		CPPFLAGS="-I$PECL_CACHE_VAR([$1_prefix])/include"
		LDFLAGS="-L$PECL_CACHE_VAR([$1_prefix])/$PHP_LIBDIR"
		LIBS="-l$4"
		PECL_EVAL_LIBLINE([$LDFLAGS $LIBS])
		
		AC_CACHE_VAL(PECL_CACHE_VAR([$1_version]), [
			pushd $PECL_CACHE_VAR([$1_prefix]) >/dev/null
			PECL_CACHE_VAR([$1_version])=$5
			popd >/dev/null
		])
		PECL_CHECKED_VERSION([$1])=$PECL_CACHE_VAR([$1_version])

		if test -n "$PECL_CHECKED_VERSION([$1])"; then
			PECL_VAR([HAVE_$1])=true
			PECL_DEFINE([HAVE_$1])
			PECL_DEFINE_UQ($1[_VERSION], "$PECL_CHECKED_VERSION([$1])")
		else
			PECL_VAR([HAVE_$1])=false
		fi
	else
		PECL_VAR([HAVE_$1])=false
	fi
	AC_MSG_RESULT([${PECL_CHECKED_VERSION([$1]):-no}])
])
dnl
dnl PECL_CHECK_CONFIG(name, prog-config, version-flag, cppflags-flag, ldflags-flag, libs-flag)
dnl
AC_DEFUN([PECL_CHECK_CONFIG], [
	PECL_SAVE_ENV([CPPFLAGS], [$1])
	PECL_SAVE_ENV([LDFLAGS], [$1])
	PECL_SAVE_ENV([LIBS], [$1])


	AC_MSG_CHECKING([for $1])
	ifelse($2, [$PKG_CONFIG $1], [
		AC_CACHE_VAL(PECL_CACHE_VAR([$1_exists]), [
			if $($2 --exists); then
				PECL_CACHE_VAR([$1_exists])=yes
			else
				PECL_CACHE_VAR([$1_exists])=no
			fi
		])
		if $PECL_CACHE_VAR([$1_exists]); then
	])
		AC_CACHE_VAL(PECL_CACHE_VAR([$1_version]), [
			PECL_CACHE_VAR([$1_version])=$($2 $3)
		])
		PECL_CHECKED_VERSION([$1])=$PECL_CACHE_VAR([$1_version])
		AC_CACHE_VAL(PECL_CACHE_VAR([$1_cppflags]), [
			PECL_CACHE_VAR([$1_cppflags])=$($2 $4)
		])
		CPPFLAGS=$PECL_CACHE_VAR([$1_cppflags])
		AC_CACHE_VAL(PECL_CACHE_VAR([$1_ldflags]), [
			PECL_CACHE_VAR([$1_ldflags])=$($2 $5)
		])
		LDFLAGS=$PECL_CACHE_VAR([$1_ldflags])
		AC_CACHE_VAL(PECL_CACHE_VAR([$1_libs]), [
			PECL_CACHE_VAR([$1_libs])=$($2 $6)
		])
		LIBS=$PECL_CACHE_VAR([$1_libs])
		PECL_EVAL_LIBLINE([$LDFLAGS $LIBS])
	ifelse($2, [$PKG_CONFIG $1], [
		fi
	])

	if test -n "$PECL_CHECKED_VERSION([$1])"; then
		PECL_VAR([HAVE_$1])=true
		PECL_DEFINE([HAVE_$1])
		PECL_DEFINE_UQ([$1_VERSION], "$PECL_CHECKED_VERSION([$1])")
	else
		PECL_VAR([HAVE_$1])=false
	fi

	AC_MSG_RESULT([${PECL_CHECKED_VERSION([$1]):-no}])
])
dnl
dnl PECL_CHECK_PKGCONFIG(pkg[, additional-pkg-config-path])
dnl
AC_DEFUN([PECL_CHECK_PKGCONFIG], [dnl
	AC_REQUIRE([PECL_PROG_PKGCONFIG])dnl
	ifelse($2,,, [
		PECL_SAVE_VAR(pkgconfig_path)="$PKG_CONFIG_PATH"
		if test -d "$2"; then
			export PKG_CONFIG_PATH="$2/lib/pkgconfig:$PKG_CONFIG_PATH"
		fi
	])
	PECL_CHECK_CONFIG([$1], [$PKG_CONFIG $1], [--modversion], [--cflags-only-I], [--libs-only-L], [--libs-only-l])
	ifelse($2,,, [
		PKG_CONFIG_PATH="$PECL_SAVE_VAR(pkgconfig_path)"
	])
])
dnl
dnl PECL_CHECK_DONE(name, success[, incline, libline])
dnl
AC_DEFUN([PECL_CHECK_DONE], [
	if $2; then
		incline=$CPPFLAGS
		libline="$LDFLAGS $LIBS"
		PECL_DEFINE([HAVE_$1])
	else
		incline=$3
		libline=$4
	fi

	PECL_RESTORE_ENV([CPPFLAGS], [$1])
	PECL_RESTORE_ENV([LDFLAGS], [$1])
	PECL_RESTORE_ENV([LIBS], [$1])

	PHP_EVAL_INCLINE([$incline])
	PHP_EVAL_LIBLINE([$libline], AS_TR_CPP(PECL_NAME[_SHARED_LIBADD]))
])

dnl
dnl PECL_CHECK_CA([additional-ca-paths,[ additional-ca-bundles]])
dnl
AC_DEFUN([PECL_CHECK_CA], [
	AC_CACHE_CHECK([for default CA path], PECL_CACHE_VAR([CAPATH]), [
		PECL_VAR([CAPATH])=
		for ca_path in $1 \
			/etc/ssl/certs \
			/System/Library/OpenSSL
		do
			# check if it's actually a hashed directory
			if test -d "$ca_path" && ls "$ca_path"/@<:@0-9a-f@:>@@<:@0-9a-f@:>@@<:@0-9a-f@:>@@<:@0-9a-f@:>@@<:@0-9a-f@:>@@<:@0-9a-f@:>@@<:@0-9a-f@:>@@<:@0-9a-f@:>@.0 >/dev/null 2>&1; then
				PECL_CACHE_VAR([CAPATH])=$ca_path
				break
			fi
		done
	])
	if test -n "$PECL_CACHE_VAR([CAPATH])"; then
		PECL_DEFINE_SH([CAPATH], "$PECL_CACHE_VAR([CAPATH])")
	fi

	AC_CACHE_CHECK([for default CA info], PECL_CACHE_VAR([CAINFO]), [
		for ca_info in $2 \
			/etc/ssl/{cert,ca-bundle}.pem \
			/{etc,usr/share}/ssl/certs/ca-{bundle,ceritifcates}.crt \
			/etc/{pki/ca-trust,ca-certificates}/extracted/pem/tls-ca-bundle.pem \
			/etc/pki/tls/certs/ca-bundle{,.trust}.crt \
			/usr/local/etc/{,open}ssl/cert.pem \
			/usr/local/share/certs/ca-root-nss.crt \
			/{usr,usr/local,opt}/local/share/curl/curl-ca-bundle.crt
		do
			if test -f "$ca_info"; then
				PECL_CACHE_VAR([CAINFO])=$ca_info
				break
			fi
		done
	])
	if test -n "$PECL_CACHE_VAR([CAINFO])"; then
		PECL_DEFINE_SH([CAINFO], "$PECL_CACHE_VAR([CAINFO])")
	fi
])
