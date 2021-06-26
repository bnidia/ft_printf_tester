#ifndef HELPERS_H
# define HELPERS_H

# define PRINTF(params, description) PRINTF_EXPECTED(params, params, description)

#ifdef __linux__
#define PRINTF_EXPECTED(params, expected, description) { \
	already_printed_help = 0; \
	if (current_test == test_nbr || test_nbr == 0) \
	{ \
		int child = fork(); \
		if (child == 0) \
		{ \
			int file = open("files/original_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); \
			int err = open("files/original_stderr.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); /* get rid of real printf errors*/ \
			dup2(file, 1); \
			dup2(err, 2); \
			printf expected; \
			return (0); \
		} \
		else \
		{ \
			waitpid(child, &wstatus, 0); \
		} \
		child = fork(); \
		if (child == 0) { \
			int file = open("files/user_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); \
			int err = open("files/user_stderr.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644); \
			dup2(file, 1); \
			dup2(err, 2); \
			alarm(1); \
			ft_printf params; \
			return (0); \
		} \
		else \
		{ \
			waitpid(child, &wstatus, 0); \
			if (wstatus != 0) \
			{ \
				ft_putstr(BOLD RED); \
				ft_putnbr(current_test); \
				switch(wstatus - 128) { \
					case SIGSEGV: /* classic segfault */ \
						ft_putstr(".SIGSEGV! " RESET); \
						break; \
					case 14 - 128: /* timeout */ \
						ft_putstr(".TIMEOUT! " RESET); \
						break; \
					default: /* something yet to be discovered */ \
						ft_putstr(".UNKNOWN CRASH! wstatus: "); \
						ft_putnbr(wstatus); \
						ft_putstr(RESET); \
				} \
				print_help(#params); \
			} \
			else \
			{ \
				check_result(description, #params); \
			} \
		} \
	} \
	current_test++; \
}
#elif defined __APPLE__
#define PRINTF_EXPECTED(params, expected, description) PRINTF(params, description)
#endif

#endif