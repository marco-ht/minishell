#include "../includes/minishell.h"

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;
	char	*whitespace;
	char	*symbols;

	whitespace = " \t\r\n\v";
	symbols = "<|>&;()";
	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	if (q)
		*q = s;
	if (eq)
       *eq = s;
	ret = *s;
	if (*s == '\0')
		return (0);
	else if (*s == '(' || *s == ')')
		s++;
	else if (*s == ';')
		s++;
	else if (*s == '|')
	{
		s++;
		if (s < es && *s == '|')
		{
			ret = 'o';
			s++;
		}
	}
	else if (*s == '&')
	{
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
		if (s < es && *s == '>')
		{
			ret = '+';
			s++;
		}
	}
	else if (*s == '<')
	{
		s++;
		if (s < es && *s == '<')
		{
			ret = 'h';
			s++;
		}
	}
else
    {
        ret = 'a';
        {
            char *start;
            char *out_write;
            char  quote;

			start      = s;
			out_write  = s;
             while (s < es
                 && !ft_strchr(whitespace, *s)
                 && !ft_strchr(symbols,    *s))
             {
                 if (*s == '\'' || *s == '\"')
                 {
                    quote = *s++;
                    if (ret == 'a')
                    {
                        if (quote == '\'')
                            ret = 's';
                        else
                            ret = 'd';
                    }
                     while (s < es && *s != quote)
                         *out_write++ = *s++;
                     if (s < es)
                         s++;
                 }
				 else if (*s == '\\' && s + 1 < es)
                 {
                    s++;
                    *out_write++ = *s++;
                    ret = 's';
                 }
                 else
                     *out_write++ = *s++;
             }
             if (q)
			 	*q  = start;
             if (eq)
			 	*eq = out_write;
         }
     }
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

	es = s + strlen(s);
	p = es - 1;
	while (p >= s && (*p == ' ' || *p == '\t' || *p == '\n'))
    	p--;
	if (p >= s && *p == '|')
	{
		*status = 1;
		return (NULL);
	}
	*status = 0;
	cmd = parseandor(&s, es);
	peek(&s, es, "");
	if (s != es)
	{
		printf("minishell: syntax error near '%s'\n", s);
		return (free_tree(cmd), NULL);
	}
	nulterminate(cmd);
	return (cmd);
}

// Does the parsing job
t_cmd	*parseandor(char **ps, char *es)
{
	t_cmd	*cmd;
	char	*backup;
	int		tok;

	cmd = parsepipe(ps, es);
	if(!cmd)
		return(NULL);
	while (1)
	{
		backup = *ps;
		tok = gettoken(ps, es, NULL, NULL);
		if(tok == '&' || tok == ';')
		{
			printf("minishell: unexpected ';' or single '&' near '%s'\n", *ps);
			return(free_tree(cmd), NULL);
		}
		else if(tok == 'e')
			cmd = ft_andcmd(cmd, parsepipe(ps, es));
		else if(tok == 'o')
			cmd = ft_orcmd(cmd, parsepipe(ps, es));
		else
		{
			*ps = backup;
			break;
		}
	}
	return(cmd);
}

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
		if (tok == '|')
			cmd = ft_pipecmd(cmd, parsepipe(ps, es));
		else
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
	int		tok_type;
	t_redin	rin;

	while (peek(ps, es, "<>"))
	{
		rin.subcmd = cmd;
		tok = gettoken(ps, es, 0, 0);
		tok_type = gettoken(ps, es, &rin.file, &rin.efile);
		if (tok_type != 'a' && tok_type != 's' && tok_type != 'd')
		{
			ft_putstr_fd(" missing file for redirection", 2);
			return(free_tree(cmd), NULL);
		}
		rin.allocated = 0;
		if (tok_type == 'a' && ft_strchr(rin.file, '*'))
		{
			*rin.efile = '\0';
			rin.file = expand_redirect_glob(rin.file, &rin.allocated);
			rin.efile = rin.file + ft_strlen(rin.file);
		}
		if(tok == '<')
		{
			rin.mode = O_RDONLY;
			rin.fd = 0;
			cmd = ft_redircmd(&rin);
		}
		else if(tok == '>')
		{
			rin.mode = O_WRONLY | O_CREAT | O_TRUNC;
			rin.fd = 1;
			cmd = ft_redircmd(&rin);
		}
		else if(tok == '+')
		{
			rin.mode = O_WRONLY | O_CREAT | O_APPEND;
			rin.fd = 1;
			cmd = ft_redircmd(&rin);
		}
		else if (tok == 'h')
			cmd = ft_heredoccmd(cmd, tok_type, rin.file, rin.efile);
	}
	return (cmd);
}

t_cmd	*parseblock(char **ps, char *es)
{
	t_cmd	*cmd;

	if (!peek(ps, es, "("))
	{
		printf("parseblock '(' missing)");
		return(NULL);
	}
	gettoken(ps, es, 0, 0);
	cmd = parseandor(ps, es);
	if (!peek(ps, es, ")"))
	{
		printf("parseblock ')' missing)");
		return(free(cmd), NULL);
	}
	gettoken(ps, es, 0, 0);
	cmd = parseredirs(cmd, ps, es);
	return (cmd);
}

t_cmd	*parseexec(char **ps, char *es)
{
	t_execcmd	*cmd;
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
			*ps = backup;
			break;
		}
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		cmd->qtype[argc] = tok;
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

// NUL-terminate all the tree strings.
t_cmd	*nulterminate(t_cmd *cmd)
{
	int			i;
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
        *hd->lim_end = '\0';
        nulterminate(hd->cmd);
    }
	return (cmd);
}
