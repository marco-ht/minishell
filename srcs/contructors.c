#include "../includes/minishell.h"

t_cmd   *ft_execcmd(void)
{
    t_execcmd *cmd;
    
    cmd = malloc(sizeof(* cmd));
    memset(cmd, 0, sizeof(* cmd));
    cmd ->type = EXEC;
    return(t_cmd *) cmd;
}
t_cmd   *ft_pipecmd(t_cmd *left, t_cmd * right)
{
    t_pipecmd   *cmd;

    cmd = malloc(sizeof(* cmd));
    memset(cmd, 0, sizeof(* cmd));
    cmd->type = pipe;
    cmd -> left = left;
    cmd ->right = right;
    return (t_cmd *) cmd;
}
//fai file[0] = file e file[1] = efile

t_cmd   *ft_redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd)
{
    t_redircmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = REDIR;
    cmd->cmd = subcmd;
    cmd->file = file;
    cmd->efile = efile;
    cmd->mode = mode;
    cmd->fd;
    return(t_cmd *) cmd;
}

t_cmd   *ft_listcmd(t_cmd *left, t_cmd *right)
{
    t_listcmd *cmd;

    cmd = malloc(sizeof(* cmd));
    memset(cmd, 0, sizeof(* cmd));
    cmd->type = LIST;
    cmd->left = left;
    cmd->right = right;
    return(t_cmd *) cmd;
}
