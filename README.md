#GreenUP_SIS_OP_2 - README

##Este é o repositório do projeto GreenUP_SIS_OP_2 desenvolvido por Felipe Bertoglio, Matheus Costa, Iuri Tinti e Victor Arnt.

Compilando o projeto
Para compilar o projeto, você precisa ter o g++ (GCC) instalado em seu sistema. Em seguida, utilize o comando make para compilar o código-fonte e criar o executável.
bash
make all

Executando o programa
Após a compilação, você pode executar o programa usando o seguinte comando:
bash
make run

Caso queira executar o programa na função "manager", utilize o seguinte comando:
bash
make run_manager

Compilando e executando
Para compilar e executar o programa em uma única etapa, você pode usar o seguinte comando:
bash
make present

Limpando os arquivos gerados
Caso queira limpar os arquivos gerados pela compilação, utilize o seguinte comando:
bash
make clean

Criação do arquivo tar
Para criar um arquivo tar com os arquivos importantes do projeto, você pode usar o seguinte comando:
bash
make tar

O arquivo tar gerado será nomeado como "GreenUP_SIS_OP_2.tgz".
Observações

    Certifique-se de que todas as dependências estão instaladas corretamente antes de compilar e executar o projeto.
    Certifique-se de que os arquivos "main.cpp", "makefile", "interface_subservice.cpp", "global.cpp", "management_subservice.cpp" e "monitoring_subservice.cpp" estão presentes no diretório antes de executar o comando "make tar".

Esperamos que você aproveite o projeto! Se tiver alguma dúvida ou problema, sinta-se à vontade para entrar em contato conosco.

Máquina virtual pronta para fazer e testar o trabalho:
https://1drv.ms/f/s!Alh-uqhLSeZUktop56PVUAeEVr-T8w?e=mAWaes

Trabalho de Sistemas Operacionais II

Hey por favor me leia

Pasta do projeto no google drive: https://drive.google.com/drive/folders/1lgZNSykp9H2Z6EjEVh2r_BINAl4DgeFe?usp=share_link

para Compilar: 
com o terminal da pasta code, executar
'make all'

para rodar: 
  Como cliente: 'make run'
  Como gerente: 'make run_manager'

Diagrama de interação entre Cliente e Gerente:
<iframe width="768" height="432" src="https://miro.com/app/embed/uXjVPohI4qM=/?pres=1&frameId=3458764547664582802&embedId=5908042219" frameborder="0" scrolling="no" allow="fullscreen; clipboard-read; clipboard-write" allowfullscreen></iframe> 
