# Makefile
# PROJETO DE: Felipe Bertoglio 00262669 - Matheus Costa 00297121 - Iuri Tinti 00278043 - Victor Arnt 00291097
# Nome do executável
TARGET = GreenUP_SIS_OP_2


# Comandos do terminal
GCC = g++ -Wall -o
RM = rm -f

# Regra padrão
all: $(TARGET)
	$(GCC) $(TARGET) 


# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para limpar os arquivos gerados
clean:
	$(RM)  $(TARGET).tgz

tar:
	tar -cvzf $(TARGET).tgz  'main.c' 'makefile'
# Define as regras que não são nomes de arquivos
.PHONY: all run clean tar
