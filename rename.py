import os
import re

# Function to convert PascalCase to snake_case
def pascal_to_snake(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()

# Directory to start the renaming process
start_dir = 'backup_src'  # Replace with the actual directory path

# File extensions to look for
extensions = ['.cc', '.h']

# Recursively rename files
for root, dirs, files in os.walk(start_dir):
	for file in files:
		if file.endswith(tuple(extensions)):
			old_path = os.path.join(root, file)
			file_name, file_extension = os.path.splitext(file)
			new_name = pascal_to_snake(file_name) + file_extension
			new_path = os.path.join(root, new_name)

			# Rename the file
			os.rename(old_path, new_path)
			print(f'Renamed: {old_path} -> {new_path}')