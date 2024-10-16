NAME				:= ircserv

CXX					:= g++

CXXFLAGS			:= -std=c++17 -pedantic -Wall -Wextra -Werror -g3
LDFLAGS				:= -std=c++17 -pedantic -g3
HEADERS				:=	-I ./include

ifdef leak_mac
# fsanitize flag doesn't work with `system("leaks <executable>");`
	CXXFLAGS		+= -g3 -fsanitize=address
	LDFLAGS			+= -g3 -fsanitize=address
	LDFLAGS			+= -L ../../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next
	HEADERS			+= -I ../../LeakSanitizer
endif

# Build using `make re leak_docker=1` and then run <executable>
ifdef leak_docker
	CXXFLAGS		+= -g3 -fsanitize=address
	LDFLAGS			+= -g3 -fsanitize=address
endif

SRC_DIR				:= ./src
OBJ_DIR				:= ./obj

# Collect all .cpp files in src/ and subdirectories
SRCS				:= $(shell find $(SRC_DIR) -name '*.cpp')
# Replace src/ with obj/ and .cpp with .o for object files
OBJS				:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all:				$(NAME)

$(NAME):			$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(NAME) && echo "Compilation of $(NAME) is successful"

# Rule to compile .cpp files into .o files
$(OBJ_DIR)/%.o:		$(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(HEADERS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
