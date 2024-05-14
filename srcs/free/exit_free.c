/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedi <hedi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:02:38 by vabertau          #+#    #+#             */
/*   Updated: 2024/05/14 06:23:46 by hedi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_exec(t_data *data)
{
	t_exec	*tmp_exec;
	t_exec	*tmp_exec_tf;

	tmp_exec = data->exec;
	while (tmp_exec)
	{
		if (tmp_exec->redir)
			free(tmp_exec->redir);
		if (tmp_exec->full_cmd)
			free(tmp_exec->full_cmd);
		if (tmp_exec->split_cmd)
			free(tmp_exec->split_cmd);
		tmp_exec_tf = tmp_exec;
		tmp_exec = tmp_exec->next;
		free(tmp_exec_tf);
	}
}

void	free_all(t_data *data)
{
	int	i;

	i = -1;
	if (data->cmdline != NULL)
		free(data->cmdline);
	if (data->token)
		free_token(data->token);
	if (data->exec)
		free_exec(data);
	if (data->is_bq)
		free(data->is_bq);
	if (data->pipe_fds)
		free(data->pipe_fds);
	if (data->char_env)
	{
		while (data->char_env[++i])
			if (data->char_env[i])
				free(data->char_env[i]);
		free(data->char_env);
	}
}

void	exit_free_perror(char *s, t_data *data, int exit_code)
{
	ft_putstr_fd(s, 2);
	free_all(data);
	free_env(data);
	exit(exit_code);
}

void	exit_free(t_data *data, int exit_code)
{
	free_all(data);
	free_env(data);
	exit(exit_code);
}

void	set_exitloop_free(t_data *data)
{
	data->sh_exit_loop = 1;
}
