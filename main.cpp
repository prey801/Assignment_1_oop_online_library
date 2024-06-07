#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Book class to represent a book in the library
class Book {
public:
    std::string title;
    std::string author;
    long long isbn;
    bool available;

    Book(const std::string& title, const std::string& author, long long isbn) 
        : title(title), author(author), isbn(isbn), available(true) {}

    void setAvailability(bool available) {
        this->available = available;
    }
};

// User class to represent a user of the library
class User {
public:
    std::string name;
    std::vector<Book*> borrowedBooks;

    User(const std::string& name) : name(name) {}

    void borrowBook(Book* book) {
        if (book->available) {
            borrowedBooks.push_back(book);
            book->setAvailability(false);
            std::cout << "Book '" << book->title << "' borrowed successfully.\n";
        } else {
            std::cout << "Book '" << book->title << "' is not available.\n";
        }
    }

    void returnBook(Book* book) {
        if (std::find(borrowedBooks.begin(), borrowedBooks.end(), book) != borrowedBooks.end()) {
            borrowedBooks.erase(std::remove(borrowedBooks.begin(), borrowedBooks.end(), book), borrowedBooks.end());
            book->setAvailability(true);
            std::cout << "Book '" << book->title << "' returned successfully.\n";
        } else {
            std::cout << "You have not borrowed this book.\n";
        }
    }

    void showBorrowedBooks() {
        if (borrowedBooks.empty()) {
            std::cout << "You have not borrowed any books.\n";
            return;
        }

        std::cout << "Borrowed books:\n";
        for (Book* book : borrowedBooks) {
            std::cout << "- " << book->title << " by " << book->author << std::endl;
        }
    }
};

// Library class to manage the library collection
class Library {
public:
    std::vector<Book*> books;
    std::vector<User*> users;

    void addBook(const std::string& title, const std::string& author, long long isbn) {
        books.push_back(new Book(title, author, isbn));
        std::cout << "Book '" << title << "' added successfully.\n";
    }

    void addUser(const std::string& name) {
        users.push_back(new User(name));
        std::cout << "User '" << name << "' added successfully.\n";
    }

    void manageUser(const std::string& username) {
        auto it = std::find_if(users.begin(), users.end(),
                               [username](User* user) { return user->name == username; });
        if (it != users.end()) {
            User* user = *it;
            int choice;
            do {
                std::cout << "\nUser Management Menu:\n";
                std::cout << "1. Borrow a book\n";
                std::cout << "2. Return a book\n";
                std::cout << "3. Show borrowed books\n";
                std::cout << "4. Back to main menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> choice;

                switch (choice) {
                    case 1: {
                        std::cout << "Enter ISBN of the book to borrow: ";
                        long long isbn;
                        std::cin >> isbn;
                        auto bookIt = std::find_if(books.begin(), books.end(), 
                                                    [isbn](Book* book) { return book->isbn == isbn; });
                        if (bookIt != books.end()) {
                            user->borrowBook(*bookIt);
                        } else {
                            std::cout << "Book not found.\n";
                        }
                        break;
                    }
                    case 2: {
                        std::cout << "Enter ISBN of the book to return: ";
                        long long isbn;
                        std::cin >> isbn;
                        auto bookIt = std::find_if(books.begin(), books.end(), 
                                                    [isbn](Book* book) { return book->isbn == isbn; });
                        if (bookIt != books.end()) {
                            user->returnBook(*bookIt);
                        } else {
                            std::cout << "Book not found.\n";
                        }
                        break;
                    }
                    case 3:
                        user->showBorrowedBooks();
                        break;
                    case 4:
                        std::cout << "Returning to main menu...\n";
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 4);
        } else {
            std::cout << "User '" << username << "' not found.\n";
        }
    }
};

int main() {
    Library library;

    // Initialize library with some books
    library.addBook("The Lord of the Rings", "J.R.R. Tolkien", 1234567890LL);
    library.addBook("Harry Potter and the Sorcerer's Stone", "J.K. Rowling", 9876543210LL);

    // Add some users
    library.addUser("Alice");
    library.addUser("Bob");

    int choice;
    do {
        std::cout << "\nLibrary Management System:\n";
        std::cout << "1. Add a book\n";
        std::cout << "2. Remove a book\n";
        std::cout << "3. Search for a book\n";
        std::cout << "4. Manage User\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string title, author;
                long long isbn;
                std::cout << "Enter book title: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                std::cout << "Enter book author: ";
                std::getline(std::cin, author);
                std::cout << "Enter book ISBN: ";
                std::cin >> isbn;
                library.addBook(title, author, isbn);
                break;
            }
            case 2: {
                std::cout << "Enter ISBN of the book to remove: ";
                long long isbn;
                std::cin >> isbn;
                auto it = std::remove_if(library.books.begin(), library.books.end(),
                                         [isbn](Book* book) { return book->isbn == isbn; });
                if (it != library.books.end()) {
                    delete *it;
                    library.books.erase(it, library.books.end());
                    std::cout << "Book removed successfully.\n";
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            case 3: {
                std::cout << "Enter ISBN of the book to search: ";
                long long isbn;
                std::cin >> isbn;
                auto it = std::find_if(library.books.begin(), library.books.end(),
                                       [isbn](Book* book) { return book->isbn == isbn; });
                if (it != library.books.end()) {
                    Book* book = *it;
                    std::cout << "Book found: " << book->title << " by " << book->author << std::endl;
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            case 4: {
                std::string username;
                std::cout << "Enter username: ";
                std::cin.ignore(); // Clear the buffer
                std::getline(std::cin, username);
                library.manageUser(username);
                break;
            }
            case 5:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    // Clean up dynamically allocated memory
    for (Book* book : library.books) {
        delete book;
    }
    for (User* user : library.users) {
        delete user;
    }

    return 0;
}