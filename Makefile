NAME := philo

CC := cc

#FLAGS := -fsanitize=thread -pthread -g -Wall -Wextra -Werror
FLAGS := -pthread -g -Wall -Wextra -Werror

CFILES := main.c validate_input.c init_structs.c meal.c

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
