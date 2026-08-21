import os
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from shift_cipher import decrypt


def load_dictionary():
    dictionary_path = os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
        "dictionary",
        "english_words.txt"
    )

    with open(dictionary_path, "r") as file:
        words = set(word.strip().lower() for word in file if word.strip())

    return words


def dictionary_score(text, dictionary):
    words = text.lower().split()

    score = 0

    for word in words:
        cleaned_word = word.strip(".,!?;:'\"()[]{}")

        if cleaned_word in dictionary:
            score += 1

    return score


def brute_force(ciphertext):
    results = []

    for key in range(26):
        plaintext = decrypt(ciphertext, key)

        results.append((key, plaintext))

    return results


def dictionary_attack(ciphertext):
    dictionary = load_dictionary()

    candidates = brute_force(ciphertext)

    best_key = None
    best_plaintext = None
    best_score = -1

    for key, plaintext in candidates:
        score = dictionary_score(plaintext, dictionary)

        if score > best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext

    return best_key, best_plaintext, best_score


if __name__ == "__main__":
    ciphertext = "KHOOR ZRUOG WKLV LV D FLSKHU"

    print("=== Brute Force Attack ===")

    results = brute_force(ciphertext)

    for key, plaintext in results:
        print("Key:", key, "|", plaintext)

    print("\n=== Dictionary Attack ===")

    key, plaintext, score = dictionary_attack(ciphertext)

    print("Predicted Key:", key)
    print("Plaintext:", plaintext)
    print("Dictionary Score:", score)