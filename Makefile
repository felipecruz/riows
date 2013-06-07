GCC=gcc
SRC=src
TESTS=tests
INCLUDES=-Iinclude/ -Ivendor/http-parser -Ivendor/cws/src
BUILD=build
FLAGS=

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    FLAGS += -std=gnu99
endif
ifeq ($(UNAME_S),Darwin)
    FLAGS += -std=c99
endif

SOURCES=$(SRC)/rioev.c $(SRC)/rnetwork.c
MAIN_SOURCES=$(SOURCES) $(SRC)/riows.c
TEST_SOURCES=$(TESTS)/thc.c $(TESTS)/test_rioev.c $(TESTS)/suite.c
DEPS=vendor/http-parser/http_parser.o vendor/cws/b64.o vendor/cws/cws.o

all: bin_dir http_parser.o cws.o
	$(GCC) $(INCLUDES) $(FLAGS) $(MAIN_SOURCES) -o $(BUILD)/riows -DDEBUG=0
clean:
	@rm -rf build/
bin_dir:
	@mkdir -p $(BUILD)

debug: bin_dir
	$(GCC) $(INCLUDES) -g $(FLAGS) $(DEPS) $(MAIN_SOURCES) -o $(BUILD)/riows -DDEBUG=1
leak:
	$(GCC) $(INCLUDES) -g $(FLAGS) $(DEPS) $(MAIN_SOURCES) -o $(BUILD)/riows -DDEBUG=1 && sudo valgrind --leak-check=full build/riows


ifneq ($(filter$(MAKECMDGOALS),with_args), "")
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

with_args=test,debug,leak

ta: bin_dir 
	$(GCC) $(INCLUDES) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -DALL=1 -o $(BUILD)/ta  && $(BUILD)/ta
test: bin_dir 
	$(GCC) $(INCLUDES) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -Dt$(RUN_ARGS)=1 -o $(BUILD)/$(RUN_ARGS)  && $(BUILD)/$(RUN_ARGS)

HTTP_PARSER_DIR=vendor/http-parser
CWS_DIR=vendor/cws

cws.o:
	cd $(CWS_DIR); $(MAKE)

http_parser.o:
	cd $(HTTP_PARSER_DIR); $(MAKE) http_parser.o
