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
