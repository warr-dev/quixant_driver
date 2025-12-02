# Makefile for Quixant test programs

CC = gcc
CFLAGS = -Wall -Wextra -O2
SRCDIR = examples
TARGETS = test_qxtio core_io_example

.PHONY: all clean test demo help

all: $(TARGETS)

test_qxtio: $(SRCDIR)/test_qxtio.c
	$(CC) $(CFLAGS) -o test_qxtio $(SRCDIR)/test_qxtio.c
	@echo "Build complete: test_qxtio"

core_io_example: $(SRCDIR)/core_io_example.c
	$(CC) $(CFLAGS) -o core_io_example $(SRCDIR)/core_io_example.c
	@echo "Build complete: core_io_example"

clean:
	rm -f $(TARGETS)
	@echo "Cleaned build files"

test: test_qxtio
	@echo "Running Quixant device test..."
	@echo ""
	./test_qxtio

demo: core_io_example
	@echo "Running CORE I/O example..."
	@echo ""
	./core_io_example

help:
	@echo "Quixant Test Program Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make               - Build all programs"
	@echo "  make test_qxtio    - Build basic test program"
	@echo "  make core_io_example - Build CORE I/O example"
	@echo "  make test          - Build and run basic test"
	@echo "  make demo          - Build and run CORE I/O example"
	@echo "  make clean         - Remove build files"
	@echo "  make help          - Show this help"
