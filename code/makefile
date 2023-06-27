# Makefile
# PROJETO DE: 
# Nome do executável
TARGET = GreenUP_SIS_OP_2

# Arquivos de origem
GCC_SOURCES = main.c 

# Comandos do terminal
GCC = gcc
RM = rm -f

# Regra padrão
all: $(TARGET)

# Regra para compilar os arquivos main.c
%.o: %.c
	$(GCC) -c $< -o $@


# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para limpar os arquivos gerados
clean:
	$(RM) $(BISON_OUTPUT) $(FLEX_OUTPUT) $(GCC_OBJECTS) $(TARGET) $(BISON_HEADER) $(TARGET).tgz

tar:
	tar -cvzf $(TARGET).tgz  'main.c' 'makefile'
# Define as regras que não são nomes de arquivos
.PHONY: all run clean tar
