#pragma once

#include <string>
#include <vector>
#include <map>

/// The main Address Book implementation. Extend as required.

/// The basic structure of this class was not made by me. I was given the outline for it and defined the member functions in address_book.cpp. 
/// I created the alter and printSearchResults member functions, as well as the address_book_list map

class AddressBook
{
public:
    /// A container for address book data
    struct Entry
    {
        std::string first_name;
        std::string last_name;
        std::string phone_number;
    };

    /// Add an entry. Implement in address_book.cpp.
    void add(std::string first_name,std::string last_name="",std::string phone_number="");

    /// Remove an entry. Implement in address_book.cpp.
    void remove(const std::string&);

    /// Change the details of an entry.
    void alter(std::string);

    /// Print all entries from a specific search
    void printSearchResults(const std::string&, bool* is_empty);

    /// Return all entries sorted by first names. Implement in address_book.cpp.
    std::map<std::string,Entry> sortedByFirstName();

    /// Return all entries sorted by last names. Implement in address_book.cpp.
    std::map<std::string,Entry> sortedByLastName();

    /// Return all matching entries. Implement in address_book.cpp.
    std::map<std::string,Entry> find(std::string name);


private:
    // A map is used for the address book.
    // It's private to follow the customs of encapsulation,
    // ensuring it can only be altered or obtained through an object of this class.
    // Since names are used for keys, it works well for retrieval operations,
    // as the program can instantly access an entry using a name that the user provided.
    // This is a lot faster than sequentially searching through the address book,
    // which would have a large number of entries.
    // A slight downside to this however is the automatic alphabetical sorting
    // that a map does when a new entry is added, as this slightly reduces the speed of the program.
    std::map<std::string,Entry> address_book_list;
};
