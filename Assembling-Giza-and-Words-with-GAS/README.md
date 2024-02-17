# Assembling Giza and Words with GAS

This project contains two simple programs, giza.s which takes a single command line argument, the number of lines of a pyramid to print, constructed of "*" and prints out Finito at the end. The other program is wc.s which takes no arguments, it read from standard input until it sees the end of the file. It counts the number of words in the input stream and prints "Result=x", where x is the number of words counted.

## How to run:

<b> System requirements: Linux 64 bit platform, and use GNU Linux tools. Can use 64 bit linux, Windows machine running a 64 bit WSL, etc... </b>

### giza.s:

<b>Compile with:</b>
<pre> make giza </pre>

<b>To run:</b>
<pre>./giza [# lines]</pre>

### wc.s:

<b>Compile with:</b>
<pre> make wc </pre>

<b>To run:</b>
<pre>./wc < [file name] </pre>

## Examples

<b>Compile:</b> make giza <br/>
<b>Run:</b> ./giza 7 <br/>
<b>Result:</b>
<pre>*
**
***
****
*****
******
*******
Finito </pre>

<b>Compile:</b> make wc <br/>
<b>Run:</b> ./wc < hamlet.txt <br/>
<b>Result:</b> Result: <br/>
<pre> Result=10 </pre>
  
  


