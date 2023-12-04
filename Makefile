CC			= cc
CFLAGS	= -Wall -Wextra -Werror 

SRCS		= philo.c \
					philo_utils.c \
					initialize.c \
					check_args.c \
					exec_multi_thread.c \
					simulation.c

OBJS_DIR = .objs
OBJS		= $(SRCS:%.c=$(OBJS_DIR)/%.o)
HEADER	= philo.h
NAME		= philo

all:			$(NAME)

$(NAME):	$(OBJS)
					$(CC) $(CFLAGS) -lpthread $(SRCS) -o $(NAME)

$(OBJS_DIR)/%.o:			%.c $(HEADER)
					mkdir -p $(OBJS_DIR)
					$(CC) $(CFLAGS) -c $< -o $@

clean:
					rm -rf $(OBJS_DIR)

fclean:		clean
					rm -f $(NAME)

re:				fclean all

.PHONY:		all clean fclean re