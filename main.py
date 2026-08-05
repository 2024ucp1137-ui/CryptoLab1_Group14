from utils.logger import log_activity
from analysis.file_analyzer import analyze_file


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
        log_activity("User selected Encrypt")
        print("\nEncrypt Module - Coming Soon")
        input("\nPress Enter to continue...")

    elif choice == "2":
        log_activity("User selected Decrypt")
        print("\nDecrypt Module - Coming Soon")
        input("\nPress Enter to continue...")

    elif choice == "3":
        log_activity("User selected Attack")
        print("\nAttack Module - Coming Soon")
        input("\nPress Enter to continue...")

    elif choice == "4":
        log_activity("User selected Analyze")

        print("\nAvailable sample files:")
        print("1. sample1.txt")
        print("2. sample2.txt")
        print("3. sample3.txt")
        print("4. sample4.txt")
        print("5. sample5.txt")

        filename = input("\nEnter file name (example: sample1.txt): ")

        analyze_file(filename)

        input("\nPress Enter to continue...")

    elif choice == "5":
        log_activity("Application Closed")
        print("\nThank you for using CryptoLabX!")
        break

    else:
        print("\nInvalid Choice! Please try again.")
        input("\nPress Enter to continue...")