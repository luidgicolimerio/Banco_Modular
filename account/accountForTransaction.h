int updateCheckingAccountBal(long int CPF, double val);
/*
 * nome: updateCheckingAccountBal
 *
 * acoplamento:
 *   param 1 — CPF: número de CPF do cliente
 *   param 2 — val: valor a ser somado ou subtraído do saldo
 *   ret 1 — int: código de status (0 = sucesso, 1 = saldo insuficiente, 2 = CPF não encontrado)
 *
 * condições de acoplamento:
 *   - CPF deve estar presente na árvore de contas correntes
 *   - Saldo não pode ficar negativo
 *
 * descrição:
 *   Atualiza o saldo da conta corrente identificada por CPF, se existir e se a operação for válida.
 *
 * hipóteses:
 *   - A árvore de contas correntes foi corretamente inicializada
 *   - O CPF é válido e único
 *
 * restrições:
 *   - Não permite saldo negativo
 *   - A árvore está acessível pela variável global checkingRoot
 *
 * pseudo instruções:
 *   // Busca o CPF na árvore de contas correntes
 *   // Se não encontrado, retorna 2
 *   // Se a operação deixaria o saldo negativo, retorna 1
 *   // Caso contrário, aplica a variação ao saldo e retorna 0
 */
int updateSavingsAccountBal(long int CPF, double val);
/*
 * nome: updateSavingsAccountBal
 *
 * acoplamento:
 *   param 1 — CPF: número do CPF do cliente
 *   param 2 — val: valor a aplicar no saldo
 *   ret 1 — int: código de status (0 = sucesso, 1 = saldo insuficiente, 2 = CPF não encontrado)
 *
 * condições de acoplamento:
 *   - CPF deve estar presente na árvore de contas poupança
 *   - Saldo não pode ficar negativo após a operação
 *
 * descrição:
 *   Atualiza o saldo da conta poupança identificada por CPF com o valor especificado.
 *
 * hipóteses:
 *   - A árvore AVL de contas poupança está corretamente construída
 *
 * restrições:
 *   - Não permite saldo negativo
 *
 * pseudo instruções:
 *   // Busca o CPF na árvore de contas poupança
 *   // Se não encontrado, retorna 2
 *   // Se a operação deixaria o saldo negativo, retorna 1
 *   // Caso contrário, aplica o valor ao saldo e retorna 0
 */