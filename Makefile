CC=mpicc
CFLAGS=-O2 -g -I. -D_POSIX_C_SOURCE=200112L -Wall -std=c99
LD=$(CC)
Q=@
ECHO=echo

all:

include tests/Makefile.mk
include examples/Makefile.mk
include applications/Makefile.mk

all: $(TESTS) $(EXAMPLES) $(APPLICATIONS)

LIBRARY=

# actor engine
include engine/Makefile.mk
LIBRARY += $(ENGINE)

include patterns/Makefile.mk
LIBRARY += $(PATTERNS)

# kernels for genomics
include kernels/Makefile.mk
LIBRARY += $(KERNELS)

# system stuff
include system/Makefile.mk
LIBRARY += $(SYSTEM)

# storage
include storage/Makefile.mk
LIBRARY += $(STORAGE)

# data structures
include structures/Makefile.mk
LIBRARY += $(STRUCTURES)

# hash functions
include hash/Makefile.mk
LIBRARY += $(HASH)

# inputs for actors
include input/Makefile.mk
LIBRARY += $(INPUT)

# data storage
include data/Makefile.mk
LIBRARY += $(DATA)

# formats
include formats/Makefile.mk
LIBRARY += $(FORMATS)

%.o: %.c
	$(Q)$(ECHO) "  CC $@"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(Q)$(ECHO) "  RM"
	$(Q)$(RM) $(LIBRARY)
	$(Q)$(RM) $(EXAMPLE_RING) $(EXAMPLE_MOCK) $(EXAMPLE_READER) $(EXAMPLE_REMOTE_SPAWN)
	$(Q)$(RM) $(EXAMPLE_SYNCHRONIZE) $(EXAMPLE_CONTROLLER) $(EXAMPLE_HELLO_WORLD)
	$(Q)$(RM) $(EXAMPLE_CLONE) $(EXAMPLE_MIGRATION)
	$(Q)$(RM) $(TEST_FIFO) $(TEST_FIFO_ARRAY) $(TEST_HASH_TABLE_GROUP) $(TEST_NODE)
	$(Q)$(RM) $(TEST_HASH_TABLE) $(TEST_VECTOR) $(TEST_DYNAMIC_HASH_TABLE)
	$(Q)$(RM) $(TEST_PACKER) $(TEST_DNA_SEQUENCE) $(ARGONNITE)
	$(Q)$(RM) $(EXAMPLES) $(TESTS) $(APPLICATIONS)

# tests

test_vector: $(LIBRARY) $(TEST_VECTOR)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_dna_sequence: $(LIBRARY) $(TEST_DNA_SEQUENCE)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_packer: $(LIBRARY) $(TEST_PACKER)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_node: $(LIBRARY) $(TEST_NODE)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_queue: $(LIBRARY) $(TEST_FIFO)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_hash_table: $(LIBRARY) $(TEST_HASH_TABLE)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_hash_table_group: $(LIBRARY) $(TEST_HASH_TABLE_GROUP)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_queue_group: $(LIBRARY) $(TEST_FIFO_ARRAY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

test_dynamic_hash_table: $(LIBRARY) $(TEST_DYNAMIC_HASH_TABLE)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

# examples

example_remote_spawn: $(EXAMPLE_REMOTE_SPAWN) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_ring: $(EXAMPLE_RING) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_mock: $(EXAMPLE_MOCK) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_reader: $(EXAMPLE_READER) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_synchronize: $(EXAMPLE_SYNCHRONIZE) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_controller: $(EXAMPLE_CONTROLLER) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_hello_world: $(EXAMPLE_HELLO_WORLD) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_clone: $(EXAMPLE_CLONE) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

example_migration: $(EXAMPLE_MIGRATION) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@

# applications
argonnite: $(ARGONNITE) $(LIBRARY)
	$(Q)$(ECHO) "  LD $@"
	$(Q)$(CC) $(CFLAGS) $^ -o $@
