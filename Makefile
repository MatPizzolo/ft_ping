NAME = ft_ping

SRCS_DIR = ./srcs

SRCS = $(SRCS_DIR)/main.c $(SRCS_DIR)/checkers.c $(SRCS_DIR)/utils.c $(SRCS_DIR)/getting-address-info.c $(SRCS_DIR)/icmp-handler.c $(SRCS_DIR)/socket-handler.c 

OBJS = $(SRCS:.c=.o)

CC = gcc 
CFLAGS = -Wall -Wextra #-Werror 

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re