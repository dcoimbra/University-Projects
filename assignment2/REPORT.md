# ES Report - Assignment 2

## 84708 - David Coimbra
## 84750 - Nuno Bombico

## Question 1

> Note that in the `FileSystemState` and the `FileStream` classes, all of the functions say they read `this`. Why is this important?

The *reading frame* of a function is all the memory locations that the function is allowed to read. Memory locations specified in
a "reads" clause are the only locations that may be read by the function, which means the clause essentially limits the set of 
memory locations that can be accessed by the function. This is done to prevent inconsistencies regarding the value of those functions
when writing to memory, so we can be sure that functions that did not read that part of memory have the same value they did before.
`reads this` means that the function is only able to read from the memory position that is associated with its own class instance, or object.

This is important because not only the function needs to be able to read information from its object (for example, `state()` from `FileSystemState` needs to be able to access information about files and their names in the current state of the file system) but
also restricts it from reading from other memory locations with information that doesn't pertain to its own object.

This way, we prevent inconsistencies from happening where state is modified and is no longer consistent with how the file system is
like in reality.

## Question 2

> Note that it isn’t possible to create new `FileSystemState` objects. What would problems might arise if this were possible?

`FileSystemState` objects exist only in the context of a global `HostEnvironment` which defines all environment variables the program may use. There is only one `FileSystemState` object in a single `HostEnvironment` context, and any new state is obtained solely by modifying
this existing state through open (create), and write operations.

This ensures that any change to the file system is properly verified and states are always consistent between each other.

If one were able to create new `FileSystemState` objects, it would be possible to bypass these verifications and create inconsistent states.
For example, one could write to a file, changing the previous state, and then replace the modified state with a newly created state. 


## Question 3

> Semantically, what does it mean if you add preconditions (requires) to the `Main` method?
Adding preconditions to `Main` defines a baseline/scope for what the program may verify down the line.
In a verification sense, it restricts what kind of input is allowed to be verified by the program, and it also 
places restrictions on environment variables in order to fully define the kind of environment the program works in.
In conclusion, preconditions in `Main` shape and define the environment setup the verification is allowed to run in, as well as
the inputs it is allowed to verify.


## Verifying Run-Length Encoding compression with Dafny
In our project, we develop a tool that is able to losslessly compress and decompress files, and make it a goal to verify that this tool indeed creates a smaller file for at least one given file and is also able to losslessly recover the original file.

### Defining lossless compression
Lossless compression is defined as a function where the result of decompressing a compressed input is the same as that input, i.e.,
the result of applying the decompression function on the compression function is the same as the identity function.

### The algorithm
In order to implement this functionality, we chose the `Run-Length Encoding` algorithm as it is simple to understand and implement, being
very good at compressing files that have a lot of continuous repeated information.

#### Defining RLE compression
For compressing, RLE simply searches for continuous sequences of the same byte (which we call `runs`) and substitutes each
run with a single instance of that byte and a counter for the number of bytes in that run.

For example, say you have the following string: `aaaaaabbbcccccccdddd`.
Then the result of compressing this string with RLE would be `a6b3c7d4`. In accordance with our implementation, that result is instead
`abcd06374` (with a 0 separating the compressed characters from the counter).

#### Defining RLE decompression
For decompressing, RLE applies the inverse procedure: it substitutes a (byte, counter) pair with the given byte repeated as much as the 
counter. So if you had `abcd06374` you would get `aaaaaabbbcccccccdddd`.

The result is the original, identical to the input, byte sequence.

### The results
After testing our tool with a PNG file depicting the IST logo, a BMP file with the words `Verification is fun!` and an entire textual
transcription of War and Peace by Leo Tolstoy, we managed to produce a smaller file for the BMP file. We didn't have any luck with the PNG
or TXT files as they have very few continuous repeated information (or none at all) and actually produced a bigger file for both of them. 

BMP files consist only of the RGB values for each pixel in the image, and as this particular file consisted of a lot of adjacent white or black pixels, RLE was very successful in compressing it.

Decompressing the compressed file was also easy enough for our tool as we managed to obtain the original `Verification is fun!` image
in about one second of run-time.

### Verifying the implementation with Dafny
Verification was attempted with Dafny by writing an iterative implementation that could efficiently get the job done, and specifying
a recursive function that describes what the tool is supposed to do. This is done for both compression and decompression.

A proof for the lossless property is attempted by writing a lemma that checks whether the result on calling decompress on compress is
indeed identical to the identity function.

