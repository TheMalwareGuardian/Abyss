import re
import argparse


# Clean
def clean_code(input_file):

	# Open file
	with open(input_file, 'r') as infile:
		code = infile.read()

	# Merge consecutive single-line comments (//) into a single line
	code = re.sub(r'(^|\n)\s*//(.*?)\n\s*//(.*?)', r'\1//\2 \3', code)

	# Remove multi-line comments (/* ... */)
	code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)

	# Remove single-line comments after code on the same line
	code = re.sub(r'(?<!\S)//.*', '', code)

	# Ensure no more than three consecutive blank lines
	code = re.sub(r'(\n\s*){4,}', '\n\n\n', code)

	# Remove unnecessary blank lines after '{' if followed by code
	code = re.sub(r'\{\n(\s*\S)', r'{\n\1', code)

	# Handle excessive blank lines after function declarations
	code = re.sub(r'(\)\s*\{)\n\n+', r'\1\n', code)

	# Remove leading blank lines
	code = re.sub(r'^(\s*\n)+', '', code)

	# Remove trailing blank lines
	code = re.sub(r'(\n\s*)+$', '', code)

	# Preserve indentation and remove trailing spaces/tabs
	cleaned_code = []
	for line in code.splitlines():
		cleaned_code.append(line.rstrip())

	# Join cleaned lines and ensure no more than three consecutive blank lines
	cleaned_code = '\n'.join(cleaned_code)
	cleaned_code = re.sub(r'(\n\s*){4,}', '\n\n\n', cleaned_code)

	# Clean
	return cleaned_code


# Main
def main():

	# Set up the argument parser
	parser = argparse.ArgumentParser(
		description="Remove comments and clean up C source code.",
		epilog="Example: python Remove_Comments_From_Source_Code_File.py -i input.c -o output.c"
	)
	parser.add_argument('-i', '--input', required=True, help="Path to the input .c file.")
	parser.add_argument('-o', '--output', help="Path to save the cleaned .c file. If omitted, the cleaned code will be displayed on the console.")
	args = parser.parse_args()

	# Try
	try:
		# Clean the code
		cleaned_code = clean_code(args.input)

		# Output to file or console
		if args.output:
			with open(args.output, 'w') as outfile:
				outfile.write(cleaned_code + '\n')
			print(f"Cleaned code has been saved to '{args.output}'.")
		else:
			print("\n// -------------------------------------------------------------------------------------------------------------------------------------------\n")
			print(cleaned_code)
			print("\n// -------------------------------------------------------------------------------------------------------------------------------------------\n")

	# Exception
	except FileNotFoundError:
		print(f"Error: File '{args.input}' not found.")
	except Exception as e:
		print(f"An unexpected error occurred: {e}")


# Main
if __name__ == "__main__":
	main()
