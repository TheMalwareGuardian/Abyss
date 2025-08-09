import os
import configparser
from datetime import datetime
from Functions.Utils.Utils02Select import UtilsSelect_ListTemplates
from Functions.Utils.Utils00Ini import UtilsIni_UpdateIniFile, UtilsIni_ReadIniFile
from Functions.Utils.Utils04Obfuscate import UtilsObfuscate_ObfuscateKeyXorPermutation
from Functions.Utils.Utils01Json import UtilsJson_UpdateJsonTemplates, UtilsJson_PrintCStyleJsonKeys
from Functions.Utils.Utils03Encrypt import UtilsEncrypt_DeriveKeyFromPassword, UtilsEncrypt_EncryptEncodeObfuscateConfiguration


# Define global variables
Global_FunctionsConfiguration_IniFile = 'AbyssConfiguration.ini'
Global_FunctionsConfiguration_TemplatesPath = 'Templates/'


# Update ini file and json templates
def FunctionsConfiguration_UpdateIniFileAndJsonTemplates():

	# Update ini file
	UtilsIni_UpdateIniFile(Global_FunctionsConfiguration_IniFile)

	# Update json templates
	UtilsJson_UpdateJsonTemplates(Global_FunctionsConfiguration_TemplatesPath)

	# Done
	print('Done')


# Create encrypted, encoded, obfuscated Abyss configuration
def FunctionsConfiguration_CreateEncryptedEncodedObfuscatedAbyssConfiguration():

	# Read ini file
	Key_KeySize, Key_SaltSize, Key_Iv_Size, Key_XorKey, Key_Permutation, Configuration_XorKey, Configuration_CaesarShift = UtilsIni_ReadIniFile(Global_FunctionsConfiguration_IniFile)

	# List and select a template
	template_filename = UtilsSelect_ListTemplates(Global_FunctionsConfiguration_TemplatesPath, 'Abyss')
	if template_filename is None:
		return

	# Generate a timestamp
	timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')

	# Password
	password = input('Enter the password to encrypt the configuration: ')

	# Secrets
	print('\n#------------------------------------------------------\n')
	print('Copy these values to a notepad; you may need them in the future.')

	# Generate a random salt
	salt = os.urandom(Key_SaltSize)
	salt_hex = ' '.join(f'{b:02x}' for b in salt)
	print(f'SECRET		({Key_SaltSize}-byte salt):		{salt_hex}')

	# Key
	key = UtilsEncrypt_DeriveKeyFromPassword(Key_KeySize, password, salt)
	key_hex = ' '.join(f'{b:02x}' for b in key)
	print(f'SECRET		({Key_KeySize}-byte key):		{key_hex}')

	# Generate a random IV
	iv = os.urandom(Key_Iv_Size)
	iv_hex = ' '.join(f'{b:02x}' for b in iv)
	print(f'SECRET		({Key_Iv_Size}-byte iv):		{iv_hex}')

	# Start
	print('\n#------------------------------------------------------\n')
	print('// Decrypt')

	# Obfuscate Key
	obfuscated_key = UtilsObfuscate_ObfuscateKeyXorPermutation(key, Key_XorKey, Key_Permutation)
	print(f'const UINTN Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyKeySize = {Key_KeySize};')
	print(f'const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyXorKey = 0x{Key_XorKey:02x};')
	permutation_str = ", ".join(str(i) for i in Key_Permutation)
	print(f'const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyPermutation[{Key_KeySize}] = {{{permutation_str}}};')
	obfuscated_key_str = ", ".join(f"0x{b:02x}" for b in obfuscated_key)
	print(f'const UINT8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_KeyObfuscatedKey[{Key_KeySize}] = {{{obfuscated_key_str}}};')

	# Encrypt, encode and obfuscate the configuration template
	encrypted_encoded_obfuscated_config = UtilsEncrypt_EncryptEncodeObfuscateConfiguration(key, iv, Configuration_XorKey, Configuration_CaesarShift, Global_FunctionsConfiguration_TemplatesPath, template_filename)
	print(f'const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationXorKey = 0x{Configuration_XorKey:02x};')
	print(f'const INTN Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationCaesarShift = {Configuration_CaesarShift};')
	print(f'const CHAR8 Global_PreBootConfiguration_PayloadsEncryptedConfiguration_ConfigurationEncryptedConfiguration[] = "{encrypted_encoded_obfuscated_config}";')

	# Json Keys
	print('// Json Keys')
	UtilsJson_PrintCStyleJsonKeys(Global_FunctionsConfiguration_TemplatesPath, template_filename)

	# End
	print('\n#------------------------------------------------------\n')
