from shift_cipher import encrypt, decrypt
from brute_force_dictionary import brute_force, dictionary_attack
from chi_square_attack import chi_square_attack


def main():

    while True:
        print("\n==============================")
        print("      SHIFT CIPHER LAB")
        print("==============================")
        print("1. Encrypt")
        print("2. Decrypt")
        print("3. Brute Force Attack")
        print("4. Dictionary Attack")
        print("5. Chi-Square Attack")
        print("6. Exit")

        choice = input("Enter choice: ")

        # Encryption
        if choice == "1":

            plaintext = input("Enter plaintext: ")
            key = int(input("Enter key (0-25): "))

            ciphertext = encrypt(plaintext, key)

            print("\nCiphertext:", ciphertext)

        # Decryption
        elif choice == "2":

            ciphertext = input("Enter ciphertext: ")
            key = int(input("Enter key (0-25): "))

            plaintext = decrypt(ciphertext, key)

            print("\nPlaintext:", plaintext)

        # Brute Force
        elif choice == "3":

            ciphertext = input("Enter ciphertext: ")

            print("\n=== Brute Force Attack ===")

            results = brute_force(ciphertext)

            for key, plaintext in results:
                print("Key:", key, "|", plaintext)

        # Dictionary Attack
        elif choice == "4":

            ciphertext = input("Enter ciphertext: ")

            print("\n=== Dictionary Attack ===")

            key, plaintext, score = dictionary_attack(ciphertext)

            print("Predicted Key:", key)
            print("Plaintext:", plaintext)
            print("Dictionary Score:", score)

        # Chi-Square
        elif choice == "5":

            ciphertext = input("Enter ciphertext: ")

            print("\n=== Chi-Square Attack ===")

            key, plaintext, score = chi_square_attack(ciphertext)

            print("Predicted Key:", key)
            print("Plaintext:", plaintext)
            print("Chi-Square Score:", round(score, 2))

        # Exit
        elif choice == "6":

            print("Exiting...")
            break

        else:
            print("Invalid choice.")


if __name__ == "__main__":
    main()