/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedi <hedi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 00:46:16 by hedi              #+#    #+#             */
/*   Updated: 2024/05/14 06:20:56 by hedi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(char **split_cmd, t_data *shell)
{
	int	ret;

	ret = 0;
	if (split_cmd[2])
	{
		ft_putstr_fd("cd: too many arguments", 2);
		ret = 1;
	}
	if (split_cmd[1] == NULL || chdir(split_cmd[1]) != 0)
	{
		exit_free_perror("cd", shell, EXIT_FAILURE);
	}
	return (ret);
}

int	ft_pwd(t_data *shell)
{
	char *cwd;
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		free(cwd);
		return (0);
	}
	exit_free_perror("pwd", shell, 1);
	return (1);
}