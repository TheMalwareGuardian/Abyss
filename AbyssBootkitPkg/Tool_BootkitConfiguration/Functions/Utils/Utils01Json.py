import os
import re
import json
import string
import random


# Generates a random string of a random length between min_length and max_length with letters, digits, and special characters
def UtilsJson_GenerateRandomString(min_length=5, max_length=30):

	# Special characters
	SPECIAL_CHARACTERS = '|@#&$_-+.,<>:;!?'

	# Length
	length = random.randint(min_length, max_length)

	# Characters
	characters = string.ascii_letters + string.digits + SPECIAL_CHARACTERS

	# String
	return ''.join(random.choice(characters) for _ in range(length))



# Modify keys in the JSON data by replacing them with random strings, keeping _comment_ keys unchanged and keeping break lines
def UtilsJson_ModifyJsonKeys(json_str):

	# Use regex to find all keys in the JSON
	pattern = r'"(.*?)":\s*(".*?"|\d+|true|false|null|\{.*?\}|\[.*?\])'

	# Reaplece Key
	def replace_key(match):
		key = match.group(1)
		value = match.group(2)
		# Preserve comment keys and their values
		if key.startswith('_comment_'):
			return f'"{key}": {value}'
		# Replace other keys with random strings
		else:
			# Generates a random string of a random length between min_length and max_length with letters, digits, and special characters
			new_key = UtilsJson_GenerateRandomString()
			return f'"{new_key}": {value}'

	# Replace keys in the JSON string
	modified_json_str = re.sub(pattern, replace_key, json_str, flags=re.DOTALL)

	# Modified Json
	return modified_json_str



# Update the JSON templates while preserving formatting
def UtilsJson_UpdateJsonTemplates(templates_path):

	# Iterate
	for filename in os.listdir(templates_path):

		# Json files
		if filename.endswith('.json'):
			filepath = os.path.join(templates_path, filename)
			
			# Read the file as text
			with open(filepath, 'r') as file:
				json_str = file.read()

			# Modify keys in the JSON data by replacing them with random strings, keeping _comment_ keys unchanged and keeping break lines
			modified_json_str = UtilsJson_ModifyJsonKeys(json_str)
			
			# Write the modified JSON back to the file, preserving the original format
			with open(filepath, 'w') as file:
				file.write(modified_json_str)


# Print C Style Json Keys
def UtilsJson_PrintCStyleJsonKeys(templates_path, template_filename):

	# Open and read the Json file
	with open(os.path.join(templates_path, template_filename), 'r') as file:
		data = json.load(file)

	# Create a list of keys
	keys = list(data.keys())
	num_keys = len(keys)

	# Iterate
	for i in range(num_keys):
		key = keys[i]

		# Comment
		if key.startswith("_comment_"):

			# Get the variable name from the comment key
			variable_name = f'Global_PreBootConfiguration_PayloadsEncryptedConfiguration_{key[len("_comment_"):]}'

			# Find the next non-comment key
			for j in range(i + 1, num_keys):
				next_key = keys[j]
				if not next_key.startswith("_comment_"):

					# Use the next key as the variable value
					variable_value = next_key

					# Print the variable definition
					print(f'const CHAR8 {variable_name}[] = "{variable_value}";')
					break
