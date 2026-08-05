import os
from collections import Counter


def analyze_file(filename):
    """
    Reads a text file and displays:
    - Number of characters
    - Number of words
    - Number of lines
    - Number of unique characters
    - Letter frequency
    """

    current_dir = os.path.dirname(os.path.abspath(__file__))
    project_dir = os.path.dirname(current_dir)
    filepath = os.path.join(project_dir, "datasets", filename)

    if not os.path.exists(filepath):
        print("File not found!")
        return

    with open(filepath, "r", encoding="utf-8") as file:
        text = file.read()

    # Character count
    char_count = len(text)

    # Word count
    words = text.split()
    word_count = len(words)

    # Line count
    lines = text.splitlines()
    line_count = len(lines)

    # Unique characters
    unique_characters = len(set(text))

    # Letter frequency
    letters = [char.lower() for char in text if char.isalpha()]
    frequency = Counter(letters)

    # Display results
    print("\n========== FILE ANALYSIS ==========")
    print(f"File Name          : {filename}")
    print(f"Characters         : {char_count}")
    print(f"Words              : {word_count}")
    print(f"Lines              : {line_count}")
    print(f"Unique Characters  : {unique_characters}")

    print("\nLetter Frequency")
    print("---------------------------")

    for letter in sorted(frequency):
        print(f"{letter} : {frequency[letter]}")

    print("===================================")