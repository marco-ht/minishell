#include "../includes/minishell.h"

void    ft_runcmd(t_cmd *cmd)
{
    int p[2];
    t_execcmd  *ecmd;
    /* t_listcmd   *lcmd; */
    t_pipecmd   *pcmd;
    /* t_redircmd  *rcmd; */

    /* if(cmd->type == BUILTIN) //builtin nel parent
    {
        ft_builtin(cmd);
        return;
    } */

    if(cmd == NULL)
        return;
    if(cmd->type == EXEC)
    {
        ecmd = (t_execcmd *) cmd;
        if(ecmd->argv[0] == NULL)
            exit(1);
        execvp(ecmd->argv[0], ecmd->argv);
        printf("exec %s failded\n", ecmd->argv[0]);
        exit(1);
    }
    else if(cmd->type == PIPE)
    {
        pcmd = (t_pipecmd *) cmd;
        if(pipe(p) < 0)
            ft_exit_err("pipe");
        if(fork1() == 0)
        {
            dup2(p[1], 1);
            close(p[0]);
            close(p[1]);
            ft_runcmd(pcmd->left);
        }
        if(fork1() == 0)
        {
            dup2(p[0], 0);
            close(p[0]);
            close(p[1]);
            ft_runcmd(pcmd->right);
        }
        close(p[0]);
        close(p[1]);
        wait(NULL);
        wait(NULL);
        exit(0);
    }
    else
    {
        if(fork1() == 0)
            ft_exit_err("runcmd error\n");
    }
    exit(0);
}
