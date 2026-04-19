#include "app.h"
#include <libmath.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



namespace
{

    enum Status
    {
	S_FALSE = -1,
	S_OK = 0,
	S_UNKNOWN = 1,
    };

    struct Cmd
    {
	int val1;
	int val2;
	char operation;
	int result;
	int status;
    };

    // Вспомогательная функция для преобразования числа в строку
    void int_to_str(int num, char* buffer, int& len)
    {
	if (num == 0)
	{
	    buffer[0] = '0';
	    len = 1;
	    return;
	}

	char temp[32];
	int temp_len = 0;

	if (num < 0)
	{
	    buffer[0] = '-';
	    len = 1;
	    num = -num;
	}
	else
	{
	    len = 0;
	}

	while (num > 0)
	{
	    temp[temp_len++] = '0' + (num % 10);
	    num /= 10;
	}

	for (int i = temp_len - 1; i >= 0; i--)
	{
	    buffer[len++] = temp[i];
	}
    }

       size_t Strlen(const char* str)
       {
	   size_t len = 0;
	   while (str[len]) len++;
	   return len;
       }

       void write_str(const char* str)
       {
	   write(STDOUT_FILENO, str, Strlen(str));
	}

    // Функция для вывода числа через write
    void write_int(int num)
    {
	char buffer[32];
	int len;
	int_to_str(num, buffer, len);
	write(STDOUT_FILENO, buffer, len);
    }

    // Функция для вывода символа
    void write_char(char c)
    {
	write(STDOUT_FILENO, &c, 1);
    }

    void parse(int argc, char** argv, Cmd& cmd)
    {
	static struct option long_options[] =
	{
	    {"val1", required_argument, 0, 'a'},
	    {"operation", required_argument, 0, 'o'},
	    {"val2", required_argument, 0, 'b'},
	    {"factorial", required_argument, 0, 'f'},
	    {"help", no_argument, 0, 'h'},
	    {0, 0, 0, 0}
	};

	int opt;
	bool has_val1 = false, has_operation = false, has_val2 = false;

	optind = 1;

	while ((opt = getopt_long(argc, argv, "a:o:b:f:h", long_options, NULL)) != -1)
	{
	    switch (opt)
	    {
		case 'a':
		    cmd.val1 = atoi(optarg);
		    has_val1 = true;
		break;
		case 'o':
		{
		    char op = optarg[0];

		    // Проверка допустимых операций
		    if (op != '+' && op != '-' && op != '*' &&
			op != '/' && op != '^' && op != '!')
		    {
			write_str("Error: Invalid operation. Use +, -, *, /, ! or ^\n");
			exit(1);
		    }

		    cmd.operation = optarg[0];
		    has_operation = true;
		}
		break;
	    case 'f':
		    cmd.val1 = atoi(optarg);
		    cmd.operation = '!';
		    break;
	    case 'b':
		    cmd.val2 = atoi(optarg);
		    has_val2 = true;
		    break;
	    case 'h':
		write_str("Usage: ");
		write_str(argv[0]);
		write_str(" [OPTIONS]\n"
		"Options:\n"
		"  -a, --val1 <num>       First operand\n"
		"  -o, --operation <op>   Operation (+, -, *, /, %)\n"
		"  -b, --val2 <num>       Second operand\n"
		"  -f, --factorial <num>  Calculate factorial (unary operation)\n"
		"  -h, --help             Show this help\n"
		"\nExample:\n"
		"  ");
		write_str(argv[0]);
		write_str(" -a 10 -o + -b 5\n"
		"  ");
		write_str(argv[0]);
		write_str(" --a 20 --operation '*' --b 3\n"
		"  ");
		write_str(argv[0]);
		write_str(" -f 5                    (factorial: 5!)\n"
		"  ");
		write_str(argv[0]);
		write_str(" --factorial 7           (factorial: 7!)\n");
		exit(0);
	    }
	}
    }

    void calculate(Cmd& cmd)
    {
	cmd.status = S_OK;
	switch(cmd.operation)
	{
	    case '+':
		cmd.result = libmath::addition(cmd.val1, cmd.val2);
		break;
	    case '-':
		cmd.result = libmath::subtraction(cmd.val1, cmd.val2);
		break;
	    case '*':
		cmd.result = libmath::multiplication(cmd.val1, cmd.val2);
		break;
	    case '/':
		cmd.status = libmath::division(cmd.val1, cmd.val2, cmd.result);
		break;
	    case '^':
		cmd.result = libmath::power(cmd.val1, cmd.val2);
		break;
	    case '!':
		if (cmd.val1 < 0)
		{
		    cmd.status = S_FALSE;
		    return;
		}
		cmd.result = libmath::factorial(cmd.val1);
		break;
	    default:
		cmd.status = S_FALSE;
	}
    }

    void output(const Cmd& cmd)
    {
	if (cmd.status == S_OK)
	{
	    if (cmd.operation == '!')
	    {
		write_int(cmd.val1);
		write_str("! = ");
		write_int(cmd.result);
		write_char('\n');
	    }
	    else
	    {
		write_int(cmd.val1);
		write_char(' ');
		write_char(cmd.operation);
		write_char(' ');
		write_int(cmd.val2);
		write_str(" = ");
		write_int(cmd.result);
		write_char('\n');
	    }
	}
	else if (cmd.status == S_FALSE)
	{
	    write_str("Error!\n");
	}
	else if (cmd.status == S_UNKNOWN)
	{
	    write_str("Error! Unknown operation!\n");
	}
	else
	{
	    write_str("Unknown error\n");
	}
    }
}

namespace app
{
    void run(int argc, char** argv)
    {
	Cmd cmd;
	parse(argc, argv, cmd);
	calculate(cmd);
	output(cmd);
    }
}
