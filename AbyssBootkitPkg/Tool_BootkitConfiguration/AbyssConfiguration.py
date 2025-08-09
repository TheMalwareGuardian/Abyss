from InquirerPy import prompt
from Functions.Functions00Configuration import FunctionsConfiguration_UpdateIniFileAndJsonTemplates, FunctionsConfiguration_CreateEncryptedEncodedObfuscatedAbyssConfiguration


# Options
MAIN_MENU_OPTION_RANDOMIZE_INIT_VALUES_AND_TEMPLATE_FIELDS = 'Randomize Init Values (Encryption and Obfuscation) and Template Fields (JSON Keys)'
MAIN_MENU_OPTION_CREATE_ABYSS_CONFIGURATION = 'Create Abyss Configuration'
MAIN_MENU_OPTION_EXIT = 'Exit'


# Main
def main():

	# Question
	menu = [
		{
			'type': 'list',
			'message': 'Select an option',
			'choices': [MAIN_MENU_OPTION_RANDOMIZE_INIT_VALUES_AND_TEMPLATE_FIELDS, MAIN_MENU_OPTION_CREATE_ABYSS_CONFIGURATION, MAIN_MENU_OPTION_EXIT]
		}
	]

	# Loop
	while True:

		# Ask
		choice = prompt(menu)
		option = choice[0]

		# Randomize Init Values and Template Fields
		if option == MAIN_MENU_OPTION_RANDOMIZE_INIT_VALUES_AND_TEMPLATE_FIELDS:
			FunctionsConfiguration_UpdateIniFileAndJsonTemplates()
		
		# Create Abyss Configuration
		elif option == MAIN_MENU_OPTION_CREATE_ABYSS_CONFIGURATION:
			FunctionsConfiguration_CreateEncryptedEncodedObfuscatedAbyssConfiguration()

		# Exit
		elif option == MAIN_MENU_OPTION_EXIT:
			break

		# Invalid
		else:
			print('Invalid choice, try again ;)')


# Main
if __name__ == "__main__":

	main()
