def display_menu():
    print("=" * 40)
    print("        Welcome to CryptoLabX")
    print("=" * 40)
    print("1. Encrypt")
    print("2. Decrypt")
    print("3. Attack")
    print("4. Analyze")
    print("5. Exit")


while True:
    display_menu()

    choice = input("\nEnter your choice: ")

    if choice == "1":
        print("\nEncrypt Module - Coming Soon\n")

    elif choice == "2":
        print("\nDecrypt Module - Coming Soon\n")

    elif choice == "3":
        print("\nAttack Module - Coming Soon\n")

    elif choice == "4":
        print("\nAnalyze Module - Coming Soon\n")

    elif choice == "5":
        print("\nThank you for using CryptoLabX!")
        break

    else:
        print("\nInvalid Choice! Try Again.\n")