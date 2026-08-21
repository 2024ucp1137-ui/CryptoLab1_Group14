from re import search
import sqlite3
import os

DB = "library.db"


def connect():
    return sqlite3.connect(DB)


def setup():
    con = connect()
    cur = con.cursor()

    cur.execute("""
        CREATE TABLE IF NOT EXISTS users(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE,
            password TEXT
        )
    """)

    cur.execute("""
        CREATE TABLE IF NOT EXISTS books(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT,
            author TEXT,
            issued_to INTEGER
        )
    """)

    cur.execute("SELECT COUNT(*) FROM books")

    if cur.fetchone()[0] == 0:
        books = [
            ("The Alchemist", "Paulo Coelho", None),
            ("Clean Code", "Robert Martin", None),
            ("Python Crash Course", "Eric Matthes", None),
            ("Introduction to Algorithms", "Cormen", None)
        ]

        cur.executemany(
            "INSERT INTO books(title, author, issued_to) VALUES(?,?,?)",
            books
        )

    con.commit()
    con.close()


def register():
    username = input("Enter username: ")
    password = input("Enter password: ")

    con = connect()

    try:
        con.execute(
            "INSERT INTO users(username,password) VALUES(?,?)",
            (username, password)
        )
        con.commit()
        print("Registration successful.")
    except sqlite3.IntegrityError:
        print("Username already exists.")

    con.close()


def login():
    username = input("Username: ")
    password = input("Password: ")

    con = connect()

    user = con.execute(
        "SELECT id, username FROM users WHERE username=? AND password=?",
        (username, password)
    ).fetchone()

    con.close()

    if user:
        print("Login successful.")
        return user

    print("Invalid username or password.")
    return None


def search_books():
    search = input("Enter book title or author: ")

    con = connect()

    query = "SELECT * FROM books WHERE title LIKE ? OR author LIKE ?"
    rows = con.execute(query, (f"%{search}%", f"%{search}%")).fetchall()

    try:
        rows = con.execute(query).fetchall()

        if rows:
            print("\nID | Title | Author | Issued To")
            for row in rows:
                print(row)
        else:
            print("No books found.")

    except sqlite3.Error as e:
        print("Database error:", e)

    con.close()


def list_books():
    con = connect()

    rows = con.execute(
        "SELECT * FROM books"
    ).fetchall()

    print("\nAvailable Books")
    print("----------------")

    for row in rows:
        status = "Available" if row[3] is None else "Issued"
        print(row[0], "|", row[1], "|", row[2], "|", status)

    con.close()


def issue_book(user_id):
    book_id = input("Enter book ID: ")

    con = connect()

    book = con.execute(
        "SELECT * FROM books WHERE id=?",
        (book_id,)
    ).fetchone()

    if not book:
        print("Book not found.")
    elif book[3] is not None:
        print("Book is already issued.")
    else:
        con.execute(
            "UPDATE books SET issued_to=? WHERE id=?",
            (user_id, book_id)
        )
        con.commit()
        print("Book issued successfully.")

    con.close()


def return_book(user_id):
    book_id = input("Enter book ID: ")

    con = connect()

    book = con.execute(
        "SELECT * FROM books WHERE id=?",
        (book_id,)
    ).fetchone()

    if not book:
        print("Book not found.")
    elif book[3] != user_id:
        print("You cannot return this book.")
    else:
        con.execute(
            "UPDATE books SET issued_to=NULL WHERE id=?",
            (book_id,)
        )
        con.commit()
        print("Book returned successfully.")

    con.close()


def calculate_fine():
    # VULNERABILITY 2: MISSING AUTHENTICATION

    days = input("Enter number of days book was kept: ")

    try:
        days = int(days)

        if days > 14:
            fine = (days - 14) * 5
        else:
            fine = 0

        print("Fine = Rs.", fine)

    except ValueError:
        print("Invalid number.")


def view_book_file():
    # VULNERABILITY 3: DIRECTORY TRAVERSAL

    filename = input("Enter book file path: ")

    try:
        with open(filename, "r") as file:
            print("\nFile Content:")
            print(file.read())

    except Exception as e:
        print("File error:", e)


def user_menu(user):
    while True:
        print("\n===== Member Menu =====")
        print("1. Search Books")
        print("2. List Books")
        print("3. Issue Book")
        print("4. Return Book")
        print("5. Calculate Fine")
        print("6. View Book File")
        print("7. Logout")

        choice = input("Enter choice: ")

        if choice == "1":
            search_books()

        elif choice == "2":
            list_books()

        elif choice == "3":
            issue_book(user[0])

        elif choice == "4":
            return_book(user[0])

        elif choice == "5":
            calculate_fine()

        elif choice == "6":
            view_book_file()

        elif choice == "7":
            print("Logged out.")
            break

        else:
            print("Invalid choice.")


def main():
    setup()

    while True:
        print("\n==============================")
        print("   LIBRARY MANAGEMENT SYSTEM")
        print("==============================")
        print("1. Register")
        print("2. Login")
        print("3. Search Books")
        print("4. Calculate Fine")
        print("5. Exit")

        choice = input("Enter choice: ")

        if choice == "1":
            register()

        elif choice == "2":
            user = login()

            if user:
                user_menu(user)

        elif choice == "3":
            search_books()

        elif choice == "4":
            calculate_fine()

        elif choice == "5":
            print("Thank you.")
            break

        else:
            print("Invalid choice.")


if __name__ == "__main__":
    main()