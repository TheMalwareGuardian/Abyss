import os
from InquirerPy import prompt


# List template files
def UtilsSelect_ListTemplates(templates_folder, search_string):

	# Get a list of JSON files in the templates directory that contain the search string
	templates = [f for f in os.listdir(templates_folder) if f.endswith('.json') and search_string in f]

	if not templates:
		print(f'No JSON templates found in the templates ({templates_folder}) directory.')
		return

	# Prompt user to select a template
	menu = [
		{
			'type': 'list',
			'message': 'Select a configuration template:',
			'choices': templates
		}
	]

	# Ask
	choice = prompt(menu)
	return choice[0]
