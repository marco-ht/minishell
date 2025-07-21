# Nome del progetto
NAME    = minishell
NAMEB	= minishell_bonus

# Compiler e flags
CC      = gcc
CFLAGS  = -Wall -Werror -Wextra
LDLIBS   = -lreadline -lncurses

# Directory dei sorgenti e degli oggetti
SRC_DIR = srcs
OBJ_DIR = objs

# Lista dei file sorgente e relativi oggetti
SRC     = $(SRC_DIR)/builtins2_n.c $(SRC_DIR)/builtins3_n.c $(SRC_DIR)/builtins4_n.c $(SRC_DIR)/builtins5_n.c $(SRC_DIR)/builtins6_n.c $(SRC_DIR)/builtins7_n.c $(SRC_DIR)/builtins_n.c $(SRC_DIR)/constructors2_n.c $(SRC_DIR)/constructors_n.c $(SRC_DIR)/env_n.c $(SRC_DIR)/exec_utils_n.c $(SRC_DIR)/expansions2_n.c $(SRC_DIR)/expansions_n.c $(SRC_DIR)/free_tree2_n.c $(SRC_DIR)/free_tree_n.c $(SRC_DIR)/get_next_line_n.c $(SRC_DIR)/get_next_line_utils_n.c $(SRC_DIR)/main2_n.c $(SRC_DIR)/main_n.c $(SRC_DIR)/parser1a_n.c $(SRC_DIR)/parser1b_n.c $(SRC_DIR)/parser1c_n.c $(SRC_DIR)/parser2_n.c $(SRC_DIR)/parser3_n.c $(SRC_DIR)/parser4_n.c $(SRC_DIR)/parser5_n.c $(SRC_DIR)/parser6_n.c $(SRC_DIR)/parser7_n.c $(SRC_DIR)/runcmd1a_n.c $(SRC_DIR)/runcmd1b_n.c $(SRC_DIR)/runcmd1c_n.c $(SRC_DIR)/runcmd1d_n.c $(SRC_DIR)/runcmd1e_n.c $(SRC_DIR)/runcmd2_n.c $(SRC_DIR)/runcmd3_n.c $(SRC_DIR)/runcmd4_n.c $(SRC_DIR)/signals_n.c $(SRC_DIR)/utils2_n.c $(SRC_DIR)/utils_n.c $(SRC_DIR)/wildcards1b_n.c $(SRC_DIR)/wildcards1c_n.c $(SRC_DIR)/wildcards1d_n.c $(SRC_DIR)/wildcards2_n.c $(SRC_DIR)/wildcards3_n.c $(SRC_DIR)/wildcards4_n.c $(SRC_DIR)/wildcards_n.c
OBJ     = $(OBJ_DIR)/builtins2_n.o $(OBJ_DIR)/builtins3_n.o $(OBJ_DIR)/builtins4_n.o $(OBJ_DIR)/builtins5_n.o $(OBJ_DIR)/builtins6_n.o $(OBJ_DIR)/builtins7_n.o $(OBJ_DIR)/builtins_n.o $(OBJ_DIR)/constructors2_n.o $(OBJ_DIR)/constructors_n.o $(OBJ_DIR)/env_n.o $(OBJ_DIR)/exec_utils_n.o $(OBJ_DIR)/expansions2_n.o $(OBJ_DIR)/expansions_n.o $(OBJ_DIR)/free_tree2_n.o $(OBJ_DIR)/free_tree_n.o $(OBJ_DIR)/get_next_line_n.o $(OBJ_DIR)/get_next_line_utils_n.o $(OBJ_DIR)/main2_n.o $(OBJ_DIR)/main_n.o $(OBJ_DIR)/parser1a_n.o $(OBJ_DIR)/parser1b_n.o $(OBJ_DIR)/parser1c_n.o $(OBJ_DIR)/parser2_n.o $(OBJ_DIR)/parser3_n.o $(OBJ_DIR)/parser4_n.o $(OBJ_DIR)/parser5_n.o $(OBJ_DIR)/parser6_n.o $(OBJ_DIR)/parser7_n.o $(OBJ_DIR)/runcmd1a_n.o $(OBJ_DIR)/runcmd1b_n.o $(OBJ_DIR)/runcmd1c_n.o $(OBJ_DIR)/runcmd1d_n.o $(OBJ_DIR)/runcmd1e_n.o $(OBJ_DIR)/runcmd2_n.o $(OBJ_DIR)/runcmd3_n.o $(OBJ_DIR)/runcmd4_n.o $(OBJ_DIR)/signals_n.o $(OBJ_DIR)/utils2_n.o $(OBJ_DIR)/utils_n.o $(OBJ_DIR)/wildcards1b_n.o $(OBJ_DIR)/wildcards1c_n.o $(OBJ_DIR)/wildcards1d_n.o $(OBJ_DIR)/wildcards2_n.o $(OBJ_DIR)/wildcards3_n.o $(OBJ_DIR)/wildcards4_n.o $(OBJ_DIR)/wildcards_n.o

# Parametri per libft
LIBFT_DIR = libft
LIBFT    = $(LIBFT_DIR)/libft.a

# Comando di rimozione
RM      = rm -rf

# Target di default
all: $(NAME)

# Regola per creare l'eseguibile, dipendente da oggetti e dalla libft compilata
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(LDLIBS)

bonus: $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAMEB) $(LDLIBS)

# Regola per compilare i file .c in file .o all'interno della directory obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Creazione della directory degli oggetti, se non esiste
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compilazione della libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) bonus

# Pulizia dei file oggetto (interni e di libft)
clean:
	$(RM) $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

# Pulizia completa: rimuove l'eseguibile, gli oggetti e la libft (file oggetto e libft.a)
fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAMEB)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Ricompilazione totale
re: fclean all
