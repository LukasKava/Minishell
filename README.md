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
<f0 echo hi >f1 how >f2 >f3 are you >f4
echo hi  how   are you
 echo hi  how   are you
  echo hi  how   are you
<f0 echo hi >f1 how >f2 >f3 are you >f4 |>f7 >f8 grep hi <f9
echo hi how are you|grep hi
<f0 echo hi >f1 how >f2 >f3 are you  >f4 |>f7 >f8 grep hi <f9 "hh$USER" << mj to
"hi from test"|'|"'
"hi from test"|'|"'
"h"|'|"'
echo hi | grep how | <<mj cat "$USERj"| also $PAT"$PATH"
echo hi >f1"how$USER"

echo "$USER" hi"$USER"       nhj | sdf dfgh

echo "$USER" hi"$USER"       nhj | sdf dfgh

<<mj cat > f1| grep hi

echo -n hi"how are" you    j
echo "$USER" hi"$USER"       nhj$USER"$USER""$USER"$USER
echo "$USER" hi"$USER"       nhj$USER"$USER""$USER"$USER"$USER""$USER$PATH"

export ab=cho
e$ab hi
 <f4 cat <<mj hi >f1 how >f2 >f3 are you >f4

 echo "hi | ||" | | you know you are a pie "|" | << hi | to you

From Pete with <3

Need ifile in the second position. Basically: wc -l < test.txt.
Indentifiers:



Error handling
There is a leak when wrong command.
Maybe I should have seperate redirections when there is Input or Output. And seperate when there is a pipe.

cases pipe
single command
singe comman redirect output to file
singe command redirect input to file
pipe input
pipe output
pipe input and output
first node write to pipe(canot read from pipe) pipe next command
first node write to file(cannot read from pipe) pipe next command

strerr

127
126
2
non existant file
here doc with quotes
what is strerror

TODO

- Try ctrl-C after running a blocking command like cat or grep without arguments
- Test only spaces or tabs.
- Try environment variables, whitespaces, pipes, redirection in the single quotes
- Unset the $PATH and check if it is not working anymore
- Set the $PATH to a multiple directory value (directory1:directory2) and check that directories are checked in order
from left to right
exit twice after entering minishell three or more times apparently there are leaks
also it prints two prompts
ls ""
'$PATH'
'               ' this example crashes.
ls '|' cat
unset $PATH

--leak-check=full --track-fds=yes ./minishell

TnT — 03/11/2022 21:09
valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes