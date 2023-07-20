## GreenUP_SIS_OP_2 - README

Este é o repositório do projeto GreenUP_SIS_OP_2 desenvolvido por Felipe Bertoglio, Matheus Costa, Iuri Tinti e Victor Arnt.

### Compilando o projeto

Para compilar o projeto, você precisa ter o g++ (GCC) instalado em seu sistema. Em seguida, utilize o comando make para compilar o código-fonte e criar o executável.

```bash
make all
```

### Executando o programa

Após a compilação, você pode executar o programa usando o seguinte comando:

```bash
make run
```

Caso queira executar o programa na função "manager", utilize o seguinte comando:

```bash
make run_manager
```

### Compilando e executando

Para compilar e executar o programa em uma única etapa, você pode usar o seguinte comando:

```bash
make present
```

### Limpando os arquivos gerados

Caso queira limpar os arquivos gerados pela compilação, utilize o seguinte comando:

```bash
make clean
```

### Criação do arquivo tar

Para criar um arquivo tar com os arquivos importantes do projeto, você pode usar o seguinte comando:

```bash
make tar
```

O arquivo tar gerado será nomeado como "GreenUP_SIS_OP_2.tgz".

### Observações

- Certifique-se de que todas as dependências estão instaladas corretamente antes de compilar e executar o projeto.
- Certifique-se de que os arquivos "main.cpp", "makefile", "interface_subservice.cpp", "global.cpp", "management_subservice.cpp" e "monitoring_subservice.cpp" estão presentes no diretório antes de executar o comando "make tar".

Esperamos que você aproveite o projeto! Se tiver alguma dúvida ou problema, sinta-se à vontade para entrar em contato conosco.
