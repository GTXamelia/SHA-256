# SHA256 (Secure Hash Algorithm 256)
This is the SHA256 hashing algorithm which takes input data (files) and turns it into an output of a fixed length.
This SHA256 was written in C as part of my studies in GMIT in the Theory of Algorithms module.

## Setup and Usage

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

Enter in the file you would like to hash and it will return a result:

<img src="https://imgur.com/Lv9E6so.png"></img>

Note the result will also who the number of bytes hashed this includes the padding.

#### Method 2
1. Open CMD in the project directory
2. run the executable .sha256 (FILE HERE)

The second method involves adding the file name as a launch argument:

<img src="https://imgur.com/bmVBszA.png"></img>

This was a second option added as a convience during development testing.

## Tests

### Padding Test 

To test padding of files I've used a small file containing only 3 character 'abc' the total byte number is 3.
With only 3 bytes padding will run and pad the remaining bytes with '0' to ensure the hash algorithm runs correctly.

EXPECTED OUTPUT: <img src="https://imgur.com/yiUzdBP.png">

<img src="https://imgur.com/3rfPcnW.png">

RESULT: Sucessfull

## Problems Encountered

### Endianess 
During early development it was found that the program was reading files in little endian format. This casued the hash to work, but not work correctly. The hash would return a string of correct length but due to reading little endian the hash would be wrong when compared to others. The <a href="https://www.nist.gov/publications/secure-hash-standard">SHA256 documentation</a> asks for the input to be in big endian value so it can be corectly hashed and tested to others to ensure correct hashing.

Fixing this issue took time to figure out which parts of the code needed to be converted to big endian, and how they would be achieved. First attempts were aimed at the file reader 'fopen' and 'fread' to see if there was any built in functionality that I could offload the work to. This failed and the next part was to research how and what needed to be converted. The latter part was easy the former took most of that time. Multiple articles and posts talking about endianness and how to solve it in both directions. A good source I found was from <a href="https://developer.ibm.com/articles/au-endianc/">IBM</a>, they talked about its affects and differences.

Once I had researched completely how to deal with endianness in C I was able to get the correct hash when comparing to examples online.

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

1. <a href="https://github.com/cian2009/SHA-256/blob/master/sha256.h">Header File</a> - The header file was used to clean up the main block of code in sha256.c by offloading basic function/macros, imports and constanats.
2. <a href="https://docs.microsoft.com/en-us/windows/console/setconsoletextattribute">Colour</a> - A small UI imporvment was added to the project by adding colour the CMD using the Console API.

## Sources
1. <a href="https://github.com/cian2009/SHA-256/blob/master/project.pdf">Project Outline</a>
2. <a href="https://www.nist.gov/publications/secure-hash-standard">Secure Hash Standard</a>
3. <a href="http://www.open-std.org/jtc1/sc22/wg14/">ISO/IEC 9899 - Programming Languages - C</a>
4. <a href="https://developer.ibm.com/articles/au-endianc/">Writing Endian-Independent Code in C</a>
5. <a href="https://stackoverflow.com/questions/1552749/difference-between-cr-lf-lf-and-cr-line-break-types">Difference between CR LF, LF and CR</a>
6. <a href="https://www.w3schools.in/c-tutorial/c-header-files/">Header Files</a>
7. <a href="https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func">Swap Endian in C</a>