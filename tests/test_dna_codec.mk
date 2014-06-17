TEST_DNA_CODEC_NAME=dna_codec
TEST_DNA_CODEC_EXECUTABLE=tests/test_$(TEST_DNA_CODEC_NAME)
TEST_DNA_CODEC_OBJECTS=tests/test.o tests/test_$(TEST_DNA_CODEC_NAME).o
TEST_EXECUTABLES+=$(TEST_DNA_CODEC_EXECUTABLE)
TEST_OBJECTS+=$(TEST_DNA_CODEC_OBJECTS)
$(TEST_DNA_CODEC_EXECUTABLE): $(LIBRARY_OBJECTS) $(TEST_DNA_CODEC_OBJECTS)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@
TEST_DNA_CODEC_RUN=test_run_$(TEST_DNA_CODEC_NAME)
$(TEST_DNA_CODEC_RUN): $(TEST_DNA_CODEC_EXECUTABLE)
	./$^
TEST_RUNS+=$(TEST_DNA_CODEC_RUN)

