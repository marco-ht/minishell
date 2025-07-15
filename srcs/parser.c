#include "../includes/minishell.h"

//metti in file dedicato e spezza
int	gettoken(char **ps, char *es, char **q, char **eq) 
{
	char	*s;
	int		ret;
	char	*whitespace;
	char	*symbols;

	whitespace = " \t\r\n\v";
	symbols = "<|>&;()";	//devo mettere gli apici in symbols per fermare scansione di quel token "a" o no?
	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	if (q)
		*q = s;
	if (eq)
       *eq = s;
	ret = *s;
	// se simbolo operatore singolo e' gia' settato correttamente il ritorno,
	// quindi scorriamo solo, senno' lo settiamo
	if (*s == '\0') // caso raggiunta fine buffer, no nuovo token
		return (0);
	/* else if (*s == '\'') // se caso singolo apice
	{
		//ret = 's'; VEDERE DOPO COME IMPLEMENTARE
		ret = 's';
		s++;                   // salta l'apice di apertura
       	if (q)
			*q = s;         // inizio del contenuto
		while (s < es && *s != '\'')
			// scorriamo fino all'altro apice singolo ingorando metacaratteri ($ NON verra considerato in espansione variab in execution)
			s++;
		if (eq)
		*eq = s;       // fine esclusivo
   		if (s < es)
		s++;       // salta l'apice di chiusura se presente (cioe se non ci siamo fermati per fine stringa)
	}
	else if (*s == '\"') // se caso doppio apice
	{
		//ret = 'd'; VEDERE DOPO COME IMPLEMENTARE
		ret = 'd';
		s++;                   // salta l'apice di apertura
       	if (q)
			*q = s;         // inizio del contenuto
		while (s < es && *s != '\"')
			// scorriamo fino all'altro apice singolo ingorando metacaratteri ($ NON verra considerato in espansione variab in execution)
			s++;
		if (eq)
		*eq = s;       // fine esclusivo
   		if (s < es)
		s++;       // salta l'apice di chiusura se presente (cioe se non ci siamo fermati per fine stringa)
	} */
	else if (*s == '(' || *s == ')') // simbolo operatore singolo
		s++;
	else if (*s == ';') // simbolo operatore list (poi runcmd dara' errore syntax)
		s++;
	else if (*s == '|')
	{
		s++; // se caso operatore singolo pipe "|" non entra in if, scorre solo,
				// ret e gia' settato correttamente
		if (s < es && *s == '|')
		{
			ret = 'o'; // caso operatore or "||"
			s++;
		}
	}
	else if (*s == '&') // we don't need to handle single "&",
	{					// ret e gia' settato correttamente
		s++;
		if(s < es && *s == '&')
		{
			ret = 'e';
			s++;
		}
	}
	else if (*s == '>')
	{
		s++;
		// se caso operatore singolo redirect ">" (redirect output) non entra in if,
		// scorre solo, ret e gia' settato correttamente
		if (s < es && *s == '>')
		{
			ret = '+'; // caso operatore ">>" (redirect output in append mode)
			s++;
		}
	}
	else if (*s == '<')
	{
		s++;
		// se caso operatore singolo redirect "<" (redirect input) non entra in if,
		// scorre solo, ret e gia' settato correttamente
		if (s < es && *s == '<')
		{
			ret = 'h'; // caso operatore "<<" (here_doc)
			s++;
		}
	}
else
    {
        /* un unico ramo che gestisce casi token 'a', 's' e 'd' */
        ret = 'a';            /* caso token di tipo 'a' o misto */
        {
            char *start;
            char *out_write;
            char  quote;

			start      = s;
			out_write  = s;
             /* scansiono fino a spazio o symbols (<|>&;()) */
             while (s < es
                 && !ft_strchr(whitespace, *s)
                 && !ft_strchr(symbols,    *s))
             {
                 if (*s == '\'' || *s == '\"')
                 {
                     /* trovo una quote: la salto e salvo tipo */
                    quote = *s++;
                    /* se era ancora 'a', ora imposto il tipo */
                    if (ret == 'a')
                    {
                        if (quote == '\'')
                            ret = 's';
                        else
                            ret = 'd';
                    }
                     /* copio tutto fino alla chiusura */
                     while (s < es && *s != quote)
                         *out_write++ = *s++;
                     if (s < es)
                         s++;
                 }
				 else if (*s == '\\' && s + 1 < es) /* gestione backslash escape */
                 {
                    s++; /* salto il backslash */
                    *out_write++ = *s++; /* copio il carattere escapato */
                    ret = 's'; /* lo gestisco come se fosse single quoted */
                 }
                 else /* includo anche $ dentro lo stesso token */
                     *out_write++ = *s++;
             }
             /* null termino token e assegno q ed eq */
             /* *out_write = '\0'; */
             if (q)
			 	*q  = start;
             if (eq)
			 	*eq = out_write;
         }
     }
	/* if (eq && *eq && **eq != '\'' && **eq != '\"')
		*eq = s; */
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	*ps = s;
	return (ret);
}

// skip whitespaces and return bolean,
//= 1 if the character exists and it is one of the toks
int	peek(char **ps, char *es, char *toks)
{
	char *s;
	char *whitespace;

	whitespace = " \t\r\n\v";
	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}

t_cmd	*ft_parsecmd(char *s, int *status)
{
	char	*es;
	char	*p;
	t_cmd	*cmd;

	es = s + strlen(s);      // find end of the string
	p = es - 1;
	while (p >= s && (*p == ' ' || *p == '\t' || *p == '\n'))
    	p--;
	if (p >= s && *p == '|') {
		// pipe “libera” alla fine: segnalo incomplete
		*status = 1;
		return (NULL);
	}
	*status = 0;
	cmd = parseandor(&s, es); // do the parsing job
	peek(&s, es, "");        // skip whitespaces
	if (s != es)
	{
		// if we didn't reach the end of buffer after parseandor and skipping spaces,
		// it means ther's an error!!
		printf("minishell: syntax error near '%s'\n", s);
		return (free_tree(cmd), NULL);
	}
	nulterminate(cmd);
	// go trough the tree recoursively and null terminate in the nodes all the strings of commands names,
	// commands arguments and filenames
	return (cmd); // returns the tree it built
}

t_cmd	*parseandor(char **ps, char *es)
{
	t_cmd	*cmd;
	char	*backup;
	int		tok;

	cmd = parsepipe(ps, es);
	if(!cmd)
		return(NULL);  //propago all'indietro errore per tornare al prompt
	while (1)	//controlla se il prossimo token e' "o" o "e", peek solo 1 token quindi chiamiamo getoken in buffer temp
	{			//continuo finche' continuo a trovare token && o ||
		backup = *ps;
		tok = gettoken(ps, es, NULL, NULL);
		if(tok == '&' || tok == ';')
		{
			printf("minishell: unexpected ';' or single '&' near '%s'\n", *ps); //free treee!!!!!! e free buffer!!!!!!
			return(free_tree(cmd), NULL);
		}
		else if(tok == 'e') //abbiamo &&
			cmd = ft_andcmd(cmd, parsepipe(ps, es)); //costruttore nodo AND
		else if(tok == 'o') //abbiamo ||
			cmd = ft_orcmd(cmd, parsepipe(ps, es)); //costruttore nodo OR
		else
		{	//non era ne && ne ||, ripristino *ps ed esco dal while
			*ps = backup;
			break;
		}
	}
	return(cmd);
}

/* t_cmd	*parsepipe(char **ps, char *es) //OLD ONE WRONG
{
	t_cmd	*cmd;
	t_execcmd *e;
	char	*backup;
	int		tok;

	cmd = parseexec(ps, es);
	while (1)
	{
		backup = *ps;
		tok = gettoken(ps, es, 0, 0);
		if (tok == '|') // Abbiamo trovato un singolo | = pipe
		{
			cmd = ft_pipecmd(cmd, parsepipe(ps, es));
			if (((t_pipecmd *)cmd)->right->type == EXEC)
			{
        		e = (t_execcmd *) ((t_pipecmd *)cmd)->right;
        		if (e->argv[0] == NULL)
					return(printf("error token `|'\n"), free_tree(cmd), NULL);
    		}
		}
		else // Non era un '|' singolo: ripristino e torno
		{
			*ps = backup;
            break;
		}
	}
	return (cmd);
} */

t_cmd	*parsepipe(char **ps, char *es)
{
	t_cmd	*cmd;
	char	*backup;
	int		tok;

	cmd = parseexec(ps, es);
	if(!cmd)
		return(NULL);
	while (1)
	{
		backup = *ps;
		tok = gettoken(ps, es, 0, 0);
		if (tok == '|') // Abbiamo trovato un singolo | = pipe
			cmd = ft_pipecmd(cmd, parsepipe(ps, es));
		else // Non era un '|' singolo: ripristino e torno
		{
			*ps = backup;
            break;
		}
	}
	return (cmd);
}

t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es)
{
	int	tok;
	char  *q;
  	char  *eq;
	int		tok_type;

	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		tok_type = gettoken(ps, es, &q, &eq);
		if (tok_type != 'a' && tok_type != 's' && tok_type != 'd')
		{
			ft_putstr_fd(" missing file for redirection", 2);
			return(free_tree(cmd), NULL);
		}
    if(tok == '<')
      cmd = ft_redircmd(cmd, q, eq, O_RDONLY, 0);
    else if(tok == '>')
      cmd = ft_redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
    else if(tok == '+') // >>
      cmd = ft_redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
	else if (tok == 'h')
    	cmd = ft_heredoccmd(cmd, tok_type, q, eq); // q, eq inizio e fine stringa limiter in buffer
	}
	return (cmd);
}

t_cmd	*parseblock(char **ps, char *es)
{
	t_cmd	*cmd;

	if (!peek(ps, es, "("))
	{
		printf("parseblock '(' missing)"); //aggiustato perche no exit da minishell, solo free e return
		return(NULL);
	}
	gettoken(ps, es, 0, 0);
	cmd = parseandor(ps, es);
	if (!peek(ps, es, ")"))
	{
		printf("parseblock ')' missing)");  //aggiustato perche no exit da minishell, solo free e return
		return(free(cmd), NULL);
	}
	gettoken(ps, es, 0, 0);
	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}

/* t_cmd	*parseexec(char **ps, char *es)  //OLD, WRONG
{
	t_execcmd	*cmd;		//ci vorra' struttura per ridurre righe e gestire cosi tante variabili in una funzione
	t_cmd		*ret;
	char		*q;
	char		*eq;
	int			tok;
	int			argc;

	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	ret = ft_execcmd();
	cmd = (t_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (!peek(ps, es, "|);&<>"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break;
		if (tok != 'a')
		{
			printf("syntax");
			return(free(cmd), free(ret), NULL);
		}
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		argc++;
		if (argc >= MAX_ARGS)
		{
			printf("too many args");
			return(free(cmd), free(ret), NULL);
		}
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (ret);
} */

t_cmd	*parseexec(char **ps, char *es)
{
	t_execcmd	*cmd;		//ci vorra' struttura per ridurre righe e gestire cosi tante variabili in una funzione
	t_cmd		*ret;
	char		*q;
	char		*eq;
	char		*backup;
	int			tok;
	int			argc;

	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	ret = ft_execcmd();
	cmd = (t_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es);
	while (1)
	{
		backup = *ps;
		tok = gettoken(ps, es, &q, &eq);
		if (tok != 'a' && tok != 's' && tok != 'd')
		{
			*ps = backup;	//torniamo indietro di un carattere
			break;
		}
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		cmd->qtype[argc] = tok;   // registra il tipo
		if (++argc >= MAX_ARGS)
		{
			printf("too many args");
			return(free(cmd), free(ret), NULL);
		}
		ret = parseredirs(ret, ps, es);
	}
	cmd->argv[argc] = NULL;
	cmd->eargv[argc] = NULL;
	return (ret);
}

// NUL-terminate all the counted strings.
t_cmd	*nulterminate(t_cmd *cmd)
{									//da dividere in nullterminate 1 e 2 e forse 3
	int			i;					//metti anche in file a parte
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;
	t_andcmd	*acmd;
	t_orcmd		*ocmd;
	t_heredoccmd *hd;

	if (cmd == 0)
		return (0);
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		i = -1;
		while (ecmd->argv[++i])
			*ecmd->eargv[i] = 0;
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		nulterminate(rcmd->cmd);
		*rcmd->efile = 0;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		nulterminate(pcmd->left);
		nulterminate(pcmd->right);
	}
	else if (cmd->type == AND)
	{
		acmd = (t_andcmd *)cmd;
		nulterminate(acmd->left);
		nulterminate(acmd->right);
	}
	else if (cmd->type == OR)
	{
		ocmd = (t_orcmd *)cmd;
		nulterminate(ocmd->left);
		nulterminate(ocmd->right);
	}
	else if (cmd->type == HEREDOC)
	{
        hd = (t_heredoccmd *) cmd;
        *hd->lim_end = '\0';  // null term stringa limiter
        nulterminate(hd->cmd); //discendo nel sotto-albero
    }
	return (cmd);
}
