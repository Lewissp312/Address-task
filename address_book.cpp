#include "include/address_book.h"
#include <stdexcept>
#include <iostream>
#include <regex>

void AddressBook::add(std::string first_name,std::string last_name,std::string phone_number) {
    // Regex is used to remove all spaces in the first and last names.
    // This ensures neatly formatted keys as well as catching any blank spaces for first names
    first_name = std::regex_replace(first_name, std::regex("\\s+"), "");
    last_name = std::regex_replace(last_name, std::regex("\\s+"), "");
    while (first_name.empty()) {
        // The user must provide a name that is not blank
        std::cout << "Please enter a valid first name" << std::endl;
        // getline is used throughout the program as it takes spaces as part of the input,
        // as opposed to cin which separates an input by spaces and treats them as separate inputs
        std::getline(std::cin,first_name);
        first_name = std::regex_replace(first_name, std::regex("\\s+"), "");
    }
    std::string full_name;
    // Ensures that a key consisting of just a first name does not have a space on the end
    if (last_name.empty()) {
        full_name = first_name;
    } else {
        full_name = first_name + " " + last_name;
    }
    try{
        // .at() can be used to instantly check the entry associated with a certain key. 
        // This avoids having to search through the list sequentially for the name, 
        // which would not be efficient for a large list.
        this->address_book_list.at(full_name);
        std::cout << "This entry already exists" << std::endl;
    } catch(std::exception& ex){
        // Catches the exception thrown by .at() if the name is not present in the address book
        Entry new_entry;
        new_entry.first_name = first_name;
        new_entry.last_name = last_name;
        new_entry.phone_number = phone_number;
        this->address_book_list.insert(std::pair<std::string, Entry>(full_name, new_entry));
        std::cout << full_name << " successfully added" << std::endl;
    }
}

void AddressBook::remove(const std::string& entry_to_remove)
{
    // Checks to see if the user entered a blank space or if the address book is empty.
    // If either is true, the proceeding code is ignored as it won't yield any results
    if(std::regex_replace(entry_to_remove, std::regex("\\s+"), "").empty()
    || this->address_book_list.empty()){
        std::cout << "No matching entries for that name" << std::endl;
    } else{
        try{
            // The entry can be removed immediately if the user enters the exact key for that entry,
            // speeding up the removal process.
            // However, if the second name is blank, then an exception is thrown.
            // This is done because an entry that only has a first name and no second name may share
            // that first name with other entries. If this check was not done, then only that entry would
            // be found here. For example, if an entry had just "Daniel" as a first name and no second name,
            // only that entry would be returned, while ignoring all the other "Daniel"s with second names
            std::exception ex;
            if (this->address_book_list.at(entry_to_remove).last_name.empty()){
                throw ex;
            }
            this->address_book_list.erase(entry_to_remove);
            std::cout << "Entry successfully removed" << std::endl;
        } catch(std::exception& ex){
            bool is_empty;
            is_empty = false;
            // The user is presented with a series of options to remove based on their search
            // if it didn't match a singular entry, making it easier to remove entries
            this->printSearchResults(entry_to_remove,&is_empty);
            // Proceeding code is ignored (is_empty is changed to true) if there are no search results.
            // The user is informed of this in printSearchResults
            if (!is_empty){
                std::cout << "Please select an entry to be removed by typing \"*first name* *last name*\" " <<
                          "(or \"*first name*\" if there's no last name) " <<
                          "or type \"Q\" to quit and return to the menu" <<
                          std::endl;
                bool quit = false;
                std::string entry_choice;
                std::getline(std::cin,entry_choice);
                // Gives the user a chance to go back to the menu so they aren't forced to remove an entry
                if (entry_choice == "Q"){
                    quit = true;
                }
                while (!quit){
                    try{
                        this->address_book_list.at(entry_choice);
                        this->address_book_list.erase(entry_choice);
                        std::cout << "Entry successfully removed" << std::endl;
                        quit = true;
                    } catch (std::exception& ex){
                        std::cout << "Please select an entry to be removed by typing \"*first name* *last name*\" " <<
                                  "(or \"*first name*\" if there's no last name) " <<
                                  "or type \"Q\" to quit and return to the menu" <<
                                  std::endl;
                        std::getline(std::cin,entry_choice);
                        if (entry_choice == "Q"){
                            quit = true;
                        }
                    }
                }
            }
        }
    }
}

void AddressBook::alter(std::string entry_to_alter) {
    if(std::regex_replace(entry_to_alter, std::regex("\\s+"), "").empty()
    || this->address_book_list.empty()) {
        std::cout << "No matching entries for that name" << std::endl;
    } else{
        bool verified_entry = false;
        try {
            std::exception ex;
            if (this->address_book_list.at(entry_to_alter).last_name.empty()){
                throw ex;
            }
            verified_entry = true;
        } catch(std::exception& ex){
            bool is_empty;
            is_empty = false;
            this->printSearchResults(entry_to_alter,&is_empty);
            if (!is_empty){
                std::cout << "Please select an entry to be altered by typing \"*first name* *last name*\" " <<
                             "(or \"*first name*\" if there's no last name) " <<
                             "or type \"Q\" to quit and return to the menu" <<
                             std::endl;
                bool quit = false;
                std::string user_choice;
                std::getline(std::cin,user_choice);
                if (user_choice == "Q"){
                    quit = true;
                }
                while (!quit){
                    try{
                        this->address_book_list.at(user_choice);
                        verified_entry = true;
                        // entry_to_alter changed to user_choice as the previous value did not match an entry
                        entry_to_alter = user_choice;
                        quit = true;
                    } catch (std::exception& ex){
                        std::cout << "Please select an entry to be altered by typing \"*first name* *last name*\" " <<
                                  "(or \"*first name*\" if there's no last name) " <<
                                  "or type \"Q\" to quit and return to the menu" <<
                                  std::endl;
                        std::getline(std::cin,user_choice);
                        if (user_choice == "Q"){
                            quit = true;
                        }
                    }
                }
            }
        }
        if (verified_entry){
            verified_entry = false;
            std::string user_choice;
            // Default values for all the altered information
            std::string new_first_name = address_book_list.at(entry_to_alter).first_name;
            std::string new_last_name = address_book_list.at(entry_to_alter).last_name;
            std::string new_phone_number = address_book_list.at(entry_to_alter).phone_number;
            std::string new_full_name = entry_to_alter;
            while(!verified_entry){
                std::cout << "What would you like to do? (1,2,3,4) \n "
                             "1.) Alter the first name (currently not working, details in code) \n " <<
                             "2.) Alter the last name (currently not working, details in code) \n " <<
                             "3.) Alter the phone number \n " <<
                             "4.) Quit and save the new edits "<< std::endl;
                std::getline(std::cin,user_choice);
                while (user_choice != "1" && user_choice != "2" && user_choice != "3" && user_choice != "4"){
                    std::cout << "Please choose one of the options (1,2,3,4)" << std::endl;
                    std::getline(std::cin,user_choice);
                }
                // Converts user's choice to an integer
                switch(std::stoi(user_choice)){
                    case 1:
                        std::cout << "Please enter the new first name" << std::endl;
                        std::getline(std::cin,new_first_name);
                        while (std::regex_replace(new_first_name, std::regex("\\s+"), "").empty()){
                            std::cout << "This is not valid, please enter the new first name" << std::endl;
                            std::getline(std::cin,new_first_name);
                        }
                        new_first_name = std::regex_replace(new_first_name, std::regex("\\s+"), "");
                        std::cout << "New details stored" << std::endl;
                        break;
                    case 2:
                        std::cout << "Please enter the new last name (optional)" << std::endl;
                        std::getline(std::cin,new_last_name);
                        std::cout << "New details stored" << std::endl;
                        new_last_name = std::regex_replace(new_last_name, std::regex("\\s+"), "");
                        break;

                    case 3:
                        std::cout << "Please enter the new phone number (optional)" << std::endl;
                        std::getline(std::cin,new_phone_number);
                        std::cout << "New details stored" << std::endl;
                        break;

                    case 4:
                        // If the new first and last names are the same as the original ones
                        if (new_first_name == this->address_book_list.at(entry_to_alter).first_name
                        && new_last_name == this->address_book_list.at(entry_to_alter).last_name){
                            this->address_book_list.at(entry_to_alter).phone_number = new_phone_number;
                            std::cout << "Details successfully changed" << std::endl;
                            verified_entry = true;
                        } else{
                            if (new_last_name.empty()){
                                new_full_name = new_first_name;
                            } else{
                                new_full_name = new_first_name + " " + new_last_name;
                            }
                            try{
                                this->address_book_list.at(new_full_name);
                                std::cout << "This entry already exists, please change the first or last name"
                                << std::endl;
                            } catch (std::exception& ex){
                                // If I was able to use later versions of C++ here, I would use the map's
                                // extract member function to change the key of the entry to the new full name.
                                // If the key is not changed, there is no point to changing the first and last names
                                address_book_list.at(entry_to_alter).phone_number = new_phone_number;
                                //address_book_list.at(entry_to_alter).first_name = new_first_name;
                                //address_book_list.at(entry_to_alter).last_name = new_last_name;
                                //address_book_list.extract(entry_to_alter).key() = new_full_name;
                                std::cout << "Details successfully changed " <<
                                             "(first and last names not changed)" <<
                                             std::endl;
                                verified_entry=true;
                            }
                        }
                }
            }

        }
    }
}

std::map<std::string,AddressBook::Entry> AddressBook::sortedByFirstName()
{
    // Maps are automatically sorted alphabetically, so there is no need for adjustment
    return this->address_book_list;
}

std::map<std::string,AddressBook::Entry> AddressBook::sortedByLastName()
{
    // Because of this automatic sorting however, a separate map must be created for sorting by last name.
    // This would not be ideal for a large number of entries
    std::map<std::string,Entry> reversed_map;
    for (std::pair<std::string,Entry> i : this->address_book_list){
        if (i.second.last_name.empty()){
            // Entries with a blank last name are sorted by just their first name
            reversed_map.insert(std::pair<std::string,Entry>(i.second.first_name,i.second));
        } else{
            reversed_map.insert(std::pair<std::string,Entry>(i.second.last_name + " " + i.second.first_name,i.second));
        }
    }
    return reversed_map;
}

std::map<std::string,AddressBook::Entry> AddressBook::find(std::string name)
{
    std::map<std::string,Entry> matches_map;
    if(std::regex_replace(name, std::regex("\\s+"), "").empty()
           || this->address_book_list.empty()) {
        // Returns an empty map. The user is informed its empty in printSearchResults
        return matches_map;
    }
    try{
        // The desired entry can be returned immediately if the user enters the exact key for that entry
        // (except if the last name is blank).
        // This process could be improved with the previously mentioned extract member function,
        // as it could be used to find the key quicker
        std::exception ex;
        if (this->address_book_list.at(name).last_name.empty()){
            throw ex;
        }
        std::string first_name = this->address_book_list.at(name).first_name;
        std::string last_name = this->address_book_list.at(name).last_name;
        matches_map.insert(std::pair<std::string,Entry>(first_name + " " + last_name,this->address_book_list.at(name)));
    } catch(std::exception& ex){
        // Removing spaces from the user's search
        name = std::regex_replace(name, std::regex("\\s+"), "");
        std::string lower_case_name = name;
        // Creating a copy of the user's search but in lower case
        for (int i = 0 ; i<name.length() ; i++){
            lower_case_name[i] = std::tolower(name[i]);
        }
        // This pattern will produce a match if the start of the name is equal to what the user searched.
        // It is the name followed by any number of characters
        std::regex pattern = std::regex(lower_case_name + ".*");
        std::string lower_case_first_name;
        for (std::pair<std::string,Entry> i : this->address_book_list){
            lower_case_first_name = i.second.first_name;
            // Creating a copy of an entry's first name in lower case
            for (int k = 0 ; k<i.second.first_name.length() ; k++){
                lower_case_first_name[k] = std::tolower(i.second.first_name[k]);
            }
            // Compares the lower case first name against the pattern.
            // This is quicker and neater than sequentially searching through the string for a match in a for loop
            if (std::regex_match(lower_case_first_name,pattern)){
                matches_map.insert(i);
            // The proceeding code is ignored if the second name is a blank string,
            // as there's no point in checking it
            } else if (!i.second.last_name.empty()){
                std::string lower_case_last_name;
                lower_case_last_name = i.second.last_name;
                for (int k = 0 ; k<i.second.last_name.length() ; k++){
                    lower_case_last_name[k] = std::tolower(i.second.last_name[k]);
                }
                if (std::regex_match(lower_case_last_name,pattern)){
                    matches_map.insert(i);
                }
            }
        }
    }
    return matches_map;
}

void AddressBook::printSearchResults(const std::string& name, bool* is_empty) {
    std::map<std::string, Entry> search_results;
    search_results = this->find(name);
    if (search_results.empty()) {
        std::cout << "No matching entries for that name" << std::endl;
        // If this value is made true, then most member functions will ignore
        // their proceeding code
        *is_empty = true;
    } else {
        std::cout << "Here are the matching entries: " << std::endl;
        for (std::pair<std::string, AddressBook::Entry> i: search_results) {
            std::cout << "First name: " << i.second.first_name <<
                      " / Last name: " << i.second.last_name <<
                      " / Phone number: " << i.second.phone_number << std::endl;
        }
    }
}

// A menu that provides a user-friendly way to interact with the address book.
// This menu is on a loop and the program will only stop once the user makes it stop.
// This is a temporary way to retain the information stored in the address book, as it is not permanently saved.
// If given more time, it would be ideal to serialise the map that stores the address book.
// This would make any changes permanent.
// Instead of making a new map, the AddressBook constructor would deserialize the existing map.

int main(){
    bool quit = false;
    AddressBook addressBook;
    std::string menu_choice;
    while (!quit){
        std::cout << "\nWhat would you like to do? (1,2,3,4,5,6,7) \n " <<
                  "1.) Add an entry to the address book \n " <<
                  "2.) Remove an entry from the address book \n " <<
                  "3.) Alter an entry in the address book \n " <<
                  "4.) Get the list in alphabetical order (by first name) \n " <<
                  "5.) Get the list in alphabetical order (by last name) \n " <<
                  "6.) Find an entry in the address book \n " <<
                  "7.) Quit" << std::endl;
        std::getline(std::cin,menu_choice);
        while(menu_choice != "1" && menu_choice != "2" && menu_choice != "3" && menu_choice != "4"
        && menu_choice != "5" && menu_choice != "6" && menu_choice != "7"){
            std::cout << "Please choose one of the options (1,2,3,4,5,6,7)" << std::endl;
            std::getline(std::cin,menu_choice);
        }
        switch (std::stoi(menu_choice)) {
            case 1:
            {
                std::string first_name;
                std::cout << "Please enter the first name" << std::endl;
                std::getline(std::cin,first_name);
                std::string last_name;
                std::cout << "Please enter the last name (optional)" << std::endl;
                std::getline(std::cin,last_name);
                std::string phone_number;
                std::cout << "Please enter the phone number (optional)" << std::endl;
                std::getline(std::cin,phone_number);
                addressBook.add(first_name,last_name,phone_number);
            }
                break;

            case 2:
            {
                std::string entry_to_remove;
                std::cout << "Please enter a name" << std::endl;
                std::getline(std::cin,entry_to_remove);
                addressBook.remove(entry_to_remove);
            }
                break;

            case 3:
            {
                std::string entry_to_alter;
                std::cout << "Please enter a name" << std::endl;
                std::getline(std::cin,entry_to_alter);
                addressBook.alter(entry_to_alter);
            }
                break;

            case 4:
            {
                std::map<std::string,AddressBook::Entry> first_name_order;
                first_name_order = addressBook.sortedByFirstName();
                if (first_name_order.empty()){
                    std::cout << "The address book is currently empty" << std::endl;
                } else{
                    std::cout << "The address book organised by first name: " << std::endl;
                    for (std::pair<std::string,AddressBook::Entry> i : first_name_order){
                        if (i.second.last_name.empty()){
                            std::cout << "Name: " << i.second.first_name <<
                                      " / Phone number: " << i.second.phone_number << std::endl;
                        } else{
                            std::cout << "First name: " << i.second.first_name <<
                                      " / Last name: " << i.second.last_name <<
                                      " / Phone number: " << i.second.phone_number << std::endl;
                        }
                    }
                }
            }
                break;

            case 5:
            {
                std::map<std::string,AddressBook::Entry> last_name_order;
                last_name_order = addressBook.sortedByLastName();
                if (last_name_order.empty()){
                    std::cout << "The address book is currently empty" << std::endl;
                } else{
                    std::cout << "The address book organised by last name: " << std::endl;
                    for (std::pair<std::string,AddressBook::Entry> i : last_name_order){
                        if (i.second.last_name.empty()){
                            std::cout << "Name: " << i.second.first_name <<
                                      " / Phone number: " << i.second.phone_number << std::endl;
                        } else{
                            std::cout << "Last name: " << i.second.last_name <<
                                      " / First name: " << i.second.first_name <<
                                      " / Phone number: " << i.second.phone_number << std::endl;
                        }
                    }
                }
            }
                break;

            case 6:
            {
                std::string name;
                std::map<std::string, AddressBook::Entry> search_results;
                std::cout << "Please enter a name" << std::endl;
                std::getline(std::cin,name);
                bool is_empty = false;
                addressBook.printSearchResults(name,&is_empty);
            }
                break;

            case 7:
            {
                quit = true;
            }
                break;

        }
    }
    return 0;
}
