# Nome del progetto
NAME    = minishell

# Compiler e flags
CC      = gcc
CFLAGS  = -Wall -Werror -Wextra
LDLIBS   = -lreadline -lncurses

# Directory dei sorgenti e degli oggetti
SRC_DIR = srcs
OBJ_DIR = objs

# Lista dei file sorgente e relativi oggetti
SRC     = $(wildcard $(SRC_DIR)/*.c)
OBJ     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

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
	$(MAKE) -C $(LIBFT_DIR) fclean

# Ricompilazione totale
re: fclean all
