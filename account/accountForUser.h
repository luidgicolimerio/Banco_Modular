int createSavingsAccount(long int CPF);
/*
 * nome: createSavingsAccount
 *
 * acoplamento:
 *   param 1 — CPF: numero de CPF do cliente
 *   ret 1 — int: codigo de status (0 = sucesso, 1 = conta ja existe, -1 = erro de alocacao)
 *
 * condicoes de acoplamento:
 *   - CPF deve ser valido e nao pode existir previamente na arvore de contas poupanca
 *
 * descricao:
 *   Cria uma nova conta poupanca com saldo 0 e juros 0 para o CPF fornecido e insere na arvore AVL.
 *
 * hipoteses:
 *   - A estrutura da arvore AVL esta valida
 *
 * restricoes:
 *   - A conta nao pode ser duplicada
 *   - O CPF deve ser unico na arvore
 *
 * pseudo instrucoes:
 *   // Verifica se o CPF ja existe
 *   // Se sim, retorna 1
 *   // Aloca nova struct
 *   // Insere na arvore e retorna 0
 *   // Em caso de falha de alocacao, retorna -1
 */

int createCheckingAccount(long int CPF);
/*
 * nome: createCheckingAccount
 *
 * acoplamento:
 *   param 1 — CPF: numero de CPF do cliente
 *   ret 1 — int: codigo de status (0 = sucesso, 1 = conta ja existe, -1 = erro de alocacao)
 *
 * condicoes de acoplamento:
 *   - CPF deve ser valido e unico na arvore de contas correntes
 *
 * descricao:
 *   Cria uma nova conta corrente com saldo 0 para o CPF especificado e insere na arvore AVL.
 *
 * hipoteses:
 *   - A arvore AVL esta funcionando corretamente
 *
 * restricoes:
 *   - O CPF nao pode estar duplicado
 *
 * pseudo instrucoes:
 *   // Verifica se o CPF ja existe
 *   // Se sim, retorna 1
 *   // Aloca struct da conta corrente
 *   // Insere na arvore e retorna 0
 *   // Se malloc falhar, retorna -1
 */
