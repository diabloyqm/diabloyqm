#include <iostream>
#include <unordered_map>
#include <string>
#include <iostream>
#include <unordered_set>
#include <iostream>
#include <vector>
using namespace std;


int main() {
    // std::vector<int> vec = {1, 2, 3, 4, 5};

    // // Obtain iterators to the beginning and end of the vector
    // std::vector<int>::iterator itBegin = vec.begin();
    // std::vector<int>::iterator itEnd = vec.end();

    // // Traverse and print elements
    // for (std::vector<int>::iterator it = itBegin; it != itEnd; ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;
    vector<vector<std::vector<bool> > > visited(2, vector<vector<bool>>(3, vector<bool>(1 << 4, false)));
    for(int i = 0; i < visited[0][0].size(); i++) {
        std::cout << visited[0][0][i] << " ";
    }
    std::cout <<  std::endl;
    return 0;

    
   
}
// int main() {
//     // Creating an unordered_multiset of integers
//     std::unordered_multiset<int> myUnorderedMultiSet;

//     // Inserting elements into the unordered_multiset
//     myUnorderedMultiSet.insert(30);
//     myUnorderedMultiSet.insert(10);
//     myUnorderedMultiSet.insert(50);
//     myUnorderedMultiSet.insert(20);
//     myUnorderedMultiSet.insert(50); // Multiple occurrences of the same value

//     // Checking if an element is present in the unordered_multiset
//     int searchValue = 20;
//     if (myUnorderedMultiSet.find(searchValue) != myUnorderedMultiSet.end()) {
//         std::cout << searchValue << " found in the unordered_multiset." << std::endl;
//     } else {
//         std::cout << searchValue << " not found in the unordered_multiset." << std::endl;
//     }

//     // Iterating through all elements in the unordered_multiset
//     std::cout << "All elements in the unordered_multiset:" << std::endl;
//     for (const auto& element : myUnorderedMultiSet) {
//         std::cout << element << std::endl;
//     }

//     return 0;
// }
// int main() {
//     // Create an unordered_multimap of string (key) to string (value)
//     std::unordered_multimap<std::string, std::string> authorToBooks;

//     // Inserting elements into the unordered_multimap
//     authorToBooks.insert({"A", "aa"});
//     authorToBooks.insert({"A", "aaa"});
//     authorToBooks.insert({"B", "bb"});
//     authorToBooks.insert({"B", "bbb"});

//     // Print all books by J.K. Rowling
//     std::cout << "A:" << std::endl;
//     auto range = authorToBooks.equal_range("A");
//     for (auto i = range.first; i != range.second; ++i) {
//         std::cout << " - " << i->second << std::endl;
//     }

//     // Erasing elements using the key
//     authorToBooks.erase("B");

//     // Print the updated list of authors and books
//     std::cout << "\nUpdated list of authors and books:" << std::endl;
//     for (const auto& pair : authorToBooks) {
//         std::cout << pair.first << ": " << pair.second << std::endl;
//     }

//     return 0;
// }
