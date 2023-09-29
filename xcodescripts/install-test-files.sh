#!/bin/sh
set -e -x

install -d -m 0755 \
	"$DSTROOT"/AppleInternal/Tests/developer_cmds

install -d -m 0755 \
	"$DSTROOT"/AppleInternal/Tests/developer_cmds/indent
install -m 0644 "$SRCROOT"/indent/tests/*0* \
	"$DSTROOT"/AppleInternal/Tests/developer_cmds/indent
install -m 0755 "$SRCROOT"/indent/tests/functional_test.sh \
	"$DSTROOT"/AppleInternal/Tests/developer_cmds/indent/indent_test.sh

install -d -m 0755 \
	"$DSTROOT"/AppleInternal/CoreOS/BATS/unit_tests

# The plist will use TARGET_* conditionals to avoid atf-sh tests on !macOS
tmplist=$(mktemp -t developer_cmds_test_plist)
trap 'rm "$tmplist"' EXIT
xcrun clang -x c -C -P -E -imacros TargetConditionals.h	\
	-Wno-invalid-pp-token				\
	"$SRCROOT"/tests/developer_cmds.plist.in	\
	-o "$tmplist"
plutil -lint "$tmplist"
install -m 0644 "$tmplist" \
	"$DSTROOT"/AppleInternal/CoreOS/BATS/unit_tests/developer_cmds.plist
