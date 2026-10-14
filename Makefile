CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -Wno-unused-result
TARGET = proj
SOURCE = project.c

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $<

test: $(TARGET)
	@set -eu; \
		if [ ! -d public-tests ]; then unzip -q public-tests.zip; fi; \
		$(MAKE) -C public-tests EXE=../$(TARGET); \
		passed=$$(wc -l < public-tests/tests.log); \
		total=$$(find public-tests -maxdepth 1 -name 'test*.in' | wc -l); \
		if [ "$$passed" -ne "$$total" ]; then \
			printf 'Falharam %s de %s testes públicos.\n' "$$((total - passed))" "$$total"; \
			exit 1; \
		fi

clean:
	rm -f $(TARGET)
	@if [ -d public-tests ]; then $(MAKE) -C public-tests clean; fi
