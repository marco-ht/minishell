#include "../includes/minishell.h"

char	*get_env_value(const char *key, char **envp)
{
	int	key_len;

	if (!key || !envp)
		return ("");
	key_len = ft_strlen(key);
	while (*envp)
	{
		if (ft_strncmp(*envp, key, key_len) == 0 && (*envp)[key_len] == '=')
		{
			return (&(*envp)[key_len + 1]);
		}
		envp++;
	}
	return ("");
}

static void	perform_expansion(char **arg_ptr, char **envp)
{
	char	buffer[4096];
	char	*arg;
	int		i;
	int		j;
	char	var_name[256];
	char	*var_value;

	arg = *arg_ptr;
	ft_bzero(buffer, sizeof(buffer));
	i = 0;
	while (*arg)
	{
		if (*arg == '$' && (ft_isalpha(*(arg + 1)) || *(arg + 1) == '_'))
		{
			arg++;
			ft_bzero(var_name, sizeof(var_name));
			j = 0;
			while (ft_isalnum(*arg) || *arg == '_')
				var_name[j++] = *arg++;
			var_value = get_env_value(var_name, envp);
			ft_strlcat(buffer, var_value, sizeof(buffer));
			i = ft_strlen(buffer);
		}
		else
			buffer[i++] = *arg++;
	}
	free(*arg_ptr);
	*arg_ptr = ft_strdup(buffer);
}

void	expand_variables(t_execcmd *ecmd, char **envp)
{
	int		i;
	char	*temp_arg;

	i = 0;
	while (ecmd->argv[i])
	{
		if (ecmd->qtype[i] != 's' && ft_strchr(ecmd->argv[i], '$'))
		{
			temp_arg = ft_strdup(ecmd->argv[i]);
			perform_expansion(&temp_arg, envp);
			if (ecmd->allocated[i])
				free(ecmd->argv[i]);
			ecmd->argv[i] = temp_arg;
			ecmd->allocated[i] = 1;
		}
		i++;
	}
}
