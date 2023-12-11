#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>



#define MAX_ALUNOS 100

int num_alunos = 0;


struct Aluno
{
  char nome[50];
  int dia, mes, ano;
  int idade;
  char cpf[12];
  float nota1, nota2;
  int matricula;
};

void exibirMenu() {
  printf("\nMenu inicial, digite a opção desejada: \n");
  printf("1 - Cadastrar aluno \n");
  printf("2 - Excluir aluno \n");
  printf("3 - Pesquisar aluno por matrícula\n");
  printf("4 - Alterar registros do aluno\n");
  printf("5 - Lista de alunos\n");
  printf("6 - Sair\n");

}

//Validação de CPF
bool validar_cpf(char cpf[]) {
    int i, j, digito1 = 0, digito2 = 0;
    if (strlen(cpf) != 11) {
        return false;
    }
    for (i = 0, j = 10; i < 9; i++, j--) {
        digito1 += (cpf[i] - '0') * j;
    }
    digito1 %= 11;
    if (digito1 < 2) {
        digito1 = 0;
    } else {
        digito1 = 11 - digito1;
    }
    for (i = 0, j = 11; i < 10; i++, j--) {
        digito2 += (cpf[i] - '0') * j;
    }
    digito2 %= 11;
    if (digito2 < 2) {
        digito2 = 0;
    } else {
        digito2 = 11 - digito2;
    }
    if (cpf[9] - '0' != digito1 || cpf[10] - '0' != digito2) {
        return false; // CPF inválido
    }
    return true; // CPF válido
}



void cadastrarAluno(struct Aluno alunos[], int *pos, FILE *arq) {
 

  struct Aluno aluno;
  printf("Digite o nome do aluno: ");
  getchar();
  fgets(aluno.nome, 50, stdin);
  fprintf(arq, "Nome:%s\n", aluno.nome);

  // Remove os espaços em branco do final da string
  while (aluno.nome[strlen(aluno.nome) - 1] == ' ') {
    aluno.nome[strlen(aluno.nome) - 1] = '\0';
  }
 
  printf("Pressione Enter para continuar");
  getchar(); 
  printf("Digite o CPF do aluno: ");
  fflush(stdin);
  fgets(aluno.cpf, 12, stdin);
  strtok(aluno.cpf, "\n");

  while (!validar_cpf(aluno.cpf)) { // enquanto CPF for inválido
    printf( "CPF inválido! Digite novamente: ");
    fflush(stdin);
    fgets(aluno.cpf, 12, stdin);
    strtok(aluno.cpf, "\n");
  }
  fprintf(arq,"CPF:%s\n", aluno.cpf);
  printf("Digite a matrícula do aluno: ");
  scanf("%d", &aluno.matricula);
  fprintf(arq,"Matrícula:%d\n", aluno.matricula);
  printf("Digite a data de nascimento do aluno (DD/MM/AAAA): ");
  scanf("%d/%d/%d", &aluno.dia, &aluno.mes, &aluno.ano);
  fprintf(arq, "Data de nascimento: %d/%d/%d\n", aluno.dia, aluno.mes, aluno.ano);

  
   // Obter a data atual
  time_t now = time(0);
  struct tm* dataAtual = localtime(&now);

  // Calcular a idade do aluno
  int idade = dataAtual->tm_year + 1900 - aluno.ano;
  if (dataAtual->tm_mon + 1 < aluno.mes ||
      (dataAtual->tm_mon + 1 == aluno.mes && dataAtual->tm_mday < aluno.dia)) { 
    idade--;
  }
  printf("Idade: %d\n", idade);
  fprintf(arq, "Idade: %d\n", idade);
  printf("Digite a primeira nota do aluno: \n"); 
  scanf("%f", &aluno.nota1);
  fprintf(arq, "Nota1:%.2f", aluno.nota1); 
  printf("Digite a segunda nota do aluno: ");
  scanf("%f", &aluno.nota2);
  fprintf(arq, "Nota2:%.2f", aluno.nota2);
  alunos[num_alunos++] = aluno;
  alunos[*pos] = aluno;
  (*pos)++;
  printf("Aluno cadastrado com sucesso!\n");

}


//Excluir aluno
void excluirAluno(struct Aluno alunos[], int *pos, FILE *arq) {
  struct Aluno aluno;
  char linha[100];
  char nome[50];
  printf("Digite o nome do aluno a ser excluído: ");
  fflush(stdin);
  scanf("%s", nome);
  nome[strcspn(nome, "\n")] = '\0'; 
  while (getchar() != '\n');

  int posicao = -1; // posição do aluno na lista


  for (int i = 0; i < *pos; i++) {
    if (strncmp(nome, alunos[i].nome, strlen(nome) - 1) == 0) { // se o nome do aluno for igual
      posicao = i; // armazena a posição do aluno na lista
      break;
    }
  }

  if (posicao == -1) {
    printf("Aluno não encontrado!\n");
  } else {
     
    for (int i = posicao; i < *pos - 1; i++) {
      alunos[i] = alunos[i + 1]; // deslocar os alunos após o aluno excluído uma posição para a esquerda
    }
    (*pos)--;
    num_alunos--;
    printf("Aluno excluído com sucesso!\n");
    FILE *arqTemp = fopen("alunos_temp.txt", "w"); // arquivo temporário para escrever os dados dos alunos

for (int i = 0; i < *pos; i++) {

  
  fprintf(arqTemp, "Nome:%s\n", alunos[i].nome);
  fprintf(arqTemp, "CPF:%s\n", alunos[i].cpf);
  fprintf(arqTemp, "Matrícula:%d\n", alunos[i].matricula);
  fprintf(arqTemp, "Data de nascimento:%d/%d/%d\n", alunos[i].dia, alunos[i].mes, alunos[i].ano);
  fprintf(arqTemp, "Nota1:%.2f\n", alunos[i].nota1);
  fprintf(arqTemp, "Nota2:%.2f\n", alunos[i].nota2);
    // Obter a data atual
  time_t now = time(0);
  struct tm* dataAtual = localtime(&now);

  // Calcular a idade do aluno
  int idade = dataAtual->tm_year + 1900 - alunos[i].ano;
  if (dataAtual->tm_mon + 1 < alunos[i].mes ||
      (dataAtual->tm_mon + 1 == alunos[i].mes && dataAtual->tm_mday < alunos[i].dia)) { 
    idade--;
  
  }
 
  fprintf(arqTemp, "Idade:%d\n", idade);
  fprintf(arqTemp, "\n");
}
fclose(arqTemp);
remove("alunos.txt"); // exclui o arquivo antigo
rename("alunos_temp.txt", "alunos.txt"); // renomeia o arquivo temporário para o nome do arquivo antigo

  }
}

void consultarAluno(struct Aluno alunos[], int *pos, FILE *arq){
 int matricula;
  struct Aluno aluno;
  
  printf("Digite a matrícula do aluno: ");
  scanf("%d", &matricula);

  for (int i = 0; i < *pos; i++) {
    if (alunos[i].matricula == matricula) {
      printf("Nome: %s\n", alunos[i].nome);
      printf("CPF: %s\n", alunos[i].cpf);
      printf("Data de nascimento: %02d/%02d/%04d\n", alunos[i].dia, alunos[i].mes, alunos[i].ano);
         // Obter a data atual
  time_t now = time(0);
  struct tm* dataAtual = localtime(&now);

  // Calcular a idade do aluno
  int idade = dataAtual->tm_year + 1900 - alunos[i].ano;
  if (dataAtual->tm_mon + 1 < alunos[i].mes ||
      (dataAtual->tm_mon + 1 == alunos[i].mes && dataAtual->tm_mday < alunos[i].dia)) { 
    idade--;

  }
      printf("A idade é %d anos.\n", alunos[i].idade);
      printf("Nota 1: %.2f\n", alunos[i].nota1);
      printf("Nota 2: %.2f\n", alunos[i].nota2);
      printf("Média: %.2f\n", (alunos[i].nota1 + alunos[i].nota2) / 2);
      printf("Matrícula: %d\n", alunos[i].matricula);
     
      
      return;
    }
  

  printf("Aluno não encontrado!\n");
}
}

void alterarDados(struct Aluno alunos[], int *pos, FILE *arq) {
struct Aluno aluno;
  int matricula;
  char cpf[12], linha[100];
  int i;


  printf("Digite a matrícula do aluno que deseja alterar: ");
  scanf("%d", &matricula);

  int posicao = -1;
  for (int i = 0; i < *pos; i++) {
    if (alunos[i].matricula == matricula) {
      posicao = i;
      break;
    }
  }
  
  if (posicao == -1) {
    printf("Nenhum aluno encontrado com a matrícula %d\n", matricula);
    return;
  }

  
  
  printf( "\nAluno encontrado: \n");
  printf( "Nome: %s\n", alunos[posicao].nome);
  printf( "CPF: %s\n", alunos[posicao].cpf);
  printf( "Data de nascimento: %02d/%02d/%04d\n", alunos[posicao].dia, alunos[posicao].mes, alunos[posicao].ano);
  printf("Nota 1: %.2f\n", alunos[posicao].nota1);
  printf("Nota 2: %.2f\n", alunos[posicao].nota2);
  printf("\n");
  printf("Digite o novo nome do aluno: \n");
  scanf("%s", aluno.nome);
  printf("Digite o novo CPF do aluno: \n");
  scanf("%s", aluno.cpf);
  printf("Digite a NOVA MATRÍCULA do aluno: \n");
  scanf("%d", &aluno.matricula);
  printf("Digite a nova data de nascimento do aluno (DD/MM/AAAA): ");
  scanf("%d/%d/%d", &aluno.dia, &aluno.mes, &aluno.ano);
  printf("Digite a nova Nota 1 do aluno: \n");
  scanf("%f", &aluno.nota1);
  printf("Digite a nova nota 2 do aluno: \n");
  scanf("%f", &aluno.nota2);

  alunos[posicao] = aluno;

  // Atualizar a idade do aluno
  time_t now = time(0);
  struct tm* dataAtual = localtime(&now);
  int idade = dataAtual->tm_year + 1900 - alunos[posicao].ano;
  if (dataAtual->tm_mon + 1 < alunos[posicao].mes || (dataAtual->tm_mon + 1 == alunos[posicao].mes && dataAtual->tm_mday < alunos[posicao].dia)) {
    idade--;
  }
  alunos[posicao].idade = idade;

  FILE *arqTemp = fopen("alunos_temp.txt", "w");
  for (int i = 0; i < num_alunos; i++) {
    fprintf(arqTemp, "Nome:%s\n", alunos[i].nome);
    fprintf(arqTemp, "CPF:%s\n", alunos[i].cpf);
    fprintf(arqTemp, "Matrícula:%d\n", alunos[i].matricula);
    fprintf(arqTemp, "Data de nascimento:%d/%d/%d\n", alunos[i].dia, alunos[i].mes, alunos[i].ano);
    fprintf(arqTemp, "Nota1:%.2f\n", alunos[i].nota1);
    fprintf(arqTemp, "Nota2:%.2f\n", alunos[i].nota2);
    fprintf(arqTemp, "Idade:%d\n", idade);
    fprintf(arqTemp, "\n");
  }
  fclose(arqTemp);
  remove("alunos.txt");
  rename("alunos_temp.txt", "alunos.txt");

  

  printf("Dados alterados com sucesso!");

}


int main(void) {
  FILE *arq = fopen("alunos.txt", "r+");
  FILE *arq_tmp;
  setlocale(LC_ALL, "Portuguese");
  struct Aluno alunos[MAX_ALUNOS];
  int pos = 0;
  int opcao = 0;


  while (opcao != 6) {
    exibirMenu(); // Exibir Menu
    scanf("%d", &opcao); // Lê a opção digitada
    switch(opcao) {
      case 1:
      	system("cls || clear");
        cadastrarAluno(alunos, &pos, arq);
        break;
      case 2:
      	system("cls || clear");
        excluirAluno(alunos, &pos, arq);
        break;
      case 3:
      	system("cls || clear");
    	consultarAluno(alunos, &pos, arq);
        break;
      case 4:
        system("cls || clear");
      alterarDados(alunos, &pos, arq);
        break;
      case 5:
      		printf("Lista de alunos cadastrados:\n");
      	    for(int i = 0; i < num_alunos; i++) {
      	    struct Aluno aluno = alunos[i];	
      	    printf("Nome: %s\n", aluno.nome);
			      printf("CPF: %s\n", aluno.cpf);	
            printf("Matricula: %d\n", aluno.matricula);
 // Obter a data atual
  time_t now = time(0);
  struct tm* dataAtual = localtime(&now);

  // Calcular a idade do aluno
  int idade = dataAtual->tm_year + 1900 - aluno.ano;
  if (dataAtual->tm_mon + 1 < aluno.mes ||
      (dataAtual->tm_mon + 1 == aluno.mes && dataAtual->tm_mday < aluno.dia)) { 
    idade--;
  }
			printf("A idade de %s é %d anos.\n", aluno.nome, idade);
			printf("Notas: %.2f %.2f\n", aluno.nota1, aluno.nota2);
      }
      
      	    break;
      	
      case 6:
        printf("Encerrando o programa...\n");
        break;
      default:
        printf("Opção inválida! Tente novamente.\n");
        break;
    }

  }
  fclose(arq);
  return 0;
}
