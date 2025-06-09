#include "../includes/minishell.h"

static void print_indent(int depth)
{
    int i;
    
    for (i = 0; i < depth; i++)
        printf("    ");
}

static void print_node_type(int type)
{
    switch(type)
    {
        case EXEC:
            printf("[EXEC]");
            break;
        case REDIR:
            printf("[REDIR]");
            break;
        case PIPE:
            printf("[PIPE]");
            break;
        case AND:
            printf("[AND]");
            break;
        case OR:
            printf("[OR]");
            break;
        case HEREDOC:
            printf("[HEREDOC]");
            break;
        case BUILTIN:
            printf("[BUILTIN]");
            break;
        default:
            printf("[UNKNOWN]");
            break;
    }
}

static void print_argv(char **argv)
{
    int i;
    
    printf(" argv: [");
    if (argv[0] == NULL)
    {
        printf("NULL");
    }
    else
    {
        for (i = 0; argv[i] != NULL && i < MAX_ARGS; i++)
        {
            if (i > 0)
                printf(", ");
            printf("\"%s\"", argv[i]);
        }
    }
    printf("]");
}

static void print_qtype(char *argv)
{
    int i;
    
    printf(" qtype: [");
        for (i = 0; i < MAX_ARGS; i++)
        {
            if (i > 0)
                printf(", ");
            printf("\"%c\"", argv[i]);
        }
    printf("]");
}

static void print_allocated(int *argv)
{
    int i;
    
    printf(" allocated: [");
        for (i = 0; i < MAX_ARGS; i++)
        {
            if (i > 0)
                printf(", ");
            printf("\"%d\"", argv[i]);
        }
    printf("]");
}

static void print_redirection_info(t_redircmd *rcmd)
{
    printf(" file: \"%s\"", rcmd->file ? rcmd->file : "NULL");
    printf(", mode: ");
    
    // Mostra il tipo di redirezione basandosi sul mode
    if (rcmd->mode == O_RDONLY)
        printf("READ(<)");
    else if (rcmd->mode == (O_WRONLY | O_CREAT | O_TRUNC))
        printf("WRITE(>)");
    else if (rcmd->mode == (O_WRONLY | O_CREAT | O_APPEND))
        printf("APPEND(>>)");
    else
        printf("0x%x", rcmd->mode);
    
    printf(", fd: %d", rcmd->fd);
}

void ft_printtree_recursive(t_cmd *cmd, int depth)
{
    t_execcmd *ecmd;
    t_pipecmd *pcmd;
    t_redircmd *rcmd;
    t_andcmd *acmd;
	t_orcmd *ocmd;
	t_heredoccmd *hcmd;

    if (cmd == NULL)
    {
        print_indent(depth);
        printf("NULL\n");
        return;
    }

    print_indent(depth);
    print_node_type(cmd->type);

    if (cmd->type == BUILTIN || cmd->type == EXEC)
    {
        ecmd = (t_execcmd *)cmd;
        print_argv(ecmd->argv);
        printf("\n");
        print_qtype(ecmd->qtype);
        printf("\n");
        print_allocated(ecmd->allocated);
        printf("\n");
    }
    else if (cmd->type == REDIR)
    {
        rcmd = (t_redircmd *)cmd;
        print_redirection_info(rcmd);
        printf("\n");
        
        print_indent(depth);
        printf("└── subcmd:\n");
        ft_printtree_recursive(rcmd->cmd, depth + 1);
    }
    else if (cmd->type == AND)
    {
        acmd = (t_andcmd *)cmd;
        printf("\n");
        
        print_indent(depth);
        printf("├── left:\n");
        ft_printtree_recursive(acmd->left, depth + 1);
        
        print_indent(depth);
        printf("└── right:\n");
        ft_printtree_recursive(acmd->right, depth + 1);
    }
    else if (cmd->type == OR)
    {
        ocmd = (t_orcmd *)cmd;
        printf("\n");
        
        print_indent(depth);
        printf("├── left:\n");
        ft_printtree_recursive(ocmd->left, depth + 1);
        
        print_indent(depth);
        printf("└── right:\n");
        ft_printtree_recursive(ocmd->right, depth + 1);
    }
    else if (cmd->type == HEREDOC)
    {
        hcmd = (t_heredoccmd *)cmd;
        printf("\n");
        
        print_indent(depth);
        printf("└── subcmd:\n");
        ft_printtree_recursive(hcmd->cmd, depth + 1);
        
        print_indent(depth);
        printf("└── limiter: %s\n", hcmd->lim_start);
    }
    else if (cmd->type == PIPE)
    {
        pcmd = (t_pipecmd *)cmd;
        printf("\n");
        
        print_indent(depth);
        printf("├── left (stdout):\n");
        ft_printtree_recursive(pcmd->left, depth + 1);
        
        print_indent(depth);
        printf("└── right (stdin):\n");
        ft_printtree_recursive(pcmd->right, depth + 1);
    }
    else
    {
        printf(" [TIPO SCONOSCIUTO: %d]\n", cmd->type);
    }
}

void ft_printtree(t_cmd *cmd)
{
    printf("\n=== STRUTTURA ALBERO COMANDI ===\n");
    ft_printtree_recursive(cmd, 0);
    printf("================================\n\n");
}
