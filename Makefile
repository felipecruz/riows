GCC=gcc
SRC=src
TESTS=tests
INCLUDES=-Iinclude/ -Ivendor/http-parser -Ivendor/cws/src -Ivendor/cdats/include
BUILD=build
FLAGS=-lcrypto -O2

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    FLAGS += -std=gnu99
endif
ifeq ($(UNAME_S),Darwin)
    FLAGS += -std=c99
endif

SOURCES=$(SRC)/rioev.c $(SRC)/rhttp.c $(SRC)/rstatic.c $(SRC)/rnetwork.c
MAIN_SOURCES=$(SOURCES) $(SRC)/riows.c
TEST_SOURCES=$(TESTS)/thc.c $(TESTS)/test_rstatic.c $(TESTS)/test_rioev.c $(TESTS)/suite.c
DEPS=vendor/http-parser/http_parser.o vendor/cws/b64.o vendor/cws/cws.o \
	 vendor/cdats/hash.o

all: bin_dir http_parser.o cws.o cdats.o
	$(GCC) $(INCLUDES) $(FLAGS) $(DEPS) $(MAIN_SOURCES) -o $(BUILD)/riows
clean:
	@rm -rf build/
bin_dir:
	@mkdir -p $(BUILD)
install:
	cp $(BUILD)/riows /usr/local/bin/

debug: bin_dir
	$(GCC) $(INCLUDES) -g $(FLAGS) $(DEPS) $(MAIN_SOURCES) -o $(BUILD)/riows -DDEBUG=1
leak:
	$(GCC) $(INCLUDES) -g $(FLAGS) $(DEPS) $(MAIN_SOURCES) -o $(BUILD)/riows -DDEBUG=1 && sudo valgrind --leak-check=full build/riows


ifneq ($(filter$(MAKECMDGOALS),with_args), "")
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

with_args=test,debug,leak

ta: bin_dir http_parser.o cws.o cdats.o
	$(GCC) $(INCLUDES) $(DEPS) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -DALL=1 -o $(BUILD)/ta  && $(BUILD)/ta
test: bin_dir http_parser.o cws.o cdats.o
	$(GCC) $(INCLUDES) $(DEPS) $(SOURCES) $(TEST_SOURCES) $(FLAGS) -Dt$(RUN_ARGS)=1 -o $(BUILD)/$(RUN_ARGS)  && $(BUILD)/$(RUN_ARGS)

HTTP_PARSER_DIR=vendor/http-parser
CWS_DIR=vendor/cws
CDATS_DIR=vendor/cdats

cws.o:
	cd $(CWS_DIR); $(MAKE)

http_parser.o:
	cd $(HTTP_PARSER_DIR); $(MAKE) http_parser.o

cdats.o:
	cd $(CDATS_DIR); $(MAKE)
