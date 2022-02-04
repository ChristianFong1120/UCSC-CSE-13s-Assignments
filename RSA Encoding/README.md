### Functionality
The purpose of this program is to encrypt and decrypt a given file using the RSA algorithm. Any user can encrypt a file given a set of public keys, but only the holder of the private keys can decrypt them. This algorithm makes use of modular inverses, modular exponentiation, as well as probablistic test of primality.

### Building
The program can be compiled with the command "make" courtesy of the included makefile. Make compiles the program with the no prisoner flags: -Wall -Wpedantic -Werror -Wextra. After creation of the binary object files of keygen, encrypt and decrypt. They can be executed with: ./<executable name>  <command line arguments>.

### Command Line Arguments
Keygen\
-h             Display program help and usage.\
-v             Display verbose program output.\
-b bits        Minimum bits needed for public key n (default: 256)\
-i confidence  Miller-Rabin iterations for testing primes (default: 50)\
-n pbfile     Public key file (default: rsa.pub).\
-d pvfile      Private key file (default: rsa.priv).\
-s seed        Random seed for testing.

Encrypt\
-h             Display program help and usage.\
-v             Display verbose program output.\
-i infile      Input file of data to encrypt (default: stdin).\
-o outfile     Output file for encrypted data (default: stdout).\
-n pbfile      Public key file (default: rsa.pub).

Decrypt\
-h             Display program help and usage.\
-v             Display verbose program output.\
-i infile      Input file of data to decrypt (default: stdin).\
-o outfile     Output file for decrypted data (default: stdout).\
-n pvfile      Private key file (default: rsa.priv).

