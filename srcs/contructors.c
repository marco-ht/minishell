#include "../includes/minishell.h"

t_cmd *ft_execcmd(void)
{
	t_execcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
    	ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return (t_cmd *)cmd;
}

t_cmd *ft_pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd *cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
    	ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return (t_cmd *)cmd;
}
// fai file[0] = file e file[1] = efile per ridurre a 4 variabili

t_cmd *ft_redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd, int allocated)
{
	t_redircmd *cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
    	ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	cmd->fd = fd;
	cmd->allocated = allocated;
	return (t_cmd *)cmd;
}

t_cmd *ft_andcmd(t_cmd *left, t_cmd *right)
{
	t_andcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
    	ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = AND;
	cmd->left = left;
	cmd->right = right;
	return (t_cmd *)cmd;
}

t_cmd *ft_orcmd(t_cmd *left, t_cmd *right)
{
	t_orcmd *cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
    	ft_exit_err("malloc");
	ft_memset(cmd, 0, sizeof(*cmd));
	cmd->type = OR;
	cmd->left = left;
	cmd->right = right;
	return (t_cmd *)cmd;
}
t_cmd *ft_heredoccmd(t_cmd *subcmd, int	tok_type, char *lim_start, char *lim_end)
{
    t_heredoccmd *hd;

	hd = malloc(sizeof(*hd));
	if (!hd)
    	ft_exit_err("malloc");
    ft_memset(hd, 0, sizeof(*hd));
	hd->expand = 0;
	if (tok_type == 'a')
		hd->expand = 1;
    hd->type      = HEREDOC;
    hd->cmd       = subcmd;
    hd->lim_start = lim_start;
    hd->lim_end   = lim_end;
    return (t_cmd *)hd;
}
