# Minishell
FAIRE mettre a jour la version non interactive

<!-- MODIFIER get_next_line (multiple fd) -->
<!-- MODIFIER le heredoc pour le faire sans fork? -->
<!-- MODIFIER tout les signaux pour faire des fonctions avec sigaction? -->
<!-- MODIFIER enlever les exit et ne pas fork pour les builtin -->

- env qui copie pas l'env du Wesh actuel (il copie celui du bash), on sort pas notre env a la fin (je sais pas si c'est un prblm mais en ca incremente pas shlvl quand on wesh dans un wesh) //akip c'est normal

<!-- - PROBLEME avec ft_get_command de la libft (je crois c'est le extern g_exit_status qu'il aime pas) -->

tests qui ne marchent pas :

- echo $hola* //faut juste, dans l'execution, mettre le bloc wildcards apres celui d'expand mais je crois que ca posais prblm sur un cas c'est pour ca qu'on l'a mit dans cet ordre je crois
- echo ""$HOME* //c'est pas du tout logique celui la


A FAIRE: check les retour dexit status pour les tests

QUESTION: extern environ dans do_builtin_or_execve.c , t'es sur que ya besoin dans lexec et pas mieux de trouver le path dans notre env a nous ?


minshell.c
parser.h

env_utils2
env.c


leak de l'envs avec les pipes
leak de quotes pas fermees
leak de command not foud
export marche plus