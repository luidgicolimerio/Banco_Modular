#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#ifdef ACCESS_CONTROL_OWN
    #define ACCESS_SCOPE 
#else
    #define ACCESS_SCOPE extern
#endif

#include <stdio.h>

// --------------------------------------
// Estrutura do login
typedef struct login {
    long int currentCPF;
    char* currentPassword;
} Login;

// Estrutura da árvore de logins
typedef struct loginTree {
    Login* loginData;
    struct loginTree* left;
    struct loginTree* right;
} LoginTree;

// --------------------------------------
// Variáveis globais
ACCESS_SCOPE LoginTree* loginRoot;
ACCESS_SCOPE Login* currentLogin;

// --------------------------------------
// Funções externas (interface principal)

//*****************************************************************************************************************
int login(long int CPF, char* password);
/*
-->Nome: login

-->Objetivo: Autenticar um usuário com base em seu CPF e senha, e armazenar o login atual em uma variável global.

-->Acoplamento:
    - Parâmetros: long int CPF, char* password
    - Retorno: int (códigos de erro ou sucesso)

-->Condições de acoplamento:
    Assertiva de entrada: 
        - O CPF deve ser um número positivo (> 0).
        - A senha deve ser uma string válida.
    Assertiva de saída: 
        - Retorna 0 se o login for bem-sucedido.
        - Retorna 1 se o CPF for inválido.
        - Retorna 2 se o CPF não for encontrado na árvore de logins.
        - Retorna 3 se a senha estiver incorreta.

-->Descrição: 
    A função realiza autenticação comparando o CPF fornecido com os registros existentes na árvore `loginRoot`.
    Se a autenticação for bem-sucedida, armazena o ponteiro para o login atual em `currentLogin`.

-->Hipóteses: 
    - A árvore `loginRoot` já foi inicializada previamente com logins válidos.
    - A senha passada é segura e terminada por `\0`.
    - A função `findLogin` retorna um ponteiro válido ou `NULL`.
    
-->Restrições: 
    - O sistema só permite um login ativo por vez (armazenado em `currentLogin`).
    - Não há tratamento para múltiplos logins simultâneos.
    - O login falha silenciosamente para CPFs inválidos ou senhas incorretas, sem diferenciar em logs.
*/
//*****************************************************************************************************************
int getLoginCPF(long int* CPF);
/*
-->Nome: getLoginCPF

-->Objetivo:  
    Recuperar o CPF do usuário atualmente logado no sistema.

-->Acoplamento:  
    - Parâmetros: ponteiro para long int (long int* CPF)  
    - Retorno: int (códigos de erro ou sucesso)

-->Condições de acoplamento:  
    Assertiva de entrada:  
        - O ponteiro `CPF` deve ser válido (não nulo).
    Assertiva de saída:  
        - O valor apontado por `CPF` conterá o CPF do usuário logado se o retorno for 0.
        - Retorna 1 se nenhum usuário estiver logado.

-->Descrição:  
    A função verifica se existe um usuário atualmente autenticado (armazenado na variável global `currentLogin`).  
    Se houver, copia seu CPF para a variável apontada por `CPF`.  
    Caso contrário, indica erro.

-->Hipóteses:  
    - A variável global `currentLogin` representa corretamente o estado de autenticação do sistema.
    - A variável `CPF` passada como argumento é válida e pode receber o valor do CPF.

-->Restrições:  
    - A função só é útil após um login bem-sucedido.
    - O ponteiro `CPF` precisa estar previamente alocado pelo chamador.
*/
//*****************************************************************************************************************
int registerUser(char* name, char* numeroTel, char* birthdate, long int CPF, char* password);
/*
-->Nome: registerUser

-->Objetivo:  
    Criar um novo perfil de usuário no sistema e associar um login correspondente ao mesmo CPF.

-->Acoplamento:  
    - Parâmetros:
        - name (char*): nome do usuário
        - numeroTel (char*): número de telefone
        - birthdate (char*): data de nascimento
        - CPF (long int): identificador único do usuário
        - password (char*): senha de acesso
    - Retorno: int (indicando sucesso ou tipo de erro)

-->Condições de acoplamento:

    Assertiva de entrada:  
        - Todos os parâmetros devem ser válidos (ponteiros não nulos e CPF positivo).
        - A função `createUserProfile` deve validar os dados e retornar um código apropriado.

    Assertiva de saída:  
        - O login é adicionado à árvore global `loginRoot` se o cadastro for bem-sucedido.
        - Retorna:
            - 0 em caso de sucesso.
            - Valor > 0 se `createUserProfile` falhar.
            - -1 se houver erro de alocação.

-->Descrição:  
    A função encapsula a criação de um novo usuário. Primeiro, chama o módulo de perfil (USER) para criar o perfil do usuário.  
    Depois, aloca e insere o login correspondente (CPF + senha) na árvore de logins (`loginRoot`).

-->Hipóteses:  
    - A função `createUserProfile` realiza todas as validações de dados de entrada.
    - O CPF é único e ainda não está cadastrado no sistema (validado por `createUserProfile` ou pelo chamador).
    - A senha é válida e segura, conforme já validado no módulo USER.

-->Restrições:  
    - A árvore `loginRoot` deve estar acessível e manipulável.
    - O sistema depende de memória heap (uso de `malloc` e `strdup`).
    - A função assume que o módulo USER é confiável e trata os erros de validação.

*/
//*****************************************************************************************************************
int logout(long int CPF);
/*
-->Nome: logout

-->Objetivo:  
    Finalizar a sessão do usuário autenticado no sistema, liberando o login atual da memória.

-->Acoplamento:  
    - Parâmetro:
        - CPF (long int): número identificador do usuário que deseja deslogar.
    - Retorno: int
        - 0: logout realizado com sucesso.
        - 1: CPF não corresponde ao usuário logado.
        - 2: nenhum usuário está logado.

-->Condições de acoplamento:

    Assertiva de entrada:
        - O parâmetro CPF deve conter um valor positivo válido.
    
    Assertiva de saída:
        - A variável global `currentLogin` será definida como `NULL` caso o logout seja bem-sucedido.

-->Descrição:  
    A função verifica se há um usuário atualmente logado no sistema.  
    Se houver, verifica se o CPF fornecido corresponde ao login atual.  
    Se ambos os critérios forem atendidos, desloga o usuário limpando a variável global `currentLogin`.

-->Hipóteses:  
    - O CPF passado como argumento já foi autenticado previamente (ou ao menos deve ser comparado com o atual).
    - A sessão do usuário é representada apenas por `currentLogin`.

-->Restrições:  
    - O sistema não permite múltiplos logins simultâneos (apenas um usuário pode estar logado por vez).
    - A função não realiza nenhuma persistência em disco ou log; é puramente operacional na memória.

*/
//*****************************************************************************************************************
int changePassword(long int CPF, char* currentPass, char* newPass);
/*
-->Nome: changePassword

-->Objetivo:  
    Atualizar a senha do usuário atualmente logado, tanto na estrutura de login quanto no módulo de perfil de usuário.

-->Acoplamento:
    - Parâmetros:
        - CPF (long int): identificador do usuário.
        - currentPass (char*): senha atual informada pelo usuário.
        - newPass (char*): nova senha desejada pelo usuário.
    - Retorno: int
        - 0: senha alterada com sucesso.
        - 1: CPF não corresponde ao usuário logado.
        - 2: senha atual incorreta.
        - 3: nova senha inválida (menos de 8 caracteres).
        - 4: nenhum usuário logado.

-->Condições de acoplamento:

    Assertiva de entrada:
        - O CPF deve ser um número válido.
        - currentPass e newPass devem ser ponteiros válidos para strings terminadas em `\0`.

    Assertiva de saída:
        - A senha do usuário na estrutura de login e no perfil (via `alterPassword`) será atualizada com sucesso.

-->Descrição:
    Esta função verifica se há um usuário autenticado. Caso positivo, compara o CPF fornecido com o do usuário logado.
    Se o CPF for válido e a senha atual estiver correta, a senha será atualizada tanto na estrutura de login (`currentLogin`)
    quanto no módulo de usuário (função `alterPassword`).

-->Hipóteses:
    - O ponteiro `currentLogin` aponta para o usuário autenticado.
    - O módulo de usuário fornece a função `alterPassword`, que atualiza o backend do perfil do usuário.

-->Restrições:
    - A nova senha deve conter no mínimo 8 caracteres.
    - A operação será cancelada se não houver usuário logado.
    - A senha atual deve estar correta para autorizar a alteração.

*/
//*****************************************************************************************************************
int changeNumber(long int CPF, char* password, char* newNumber);
/*
-->Nome: changeNumber

-->Objetivo:  
    Alterar o número de telefone do usuário atualmente logado, após validação da senha e do novo número.

-->Acoplamento:
    - Parâmetros:
        - CPF (long int): identificador do usuário logado.
        - password (char*): senha atual do usuário.
        - newNumber (char*): novo número de telefone a ser associado ao perfil.
    - Retorno: int
        - 0: número alterado com sucesso.
        - 1: CPF não corresponde ao usuário logado.
        - 2: senha incorreta.
        - 3: novo número inválido.
        - 4: nenhum usuário logado.

-->Condições de acoplamento:

    Assertiva de entrada:
        - CPF deve ser válido (positivo e existente na base de usuários).
        - password e newNumber devem ser ponteiros válidos e strings terminadas em `\0`.

    Assertiva de saída:
        - O número de telefone do usuário será atualizado com sucesso no sistema de perfis.

-->Descrição:
    Esta função permite alterar o número de telefone de um usuário que está autenticado no sistema.
    Para isso, valida se:
    - o usuário está logado;
    - o CPF informado é do usuário logado;
    - a senha fornecida está correta;
    - o novo número é válido (10 ou 11 dígitos numéricos).
    Caso todas as validações sejam satisfeitas, chama a função `alterNumber` do módulo USER para efetivar a mudança.

-->Hipóteses:
    - A função `alterNumber` do módulo USER é confiável e realiza corretamente a alteração no backend.
    - A estrutura `currentLogin` está corretamente preenchida no momento do login.

-->Restrições:
    - O novo número de telefone deve conter entre 10 e 11 dígitos numéricos.
    - O usuário precisa estar autenticado.
    - A senha precisa ser confirmada corretamente para autorizar a alteração.

*/
//*****************************************************************************************************************
int saveLogins(FILE* file, LoginTree* root);
/*
-->Nome: saveLogins

-->Objetivo:
    Salvar todos os logins presentes na árvore binária em um arquivo de texto, utilizando o formato "CPF;senha\n".

-->Acoplamento:
    - Parâmetros:
        - file (FILE*): ponteiro para o arquivo onde os dados serão gravados.
        - root (LoginTree*): raiz da árvore binária de logins.
    - Retorno: int
        - 0: sucesso na gravação.
        - 1: ponteiro de arquivo inválido (nulo).
        - 2: árvore vazia (nada a salvar).

-->Condições de acoplamento:

    Assertiva de entrada:
        - O ponteiro `file` deve ser válido (aberto para escrita).
        - O ponteiro `root` pode ser nulo (significando árvore vazia).

    Assertiva de saída:
        - O arquivo conterá uma linha para cada login, no formato `CPF;senha`.

-->Descrição:
    A função percorre recursivamente a árvore binária de logins (`LoginTree`) e escreve em um arquivo cada nó com os dados de login (CPF e senha), separados por ponto e vírgula. 
    Cada linha do arquivo representa um login.

-->Hipóteses:
    - A árvore está corretamente estruturada.
    - Cada nó da árvore possui `loginData` válido com CPF e senha.

-->Restrições:
    - O arquivo deve estar aberto e com permissão de escrita.
    - A senha deve ser uma string válida (terminada em '\0').
*/
//*****************************************************************************************************************
int readLogins(FILE* file, LoginTree** root);
/*
-->Nome: readLogins

-->Objetivo:
    Ler dados de logins armazenados em um arquivo texto e reconstruir a árvore binária de logins (`LoginTree`).

-->Acoplamento:
    - Parâmetros:
        - file (FILE*): ponteiro para o arquivo aberto em modo leitura.
        - root (LoginTree**): ponteiro para o ponteiro da raiz da árvore onde os logins serão inseridos.
    - Retorno: int
        - 0: sucesso na leitura e inserção.
        - 1: ponteiro de arquivo inválido (nulo).
        - 2: ponteiro para raiz da árvore inválido (nulo).
        - -1: erro de alocação de memória.

-->Condições de acoplamento:

    Assertiva de entrada:
        - `file` deve ser um ponteiro válido para um arquivo aberto para leitura.
        - `root` deve ser um ponteiro válido para a árvore de logins (mesmo que vazia inicialmente).

    Assertiva de saída:
        - A árvore `*root` conterá todos os logins lidos do arquivo.
        - O arquivo deve estar no formato: uma linha por login, no padrão `CPF;senha`.

-->Descrição:
    A função lê o arquivo linha por linha, extrai o CPF e a senha de cada login, cria um novo nó com esses dados
    e o insere na árvore binária de logins por meio da função `addLoginToTree`.

-->Hipóteses:
    - O conteúdo do arquivo segue o formato `long int CPF;string senha` por linha.
    - As senhas não contêm quebras de linha.
    - A função `addLoginToTree` gerencia corretamente a inserção e possíveis duplicações.

-->Restrições:
    - O tamanho de cada linha não pode ultrapassar 255 caracteres.
    - O campo de senha é limitado a 199 caracteres.
    - O arquivo deve conter dados válidos no formato esperado ou as linhas serão ignoradas.
    - O programa deve ter permissão para alocar memória.

-->Pseudoinstruções:
    - Verifica se os ponteiros são válidos.
    - Lê cada linha do arquivo.
    - Usa `sscanf` para extrair os campos `CPF` e `senha`.
    - Ignora linhas mal formatadas.
    - Aloca memória e duplica os dados.
    - Insere o novo login na árvore.
*/
//*****************************************************************************************************************


#endif
