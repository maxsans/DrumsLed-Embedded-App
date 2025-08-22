# Message Generator Tool

This tool automatically generates InterMsg-derived classes with serialization and deserialization methods.

## Features

- Automatic generation of `serializePriv()` and `deserializePriv()` methods
- Network byte order handling for multi-byte types
- Support for various data types (integers, floats, character arrays)
- Configurable via JSON files
- Generates both header (.hpp) and implementation (.cpp) files

## Usage

### Basic Usage
```bash
python3 msg_generator.py config.json --output-dir ./output
```

### Using Makefile
```bash
# Generate from specific config
make generate CONFIG=examples/interMsgExample.json

# Generate example
make example

# Clean generated files
make clean
```

## Configuration Format

```json
{
  "class_name": "InterMsgExample",
  "filename": "interMsgExample",
  "description": "Example inter-message structure for communication",
  "msg_id": "Example",
  "send_type": "UnicastWithControl",
  "fields": [
    {
      "name": "exampleData",
      "type": "uint32_t",
      "description": "Example data for the message"
    },
    {
      "name": "exampleString",
      "type": "char[]",
      "size": 256,
      "description": "Example string for the message"
    }
  ]
}
```

## Supported Types

- `uint8_t`, `uint16_t`, `uint32_t`
- `int8_t`, `int16_t`, `int32_t`
- `float`
- `char[]` (fixed-size character arrays)

## Send Types

- `UnicastWithControl`
- `UnicastWithoutControl`
- `Broadcast`
