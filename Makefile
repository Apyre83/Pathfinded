
MY_SOURCES	=	main.cpp	\
				pathfinding.cpp


ifeq ($(OS),Windows_NT)
	MY_SOURCES	:=	$(addprefix src\,$(MY_SOURCES))
else
	MY_SOURCES	:=	$(addprefix src/,$(MY_SOURCES))
endif

MY_OBJECTS	=	$(MY_SOURCES:.cpp=.o)


INCLUDES	=	-Iinclude/ -ISFML/include

STD			=	-std=c++2a

FLAGS		=	$(STD) -Wall -Wextra -Werror $(INCLUDES)
ifeq ($(OS),Windows_NT)
	FLAGS	+=	-LSFML/lib_win
else
	FLAGS	+=	-LSFML/lib_linux
endif


NAME		=	pathfinded
ifeq ($(OS),Windows_NT)
	PATH_BIN	=	bin\$(NAME)
	NAME		:=	$(NAME).exe
else
	PATH_BIN	=	bin/$(NAME)
endif

all: $(PATH_BIN)

.cpp.o:
	@g++ $(FLAGS) -c $< -o $(<:.cpp=.o)
	@echo Compiling : $<

$(PATH_BIN): $(LIB) $(MY_OBJECTS)
ifeq ($(OS),Windows_NT)
	@cls
	@g++ $(MY_OBJECTS) -o $(NAME) -LSFML/lib_win -lsfml-graphics -lsfml-window -lsfml-system
	@if not exist bin mkdir bin
	@move $(NAME) bin/
else
	@clear
	@echo "Password required to move the SFML libraries to /usr/lib/"
	@sudo cp SFML/lib_linux/libsfml-*.so.3.0 /usr/lib/
	@clear
	@g++ $(MY_OBJECTS) -o $(NAME) -LSFML/lib_linux -lsfml-graphics -lsfml-window -lsfml-system
	@mkdir -p bin
	@mv $(NAME) bin/
endif

	@echo Compiled !
	@echo See $(PATH_BIN) for the executable.

run: all
	@./$(PATH_BIN)

clean:
ifeq ($(OS),Windows_NT)
	@del $(MY_OBJECTS)
else
	@rm -f $(MY_OBJECTS)
endif

	@echo .o files cleaned.

fclean: clean
ifeq ($(OS),Windows_NT)
	@del $(PATH_BIN)
else
	@rm -f $(PATH_BIN)
endif

	@echo Everything cleaned.

re: fclean all

.PHONY: all clean fclean re
