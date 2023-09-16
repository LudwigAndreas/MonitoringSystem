import os
import re

# Function to convert PascalCase to snake_case
def pascal_to_snake(name):
	return re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', name).lower()

# Define the directory where your files are located
files = [
	"src/CMakeLists.txt",
	"test/CMakeLists.txt",
	"src/modules/bonus/CMakeLists.txt",
	"src/modules/cpu/CMakeLists.txt",
	"src/modules/memory/CMakeLists.txt",
	"src/modules/network/CMakeLists.txt",
]

# Iterate through files in the directory
for file_path in files:

	# Rename the file from PascalCase to snake_case
	# new_filename = pascal_to_snake(os.path.splitext(filename)[0]) + os.path.splitext(filename)[1]
	# new_file_path = os.path.join(root, new_filename)
	# os.rename(file_path, new_file_path)

	updated_contents = []
	# Update include directives in the file
	with open(file_path, 'r') as file:
		file_contents = file.readlines()
		for line in file_contents:
			if line.find(".cc") != -1 or line.find(".h") != -1:
				split = line.split("/")
				split[-1] = pascal_to_snake(split[-1])
				line = "/".join(split)
			updated_contents.append(line)

	with open(file_path, 'w') as file:
		file.write("".join(updated_contents))

