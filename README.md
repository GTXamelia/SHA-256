# SHA256 (Secure Hash Algorithm 256)
This is the SHA256 hashing algorithm which takes input data (files) and turns it into an output of a fixed length.
This SHA256 was written in C as part of my studies in GMIT in the Theory of Algorithms module.

## Setup and Usage

### Prerequisites

1. <a href="https://gcc.gnu.org/install/download.html">Install C compiler (GCC)</a>
2. <a href="https://github.com/cian2009/SHA-256/archive/master.zip">Clone repo</a>

### Running the Program
First compile the program using gcc

> gcc -o sha256 sha256.c .\sha256.h

This command will compile sha256.c and include the sha256.h header file into and executable (.exe)

#### Method 1
1. Open CMD in the project directory
2. Run the executable sha256.exe

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

This was a second option added as a convenience during development testing.

## Tests

### Padding Test 

To test padding of files I've used a small file containing only 3 character 'abc' the total byte number is 3.
With only 3 bytes padding will run and pad the remaining bytes with '0' to ensure the hash algorithm runs correctly.

EXPECTED OUTPUT: <img src="https://imgur.com/yiUzdBP.png">

<img src="https://imgur.com/3rfPcnW.png">

RESULT: Successful!

The sha256 algorithm successfully pads block lower than 64 bytes (Only 3 bytes in this case)

### Large File

To fully test the algorithm I will test a large file which will test to see if it is correctly adding the blocks of 64 bytes together and getting the expected hash at the end.

EXPECTED OUTPUT: <img src="https://imgur.com/nSmHX65.png">

<img src="https://imgur.com/pT3r8gO.png">

RESULT: Successful!

The algorithms successfully  runs through the large file and adds the block together to get the correct hash of the file.

### Wrong/No File Found

Test the program to see how it handles a user entering a file that doesn't exist.

EXPECTED OUTPUT: User told file was not found

<img src="https://imgur.com/HNCNsEW.png">

RESULT: Successful!

User is informed that the requested file to be hashed couldn't be hashed.

## Problems Encountered

### Endianess 
During early development it was found that the program was reading files in little endian format. This caused the hash to work, but not work correctly. The hash would return a string of correct length but due to reading little endian the hash would be wrong when compared to others. The <a href="https://www.nist.gov/publications/secure-hash-standard">SHA256 documentation</a> asks for the input to be in big endian value so it can be correctly hashed and tested to others to ensure correct hashing.

Fixing this issue took time to figure out which parts of the code needed to be converted to big endian, and how they would be achieved. First attempts were aimed at the file reader 'fopen' and 'fread' to see if there was any built in functionality that I could offload the work to. This failed and the next part was to research how and what needed to be converted. The latter part was easy the former took most of that time. Multiple articles and posts talking about endianness and how to solve it in both directions. A good source I found was from <a href="https://developer.ibm.com/articles/au-endianc/">IBM</a>, they talked about its effects and differences.

Once I had researched completely how to deal with endianness in C I was able to get the correct hash when comparing to examples online.

STATUS: FIXED

### Newline 
During development and testing I encountered and error when running the algorithm on Windows devices. When I looked further into the error specific to the OS, I found that files with a newline control character caused the hash to differ from the hash online and Linux. The specific character was in hex the '0D' which is followed by '0A' in Windows. This control character reffers to the 
<a href="https://stackoverflow.com/questions/1552749/difference-between-cr-lf-lf-and-cr-line-break-types">CRLF</a> which is the windows standard for newline. The first character (0D) is the carriage control character which is not used in Linux, Linux and mac OS use LF control character.

Below is an example of how a file is read when using CRLF:

<img src="https://i.imgur.com/7bBbnEN.png"></img>

The text file is:

1(31) space(20) 2(32) \r(0D) \n(0A)

3(33)

The hash algorithm had no problem adding the carriage to the hashing process but it caused the hash to differ from examples online.
This problem is Windows specific due to the fact it reads and writes newlines as \r\n (CRLF), while Linux used \n (LF). The carriage causes an issue when hashing a file and compairing to online examples online.

STATUS: Linux works / Windows fails on newline files due to CRLF

SOLUTION: Fixing the issue for Windows could be done by filtering the '0D' byte from the message block by skipping it when read. Or creat a temp file that is a copy of the original except for all carriage bytes are removed and use that to hash and then delete after hash. The first option would be more effieicent and less resource intense.

### Conclusion
The algorithm works as intended but has difficulty with how windows processes new lines.

## Extras

1. <a href="https://github.com/cian2009/SHA-256/blob/master/sha256.h">Header File</a> - The header file was used to clean up the main block of code in sha256.c by offloading basic function/macros, imports and constants.
2. <a href="https://docs.microsoft.com/en-us/windows/console/setconsoletextattribute">Colour</a> - A small UI improvement was added to the project by adding colour the CMD using the Console API.

## Tools
1. <a href="https://git-scm.com/">GIT</a> - Version control used during project development
2. <a href="https://github.com">GitHub</a> - Web-based hosting service for version control using Git
3. <a href="https://www.fileformat.info/tool/hash.htm">Hash Online</a> - Online hashing website
4. <a href="https://www.onlinehexeditor.com/">Online Hex Editor</a> - Online hex editor
5. <a href="https://gcc.gnu.org/">GCC</a> - C compiler

## Sources
1. <a href="https://github.com/cian2009/SHA-256/blob/master/project.pdf">Project Outline</a>
2. <a href="https://www.nist.gov/publications/secure-hash-standard">Secure Hash Standard</a>
3. <a href="http://www.open-std.org/jtc1/sc22/wg14/">ISO/IEC 9899 - Programming Languages - C</a>
4. <a href="https://developer.ibm.com/articles/au-endianc/">Writing Endian-Independent Code in C</a>
5. <a href="https://stackoverflow.com/questions/1552749/difference-between-cr-lf-lf-and-cr-line-break-types">Difference between CR LF, LF and CR</a>
6. <a href="https://www.w3schools.in/c-tutorial/c-header-files/">Header Files</a>
7. <a href="https://stackoverflow.com/questions/2182002/convert-big-endian-to-little-endian-in-c-without-using-provided-func">Swap Endian in C</a>
8. <a href="https://docs.microsoft.com/en-us/windows/console/console-functions">Console API</a>

## Built With

* [Visual Studio Code](https://code.visualstudio.com/) - IDE used during development
* [C](https://en.wikipedia.org/wiki/C_(programming_language)) - Programming laguage used
* [GCC](https://gcc.gnu.org/) -  GNU Compiler Collection which includes a C compiler

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details