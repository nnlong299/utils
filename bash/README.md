# Let's get started!

## Linux commands:

### Easy (gotta know)
- <span style="color:yellow">man</span> (<span style="color:blue">man</span>ual) : manual user guide
> **_NOTE:_**  Please do read the guide before using anything, otherwise stop blaming
- <span style="color:yellow">cd</span> (<span style="color:blue">c</span>hange <span style="color:blue">d</span>irectory)
- <span style="color:yellow">ls</span> (<span style="color:blue">l</span>i<span style="color:blue">s</span>t)
- <span style="color:yellow">cp</span> (<span style="color:blue">c</span>o<span style="color:blue">p</span>y)
- <span style="color:yellow">mv</span>(<span style="color:blue">m</span>o<span style="color:blue">v</span>e) : to move/remove file(s)
> **_NOTE:_**  Everything in Linux is file (regular file, directory, etc.)
- <span style="color:yellow">rm</span> : <span style="color:blue">r</span>e<span style="color:blue">m</span>ove file(s)
- <span style="color:yellow">pwd</span> : <span style="color:blue">p</span>rint name of current/<span style="color:blue">w</span>orking <span style="color:blue">d</span>irectory
- <span style="color:yellow">cat</span> : concatenate files and print on the standard output
- <span style="color:yellow">grep</span> (<span style="color:blue">g</span>lobal <span style="color:blue">r</span>egular <span style="color:blue">e</span>xpression <span style="color:blue">p</span>rint) : print lines that match patterns
- <span style="color:yellow">echo</span> : display a line of text
- <span style="color:yellow">ps</span>: proc stat
- <span style="color:yellow">top</span>/htop: like ps but adding more animation like Window Task Manager
- <span style="color:yellow">kill</span> : kill a process by PID
### Medium (could know)
- <span style="color:yellow">wget</span> : download file(s) and only support HTTP, HTTPS and FTP
- <span style="color:yellow">curl</span> : download file(s) and supports FTP, FTPS, Gopher, HTTP, HTTPS, SCP, SFTP, TFTP, TELNET, DICT, LDAP, LDAPS, FILE, POP3, IMAP, SMB/CIFS, SMTP, RTMP and RTSP
> **_NOTE:_**  I copied the info from internet, don't ask me how comes I know
- <span style="color:yellow">ssh</span> (Secure Shell or Secure Socket Shell) : to let you access to another computer and work on CLI (Command Line Interface)
- <span style="color:yellow">scp</span> : is a friend of ssh, using ssh connection to transfer the file
- <span style="color:yellow">sort</span>
- <span style="color:yellow">uniq</span>
- <span style="color:yellow">wc</span> : word count
- <span style="color:yellow">df</span> : disk free
- <span style="color:yellow">du</span> : disk usage
- <span style="color:yellow">ln</span> : create shortcut to a file
- <span style="color:yellow">diff</span>
- <span style="color:yellow">tar</span>/<span style="color:yellow">zip</span>/<span style="color:yellow">unzip</span>/<span style="color:yellow">7za</span>
- <span style="color:yellow">less</span>/<span style="color:yellow">more</span> : display the contents of a file in a terminal and <span style="color:green">let you interact without messing the current terminal</span>. It means that the output of less/more will be disappeared after exitting
- <span style="color:yellow">tail</span>/<span style="color:yellow">head</span> : display a number of lines in head/tail of file, 
> **_NOTE:_** yes, it's like 'cat', but 'cat' displays all content of file which could mess up your terminal
- <span style="color:yellow">clear</span> : to clear the messy terminal

### Hard (good to know)

- <span style="color:yellow">xargs</span> : build and execute command lines from standard input
- <span style="color:yellow">cut</span>
- <span style="color:yellow">column</span>
- <span style="color:yellow">sed</span> (<span style="color:blue">s</span>tream <span style="color:blue">ed</span>itor) : for filtering and transforming text
- <span style="color:yellow">awk</span> (by <span style="color:red">A</span>ho, <span style="color:red">W</span>einberger, <span style="color:red">K</span>ernighan)

## Linux command examples:
Let's combine the command to get wonderful work. Your new friends now are 

> https://explainshell.com/

> https://chatgpt.com/

1. Multiple patterns grep
> git ls-files | grep '\\.cpp\|\\.h'

or

> git ls-files  | grep -e '\\.cpp' -e '\\.h'

2. To get all .cpp and .h, then count how many line the file has
> $ git ls-files  | grep -e '\\.cpp' -e '\\.h'  | xargs wc -l | sort -n

3. Remove trailing space:
> $ git ls-files  | grep -e '\\.cpp' -e '\\.h' |xargs sed -ri 's/\\s+\$//'

4. Kill process if found
> $ ps aux | grep processAAA | awk '{print $1}' | xargs kill -9

5. Count how many time the pattern appears in .xml file in current directory
> $ grep aaa *.xml | uniq -c

6. disk free check
> df -h | grep home

7. disk usage check (only recursive 1 level)
> du -h --max-depth=1

8. Replace the pattern AAA to BBB in all files in current and sub directory
> grep -rl AAA | xargs sed -ri 's/AAA/BBB/'

## Regex (Regular Expression):
New friend https://regex101.com/

# Shell scripting
## What is Linux shell
1. is a special user program that provides an interface for the user to use operating system services.
2. Shell accepts human-readable commands from users and converts them into something which the kernel can understand
![Linux kernel & shell](./img/linux_org.png)

## Type of shell:
1. BASH (Bourne Again SHell) It is the most widely used shell in Linux systems
2. CSH (C SHell) – The C shell’s syntax and its usage are very similar to the C programming language.
3. KSH (Korn SHell) – The Korn Shell was also the base for the POSIX Shell standard specifications etc.

> **_NOTE:_**  We use Terminal to interact with shell

## Shell scripting:
1. is set of command which is organized on purpose
2. any commands you use, you can put in script
3. to do automation tasks
> **_NOTE:_** Scripting is large and nessessary for human, we do have high level scripting like Python, but Shell is integrated in every single system, but not Python

## How script looks like
> #!/bin/bash<br>echo "Hello world"

(hello.sh)

above script has 2 lines:
1. first line for <span style="color:yellow">shebang</span> which tells the executer that please use 'bash' to execute my script, otherwise executer will take your current shell type to do instead and could cause the syntax error among shells.
2. second line is to print out text to console

## How to execute
> $ sh hello.sh

or

> $ chmod 755 hello.sh<br>\$ ./hello.sh<br>Hello world

the script can run under background by putting '&' at the end

> $ sh hello.sh &<br>[1] 123

## Comment
> #!/bin/bash<br>
#i'm human, i comment for complex code<br>
echo "Hello world"<br>

(hello.sh)

## Variable
You can assign the value to a variable by

> $ ABC="123 dzo 123 dzo 23 uong"

> **_NOTE:_**  Do not add more white space before and after '='

or a return value of a command by

> $ ABC=\$(ls)<br>
> \$ ABC=\$(cat)

<span style="color:red">Variable scrope</span> : global scope by default, define before use

> #!/bin/bash<br>
function hello<br>
{<br>
    &nbsp;&nbsp;&nbsp;&nbsp; A="AAA"<br>
    &nbsp;&nbsp;&nbsp;&nbsp; echo "Hello world"<br>
}<br>
hello<br>
echo $A<br>

output
>Hello world<br>
AAA

although you see that A is defined within hello(), but :)))

if you mean var A only in hello(), add <span style="color:yellow">local</span> 

> #!/bin/bash<br>
function hello<br>
{<br>
    &nbsp;&nbsp;&nbsp;&nbsp; <span style="color:yellow">local</span>  A="AAA"<br>
    &nbsp;&nbsp;&nbsp;&nbsp; echo "Hello world"<br>
}<br>
hello<br>
echo $A<br>

output
>Hello world<br>
#a blank line here because there is not var A defined yet

## IF statement

> if [ condition-is-true ]<br>
then<br>
&nbsp;&nbsp;&nbsp;&nbsp; command 1<br>
&nbsp;&nbsp;&nbsp;&nbsp; command 2<br>
elif [ condition-is-true ]<br>
then<br>
&nbsp;&nbsp;&nbsp;&nbsp; command 3<br>
else<br>
&nbsp;&nbsp;&nbsp;&nbsp; command 4<br>
fi

> **_NOTE:_**  Add one more white space after '[' and before ']'

New friend comes bro: https://devhints.io/bash

## FOR statement

> for val in A B C<br>
do<br>
&nbsp;&nbsp;&nbsp;&nbsp; echo $val<br>
done

## Argument passed in

> $ sh hello.sh Kitty Poro

To access 1st arg -> $1

To access 2nd arg -> $2

Max access is $9

Print all args -> $@

## Exit code

from 0-255

0 is OK

[1-255] is NOK

To get exit code of last executed command
> $ echo \$?

## Function (<span style="color:red">define before use</span>) 

### How it looks like

> function foo()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;  foo1<br>
&nbsp;&nbsp;&nbsp;&nbsp;  foo2<br>
}<br>

or 
> bar()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;  vui vay thoi chu bar dan con nay 1<br>
&nbsp;&nbsp;&nbsp;&nbsp;  vui vay thoi chu bar dan con nay 2<br>
}<br>

### Call a function
> function foo()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;  foo1<br>
&nbsp;&nbsp;&nbsp;&nbsp;  foo2<br>
}<br>
<br>
#without function specifier, here is main entry<br>
#without parameter<br>
foo<br><br>
#with parameter<br>
foo arg1 arg2<br>

### Function exit code
go with <span style="color:yellow">return</span> 

> function foo()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;  foo1<br>
&nbsp;&nbsp;&nbsp;&nbsp;  if [ condition-is-true ]<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  return 0<br>
&nbsp;&nbsp;&nbsp;&nbsp;  else <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  return 1<br>
}<br>
<br>
#without function specifier, here is main entry<br>
#without parameter<br>
foo<br><br>
#print exit code<br>
echo $?
