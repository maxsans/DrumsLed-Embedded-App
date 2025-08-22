.PHONY: help generate clean

PYTHON = python3
GENERATOR = msg_generator.py
OUTPUT_DIR = ../../src/network/interCom/interMsg/interMsgList
EXAMPLES_DIR = examples

help:
	@echo "Available targets:"
	@echo "  generate CONFIG=<config.json>  - Generate message class from config"
	@echo "  example                        - Generate InterMsgExample"
	@echo "  clean                          - Clean generated files"

generate:
ifndef CONFIG
	$(error CONFIG is undefined. Usage: make generate CONFIG=examples/interMsgExample.json)
endif
	$(PYTHON) $(GENERATOR) $(CONFIG) --output-dir $(OUTPUT_DIR)

example:
	$(PYTHON) $(GENERATOR) $(EXAMPLES_DIR)/interMsgExample.json --output-dir $(OUTPUT_DIR)

clean:
	rm -rf generated/
	@echo "Cleaned generated files"
