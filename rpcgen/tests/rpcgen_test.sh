atf_test_case rdar152088884
rdar152088884_head() {
	atf_set "descr" "verify that rpcgen can handle long " \
	    "preprocessor paths"
}
rdar152088884_body() {
	cat >test.x <<EOF
program Test {
  version v1 {
    void func(int arg) = 1;
  } = 1;
} = 0x20000001;
EOF
	long=$(jot -b 'long_directory_name' -s '/' 100)
	atf_check -s not-exit:0 \
	    -e match:"cannot find C preprocessor: /$long/cpp" \
	    rpcgen -Y "/$long" test.x
}

atf_test_case rdar152097962
rdar152097962_head() {
	atf_set "descr" "verify that rpcgen can handle long " \
	    "argument names"
}
rdar152097962_body() {
	# create a very long identifier, but not so long that we
	# exceed rpcgen's 1024-character line length limit.
	arg=$(jot -b 'arg' -s '_' 240)
	cat >test.x <<EOF
program Test {
  version v1 {
    void func(int $arg) = 1;
  } = 1;
} = 0x20000001;
EOF
	atf_check rpcgen test.x
	atf_check test -f test.h
	atf_check test -f test_clnt.c
	atf_check test -f test_svc.c
}

atf_init_test_cases() {
	atf_add_test_case rdar152088884
	atf_add_test_case rdar152097962
}
