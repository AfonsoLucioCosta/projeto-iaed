# Projeto IAED 2024/2025

Implementação em C de um sistema de gestão de lotes de vacinas e respetivas inoculações, desenvolvido no âmbito da unidade curricular Introdução aos Algoritmos e Estruturas de Dados (IAED).

O programa lê comandos da entrada padrão e escreve os resultados na saída padrão. Permite registar e listar lotes, administrar doses a utentes, consultar e remover inoculações e avançar a data simulada.

## Conteúdo

- `project.c` — implementação do programa.
- `enunciado.md` — enunciado e especificação oficial do projeto.
- `guidelines.md` e `debugging.md` — recomendações de desenvolvimento e depuração.
- `public-tests.zip` — arquivo dos testes públicos, extraído automaticamente ao executar `make test` se a pasta de testes ainda não existir.
- `vacina.jpg` — imagem usada no enunciado.

## Requisitos

- `gcc` compatível com as opções de compilação do projeto.
- `make` e `unzip` para compilar e executar os testes através dos atalhos abaixo.

## Compilar

Na pasta do repositório:

```sh
make
```

Isto cria o executável `proj`, usando as opções de compilação indicadas no enunciado. Também é possível compilar diretamente:

```sh
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj project.c
```

## Executar

O programa recebe os comandos pela entrada padrão:

```sh
./proj < entrada.txt
```

Para usar as mensagens em português, passa-se `pt` como argumento:

```sh
./proj pt < entrada.txt
```

Exemplo:

```text
c A0C0 31-7-2025 210 malária
l
q
```

Guarda estas linhas num ficheiro e executa `./proj < ficheiro.txt`. A sintaxe e o comportamento completo dos comandos estão descritos em [`enunciado.md`](enunciado.md).

## Testes públicos

Executa a bateria de testes fornecida com o projeto:

```sh
make test
```

O comando compila o programa, extrai `public-tests.zip` se necessário e compara os resultados com os outputs esperados. Para remover o executável e os resultados gerados pelos testes:

```sh
make clean
```

## Submissão do projeto

O ficheiro `.gitlab-ci.yml` faz parte do fluxo de submissão do curso e não deve ser alterado. Os testes públicos são úteis para validação local, mas não garantem o resultado da avaliação automática.
