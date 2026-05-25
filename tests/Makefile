# по умолчанию [make], [make test] соберут все тесты в папке (test_*.c)
# можно собирать и запускать тесты по отдельности,
# передав часть имени файла (после test_) через аргумент ONLY=
# [make ONLY=func] -> соберет тест из test_func.c
# [make run ONLY=func] -> соберет и запустит тест из test_func.c


CC        ?= gcc
CFLAGS    := -Wall -Wextra -Werror -std=c17 
LDLIBS    := -lm
DEBUG     ?=

CFLAGS$   += $(if $(DEBUG), -g -O0 -fsanitize=address -fno-omit-frame-pointer)

DIR_SRC   := ../src/
DIR_INC   := ../include/
DIR_BIN   := bin_test/

SRC       := $(wildcard $(DIR_SRC)*.c)
OBJ       := $(patsubst $(DIR_SRC)%.c,$(DIR_BIN)%.o,$(SRC)) 

SRC_TEST  := $(wildcard test_*.c)
TESTS     := $(patsubst %.c,$(DIR_BIN)%,$(SRC_TEST))

ONLY      ?=

TEST_RUN  := $(if $(ONLY),$(filter %$(ONLY),$(TESTS)),$(TESTS))



.PHONY: all clean run test


test: $(TEST_RUN)

$(DIR_BIN)test_%: test_%.c $(OBJ) | $(DIR_BIN)
	$(CC) $(CFLAGS) -I$(DIR_INC) $< $(OBJ) -o $@ $(LDLIBS)


$(DIR_BIN)%.o: $(DIR_SRC)%.c | $(DIR_BIN)
	$(CC) $(CFLAGS) -I$(DIR_INC) -c $< -o $@


#	#$(foreach x,$(TEST_RUN),.$(x);) # работает, но код ниже предпочтительней
run: $(TEST_RUN)
	@echo "DEBUG = $(DEBUG)"
	@for t in $(TEST_RUN); do \
    echo "===== RUN $$t ====="; \
    $$t; \
    echo; \
	done

$(DIR_BIN):
	mkdir -p $(DIR_BIN)

clean: 
	rm -rf $(DIR_BIN)