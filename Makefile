NAME := philo

CC := gcc

FLAGS := -pthread -Wall -Wextra -Werror

CFILES := main.c validate_input.c

OFILES := $(CFILES:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OFILES)
	$(CC) $(FLAGS) $^ -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OFILES)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
