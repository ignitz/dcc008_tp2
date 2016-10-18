# Trabalho Prático 2 - Ligador

> Lucas Machado  
> Yuri Niitsuma  
> Nathalia Campos

### Introdução

O trabalho consiste em criar um *linker* para o montador Wombat2.

### Decisões (Aqui coloco minhas epifanias durante o desenvolvimento)

- Foi preciso modificar o **montador**:
  - para que ele guarde em um arquivo a tabela de símbolos e todas as posições da ocorrência para utilizar no linkador/ligador
  - Colocar no início do arquivo o tamanho do programa.
  - Ajuste para que o tamanho do programa sempre termine em um tamanho ímpar (ou par (depende da sua métrica)) **Isso farei no linkador**.
  - Inseri o comando para EXTERN (chamada de função call para um label externo). Não testei, tem que testar, não quero testar...
    - Fiz um teste bobo no **all.a**.
- Passos para o sucesso
  - Abrir todos os módulos dicamicamente ✔.
  - Atualizar tabelas dos símbolos
  - Encontrar todos os externs e atualizar na table symbol global.
  - Encontrar todas as ocorrências dos símbolos e atualizá-las.

### Referência
- http://www.boost.org/doc/libs/1_61_0/libs/algorithm/doc/html/index.html
