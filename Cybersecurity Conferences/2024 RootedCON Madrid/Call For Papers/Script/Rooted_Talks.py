import os
import re
import html
import json
import requests
from InquirerPy import prompt
from typing import List, Tuple



def search_in_json_files() -> None:
	"""
	Searches for a given keyword in JSON files starting with 'RootedTalks' in the current directory.
	Displays titles and descriptions that contain the keyword.
	If no matches are found, prints a message indicating no results.
	:return:
	"""

	# Keyword
	keyword = input('Enter a keyword to search: ')

	# Talks
	for filename in os.listdir('Talks/'):
		if filename.startswith('RootedTalks') and filename.endswith('.json'):

			# Flag
			found_keyword = False

			# Read
			with open('Talks/' + filename, 'r', encoding='utf-8') as file:
				data = json.load(file)
				print(f'File: {filename}')

				# Data
				for item in data:
					title = item.get('Title', '')
					description = item.get('Description', '')
					if keyword.lower() in title.lower() or keyword.lower() in description.lower():
						found_keyword = True
						print(f'Title: {title}')
						print(f'Description: {description}\n')

			# No matches
			if not found_keyword:
				print('No matches found for the given keyword.\n')



def extract_matches_from_url_pattern_rooted_talks(url: str) -> Tuple[List[str], List[str]]:
	"""
	Extracts matches from a given URL based on predefined patterns (Rooted Talks).
	Returns two lists: one for <h4> tag matches and one for <p class='theme-description'> tag matches.
	:param url:
	:return:
	"""

	# Variables
	matches_title = []
	matches_description = []

	try:

		# Request
		response = requests.get(url)
		response.raise_for_status()
		content = response.content.decode('utf-8')

		# Pattern
		pattern_title = r'<h4>(.*?)</h4>'
		pattern_description = r'<p class="theme-description">(.*?)</p>'

		# Matches
		matches_title = re.findall(pattern_title, content, re.DOTALL)
		matches_description = re.findall(pattern_description, content, re.DOTALL)

	except requests.RequestException as e:
		print(f'HTTP request error: {e}')
	except Exception as e:
		print(f'An error occurred: {e}')

	return matches_title, matches_description



def print_and_save_matches(matches_title: List, matches_description: List, filename: str) -> None:
	"""
	Prints and saves non-empty matches to a file.
	:param matches_title:
	:param matches_description:
	:param filename:
	:return:
	"""

	# Data
	data = []
	for title, description in zip(matches_title, matches_description):

		# Decode HTML entities
		decoded_title = html.unescape(title)
		decoded_description = html.unescape(description).replace('<br/>', '').replace('<p>', '')

		# Talks
		if decoded_title and decoded_description:
			print(f'Title: {decoded_title}')
			print(f'Description: {decoded_description}\n')
			data.append({'Title': decoded_title, 'Description': decoded_description})

	# File
	with open('Talks/' + filename, 'w', encoding='utf-8') as file:
		json.dump(data, file, ensure_ascii=False, indent=4)



def get_rooted_talks_info(rooted_url: str, file_name: str) -> None:
	"""
	Retrieves and processes information from a given URL.
	:param rooted_url:
	:param file_name:
	:return:
	"""

	# Data
	matches_title, matches_description = extract_matches_from_url_pattern_rooted_talks(rooted_url)

	# Print and Save
	if matches_title and matches_description:
		print_and_save_matches(matches_title, matches_description, file_name)



# Main
def main():
	"""
	Main function to run the script. Provides a menu for user interaction.
	:return:
	"""

	try:
		
		# Options
		options = {
			'Rooted Talks 2024': 'https://www.rootedcon.com/agenda-rooted2024',
			'Rooted Talks 2023': 'https://www.rootedcon.com/agenda-rooted2023',
			'Rooted Talks 2022': 'https://www.rootedcon.com/agenda-rooted2022',
			'Rooted Talks 2020': 'https://www.rootedcon.com/agenda-rooted2020',
			'Search in Files': None,
			'Exit': None
		}

		# Question
		question = [
			{
				"type": "list",
				"message": "Select an action:",
				"choices": list(options.keys())
			}
		]

		# Loop
		while True:
			result = prompt(questions=question)

			# Menu
			if result[0] == 'Exit':
				break
			elif result[0] == 'Search in Files':
				search_in_json_files()
			elif result[0] in options and options[result[0]]:
				get_rooted_talks_info(options[result[0]], f'RootedTalks{result[0][-4:]}.json')
			else:
				print('Invalid option. Please choose a valid action.')

	# Exception
	except Exception as e:
		print(e)



if __name__ == "__main__":
	main()
