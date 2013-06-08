GCC=gcc
SRC=src
TESTS=tests
INCLUDE=include
BUILD=build
FLAGS=-std=c99

SOURCES=$(SRC)/list.c $(SRC)/hash.c $(SRC)/trie.c
TEST_SOURCES=$(TESTS)/thc.c $(TESTS)/test_list.c $(TESTS)/test_hash.c $(TESTS)/test_trie.c $(TESTS)/suite.c

all:
	$(GCC) -I $(INCLUDE) $(FLAGS) -c $(SRC)/list.c
	$(GCC) -I $(INCLUDE) $(FLAGS) -c $(SRC)/hash.c
	$(GCC) -I $(INCLUDE) $(FLAGS) -c $(SRC)/trie.c
	ar rcs liblist.a list.o hash.o trie.o
clean:
	@rm -f list.o hash.o trie.o liblist.a
	@rm -rf build/
bin_dir:
	@mkdir -p $(BUILD)

ifneq ($(filter$(MAKECMDGOALS),with_args), "")
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

with_args=test,debug,leak

ta: bin_dir all
	$(GCC) -I $(INCLUDE) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -DALL=1 -o $(BUILD)/ta  && $(BUILD)/ta
test: bin_dir all
	$(GCC) -I $(INCLUDE) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -Dt$(RUN_ARGS)=1 -o $(BUILD)/$(RUN_ARGS)  && $(BUILD)/$(RUN_ARGS)
debug: bin_dir
	$(GCC) -I $(INCLUDE) -g $(SOURCES) $(TEST_SOURCES) $(FLAGS) -Dt$(RUN_ARGS)=1 -o $(BUILD)/$(RUN_ARGS) && gdb $(BUILD)/$(RUN_ARGS)
leak: bin_dir
	$(GCC) -I $(INCLUDE) -g $(SOURCES) $(TEST_SOURCES) $(FLAGS) -Dt$(RUN_ARGS)=1 -o $(BUILD)/$(RUN_ARGS) && valgrind --show-reachable=yes --leak-check=full $(BUILD)/$(RUN_ARGS)
