import random
import configparser


# Generate random ini values
def UtilsIni_GenerateRandomIniValues():

	# 256 bits
	Key_KeySize = 32

	# 128 bits
	Key_SaltSize = 16

	# 128 bits
	Key_Iv_Size = 16

	# Generate a random XOR key (0x00 to 0xFF)
	Key_XorKey = f'0x{random.randint(0, 0xFF):02x}'

	# Generate a random permutation list with length equal to key_size
	Key_Permutation = ', '.join(map(str, random.sample(range(Key_KeySize), Key_KeySize)))

	# Generate a random XOR key (0x00 to 0xFF)
	Configuration_XorKey = f'0x{random.randint(0, 0xFF):02x}'

	# Generate a random Caesar shift value (0 to 25)
	Configuration_CaesarShift = random.randint(0, 25)

	# Values
	return Key_KeySize, Key_SaltSize, Key_Iv_Size, Key_XorKey, Key_Permutation, Configuration_XorKey, Configuration_CaesarShift


# Update ini file
def UtilsIni_UpdateIniFile(config_file):

	# Generate random ini values
	Key_KeySize, Key_SaltSize, Key_Iv_Size, Key_XorKey, Key_Permutation, Configuration_XorKey, Configuration_CaesarShift = UtilsIni_GenerateRandomIniValues()

	# Parser
	config = configparser.ConfigParser()

	# Create or update sections
	if not config.read(config_file):
		config.add_section('Obfuscate_Key')
		config.add_section('Obfuscate_Configuration')

	config['Obfuscate_Key'] = {
		'key_size': Key_KeySize,
		'salt_size': Key_SaltSize,
		'iv_size': Key_Iv_Size,
		'xor_key': Key_XorKey,
		'permutation': Key_Permutation
	}

	config['Obfuscate_Configuration'] = {
		'xor_key': Configuration_XorKey,
		'caesar_shift': Configuration_CaesarShift
	}

	# Write the configuration file
	with open(config_file, 'w') as configfile:
		config.write(configfile)


# Read ini file
def UtilsIni_ReadIniFile(config_file):

	# Read configuration from config.ini
	config = configparser.ConfigParser()
	config.read(config_file)

	# Extract values from the configuration
	Key_KeySize = int(config['Obfuscate_Key']['key_size'])
	Key_SaltSize = int(config['Obfuscate_Key']['salt_size'])
	Key_Iv_Size = int(config['Obfuscate_Key']['iv_size'])
	Key_XorKey = int(config['Obfuscate_Key']['xor_key'], 16)
	Key_Permutation = list(map(int, config['Obfuscate_Key']['permutation'].split(', ')))
	Configuration_XorKey = int(config['Obfuscate_Configuration']['xor_key'], 16)
	Configuration_CaesarShift = int(config['Obfuscate_Configuration']['caesar_shift'])

	return Key_KeySize, Key_SaltSize, Key_Iv_Size, Key_XorKey, Key_Permutation, Configuration_XorKey, Configuration_CaesarShift
