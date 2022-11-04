SRCS	=	srcs/main.cpp

OBJS	=	$(SRCS:.cpp=.o)

CXX	=	c++

# Compiler flags (+ where to find headers)
CXXFLAGS	=	-Wall -Wextra -std=c++11
CXXFLAGS	+=	-Iincludes
# Linking flags(+ where to find libs)
LFLAGS	=	-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
LFLAGS	+=	-Llibs -lraylib

NAME	=	raylib_test

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CXX) $^ $(LFLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re: fclean	all

.PHONY: all clean fclean all
