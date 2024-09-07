NAME				:= ircserv

CXX					:= g++

CXXFLAGS			:= -std=c++98 -pedantic -Wall -Wextra -Werror -g3
LDFLAGS				:= -std=c++98 -pedantic -g3
HEADERS				:=	-I ./include

ifdef leak_mac
# fsanitize flag doesnt work w/ `system("leaks <executable>");`
	CXXFLAGS		+= -g3 -fsanitize=address
	LDFLAGS			+= -g3 -fsanitize=address
	LDFLAGS			+= -L ../../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next
	HEADERS			+= -I ../../LeakSanitizer
endif

# build using `make re leak_docker=1` and then run <executable>
ifdef leak_docker
	CXXFLAGS		+= -g3 -fsanitize=address
	LDFLAGS			+= -g3 -fsanitize=address
endif

SRC_DIR				:= ./src/

SRCS				:=	main.cpp \
						server.cpp

OBJ_DIR				:= ./obj/

OBJS				:= $(addprefix $(OBJ_DIR), $(SRCS:.cpp=.o))

all:				$(NAME)

$(NAME):			$(OBJ_DIR) $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LDFLAGS) && echo "Compilation of $(NAME) is successful"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o:		$(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) $(HEADERS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
