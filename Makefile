CC		= cc
CFLAGS	= -Wall -Werror -Wextra -pthread

NAME	= philo
NAME_B	= checker

SRCS	= \
		utils.c \
		parse_n_init.c \
		philo_life.c \
		main.c

OBJS    = ${SRCS:.c=.o}

${NAME}	: ${OBJS}
		${CC} -o $@ ${OBJS}

all		: ${NAME}

clean	:
		rm -f ${OBJS}

fclean	:
		rm -f ${OBJS} ${NAME}

re		: fclean all

.PHONY: all clean fclean re
