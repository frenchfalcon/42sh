/*
** sepa_func.c for sepa_func.c in /home/voinne_c/rendu/42sh/tree
** 
** Made by voinne_c
** Login   <voinne_c@epitech.net>
** 
** Started on  Mon May 12 21:55:19 2014 voinne_c
** Last update Fri May 30 18:25:57 2014 gottin_o
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "my_tree.h"

t_globalinfos	*l_db_redir(t_node *node, int *fd, t_globalinfos *info)
{
  char		*line;
  int		pipefd[2];

  if (pipe(pipefd) == -1)
    return (NULL);
  while (strcmp((line = get_next_line(0)), node->right->command) != 0)
    {
      write(pipefd[1], line, strlen(line));
      write(pipefd[1], "\n", 1);
    }
  close(pipefd[1]);
  dup2(pipefd[0], 0);
  if ((node->left->command == NULL) ||
      (node->left->command && node->left->command[0] != '\0'))
    {
      if ((info = exec(node->left, fd, info)) == NULL)
	return (NULL);
    }
  dup2(fd[0], 0);
  return (info);
}

t_globalinfos	*r_db_redir(t_node *node, int *fd, t_globalinfos *info)
{
  int		file_descriptor;

  file_descriptor = write_open_file(node->right->command, APPEND);
  if (file_descriptor == -1)
    return (info);
  if (dup2(file_descriptor, 1) == -1)
    return (NULL);
  if ((node->left->command == NULL) ||
      (node->left->command && node->left->command[0] != '\0'))
    {
      if ((info = exec(node->left, fd, info)) == NULL)
	return (NULL);
    }
  close(file_descriptor);
  if (dup2(fd[1], 1) == -1)
    return (NULL);
  return (info);
}

t_globalinfos	*l_redir(t_node *node, int *fd, t_globalinfos *info)
{
  int		file_descriptor;

  file_descriptor = read_open_file(node->right->command);
  if (file_descriptor == -1)
    {
      info->exec_status = 1;
      return (info);
    }
  dup2(file_descriptor, 0);
  if ((node->left->command == NULL) ||
      (node->left->command && node->left->command[0] != '\0'))
    {
      if ((info = exec(node->left, fd, info)) == NULL)
	return (NULL);
    }
  dup2(fd[0], 0);
  close(file_descriptor);
  return (info);
}

t_globalinfos	*r_redir(t_node *node, int *fd, t_globalinfos *info)
{
  int		file_descriptor;

  file_descriptor = write_open_file(node->right->command, TRUNC);
  if (file_descriptor == -1)
    return (info);
  if (dup2(file_descriptor, 1) == -1)
    return (NULL);
  if ((node->left->command == NULL) ||
      (node->left->command && node->left->command[0] != '\0'))
    {
      if ((info = exec(node->left, fd, info)) == NULL)
	return (NULL);
    }
  if (dup2(fd[1], 1) == -1)
    return (NULL);
  close(file_descriptor);
  return (info);
}
