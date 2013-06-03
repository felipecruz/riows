GCC=gcc
SRC=src
TESTS=tests
INCLUDE=include
BUILD=build
FLAGS=-std=c99

SOURCES=$(SRC)/rioev.c
TEST_SOURCES=$(TESTS)/thc.c $(TESTS)/test_rioev.c $(TESTS)/suite.c

all: bin_dir
	$(GCC) -I $(INCLUDE) $(FLAGS) $(SRC)/rioev.c -o $(BUILD)/riows -DDEBUG=0
clean:
	@rm -rf build/
bin_dir:
	@mkdir -p $(BUILD)

ifneq ($(filter$(MAKECMDGOALS),with_args), "")
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

with_args=test,debug,leak

ta: bin_dir 
	$(GCC) -I $(INCLUDE) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -DALL=1 -o $(BUILD)/ta  && $(BUILD)/ta
test: bin_dir 
	$(GCC) -I $(INCLUDE) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -Dt$(RUN_ARGS)=1 -o $(BUILD)/$(RUN_ARGS)  && $(BUILD)/$(RUN_ARGS)
