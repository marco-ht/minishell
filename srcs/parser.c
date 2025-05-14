#include "../includes/minishell.h"

int    is_whitespace(char c)
{
    return(c == 32 || (c >= 9 && c <= 13));
}

void    set_zeros(char *line, t_exec *exec_str)
{
    int i;
	int	j;

    exec_str->evargs = malloc(100 * sizeof(char *));
	if (!exec_str->evargs)
        return;

	/* printf("line start = %p ", line);
	printf("\n"); */
    j = 0;
    while (line[j] && is_whitespace(line[j]))
		j++;
	i = 0;
    while(line[j])
    {
		while (line[j] && !is_whitespace(line[j]))
			j++;
		if(is_whitespace(line[j]))
            exec_str->evargs[i++] = &line[j];
		while (line[j] && is_whitespace(line[j]))
			j++;
	}
	exec_str->evargs[i] = NULL;
	/* ft_check(exec_str); */
}

void set_starts(char *line, t_exec *exec_str)
{
    int i;
    int j;
	
	i = 0;
	j = 0;
    exec_str->args = malloc(100 * sizeof(char *));	//ATTENZIONE VANNO LIBERATI DOPO AVER USATO QUEL NODO!!!!!
    if (!exec_str->args)
        return;

    // Skip leading whitespace
    while (line[j] && is_whitespace(line[j]))
        j++;
    while (line[j])
    {
        // Start of a word
        exec_str->args[i++] = &line[j];
        // Move to the end of the word
        while (line[j] && !is_whitespace(line[j]))
            j++;
        // Skip any whitespace between words
        while (line[j] && is_whitespace(line[j]))
            j++;
    }
    exec_str->args[i] = NULL;
    //ft_check_args(exec_str);
}

