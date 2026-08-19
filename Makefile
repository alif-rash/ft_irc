# ---------- Project Name ----------
NAME        = ircserv

# ---------- Files ----------
OBJDIR      = obj
SRCS        = src/main.cpp \
		  src/Server.cpp \
		  src/Client.cpp \
		  src/Channel.cpp \
		  src/Parser.cpp \
		  src/Utils.cpp \
		  src/Reply.cpp \
		  src/commands/Pass.cpp \
		  src/commands/Nick.cpp \
		  src/commands/User.cpp \
		  src/commands/Join.cpp \
		  src/commands/Part.cpp \
		  src/commands/Privmsg.cpp \
		  src/commands/Kick.cpp \
		  src/commands/Invite.cpp \
		  src/commands/Topic.cpp \
		  src/commands/Mode.cpp
OBJS        = $(SRCS:%.cpp=$(OBJDIR)/%.o)

# ---------- Compiler ----------
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
INCLUDES    = -Iinclude
RM          = rm -f

# ---------- Colors ----------
GRN         = \033[32;1m
RED         = \033[31;1m
YLW         = \033[33;1m
BLU         = \033[36;1m
WHT         = \033[0m

# ===========================================
# Rules
# ===========================================

all: header $(NAME)

header:
	@printf "\n$(BLU)===================================================$(WHT)\n"
	@printf "$(GRN)                      FT_IRC$(WHT)\n"
	@printf "$(BLU)===================================================$(WHT)\n"

$(NAME): $(OBJS)
	@printf "$(YLW)🔧 Linking all objects...$(WHT)\n"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GRN)✅ Build complete. Run with ./$(NAME) <port> <password>$(WHT)\n"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@printf "$(RED)🧹 Cleaning object files...$(WHT)\n"
	@$(RM) -r $(OBJDIR)
	@printf "$(GRN)✅ Clean complete!$(WHT)\n"

fclean: clean
	@printf "$(RED)💣 Removing executable...$(WHT)\n"
	@$(RM) $(NAME)
	@printf "$(GRN)🗑️ All cleaned up!$(WHT)\n"

re: fclean all

.PHONY: all clean fclean re header
