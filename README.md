# API de Detecção de DNA Símio

## Descrição

Esta API REST foi desenvolvida em C++ com o objetivo de analisar sequências de DNA e identificar se pertencem a símios (mutantes), com base em padrões específicos de repetição.

Além da análise, a aplicação realiza a persistência dos dados em banco SQLite e disponibiliza estatísticas agregadas das verificações realizadas.

---

## Tecnologias Utilizadas

* C++17
* cpp-httplib (servidor HTTP)
* nlohmann/json (manipulação de JSON)
* SQLite3 (banco de dados)

---

## Funcionalidades

* Verificação de DNA símio
* Validação de estrutura NxN
* Persistência com unicidade de dados
* Cálculo de estatísticas
* API REST leve e eficiente

---

## Como Executar o Projeto

### Pré-requisitos

* Compilador GCC (versão 11 ou superior recomendada)
* SQLite3 instalado

---

### Compilação

```bash
g++ main.cpp -o api -std=c++17 -pthread -lsqlite3
```

---

### Execução

```bash
./api
```

A API será iniciada em:

```bash
http://localhost:PORT
```

Caso esteja em ambiente de nuvem, a porta será definida automaticamente pela variável `PORT`.

---

## Endpoints

---

### POST /simian

Verifica se uma sequência de DNA pertence a um símio.

#### Requisição

* Método: POST
* Content-Type: application/json

#### Body

```json
{
  "dna": ["ATGCGA", "CAGTGC", "TTATGT", "AGAAGG", "CCCCTA", "TCACTG"]
}
```

---

### Regras de Validação

* O DNA deve ser uma matriz NxN
* Apenas os caracteres A, T, C e G são permitidos
* A matriz não pode estar vazia

---

### Critério de Identificação

Um DNA é considerado símio quando existem pelo menos **duas sequências** de quatro letras iguais consecutivas nas seguintes direções:

* Horizontal
* Vertical
* Diagonal principal
* Diagonal secundária

---

### Respostas

#### 200 OK

```text
Simian
```

---

#### 403 Forbidden

```text
Human
```

---

#### 400 Bad Request

```text
DNA inválido
```

ou

```text
Erro no JSON
```

---

### GET /stats

Retorna estatísticas das análises realizadas.

#### Resposta

```json
{
  "count_mutant_dna": 40,
  "count_human_dna": 100,
  "ratio": 0.4
}
```

---

### Descrição dos Campos

* count_mutant_dna: quantidade de DNAs identificados como símios
* count_human_dna: quantidade de DNAs identificados como humanos
* ratio: proporção entre símios e humanos

---

## Banco de Dados

A aplicação utiliza SQLite para armazenamento dos dados.

### Estrutura da Tabela

```sql
CREATE TABLE dna (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    sequence TEXT UNIQUE,
    is_simian INTEGER
);
```

---

### Regras

* Cada DNA é armazenado apenas uma vez
* O campo `sequence` possui restrição UNIQUE
* O campo `is_simian` indica:

  * 1 → símio
  * 0 → humano

---

## Arquitetura

* Aplicação leve e sem dependência de frameworks pesados
* Processamento eficiente de sequências
* Persistência simples e confiável
* Preparada para execução em ambientes de nuvem

---

## Deploy

A aplicação é compatível com plataformas de nuvem como:

* Railway
* Render

Também pode ser executada em container Docker.

---

## Autor

Gabriel Moreira

---
