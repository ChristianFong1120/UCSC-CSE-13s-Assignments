## Assignment 7: The Great Firewall of Santa Cruz                                                                                                                                                                    

### Functionality
The purpose of this program is to implement a firewall to filter out words that are deemed inappropriate. We will catch bad words, and offer better words to use instead. We will store the words to be flagged in a bloom filter and hash table. Our bloom filter is constructed of a bitvector, and our hash table is constructed as an array of binary search trees. We use these structures along with hashing due to its O(1) time complexity, offering us instant and constant lookup and insertion times.

### Building
The program can be compiled with the command "make" courtesy of the included makefile. Make compiles the program with the no prisoner flags: -Wall -Wpedantic -Werror -Wextra. After creation of the binary object file banhammer. They can be executed with: ./<executable name>  <command line arguments>.

### Command Line Arguments
./Banhammer\
 -h           Program usage and help.\
 -s           Print program statistics.\
 -t size      Specify hash table size (default: 10000).\
 -f size      Specify Bloom filter size (default: 2^20).\

