from collections import Counter


# English letter frequencies
ENGLISH_FREQ = {
    'A': 0.0812, 'B': 0.0149, 'C': 0.0271, 'D': 0.0432,
    'E': 0.1202, 'F': 0.0230, 'G': 0.0203, 'H': 0.0592,
    'I': 0.0731, 'J': 0.0010, 'K': 0.0069, 'L': 0.0398,
    'M': 0.0261, 'N': 0.0695, 'O': 0.0768, 'P': 0.0182,
    'Q': 0.0011, 'R': 0.0602, 'S': 0.0628, 'T': 0.0910,
    'U': 0.0288, 'V': 0.0111, 'W': 0.0209, 'X': 0.0017,
    'Y': 0.0211, 'Z': 0.0007
}


def decrypt(text, key):
    result = ""

    for char in text:
        if char.isupper():
            result += chr((ord(char) - ord('A') - key) % 26 + ord('A'))

        elif char.islower():
            result += chr((ord(char) - ord('a') - key) % 26 + ord('a'))

        else:
            result += char

    return result


def chi_square_score(text):
    letters = [char for char in text.upper() if char.isalpha()]
    total = len(letters)

    if total == 0:
        return float("inf")

    counts = Counter(letters)

    score = 0

    for letter in ENGLISH_FREQ:
        observed = counts.get(letter, 0)
        expected = ENGLISH_FREQ[letter] * total

        score += ((observed - expected) ** 2) / expected

    return score


def chi_square_attack(ciphertext):
    best_key = 0
    best_score = float("inf")
    best_plaintext = ""

    print("=== Chi-Square Attack ===")

    for key in range(26):
        plaintext = decrypt(ciphertext, key)
        score = chi_square_score(plaintext)

        print(f"Key: {key:2} | Score: {score:.2f} | {plaintext}")

        if score < best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext

    print("\n=== Chi-Square Result ===")
    print("Predicted Key:", best_key)
    print("Plaintext:", best_plaintext)
    print("Chi-Square Score:", round(best_score, 2))

    return best_key, best_plaintext, best_score


if __name__ == "__main__":
    ciphertext = input("Enter ciphertext: ")

    chi_square_attack(ciphertext)