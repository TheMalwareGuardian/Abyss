import base64


# Function to apply Caesar cipher (character rotation)
def UtilsObfuscate_CaesarCipher(input_bytes, shift):
	rotated_bytes = bytearray(input_bytes)
	for i in range(len(rotated_bytes)):
		rotated_bytes[i] = (rotated_bytes[i] + shift) % 256
	return rotated_bytes


# Function to obfuscate a string with XOR and Caesar cipher
def UtilsObfuscate_ObfuscateStringXorCaesar(base64_encoded_string, xor_key, caesar_shift):

	# Encode the input string to UTF-8
	# This step ensures that the string is in a format that can be converted to bytes
	utf8_encoded_string  = base64_encoded_string.encode('utf-8')

	# Convert the UTF-8 encoded string to a bytearray for manipulation
	# This allows us to modify the individual bytes of the string
	input_bytes = bytearray(utf8_encoded_string )

	# XOR obfuscation
	# Each byte of the input is XORed with the given key
	# This provides a layer of obfuscation making it harder to reverse the string without the key
	for i in range(len(input_bytes)):
		input_bytes[i] ^= xor_key

	# Apply Caesar cipher to the XOR-obfuscated bytearray
	# This shifts each byte by the specified amount, adding another layer of obfuscation
	shuffled_string = UtilsObfuscate_CaesarCipher(input_bytes, caesar_shift)

	# Encode the result to Base64 to get the final obfuscated string
	# Base64 encoding ensures that the result is in a text format suitable for storage and transmission
	base64_encoded_result = base64.b64encode(shuffled_string)

	# Convert the Base64 bytes back to a UTF-8 string
	# This makes the final obfuscated result a readable string format
	final_result = base64_encoded_result.decode('utf-8')

	return final_result


# Function to generate an obfuscated key with XOR and a permutation
def UtilsObfuscate_ObfuscateKeyXorPermutation(key, xor_key, permutation):

	# Convert the key to a bytearray for manipulation
	# This allows us to modify the individual bytes of the key
	shuffled_key = bytearray(key)

	# XOR obfuscation
	# Each byte of the key is XORed with the given xor_key
	# This provides a layer of obfuscation making it harder to reverse the key without the xor_key
	for i in range(len(shuffled_key)):
		shuffled_key[i] ^= xor_key

	# Permutation (Shuffling)
	# The bytes of the key are rearranged according to the provided permutation array
	# This adds another layer of obfuscation by changing the order of the bytes
	shuffled_key = bytearray([shuffled_key[permutation[i]] for i in range(len(shuffled_key))])

	return shuffled_key
