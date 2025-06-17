#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static userProfileTree* raiz = NULL;

/* Funções auxiliares static */
static int validaNome(const char* nome);
static int validaCPF(long long cpf);
static int cpfUnico(long long cpf, userProfileTree* no);
static int validaDataNascimento(const char* data);
static int maiorIdade(const char* data);
static int validaSenha(const char* senha);
static int validaTelefone(const char* tel);
static int alturaAVL(userProfileTree* arvore);
static int nbalanceamento(userProfileTree* arvore);
static userProfileTree* rotacaoEsq(userProfileTree* arvore);
static userProfileTree* rotacaoDir(userProfileTree* arvore);
static userProfile* buscaPtr(long long CPF, userProfileTree* no);
static userProfileTree* insereAVL(userProfileTree* no, userProfile* perfil, int* retorno);

int createUserProfile(char* name, char* numeroTel, char* birthdate, long long CPF, char* password)
{
	if (!validaNome(name))
		return 1;
	if (!validaCPF(CPF))
		return 3;
	if (!cpfUnico(CPF, raiz))
		return 2;
	if (!validaDataNascimento(birthdate))
		return 4;
	if (!maiorIdade(birthdate))
		return 5;
	if (!validaSenha(password))
		return 6;
	if (!validaTelefone(numeroTel))
		return 7;

	userProfile* perfil = malloc(sizeof(userProfile));

	if (perfil == NULL)
		return -1;

	perfil->name = strdup(name);
	perfil->birthdate = strdup(birthdate);
	perfil->CPF = CPF;
	perfil->password = strdup(password);
	perfil->telNum = strdup(numeroTel);

	int retorno = addProfileToTree(perfil);
	if (retorno != 0)
	{
		free(perfil->name);
		free(perfil->birthdate);
		free(perfil->password);
		free(perfil->telNum);
		free(perfil);
		return retorno;
	}

	return 0;
}

int addProfileToTree(userProfile* profile)
{
	int retorno = 0; // Para poder retornar os casos de erro essa variável é passada como ponteiro para a função atribuir o valor do retorno
	raiz = insereAVL(raiz, profile, &retorno);
	return retorno;
}

int getUser(long long CPF, userProfileTree* no)
{
	if (!validaCPF(CPF))
		return 1;
	if (buscaPtr(CPF, no) != NULL)
		return 0;
	else
		return 2;
}

static userProfile* buscaPtr(long long CPF, userProfileTree* no)
{
	if (no == NULL)
		return NULL;

	if (CPF == no->profile->CPF)
		return no->profile;
	if (CPF < no->profile->CPF)
		return buscaPtr(CPF, no->left);
	else
		return buscaPtr(CPF, no->right);
}

int alterPassword(long long CPF, const char* newPass)
{
	if (!validaSenha(newPass))
		return 2;
	userProfile* perfil = buscaPtr(CPF, raiz);
	if (perfil == NULL)
		return 1;

	free(perfil->password);
	perfil->password = strdup(newPass);
	return 0;
}

int alterNumber(long long CPF, const char* numero)
{
	if (!validaTelefone(numero))
		return 2;

	userProfile* perfil = buscaPtr(CPF, raiz);
	if (perfil == NULL)
		return 1;

	free(perfil->telNum);
	perfil->telNum = strdup(numero);
	return 0;
}

static void gravaNo(FILE* f, userProfileTree* no)
{
	if (no == NULL)
		return;

	unsigned int tam; // Gravar o tamanho da string que vem a seguir, para facilitar na leitura

	tam = (unsigned int)(strlen(no->profile->name) + 1);
	fwrite(&tam, sizeof(unsigned int), 1, f);
	fwrite(no->profile->name, tam, 1, f);

	tam = (unsigned int)(strlen(no->profile->birthdate) + 1);
	fwrite(&tam, sizeof(unsigned int), 1, f);
	fwrite(no->profile->birthdate, tam, 1, f);

	tam = (unsigned int)(strlen(no->profile->password) + 1);
	fwrite(&tam, sizeof(unsigned int), 1, f);
	fwrite(no->profile->password, tam, 1, f);

	tam = (unsigned int)(strlen(no->profile->telNum) + 1);
	fwrite(&tam, sizeof(unsigned int), 1, f);
	fwrite(no->profile->telNum, tam, 1, f);

	fwrite(&no->profile->CPF, sizeof(long long), 1, f);

	gravaNo(f, no->left);
	gravaNo(f, no->right);
}

int saveUserProfiles(FILE* arquivo)
{
	if (arquivo == NULL)
		return 1;

	gravaNo(arquivo, raiz);
	return 0;
}

static char* leString(FILE* f)
{
	unsigned int tam;
	if (fread(&tam, sizeof(unsigned int), 1, f) != 1)
		return NULL;

	char* string = (char*)malloc(tam);
	if (string == NULL)
		return NULL;

	if (fread(string, tam, 1, f) != 1)
	{
		free(string);
		return NULL;
	}
	return string;
}

static int leNo(FILE* f)
{
	char* nome = leString(f);
	if (nome == NULL)
		return 0;
	char* data = leString(f);
	char* senha = leString(f);
	char* tel = leString(f);
	long long cpf;
	if (fread(&cpf, sizeof(long long), 1, f) != 1)
	{
		free(nome);
		free(data);
		free(senha);
		free(tel);
		return 0;
	}

	createUserProfile(nome, tel, data, cpf, senha);

	free(nome);
	free(data);
	free(senha);
	free(tel);
	return 1;
}

int readUserProfiles(FILE* arquivo)
{
	if (arquivo == NULL)
		return 1;

	while (leNo(arquivo))
	{
		// Para continuar até o fim do arquivo
	}
	return 0;
}

static int validaNome(const char* nome)
{
	if (nome == NULL)
		return 0;
	if (strlen(nome) == 0)
		return 0;

	int nletras = 0;
	const char* c = nome;
	while (*c)
	{
		if (isalpha((unsigned char)*c))
			nletras++;
		c++;
	}
	if (nletras >= 2)
		return 1;
	return 0;
}

static int validaCPF(long long cpf)
{
	if (cpf >= 10000000000LL && cpf <= 99999999999LL)
		return 1;
	return 0;
}

static int cpfUnico(long long cpf, userProfileTree* no)
{
	if (no == NULL)
		return 1;
	if (cpf == no->profile->CPF)
		return 0;

	if (cpf < no->profile->CPF)
		return cpfUnico(cpf, no->left);
	else
		return cpfUnico(cpf, no->right);
}

static int validaDataNascimento(const char* data)
{
	if (data == NULL)
		return 0;
	if (strlen(data) != 10)
		return 0;

	if ((isdigit((unsigned char)data[0]) && isdigit((unsigned char)data[1])))
	{
		if (data[2] == '/' && isdigit((unsigned char)data[3]) && isdigit((unsigned char)data[4]))
		{
			if (data[5] == '/' && isdigit((unsigned char)data[6]) && isdigit((unsigned char)data[7]) && isdigit((unsigned char)data[8]) && isdigit((unsigned char)data[9]))
				return 1;
		}
	}
	return 0;
}

static int maiorIdade(const char* data)
{
	int dia, mes, ano;
	if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3)
		return 0;

	time_t horaAtual = time(NULL);
	struct tm* hoje = localtime(&horaAtual);
	int anoAtual = hoje->tm_year + 1900;
	int mesAtual = hoje->tm_mon + 1;
	int diaAtual = hoje->tm_mday;

	int idade = anoAtual - ano;

	if (mesAtual < mes || (mesAtual == mes && diaAtual < dia))
		idade--;

	if (idade >= 18)
		return 1;
	return 0;
}

static int validaSenha(const char* senha)
{
	if (senha == NULL)
		return 0;
	if (strlen(senha) < 8)
		return 0;
	return 1;
}

static int validaTelefone(const char* tel)
{
	if (tel == NULL)
		return 0;
	int ndigitos = 0;
	const char* t = tel;
	while (*t != '\0') {
		if (isdigit((unsigned char)*t))
			ndigitos++;
		t++;
	}

	if (ndigitos >= 8 && ndigitos <= 13)
		return 1;
	return 0;
}

static int alturaAVL(userProfileTree* arvore)
{
	if (arvore == NULL)
		return 0;
	int alturaEsq = alturaAVL(arvore->left);
	int alturaDir = alturaAVL(arvore->right);

	if (alturaEsq > alturaDir)
		return alturaEsq + 1;
	return alturaDir + 1;
}

static int nbalanceamento(userProfileTree* arvore)
{
	if (arvore == NULL)
		return 0;
	return alturaAVL(arvore->left) - alturaAVL(arvore->right);
}

static userProfileTree* rotacaoEsq(userProfileTree* arvore)
{
	if (arvore == NULL)
		return arvore;
	if (arvore->right == NULL)
		return arvore;

	userProfileTree* rot1 = arvore->right;
	userProfileTree* rot2 = rot1->left;
	rot1->left = arvore;
	arvore->right = rot2;
	return rot1;
}

static userProfileTree* rotacaoDir(userProfileTree* arvore)
{
	if (arvore == NULL)
		return arvore;
	if (arvore->left == NULL)
		return arvore;

	userProfileTree* rot1 = arvore->left;
	userProfileTree* rot2 = rot1->right;
	rot1->right = arvore;
	arvore->left = rot2;
	return rot1;
}

static userProfileTree* insereAVL(userProfileTree* no, userProfile* perfil, int* retorno)
{
	if (no == NULL)
	{
		userProfileTree* novoNo = malloc(sizeof(userProfileTree));
		if (novoNo == NULL)
		{
			*retorno = -1;
			return NULL;
		}
		novoNo->profile = perfil;
		novoNo->left = NULL;
		novoNo->right = NULL;
		*retorno = 0;
		return novoNo;
	}
	if (perfil->CPF == no->profile->CPF)
	{
		*retorno = 1; // CPF duplicado
		return no;
	}
	if (perfil->CPF < no->profile->CPF)
		no->left = insereAVL(no->left, perfil, retorno);
	else
		no->right = insereAVL(no->right, perfil, retorno);
	if (*retorno != 0)
		return no; // Teve algum erro dentro da recursão

	int fator = nbalanceamento(no);

	// Testar para todos os casos de desbalanceamento:
	if (fator > 1 && perfil->CPF < no->left->profile->CPF)
		return rotacaoDir(no);
	if (fator < -1 && perfil->CPF > no->right->profile->CPF)
		return rotacaoEsq(no);
	if (fator > 1 && perfil->CPF > no->left->profile->CPF)
	{
		no->left = rotacaoEsq(no->left);
		return rotacaoDir(no);
	}
	if (fator < -1 && perfil->CPF < no->right->profile->CPF)
	{
		no->right = rotacaoDir(no->right);
		return rotacaoEsq(no);
	}
	return no; // Se já estiver balanceado
}