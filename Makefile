# Makefile for Quixant test programs

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = test_qxtio

.PHONY: all clean test

all: $(TARGET)

$(TARGET): test_qxtio.c
	$(CC) $(CFLAGS) -o $(TARGET) test_qxtio.c
	@echo "Build complete: $(TARGET)"
	@echo "Run with: ./$(TARGET)"

clean:
	rm -f $(TARGET)
	@echo "Cleaned build files"

test: $(TARGET)
	@echo "Running Quixant device test..."
	@echo ""
	./$(TARGET)

help:
	@echo "Quixant Test Program Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build test program"
	@echo "  make test     - Build and run test"
	@echo "  make clean    - Remove build files"
	@echo "  make help     - Show this help"
