# Minishell
 
INDENTIFIER EXPLANATION:
\
[x] everything else = -9\
[x] forced redirection ('>|') = -7  &emsp;&emsp;&emsp;Cast an error because its not manditory.\
[x] ampersand ('&') = -6            &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;Cast an error because its not manditory.\
[x] double_ampersand ('&&') = -5    &emsp;&ensp;&nbsp;Cast an error because its not manditory.\
[x] semicolon	(';') = -4            &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;&nbsp;Cast an error because its not manditory.\
[x] double_semicolon (';;') = -3    &emsp;&emsp;&emsp;&nbsp;&nbsp;Cast an error because its not manditory.\
[x] left_bracket ('(') = -2         &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;Cast an error because its not manditory.\
[x] right_bracket (')') = -1								&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;Cast an error because its not manditory.\
[|] space (' ') = 0\
[|] pipe ('|') = 1\
[|] redirection_input ('<') = 2\
[|] redirection_output ('>') = 3\
[|] redirect_append_input ('<<') = 4\
[|] redirect_append_output ('>>') = 5\
[|] escape_char ('\') == 6\
command = 7\
built-in = 8\
arguments = 9\
flags = 10\
input_file = 11\
output_file = 12\
delimitor = 13\
command block == 20\
Built-in block == 21\

COMMANDS THAT I HAVE USED FOR TESTING:

ls -l | hey -l > output.txt\
ls -l < input.txt\
ls -l > output.txt\
cat < lexer.c ls -l | > output.txt ! Check why I dont get the error\
cat < lexer.c ls -l > output.txt\

From Pete with <3

Need ifile in the second position. Basically: wc -l < test.txt. - OK
Indentifiers: - OK

*** NEED FIXES ***
Doubly linked list would come in handy
Error handling
There is a leak when wrong command.
Check out the test logs.

*** USEFULL NOTES ***
cat < test.txt > long2.txt > long3.txt
cat  test.txt > long4.txt > long5.txt
File descriptors are created. Files are created but long3.txt and long5.txt are populated, long.2.txt and long4.txt 
are created during the file descriptors but not pupulated by the command.

these cases:
 
> testfile.txt creates new file, writes to file descriptor by default stdout
< testfile.txt opened to reading on stdin 
>testfile.txt segfaults and does not give the right vaulue, it would create a file

*** TEST LOGS ***

Minihell🍌🐒> <test.txt
__after lexing__

full list1[0]: indentifier: 0 < 
full list1[0]: indentifier: 0 test.txt 
after recognise commands
__register tokens check__

full list1[0]: indentifier: 2 < 
full list1[1]: indentifier: 11 test.txt 
info->info: 0
__CHUNK LIST__
full listC: indentifier: -1: (null)
ARGUMENTS:
zsh: segmentation fault  ./minishell



//the below should open Here Doc
Mini_hell🍌🐒> <<test
________after lexing_________________

full list1[0]: indentifier: 0 << 
full list1[0]: indentifier: 0 test 
after recognise commands
________register tokens check_________________

full list1[0]: indentifier: 4 << 
full list1[1]: indentifier: 13 test 
info->info: 0
________CHUNK LIST_________________
full listC: indentifier: -1: (null)
ARGUMENTS:
zsh: segmentation fault  ./minishell