# CMake generated Testfile for 
# Source directory: /home/apoelstra/dload/code/cj-client/jansson
# Build directory: /home/apoelstra/dload/code/cj-client/jansson
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(test_array "/home/apoelstra/dload/code/cj-client/jansson/bin/test_array")
ADD_TEST(test_copy "/home/apoelstra/dload/code/cj-client/jansson/bin/test_copy")
ADD_TEST(test_dump "/home/apoelstra/dload/code/cj-client/jansson/bin/test_dump")
ADD_TEST(test_dump_callback "/home/apoelstra/dload/code/cj-client/jansson/bin/test_dump_callback")
ADD_TEST(test_equal "/home/apoelstra/dload/code/cj-client/jansson/bin/test_equal")
ADD_TEST(test_load "/home/apoelstra/dload/code/cj-client/jansson/bin/test_load")
ADD_TEST(test_loadb "/home/apoelstra/dload/code/cj-client/jansson/bin/test_loadb")
ADD_TEST(test_number "/home/apoelstra/dload/code/cj-client/jansson/bin/test_number")
ADD_TEST(test_object "/home/apoelstra/dload/code/cj-client/jansson/bin/test_object")
ADD_TEST(test_pack "/home/apoelstra/dload/code/cj-client/jansson/bin/test_pack")
ADD_TEST(test_simple "/home/apoelstra/dload/code/cj-client/jansson/bin/test_simple")
ADD_TEST(test_unpack "/home/apoelstra/dload/code/cj-client/jansson/bin/test_unpack")
ADD_TEST(test_memory_funcs "/home/apoelstra/dload/code/cj-client/jansson/bin/test_memory_funcs")
ADD_TEST(encoding-flags__object "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/object")
ADD_TEST(encoding-flags__ensure-ascii "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/ensure-ascii")
ADD_TEST(encoding-flags__indent-compact-object "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/indent-compact-object")
ADD_TEST(encoding-flags__compact-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/compact-array")
ADD_TEST(encoding-flags__indent-compact-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/indent-compact-array")
ADD_TEST(encoding-flags__preserve-order "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/preserve-order")
ADD_TEST(encoding-flags__indent-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/indent-array")
ADD_TEST(encoding-flags__array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/array")
ADD_TEST(encoding-flags__sort-keys "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/sort-keys")
ADD_TEST(encoding-flags__compact-object "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/compact-object")
ADD_TEST(encoding-flags__indent-object "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/encoding-flags/indent-object")
ADD_TEST(valid__three-byte-utf-8 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/three-byte-utf-8")
ADD_TEST(valid__three-byte-utf-8__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/three-byte-utf-8")
ADD_TEST(valid__two-byte-utf-8 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/two-byte-utf-8")
ADD_TEST(valid__two-byte-utf-8__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/two-byte-utf-8")
ADD_TEST(valid__simple-int-0 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-int-0")
ADD_TEST(valid__simple-int-0__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-int-0")
ADD_TEST(valid__negative-one "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/negative-one")
ADD_TEST(valid__negative-one__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/negative-one")
ADD_TEST(valid__empty-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-string")
ADD_TEST(valid__empty-string__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-string")
ADD_TEST(valid__real-capital-e "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-capital-e")
ADD_TEST(valid__real-capital-e__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-capital-e")
ADD_TEST(valid__empty-object "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-object")
ADD_TEST(valid__empty-object__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-object")
ADD_TEST(valid__real-capital-e-negative-exponent "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-capital-e-negative-exponent")
ADD_TEST(valid__real-capital-e-negative-exponent__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-capital-e-negative-exponent")
ADD_TEST(valid__utf-surrogate-four-byte-encoding "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/utf-surrogate-four-byte-encoding")
ADD_TEST(valid__utf-surrogate-four-byte-encoding__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/utf-surrogate-four-byte-encoding")
ADD_TEST(valid__empty-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-array")
ADD_TEST(valid__empty-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-array")
ADD_TEST(valid__string-escapes "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/string-escapes")
ADD_TEST(valid__string-escapes__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/string-escapes")
ADD_TEST(valid__real-exponent "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-exponent")
ADD_TEST(valid__real-exponent__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-exponent")
ADD_TEST(valid__real-negative-exponent "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-negative-exponent")
ADD_TEST(valid__real-negative-exponent__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-negative-exponent")
ADD_TEST(valid__one-byte-utf-8 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/one-byte-utf-8")
ADD_TEST(valid__one-byte-utf-8__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/one-byte-utf-8")
ADD_TEST(valid__null "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/null")
ADD_TEST(valid__null__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/null")
ADD_TEST(valid__real-underflow "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-underflow")
ADD_TEST(valid__real-underflow__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-underflow")
ADD_TEST(valid__escaped-utf-control-char "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/escaped-utf-control-char")
ADD_TEST(valid__escaped-utf-control-char__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/escaped-utf-control-char")
ADD_TEST(valid__simple-object "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-object")
ADD_TEST(valid__simple-object__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-object")
ADD_TEST(valid__false "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/false")
ADD_TEST(valid__false__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/false")
ADD_TEST(valid__real-positive-exponent "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-positive-exponent")
ADD_TEST(valid__real-positive-exponent__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-positive-exponent")
ADD_TEST(valid__short-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/short-string")
ADD_TEST(valid__short-string__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/short-string")
ADD_TEST(valid__real-capital-e-positive-exponent "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-capital-e-positive-exponent")
ADD_TEST(valid__real-capital-e-positive-exponent__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-capital-e-positive-exponent")
ADD_TEST(valid__simple-ascii-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-ascii-string")
ADD_TEST(valid__simple-ascii-string__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-ascii-string")
ADD_TEST(valid__true "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/true")
ADD_TEST(valid__true__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/true")
ADD_TEST(valid__negative-zero "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/negative-zero")
ADD_TEST(valid__negative-zero__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/negative-zero")
ADD_TEST(valid__simple-int-1 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-int-1")
ADD_TEST(valid__simple-int-1__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-int-1")
ADD_TEST(valid__complex-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/complex-array")
ADD_TEST(valid__complex-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/complex-array")
ADD_TEST(valid__empty-object-in-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-object-in-array")
ADD_TEST(valid__empty-object-in-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/empty-object-in-array")
ADD_TEST(valid__negative-int "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/negative-int")
ADD_TEST(valid__negative-int__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/negative-int")
ADD_TEST(valid__utf-8-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/utf-8-string")
ADD_TEST(valid__utf-8-string__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/utf-8-string")
ADD_TEST(valid__real-fraction-exponent "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-fraction-exponent")
ADD_TEST(valid__real-fraction-exponent__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/real-fraction-exponent")
ADD_TEST(valid__simple-int-123 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-int-123")
ADD_TEST(valid__simple-int-123__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-int-123")
ADD_TEST(valid__simple-real "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-real")
ADD_TEST(valid__simple-real__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/valid/simple-real")
ADD_TEST(invalid__real-garbage-after-e "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-garbage-after-e")
ADD_TEST(invalid__real-garbage-after-e__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-garbage-after-e")
ADD_TEST(invalid__invalid-negative-integer "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-negative-integer")
ADD_TEST(invalid__invalid-negative-integer__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-negative-integer")
ADD_TEST(invalid__extra-comma-in-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/extra-comma-in-array")
ADD_TEST(invalid__extra-comma-in-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/extra-comma-in-array")
ADD_TEST(invalid__unterminated-object-and-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-object-and-array")
ADD_TEST(invalid__unterminated-object-and-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-object-and-array")
ADD_TEST(invalid__garbage-after-newline "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/garbage-after-newline")
ADD_TEST(invalid__garbage-after-newline__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/garbage-after-newline")
ADD_TEST(invalid__invalid-identifier "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-identifier")
ADD_TEST(invalid__invalid-identifier__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-identifier")
ADD_TEST(invalid__brace-comma "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/brace-comma")
ADD_TEST(invalid__brace-comma__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/brace-comma")
ADD_TEST(invalid__too-big-positive-integer "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/too-big-positive-integer")
ADD_TEST(invalid__too-big-positive-integer__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/too-big-positive-integer")
ADD_TEST(invalid__negative-integer-starting-with-zero "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/negative-integer-starting-with-zero")
ADD_TEST(invalid__negative-integer-starting-with-zero__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/negative-integer-starting-with-zero")
ADD_TEST(invalid__truncated-unicode-surrogate "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/truncated-unicode-surrogate")
ADD_TEST(invalid__truncated-unicode-surrogate__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/truncated-unicode-surrogate")
ADD_TEST(invalid__unicode-identifier "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unicode-identifier")
ADD_TEST(invalid__unicode-identifier__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unicode-identifier")
ADD_TEST(invalid__null-byte-in-object-key "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/null-byte-in-object-key")
ADD_TEST(invalid__null-byte-in-object-key__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/null-byte-in-object-key")
ADD_TEST(invalid__unterminated-key "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-key")
ADD_TEST(invalid__unterminated-key__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-key")
ADD_TEST(invalid__object-no-value "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-no-value")
ADD_TEST(invalid__object-no-value__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-no-value")
ADD_TEST(invalid__real-truncated-at-point "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-truncated-at-point")
ADD_TEST(invalid__real-truncated-at-point__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-truncated-at-point")
ADD_TEST(invalid__object-in-unterminated-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-in-unterminated-array")
ADD_TEST(invalid__object-in-unterminated-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-in-unterminated-array")
ADD_TEST(invalid__lone-open-brace "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/lone-open-brace")
ADD_TEST(invalid__lone-open-brace__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/lone-open-brace")
ADD_TEST(invalid__extra-comma-in-multiline-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/extra-comma-in-multiline-array")
ADD_TEST(invalid__extra-comma-in-multiline-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/extra-comma-in-multiline-array")
ADD_TEST(invalid__object-garbage-at-end "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-garbage-at-end")
ADD_TEST(invalid__object-garbage-at-end__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-garbage-at-end")
ADD_TEST(invalid__null "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/null")
ADD_TEST(invalid__null__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/null")
ADD_TEST(invalid__too-big-negative-integer "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/too-big-negative-integer")
ADD_TEST(invalid__too-big-negative-integer__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/too-big-negative-integer")
ADD_TEST(invalid__unterminated-empty-key "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-empty-key")
ADD_TEST(invalid__unterminated-empty-key__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-empty-key")
ADD_TEST(invalid__bracket-comma "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/bracket-comma")
ADD_TEST(invalid__bracket-comma__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/bracket-comma")
ADD_TEST(invalid__object-unterminated-value "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-unterminated-value")
ADD_TEST(invalid__object-unterminated-value__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-unterminated-value")
ADD_TEST(invalid__lone-second-surrogate "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/lone-second-surrogate")
ADD_TEST(invalid__lone-second-surrogate__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/lone-second-surrogate")
ADD_TEST(invalid__ascii-unicode-identifier "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/ascii-unicode-identifier")
ADD_TEST(invalid__ascii-unicode-identifier__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/ascii-unicode-identifier")
ADD_TEST(invalid__integer-starting-with-zero "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/integer-starting-with-zero")
ADD_TEST(invalid__integer-starting-with-zero__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/integer-starting-with-zero")
ADD_TEST(invalid__null-byte-in-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/null-byte-in-string")
ADD_TEST(invalid__garbage-at-the-end "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/garbage-at-the-end")
ADD_TEST(invalid__garbage-at-the-end__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/garbage-at-the-end")
ADD_TEST(invalid__apostrophe "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/apostrophe")
ADD_TEST(invalid__apostrophe__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/apostrophe")
ADD_TEST(invalid__unterminated-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-array")
ADD_TEST(invalid__unterminated-array__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-array")
ADD_TEST(invalid__invalid-escape "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-escape")
ADD_TEST(invalid__invalid-escape__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-escape")
ADD_TEST(invalid__empty "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/empty")
ADD_TEST(invalid__empty__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/empty")
ADD_TEST(invalid__unterminated-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-string")
ADD_TEST(invalid__unterminated-string__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-string")
ADD_TEST(invalid__unterminated-array-and-object "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-array-and-object")
ADD_TEST(invalid__unterminated-array-and-object__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/unterminated-array-and-object")
ADD_TEST(invalid__invalid-second-surrogate "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-second-surrogate")
ADD_TEST(invalid__invalid-second-surrogate__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-second-surrogate")
ADD_TEST(invalid__tab-character-in-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/tab-character-in-string")
ADD_TEST(invalid__tab-character-in-string__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/tab-character-in-string")
ADD_TEST(invalid__object-apostrophes "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-apostrophes")
ADD_TEST(invalid__object-apostrophes__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-apostrophes")
ADD_TEST(invalid__bracket-one-comma "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/bracket-one-comma")
ADD_TEST(invalid__bracket-one-comma__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/bracket-one-comma")
ADD_TEST(invalid__real-truncated-at-e "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-truncated-at-e")
ADD_TEST(invalid__real-truncated-at-e__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-truncated-at-e")
ADD_TEST(invalid__null-byte-outside-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/null-byte-outside-string")
ADD_TEST(invalid__invalid-negative-real "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-negative-real")
ADD_TEST(invalid__invalid-negative-real__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/invalid-negative-real")
ADD_TEST(invalid__minus-sign-without-number "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/minus-sign-without-number")
ADD_TEST(invalid__minus-sign-without-number__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/minus-sign-without-number")
ADD_TEST(invalid__real-positive-overflow "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-positive-overflow")
ADD_TEST(invalid__real-positive-overflow__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-positive-overflow")
ADD_TEST(invalid__real-negative-overflow "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-negative-overflow")
ADD_TEST(invalid__real-negative-overflow__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/real-negative-overflow")
ADD_TEST(invalid__lone-open-bracket "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/lone-open-bracket")
ADD_TEST(invalid__lone-open-bracket__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/lone-open-bracket")
ADD_TEST(invalid__object-no-colon "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-no-colon")
ADD_TEST(invalid__object-no-colon__strip "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "--strip" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid/object-no-colon")
ADD_TEST(invalid-unicode__overlong-ascii-encoding "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/overlong-ascii-encoding")
ADD_TEST(invalid-unicode__invalid-utf-8-in-string "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-string")
ADD_TEST(invalid-unicode__invalid-utf-8-in-identifier "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-identifier")
ADD_TEST(invalid-unicode__invalid-utf-8-after-backslash "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-after-backslash")
ADD_TEST(invalid-unicode__truncated-utf-8 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/truncated-utf-8")
ADD_TEST(invalid-unicode__invalid-utf-8-in-array "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-array")
ADD_TEST(invalid-unicode__invalid-utf-8-in-escape "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-escape")
ADD_TEST(invalid-unicode__invalid-utf-8-in-exponent "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-exponent")
ADD_TEST(invalid-unicode__overlong-3-byte-encoding "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/overlong-3-byte-encoding")
ADD_TEST(invalid-unicode__encoded-surrogate-half "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/encoded-surrogate-half")
ADD_TEST(invalid-unicode__lone-invalid-utf-8 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/lone-invalid-utf-8")
ADD_TEST(invalid-unicode__overlong-4-byte-encoding "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/overlong-4-byte-encoding")
ADD_TEST(invalid-unicode__invalid-utf-8-in-bigger-int "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-bigger-int")
ADD_TEST(invalid-unicode__invalid-utf-8-in-int "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-int")
ADD_TEST(invalid-unicode__lone-utf-8-continuation-byte "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/lone-utf-8-continuation-byte")
ADD_TEST(invalid-unicode__invalid-utf-8-in-real-after-e "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/invalid-utf-8-in-real-after-e")
ADD_TEST(invalid-unicode__not-in-unicode-range "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/not-in-unicode-range")
ADD_TEST(invalid-unicode__restricted-utf-8 "/home/apoelstra/dload/code/cj-client/jansson/bin/json_process" "/home/apoelstra/dload/code/cj-client/jansson/test/suites/invalid-unicode/restricted-utf-8")
