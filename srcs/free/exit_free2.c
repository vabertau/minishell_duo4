/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hedi <hedi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:44:07 by vabertau          #+#    #+#             */
/*   Updated: 2024/05/14 06:25:37 by hedi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_token(t_token *token)
{
	t_token	*tmp;

	while (token != NULL)
	{
		tmp = token;
		token = token->next;
		if (tmp->word != NULL)
			free(tmp->word);
		if (tmp->full_path)
		{
			unlink(tmp->full_path);
			free(tmp->full_path);
		}
		free(tmp);
	}
}

void	free_bf_newprompt(t_data *data)
{
	if (data->cmdline != NULL)
		free(data->cmdline);
	free_token(data->token);
}

void	free_single_env(t_env *e)
{
	if (!e)
		return ;
	if (e->val)
		free(e->val);
	if (e->var)
		free(e->var);
	if (e->var_name)
		free(e->var_name);
	free(e);
	e = NULL;
}

void	free_env(t_data *shell)
{
	t_env	*tmp;
	int		i;

	i = -1;
	while (shell->env)
	{
		tmp = shell->env->next;
		free_single_env(shell->env);
		shell->env = tmp;
	}
}
