CC		= clang++

RM		= rm -f

COLOR_GREEN = "\033[0;32m"
COLOR_RED = "\033[0;31m"
COLOR_BLUE = "\033[0;34m"
COLOR_CLEAR = "\033[0m"

SRCDIR		= src
OBJDIR		= obj

CXXFLAGS 	+= -W -Wextra -Wall
CXXFLAGS 	+= -rdynamic -fPIC -Iinclude

NAMESERVER	= server
NAMECORE	= core

SRCCORE	= $(wildcard src/core/*.cpp)
OBJCORE	= $(SRCCORE:%.cpp=%.o)

SRCSERVER	= src/main.cpp
OBJSERVER	= $(SRCSERVER:%.cpp=%.o)

NAMESERVICE = service
SRCUSERSERVICE = $(wildcard src/service/*.cpp)
OBJUSERSERVICE = $(SRCUSERSERVICE:%.cpp=%.o)

VPATH		= $(SRCDIR)

src/core/%.o	: src/core/%.cpp
	$(CC) -c $< $(CFLAGS) -Iinclude -Isrc/core -o $@
src/server/%.o	: src/server/%.cpp
	$(CC) -c $< $(CFLAGS) -Iinclude -Isrc/core -o $@
src/main.o	: src/main.cpp
	$(CC) -c $< $(CFLAGS) -Iinclude -Isrc/core -o $@
%.o	: %.cpp
	$(CC) -c $< $(CXXFLAGS) -Iinclude -Isrc/core -o $@

all: $(NAMECORE) $(NAMESERVER) $(NAMESERVICE)




lib$(NAMESERVICE).so: $(OBJUSERSERVICE)
	$(CC) -shared $(OBJUSERSERVICE) $(NAMECORE).a -o lib$(NAMESERVICE).so
	@echo $(COLOR_BLUE)"$@"$(COLOR_GREEN)" [DONE]"$(COLOR_CLEAR)

$(NAMESERVICE): lib$(NAMESERVICE).so


$(NAMESERVER): $(OBJSERVER) $(NAMECORE)
	$(CC) $(OBJSERVER) $(NAMECORE).a $(CFLAGS) -o $(NAMESERVER)
	@echo $(COLOR_BLUE)"$@"$(COLOR_GREEN)" [DONE]"$(COLOR_CLEAR)

$(NAMECORE): $(OBJCORE)
	ar rvs $(NAMECORE).a $(OBJCORE)
	@echo $(COLOR_BLUE)"$@.a"$(COLOR_GREEN)" [DONE]"$(COLOR_CLEAR)

clean:
	@printf $(COLOR_RED)"Cleaning compiled files\n"
	$(RM) $(OBJCORE)
	$(RM) $(OBJSERVER)
	$(RM) $(OBJUSERSERVICE)
	@printf $(COLOR_CLEAR)
	
sclean:
	@$(MAKE) clean
	
fclean: clean
	@printf $(COLOR_RED)"Cleaning executable\n"
	$(RM) $(NAMECORE).a
	$(RM) $(NAMESERVER)
	$(RM) lib$(NAMESERVICE).so
	@printf $(COLOR_GREEN)
	@echo "Deep clean DONE"
	@printf $(COLOR_CLEAR)


re: fclean all sclean

.PHONY: all clean fclean re
