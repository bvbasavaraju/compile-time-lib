import re
import os
import json
import hashlib
import sys
import xml.etree.ElementTree as ET

if len(sys.argv) >= 5:
    input_file = sys.argv[1]
    cpp_output_file = sys.argv[2]
    xml_output_file = sys.argv[3]
    json_output_file = sys.argv[4]
else:
    script_dir = os.path.dirname(os.path.abspath(__file__))
    build_dir = os.path.join(script_dir, "build")
    input_file = os.path.join(script_dir, "undefined_symbols_captured.txt")
    cpp_output_file = os.path.join(script_dir, "log_tokens.cpp")
    xml_output_file = os.path.join(script_dir, "log_tokens.xml")
    json_output_file = os.path.join(script_dir, "log_tokens.json")

# Pattern to match get_token<...>() in demangled symbols
pattern = re.compile(r'get_token<(.+?)>\(\)')


def generate_token_hash(template_param):
    """Generate a unique hash for the given template parameter."""
    return int(hashlib.md5(template_param.encode()).hexdigest()[:8], 16) & 0xFFFFFFFF


def extract_string_from_variadic(template_param):
    """Extract the actual string value from variadic template parameters.
    
    Looks for patterns like: string_variadic_t<char, (char)72, (char)101, ...>
    where the numbers are ASCII codes that should be converted to characters.
    """
    # Pattern to match: (char)123 format
    char_pattern = re.compile(r'\(char\)(\d+)')
    
    # Find all character codes in the template parameter
    char_codes = char_pattern.findall(template_param)
    
    if not char_codes:
        return None
    
    try:
        # Convert character codes to actual string
        string_value = ''.join(chr(int(code)) for code in char_codes)
        return string_value
    except (ValueError, OverflowError):
        return None


def parse_symbols(file_path):
    """Parse the undefined symbols file and extract get_token template parameters."""
    tokens = {}
    
    if not os.path.exists(file_path):
        print(f"Warning: Input file {file_path} does not exist")
        return tokens
    
    # Symbols will look like this:
    # get_token<log_metadata_t<string_variadic_t<char, (char)72, ...> >>()

    with open(file_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            
            # Extract template parameters from the symbol
            matches = pattern.findall(line)
            for match in matches:
                template_param = match.strip()
                if template_param not in tokens:
                    token_value = generate_token_hash(template_param)
                    string_value = extract_string_from_variadic(template_param)
                    
                    tokens[template_param] = {
                        'value': token_value,
                        'string': string_value
                    }
    
    return tokens


def generate_cpp_source(tokens, output_file):
    """Generate the C++ source file with get_token specializations."""
    cpp_content = """// Auto-generated file - Do not edit manually
#include <cstdint>
#include "logger/metadata.hpp"

"""
    
    # Add includes and specializations for each token
    for template_param in sorted(tokens.keys()):
        token_info = tokens[template_param]
        token_value = token_info['value']
        string_value = token_info['string']
        
        # Add comment with string value if available
        if string_value:
            cpp_content += f"// Message: \"{string_value}\"\n"
        
        cpp_content += f"""template <>
auto get_token<{template_param}>() -> log_token_t {{
    return {token_value}U;
}}

"""
    
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    with open(output_file, 'w') as f:
        f.write(cpp_content)
    
    print(f"Generated C++ source: {output_file}")


def indent_xml(elem, level=0):
    """Add indentation to XML elements for pretty printing."""
    indent = "\n" + level * "  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = indent + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = indent
        for child in elem:
            indent_xml(child, level + 1)
        if not child.tail or not child.tail.strip():
            child.tail = indent
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = indent


def generate_xml_output(tokens, output_file):
    """Generate XML file with token metadata."""
    root = ET.Element("log_tokens")
    
    for template_param in sorted(tokens.keys()):
        token_info = tokens[template_param]
        token_value = token_info['value']
        string_value = token_info['string']
        
        log_token_elem = ET.SubElement(root, "log_token")
        
        # param_elem = ET.SubElement(log_token_elem, "parameter")
        # param_elem.text = template_param
        
        token_elem = ET.SubElement(log_token_elem, "token")
        token_elem.text = str(token_value)

        in_hex_elem = ET.SubElement(log_token_elem, "in-hex")
        in_hex_elem.text = hex(token_value)

        if string_value:
            string_elem = ET.SubElement(log_token_elem, "string")
            string_elem.text = string_value
    
    # Apply indentation for pretty printing
    indent_xml(root)
    
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    tree = ET.ElementTree(root)
    tree.write(output_file, encoding='utf-8', xml_declaration=True)
    
    print(f"Generated XML output: {output_file}")


def generate_json_output(tokens, output_file):
    """Generate JSON file with token metadata."""
    json_data = {
        "tokens": [
            {
                # "parameter": param,
                "token": tokens[param]['value'],
                "in-hex": hex(tokens[param]['value']),
                "string": tokens[param]['string']
            }
            for param in sorted(tokens.keys())
        ]
    }
    
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    with open(output_file, 'w') as f:
        json.dump(json_data, f, indent=2)
    
    print(f"Generated JSON output: {output_file}")


def main():
    """Main entry point."""
    # Parse the input file
    tokens = parse_symbols(input_file)
    
    if not tokens:
        print("Warning: No tokens found in input file")
    else:
        print(f"Found {len(tokens)} unique tokens")
    
    # Generate output files
    generate_cpp_source(tokens, cpp_output_file)
    generate_xml_output(tokens, xml_output_file)
    generate_json_output(tokens, json_output_file)
    
    print("Token generation completed successfully")


if __name__ == "__main__":
    main()
