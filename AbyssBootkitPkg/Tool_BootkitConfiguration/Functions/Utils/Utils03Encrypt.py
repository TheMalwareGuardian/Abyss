import os
import re
import json
import base64
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives import padding
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from Functions.Utils.Utils04Obfuscate import UtilsObfuscate_ObfuscateStringXorCaesar


# Function to derive a X-byte key from a password
def UtilsEncrypt_DeriveKeyFromPassword(x_byte_length_key, password, salt):
	kdf = PBKDF2HMAC(
		algorithm=hashes.SHA256(),
		length=x_byte_length_key,
		salt=salt,
		iterations=100000,
		backend=default_backend()
	)
	key = kdf.derive(password.encode())
	return key


# Function to encrypt, encode, and obfuscate the configuration
def UtilsEncrypt_EncryptEncodeObfuscateConfiguration(key_to_encrypt, iv_to_encrypt, xor_key_for_obfuscation, caesar_shift_for_obfuscation, templates_path, template_filename):

	# Read configuration JSON from file
	with open(os.path.join(templates_path, template_filename), "r") as file:
		json_str = file.read()

		# Remove comments with the custom '_comment' prefix
		clean_json_str = re.sub(r'"_comment[^"]*":\s*"[^"]*",?\s*', '', json_str)

		# Load the cleaned JSON string into an object
		config_template = json.loads(clean_json_str)

	# Convert configuration to JSON string
	config_json = json.dumps(config_template)

	# Pad the configuration JSON to be a multiple of the block size
	padder = padding.PKCS7(algorithms.AES.block_size).padder()
	padded_data = padder.update(config_json.encode()) + padder.finalize()

	# Encrypt the padded JSON
	cipher = Cipher(algorithms.AES(key_to_encrypt), modes.CBC(iv_to_encrypt), backend=default_backend())
	encryptor = cipher.encryptor()
	encrypted_config = encryptor.update(padded_data) + encryptor.finalize()

	# Encode the encrypted configuration in base64
	encrypted_base64_encoded_config = base64.b64encode(iv_to_encrypt + encrypted_config).decode()

	# Obfuscate the base64 encoded string
	encrypted_encoded_obfuscated_config = UtilsObfuscate_ObfuscateStringXorCaesar(encrypted_base64_encoded_config, xor_key_for_obfuscation, caesar_shift_for_obfuscation)

	return encrypted_encoded_obfuscated_config
