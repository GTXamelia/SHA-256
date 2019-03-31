# SHA256 (Secure Hash Algorithm 256)
This is the SHA256 hashing algorithm which takes input data (files) and turns it into an output of a fixed length.
This SHA256 was written in C as part of my studies in GMIT in the Theory of Algorithms module.

## Setup

### Prerequisites

1. <a href="https://gcc.gnu.org/install/download.html">Install gcc</a>
2. <a href="https://github.com/cian2009/SHA-256/archive/master.zip">Clone repo</a>

### Running the Program
First compile the program using gcc

> gcc -o sha256 sha256.c .\sha256.h

This command will compile sha256.c and include the sha256.h header file into and exectuable (.exe)

#### Method 1
1. Open CMD in the project directory
2. run the executable .sha256

You will then see this screen prompting the user to enter the file they would like to get the hash for:

<img src="https://imgur.com/rKpW5sM.png"></img>



#### Method 2

## Tests

### Windows (Windows 7 & 10)

### Linux (Ubuntu)

## Problems & Solutions

### Newline 
During development and testing I encounted and error when running the algorithm on Windows devices. When I looked further into the error specific to the OS, I found that files with a newline control character caused the hash to differ from the hash online and linux. The specific character was in hex the '0D' which is followed by '0A' in Windows. This control character reffers to the 
<a href="https://stackoverflow.com/questions/1552749/difference-between-cr-lf-lf-and-cr-line-break-types">CRLF</a> which is the windows standard for newline. The first character (0D) is the carriage control character which is not used in linux, linux and mac OS use LF control character.

Below is an example of how a file is read when using CRLF:

<img src="https://i.imgur.com/7bBbnEN.png"></img>

The text file is:

1(31) space(20) 2(32) \r(0D) \n(0A)

3(33)

The hash algorithm had no problem adding the carriage to the hashing process but it caused the hash to differ from examples online.

## Extras

## Sources