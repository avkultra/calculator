#include "app.h"
#include "libmath.h"
#include <cstdlib>
#include <iostream>
#include <getopt.h>
#include <cstring>

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
			    op != '/' && op != '^')
			{
			    std::cout << ("Error: Invalid operation. Use +, -, *, /, or ^\n") << std::endl;
			    exit(1);
			}

			cmd.operation = optarg[0];
			has_operation = true;
		    }
		    break;
		case 'b':
			cmd.val2 = atoi(optarg);
			has_val2 = true;
			break;
		case 'h':
			std::cout << "Usage: " << argv[0] << " [OPTIONS]\n"
			<< "Options:\n"
			<< "  -a, --val1 <num>       First operand\n"
			<< "  -o, --operation <op>   Operation (+, -, *, /, %)\n"
			<< "  -b, --val2 <num>       Second operand\n"
			<< "  -f, --factorial <num>  Calculate factorial (unary operation)\n"
			<< "  -h, --help             Show this help\n"
			<< "\nExample:\n"
			<< "  " << argv[0] << " -a 10 -o + -b 5\n"
			<< "  " << argv[0] << " --a 20 --operation '*' --b 3"
			<< "  " << argv[0] << " -f 5                    (factorial: 5!)\n"
			<< "  " << argv[0] << " --factorial 7           (factorial: 7!)" << std::endl;
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
		std::cout << cmd.val1 << ' ' << cmd.operation << ' ' << cmd.val2 << " = " << cmd.result << '\n';
	    }
	    else if (cmd.status == S_FALSE)
	    {
		std::cout << "Error!\n";
	    }
	    else if (cmd.status == S_UNKNOWN)
	    {
		std::cout << "Error! Unknown operation!\n";
	    }
	    else
	    {
		std::cout << "Unknown error\n";
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
