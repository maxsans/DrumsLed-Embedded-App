#!/usr/bin/env python3
"""
Message class generator for InterMsg communication system.
Generates C++ classes with automatic serialization/deserialization methods.
"""

import json
import os
import argparse
import sys
from typing import Dict, List, Any
from dataclasses import dataclass


@dataclass
class MessageField:
    name: str
    type: str
    size: int = 0  # For arrays and strings
    description: str = ""


class MessageGenerator:
    def __init__(self):
        self.type_mappings = {
            "uint8_t": {"size": 1, "serialize": "direct", "deserialize": "direct"},
            "uint16_t": {"size": 2, "serialize": "htons", "deserialize": "ntohs"},
            "uint32_t": {"size": 4, "serialize": "htonl", "deserialize": "ntohl"},
            "int8_t": {"size": 1, "serialize": "direct", "deserialize": "direct"},
            "int16_t": {"size": 2, "serialize": "htons", "deserialize": "ntohs"},
            "int32_t": {"size": 4, "serialize": "htonl", "deserialize": "ntohl"},
            "float": {
                "size": 4,
                "serialize": "float_convert",
                "deserialize": "float_convert",
            },
            "char[]": {
                "size": "variable",
                "serialize": "memcpy",
                "deserialize": "memcpy",
            },
        }

    def validate_config(self, config: Dict[str, Any]) -> bool:
        """Validate the configuration file format."""
        required_fields = [
            "class_name",
            "filename",
            "description",
            "msg_id",
            "send_type",
            "fields",
        ]

        for field in required_fields:
            if field not in config:
                print(
                    f"Error: Missing required field '{field}' in configuration",
                    file=sys.stderr,
                )
                return False

        if not isinstance(config["fields"], list):
            print("Error: 'fields' must be a list", file=sys.stderr)
            return False

        for i, field in enumerate(config["fields"]):
            if not isinstance(field, dict):
                print(f"Error: Field {i} must be a dictionary", file=sys.stderr)
                return False

            if "name" not in field or "type" not in field:
                print(f"Error: Field {i} missing 'name' or 'type'", file=sys.stderr)
                return False

            if field["type"].endswith("[]") and "size" not in field:
                print(
                    f"Error: Array field '{field['name']}' missing 'size'",
                    file=sys.stderr,
                )
                return False

        return True

    def generate_serialize_method(
        self, fields: List[MessageField], class_name: str
    ) -> str:
        serialize_body = ""
        for field in fields:
            field_type = field.type.replace("[]", "")

            if field.type.endswith("[]"):
                # Array handling
                serialize_body += (
                    f"    // Serialize {field.name} ({field.description})\n"
                )
                serialize_body += f"    std::memcpy(data + offset, m_{field.name}, sizeof(m_{field.name}));\n"
                serialize_body += f"    offset += sizeof(m_{field.name});\n\n"
            elif field_type in self.type_mappings:
                mapping = self.type_mappings[field_type]
                if mapping["serialize"] == "direct":
                    serialize_body += (
                        f"    // Serialize {field.name} ({field.description})\n"
                    )
                    serialize_body += f"    std::memcpy(data + offset, &m_{field.name}, sizeof(m_{field.name}));\n"
                    serialize_body += f"    offset += sizeof(m_{field.name});\n\n"
                elif mapping["serialize"] in ["htons", "htonl"]:
                    serialize_body += f"    // Serialize {field.name} ({field.description}) - network byte order\n"
                    serialize_body += f"    {field_type} {field.name}Network = {mapping['serialize']}(m_{field.name});\n"
                    serialize_body += f"    std::memcpy(data + offset, &{field.name}Network, sizeof({field.name}Network));\n"
                    serialize_body += f"    offset += sizeof({field.name}Network);\n\n"
                elif mapping["serialize"] == "float_convert":
                    serialize_body += f"    // Serialize {field.name} ({field.description}) - float to network order\n"
                    serialize_body += f"    uint32_t {field.name}Network = htonl(*reinterpret_cast<const uint32_t*>(&m_{field.name}));\n"
                    serialize_body += f"    std::memcpy(data + offset, &{field.name}Network, sizeof({field.name}Network));\n"
                    serialize_body += f"    offset += sizeof({field.name}Network);\n\n"

        return f"""uint32_t {class_name}::serializePriv(char *data) const
{{
    uint32_t offset = 0;

{serialize_body}
    return offset;
}}"""

    def generate_deserialize_method(
        self, fields: List[MessageField], class_name: str
    ) -> str:
        deserialize_body = ""
        for field in fields:
            field_type = field.type.replace("[]", "")

            if field.type.endswith("[]"):
                # Array handling
                deserialize_body += (
                    f"    // Deserialize {field.name} ({field.description})\n"
                )
                deserialize_body += f"    std::memcpy(m_{field.name}, data + offset, sizeof(m_{field.name}));\n"
                deserialize_body += f"    offset += sizeof(m_{field.name});\n\n"
            elif field_type in self.type_mappings:
                mapping = self.type_mappings[field_type]
                if mapping["deserialize"] == "direct":
                    deserialize_body += (
                        f"    // Deserialize {field.name} ({field.description})\n"
                    )
                    deserialize_body += f"    std::memcpy(&m_{field.name}, data + offset, sizeof(m_{field.name}));\n"
                    deserialize_body += f"    offset += sizeof(m_{field.name});\n\n"
                elif mapping["deserialize"] in ["ntohs", "ntohl"]:
                    deserialize_body += f"    // Deserialize {field.name} ({field.description}) - network byte order\n"
                    deserialize_body += f"    {field_type} {field.name}Network;\n"
                    deserialize_body += f"    std::memcpy(&{field.name}Network, data + offset, sizeof({field.name}Network));\n"
                    deserialize_body += f"    m_{field.name} = {mapping['deserialize']}({field.name}Network);\n"
                    deserialize_body += (
                        f"    offset += sizeof({field.name}Network);\n\n"
                    )
                elif mapping["deserialize"] == "float_convert":
                    deserialize_body += f"    // Deserialize {field.name} ({field.description}) - float from network order\n"
                    deserialize_body += f"    uint32_t {field.name}Network;\n"
                    deserialize_body += f"    std::memcpy(&{field.name}Network, data + offset, sizeof({field.name}Network));\n"
                    deserialize_body += (
                        f"    uint32_t {field.name}Host = ntohl({field.name}Network);\n"
                    )
                    deserialize_body += f"    m_{field.name} = *reinterpret_cast<const float*>(&{field.name}Host);\n"
                    deserialize_body += (
                        f"    offset += sizeof({field.name}Network);\n\n"
                    )

        return f"""uint32_t {class_name}::deserializePriv(const char *data)
{{
    uint32_t offset = 0;

{deserialize_body}
    return offset;
}}"""

    def generate_class(self, config: Dict[str, Any]) -> tuple[str, str]:
        class_name = config["class_name"]
        fields = [MessageField(**field) for field in config["fields"]]

        # Generate header file
        header_template = """/**
 * @file {filename}.hpp
 * @brief {description}
 * @author Generated by msg_generator.py
 * @date {date}
 */

#ifndef __{class_name_upper}_HPP__
#define __{class_name_upper}_HPP__

#include "network/interCom/interMsg/interMsg.hpp"
#include <cstdint>

/**
 * @brief {description}
 */
class {class_name} : public InterMsg
{{
    private:
    static constexpr const SendType m_sendType = SendType::{send_type};

{member_variables}

    public:
    /**
     * @brief Create a {class_name} with specified data.
     */
    {class_name}(Client client{constructor_params});

    /**
     * @brief Constructor for {class_name} with a callback.
     */
    {class_name}(MessageReceivedCallback callback);

    /**
     * @see InterMsg::serializePriv
     */
    uint32_t serializePriv(char *data) const override;

    /**
     * @see InterMsg::deserializePriv
     */
    uint32_t deserializePriv(const char *data) override;

{getters}

    /**
     * @brief Get a description of the message.
     */
    std::string toString() const override;
}};

#endif // __{class_name_upper}_HPP__"""

        # Generate implementation file
        impl_template = """/**
 * @file {filename}.cpp
 * @author Generated by msg_generator.py
 * @date {date}
 */

#include "{filename}.hpp"
#include "tools/logStream/logStream.hpp"
#include <arpa/inet.h>

{class_name}::{class_name}(Client client{constructor_params})
    : InterMsg(client, InterMsgId::{msg_id}, m_sendType)
{{
{constructor_body}
}}

{class_name}::{class_name}(MessageReceivedCallback callback)
    : InterMsg(callback, InterMsgId::{msg_id})
{{
}}

{serialize_method}

{deserialize_method}

{getter_implementations}

std::string {class_name}::toString() const
{{
    return "{class_name}: {toString_fields}, client = " + getClient().getIP().getIpString();
}}"""

        # Fill template variables
        import datetime

        member_variables = ""
        constructor_params = ""
        constructor_body = ""
        getters = ""
        getter_implementations = ""
        toString_fields = ""

        for i, field in enumerate(fields):
            # Member variables
            if field.type.endswith("[]"):
                member_variables += (
                    f"    /**\n     * @brief {field.description}\n     */\n"
                )
                member_variables += f"    char m_{field.name}[{field.size}];\n\n"
            else:
                member_variables += (
                    f"    /**\n     * @brief {field.description}\n     */\n"
                )
                member_variables += f"    {field.type} m_{field.name};\n\n"

            # Constructor parameters and body
            if not field.type.endswith("[]"):
                constructor_params += f", {field.type} {field.name}"
                constructor_body += f"    m_{field.name} = {field.name};\n"

            # Getters
            if field.type.endswith("[]"):
                getters += f"    /**\n     * @brief Get {field.name}.\n     */\n"
                getters += f"    const char* get{field.name.capitalize()}() const;\n\n"
                getter_implementations += f"const char* {class_name}::get{field.name.capitalize()}() const\n{{\n    return m_{field.name};\n}}\n\n"
            else:
                getters += f"    /**\n     * @brief Get {field.name}.\n     */\n"
                getters += f"    {field.type} get{field.name.capitalize()}() const;\n\n"
                getter_implementations += f"{field.type} {class_name}::get{field.name.capitalize()}() const\n{{\n    return m_{field.name};\n}}\n\n"

            # toString fields
            if i > 0:
                toString_fields += ' + ", '
            if field.type.endswith("[]"):
                toString_fields += f'{field.name} = " + std::string(m_{field.name})'
            else:
                toString_fields += f'{field.name} = " + std::to_string(m_{field.name})'

        header_content = header_template.format(
            filename=config["filename"],
            description=config["description"],
            date=datetime.date.today().strftime("%Y-%m-%d"),
            class_name=class_name,
            class_name_upper=class_name.upper(),
            send_type=config["send_type"],
            member_variables=member_variables.rstrip(),
            constructor_params=constructor_params,
            getters=getters.rstrip(),
        )

        impl_content = impl_template.format(
            filename=config["filename"],
            date=datetime.date.today().strftime("%Y-%m-%d"),
            class_name=class_name,
            msg_id=config["msg_id"],
            constructor_params=constructor_params,
            constructor_body=constructor_body.rstrip(),
            serialize_method=self.generate_serialize_method(fields, class_name),
            deserialize_method=self.generate_deserialize_method(fields, class_name),
            getter_implementations=getter_implementations.rstrip(),
            toString_fields=toString_fields,
        )

        return header_content, impl_content


def main():
    parser = argparse.ArgumentParser(description="Generate InterMsg classes")
    parser.add_argument("config", help="JSON configuration file")
    parser.add_argument("--output-dir", default="./generated", help="Output directory")
    parser.add_argument(
        "--validate-only", action="store_true", help="Only validate configuration"
    )

    args = parser.parse_args()

    try:
        with open(args.config, "r") as f:
            config = json.load(f)
    except FileNotFoundError:
        print(f"Error: Configuration file '{args.config}' not found", file=sys.stderr)
        return 1
    except json.JSONDecodeError as e:
        print(f"Error: Invalid JSON in '{args.config}': {e}", file=sys.stderr)
        return 1

    generator = MessageGenerator()

    if not generator.validate_config(config):
        return 1

    if args.validate_only:
        print(f"Configuration file '{args.config}' is valid")
        return 0

    try:
        header_content, impl_content = generator.generate_class(config)

        os.makedirs(args.output_dir, exist_ok=True)

        filename = config["filename"]
        header_path = os.path.join(args.output_dir, f"{filename}.hpp")
        impl_path = os.path.join(args.output_dir, f"{filename}.cpp")

        with open(header_path, "w") as f:
            f.write(header_content)

        with open(impl_path, "w") as f:
            f.write(impl_content)

        print(f"Generated {filename}.hpp and {filename}.cpp in {args.output_dir}")
        return 0

    except Exception as e:
        print(f"Error generating files: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
