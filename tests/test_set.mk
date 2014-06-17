TEST_SET_NAME=set
TEST_SET_EXECUTABLE=tests/test_$(TEST_SET_NAME)
TEST_SET_OBJECTS=tests/test.o tests/test_$(TEST_SET_NAME).o
TEST_EXECUTABLES+=$(TEST_SET_EXECUTABLE)
TEST_OBJECTS+=$(TEST_SET_OBJECTS)
$(TEST_SET_EXECUTABLE): $(LIBRARY_OBJECTS) $(TEST_SET_OBJECTS)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@
TEST_SET_RUN=test_run_$(TEST_SET_NAME)
$(TEST_SET_RUN): $(TEST_SET_EXECUTABLE)
	./$^
TEST_RUNS+=$(TEST_SET_RUN)

