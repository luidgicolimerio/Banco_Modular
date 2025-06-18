int updateCheckingAccountBal(long int CPF, double val);
/*
 * nome: updateCheckingAccountBal
 *
 * acoplamento:
 *   param 1 � CPF: n�mero de CPF do cliente
 *   param 2 � val: valor a ser somado ou subtra�do do saldo
 *   ret 1 � int: c�digo de status (0 = sucesso, 1 = saldo insuficiente, 2 = CPF n�o encontrado)
 *
 * condi��es de acoplamento:
 *   - CPF deve estar presente na �rvore de contas correntes
 *   - Saldo n�o pode ficar negativo
 *
 * descri��o:
 *   Atualiza o saldo da conta corrente identificada por CPF, se existir e se a opera��o for v�lida.
 *
 * hip�teses:
 *   - A �rvore de contas correntes foi corretamente inicializada
 *   - O CPF � v�lido e �nico
 *
 * restri��es:
 *   - N�o permite saldo negativo
 *   - A �rvore est� acess�vel pela vari�vel global checkingRoot
 *
 * pseudo instru��es:
 *   // Busca o CPF na �rvore de contas correntes
 *   // Se n�o encontrado, retorna 2
 *   // Se a opera��o deixaria o saldo negativo, retorna 1
 *   // Caso contr�rio, aplica a varia��o ao saldo e retorna 0
 *
 * Assertiva de entrada:
 *   - CPF > 0
 *   - val pode ser positivo (depósito) ou negativo (saque)
 * Assertiva de saída:
 *   - Se retorno 0, saldo da conta corrente foi atualizado.
 */
int updateSavingsAccountBal(long int CPF, double val);
/*
 * nome: updateSavingsAccountBal
 *
 * acoplamento:
 *   param 1 � CPF: n�mero do CPF do cliente
 *   param 2 � val: valor a aplicar no saldo
 *   ret 1 � int: c�digo de status (0 = sucesso, 1 = saldo insuficiente, 2 = CPF n�o encontrado)
 *
 * condi��es de acoplamento:
 *   - CPF deve estar presente na �rvore de contas poupan�a
 *   - Saldo n�o pode ficar negativo ap�s a opera��o
 *
 * descri��o:
 *   Atualiza o saldo da conta poupan�a identificada por CPF com o valor especificado.
 *
 * hip�teses:
 *   - A �rvore AVL de contas poupan�a est� corretamente constru�da
 *
 * restri��es:
 *   - N�o permite saldo negativo
 *
 * pseudo instru��es:
 *   // Busca o CPF na �rvore de contas poupan�a
 *   // Se n�o encontrado, retorna 2
 *   // Se a opera��o deixaria o saldo negativo, retorna 1
 *   // Caso contr�rio, aplica o valor ao saldo e retorna 0
 *
 * Assertiva de entrada:
 *   - CPF > 0
 *   - val pode ser positivo ou negativo
 * Assertiva de saída:
 *   - Se retorno 0, saldo da conta poupança foi atualizado.
 */