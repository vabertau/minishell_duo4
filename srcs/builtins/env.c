/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedi <hedi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 00:47:00 by hedi              #+#    #+#             */
/*   Updated: 2024/05/14 07:28:34 by hedi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strndup_var(const char *s, int n)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	while (s[i])
		i++;
	if (i <= n)
		ret = malloc(sizeof(char) * (i + 1));
	else
		ret = malloc(sizeof(char) * (n + 1));
	if (!ret)
		return (0);
	while (j < i && j < n)
	{
		ret[j] = s[j];
		j++;
	}
	if (ret[j - 1] == '=')
		--j;
	ret[j] = '\0';
	return (ret);
}

char	**malloc_var(int i, char *s, int j, int n)
{
	char	**ret;

	ret = malloc(sizeof(char *) * (n + 1));
	if (!ret)
		return (NULL);
	ret[0] = ft_strndup_var(s, (j));
	if (!ret[0])
	{
		free(ret);
		return (NULL);
	}
	if (n == 2)
	{
		ret[1] = ft_strdup(s + i);
		if (!ret[1])
		{
			free(ret[0]);
			free(ret);
			return (NULL);
		}
		ret[2] = NULL;
	}
	else
		ret[1] = NULL;
	return (ret);
}
char	**split_var(char *s, t_data *shell)
{
	int		i;
	int		n;
	int		j;
	char	**ret;

	i = 0;
	while (s && s[i] && s[i] != '=' && s[i] != '+')
		i++;
	n = 1;
	j = i;
	if (s[i])
		n = 2;
	if (s[i] == '+' && s[i + 1] == '=')
		i += 2;
	else if (s[i] == '=')
		i++;
	ret = malloc_var(i, s, j, n);
	return (ret);
}
int	find_index_env(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

int	have_equal(char *env)
{
	int	i;

	i = -1;
	while (env[++i])
		if (env[i] == '=')
			return (i);
	return (0);
}

int	var_in_env(char *s, t_data *shell)
{
	int		ret;
	int		j;
	t_env	*tmp;
	char	**var;

	var = split_var(s, shell);
	tmp = shell->env;
	if (!var)
		exit_free_perror("malloc", shell, -2);
	ret = -1;
	j = -1;
	while (tmp)
	{
		if (!ft_strcmp(tmp->var_name, var[0]))
			ret = tmp->index;
		tmp = tmp->next;
	}
	if (var[1])
		free(var[1]);
	free(var[0]);
	free(var);
	return (ret);
}
int	check_plus(char *s)
{
	int	i;

	i = 0;
	while (s && s[i] && s[i] != '=' && s[i] != '+')
		i++;
	if (s[i] == '+')
		return (1);
	else
		return (0);
	return (-1);
}

void	ft_update_env(char *str, t_data *shell, int pos)
{
	char	**s;
	t_env	*tmp;
	char	*new_val;

	s = split_var(str, shell);
	if (!s)
		return ;
	tmp = shell->env;
	while (tmp && tmp->index < pos)
		tmp = tmp->next;
	if (!tmp)
		return ;
	if (!s[1] || !s[1][0])
	{ // Cas où la valeur est vide ou non définie
		free(tmp->var);
		tmp->var = ft_strjoin(tmp->var_name, "=\"\""); // Ajout de guillemets
		free(tmp->val);
		tmp->val = ft_strdup("");
	}
	else if (check_plus(str))
	{
		if (!tmp)
		{
			fprintf(stderr, "bash: export: `%s': not valid in this context\n",
				str);
			free(s[0]);
			if (s[1])
				free(s[1]);
			free(s);
			return ;
		}
		new_val = join_free1(tmp->val, s[1]);
		free(tmp->val);
		tmp->val = new_val;
		free(tmp->var);
		tmp->var = join_free1(ft_strjoin(tmp->var_name, "="), tmp->val);
	}
	else
	{
		free(tmp->val);
		tmp->val = ft_strdup(s[1]);
		free(tmp->var);
		tmp->var = join_free1(ft_strjoin(tmp->var_name, "="), tmp->val);
	}
	free(s[0]);
	if (s[1])
		free(s[1]);
	free(s);
}

void	ft_add_env(char *s, t_data *shell)
{
	char	**str;
	t_env	*new_node;
	t_env	*e;

	str = split_var(s, shell);
	if (!str)
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		exit_free_perror("malloc", shell, -2);
	new_node->var_name = ft_strdup(str[0]);
	new_node->next = NULL;
	if ((!str[1] || !str[1][0]) && have_equal(s))
	{
		// Cas où `export yy=` est utilisé,

		new_node->var = ft_strjoin(str[0], "=\"\"");
		new_node->val = ft_strdup("\"\"");
	}
	else if (str[1] != NULL)
	{
		// Cas où une valeur est assignée, par exemple `export yy=somevalue`
		new_node->var = join_free1(ft_strjoin(str[0], "="), str[1]);
		new_node->val = ft_strdup(str[1]);
	}
	else
	{
		// Cas où seulement `export yy` est utilisé, sans `=`
		new_node->var = ft_strdup(str[0]);
		new_node->val = NULL; // Pas de valeur assignée
	}
	if (shell->env == NULL)
	{
		shell->env = new_node;
		new_node->index = 0;
	}
	else
	{
		e = shell->env;
		while (e->next != NULL)
			e = e->next;
		e->next = new_node;
		new_node->index = e->index + 1;
	}
	// Libérer la mémoire allouée par split_var
	free(str[0]);
	if (str[1] != NULL)
		free(str[1]);
	free(str);
}

int	ft_putenv(char *s, t_data *shell)
{
	int	pos;

	pos = var_in_env(s, shell);
	if (pos > -1)
		ft_update_env(s, shell, pos);
	else
		ft_add_env(s, shell);
	return (1);
}

void	ft_printf_var_env(char *var)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, var, ft_strlen(var));
	write(STDOUT_FILENO, "\n", 1);
}

int	check_s(char *s)
{
	int	i;

	i = -1;
	while (++i && s && s[i])
		if (s[i] != '_' && !ft_isalpha(s[i]))
			return (0);
	return (1);
}

int	check_cmd(char *s)
{
	int	i;

	i = 0;
	if (!s[0] || (s[0] != '_' && !ft_isalpha(s[0])))
		return (0);
	while (++i && s && s[i] && s[i] != '=' && s[i] != '+')
		if ((s[i] != '_' && !ft_isalnum(s[i])))
			return (0);
	if (s[i] == '+' && s[i + 1] != '=')
		return (0);
	return (1);
}

int	ft_export(char **split_cmd, t_data *shell)
{
	int		i;
	char	*tmp;
	int		ret;
	t_env	*e;

	ret = 0;
	e = shell->env;
	if (split_cmd[1] == NULL)
	{
		i = -1;
		while (e)
		{
			ft_printf_var_env(e->var);
			e = e->next;
		}
	}
	i = 0;
	while (split_cmd[++i] != NULL)
	{
		if (!check_cmd(split_cmd[i]))
		{
			tmp = ft_strjoin("bash: export: `", split_cmd[i]);
			ft_putstr_fd_free(join_free1(tmp, "': not a valid identifier\n"),
				2);
			ret = 1;
		}
		else if (ft_putenv(split_cmd[i], shell) != 1)
			exit_free_perror("export", shell, 1);
	}
	return (ret);
}

void	decrem_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		tmp->index--;
		tmp = tmp->next;
	}
}

int	ft_unset(t_data *shell, char **split_cmd)
{
	int		i;
	t_env	*prev;
	t_env	*current;
	int		ret;

	i = 1;
	ret = 0;
	while (split_cmd[i])
	{
		if (!check_s(split_cmd[i]))
		{
			ft_putstr_fd("bash: unset: `", 2);
			ft_putstr_fd(split_cmd[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
			continue ;
		}
		current = shell->env;
		prev = NULL;
		while (current != NULL && ft_strcmp(current->var_name,
				split_cmd[i]) != 0)
		{
			prev = current;
			current = current->next;
		}
		if (current != NULL)
		{
			if (prev == NULL)
				shell->env = current->next;
			else
				prev->next = current->next;
			free_single_env(current);
			decrem_env(prev ? prev->next : shell->env);
		}
		i++;
	}
	return (ret);
}

int	ft_env(t_data *shell, char **f)
{
	t_env *tmp;

	tmp = shell->env;
	if (f[1])
		return ((void)ft_putstr_fd("No such file or directory", 2), 127);
	while (tmp)
	{
		ft_putstr_fd(tmp->var, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		tmp = tmp->next;
	}
	return (0);
}