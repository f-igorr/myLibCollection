

# [make test] соберет все тесты ./tests/test_*.c
# можно собирать и запускать тесты по отдельности c TEST_ONLY= часть имени файла (после test_)
# например [make test TEST_ONLY=func] -> соберет тест для func.c
# например [make run_test TEST_ONLY=func] -> соберет и запустит тест для func.c


CC        ?= gcc
CFLAGS    := -Wall -Wextra -Werror -std=c17 
LDLIBS    := -lm
DEBUG     ?=
AR        := ar
ARFLAGS   := rcs
NAME_LIB  := libmycollection


CFLAGS    += $(if $(DEBUG), -g -O0 -fsanitize=address -fno-omit-frame-pointer)

# пути
DIR_SRC       := src/
DIR_INC       := include/
DIR_BIN       := bin/

DIR_TEST      := tests/
DIR_BIN_TEST  := $(DIR_TEST)bin_test/


# файлы
SRC       := $(wildcard $(DIR_SRC)*.c)
OBJ       := $(patsubst $(DIR_SRC)%.c,$(DIR_BIN)%.o,$(SRC)) 
TARGET    := $(DIR_BIN)$(NAME_LIB).a

SRC_TEST  := $(wildcard $(DIR_TEST)test_*.c)
TESTS     := $(patsubst $(DIR_TEST)%.c,$(DIR_BIN_TEST)%,$(SRC_TEST))
TEST_ONLY ?=
TEST_RUN  := $(if $(TEST_ONLY),$(filter %$(TEST_ONLY),$(TESTS)),$(TESTS))



.PHONY: lib test all run_test clean clean_test clean_all


# готовая статическая библиотека
lib: $(TARGET) | $(DIR_BIN)

# сборка всех тестов
test: $(TEST_RUN) | $(DIR_BIN_TEST)

all: lib test


$(TARGET): $(OBJ) | $(DIR_BIN)
	$(AR) $(ARFLAGS) $@ $^

# .c -> .o (библиотека)
$(DIR_BIN)%.o: $(DIR_SRC)%.c | $(DIR_BIN)
	$(CC) $(CFLAGS) -I$(DIR_INC) -c $< -o $@

# .c -> execut bin (тесты)
$(DIR_BIN_TEST)test_%: $(DIR_TEST)test_%.c $(OBJ) | $(DIR_BIN_TEST)
	$(CC) $(CFLAGS) -I$(DIR_INC) $< $(OBJ) -o $@ $(LDLIBS)


$(DIR_BIN):
	mkdir -p $(DIR_BIN)

$(DIR_BIN_TEST):
	mkdir -p $(DIR_BIN_TEST)


# запуск тестов
#	#$(foreach x,$(TEST_RUN),.$(x);) # работает, но код ниже предпочтительней
run_test: $(TEST_RUN)
	@echo "DEBUG = $(DEBUG)"
	@for t in $(TEST_RUN); do \
    echo "===== RUN $$t ====="; \
    $$t; \
    echo; \
	done


clean: 
	rm -rf $(DIR_BIN)

clean_test: 
	rm -rf $(DIR_BIN_TEST)

clean_all: clean clean_test