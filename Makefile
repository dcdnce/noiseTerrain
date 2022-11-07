SRCS	=	srcs/main.cpp \
			srcs/noise.cpp \
			srcs/terrain.cpp \
			srcs/utils.cpp

OBJS	=	$(SRCS:.cpp=.o)

CXX	=	c++

OS	:=	$(shell uname)

# Compiler flags (+ where to find headers)
CXXFLAGS	=	-Wall -Wextra -std=c++11
CXXFLAGS	+=	-Iincludes -I/usr/local/include

# Linking flags(+ where to find libs)
LFLAGS	=	
ifeq ($(OS), Darwin)
	LFLAGS	=	-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	LFLAGS	+=	-Llibs -lraylib
else
	LFLAGS = -L/usr/local/lib -lraylib
endif

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
