# Makefile
# PROJETO DE: Felipe Bertoglio 00262669 - Matheus Costa 00297121 - Iuri Tinti 00278043 - Victor Arnt 00291097
# Nome do executável
TARGET = GreenUP_SIS_OP_2
GCC_OBJECTS = main.cpp interface_subservice.cpp adminTabela.cpp management_subservice.cpp monitoring_subservice.cpp 

# Comandos do terminal
GCC = g++ -Wall -o
RM = rm -f

# Regra padrão
all: $(TARGET)
$(TARGET): $(GCC_OBJECTS)
	$(GCC) $(TARGET) $(GCC_OBJECTS)


# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para limpar os arquivos gerados
clean:
	$(RM)  $(TARGET).tgz

tar:
	tar -cvzf $(TARGET).tgz  'main.cpp' 'makefile' 'interface_subservice.cpp' 'adminTabela.cpp' 'management_subservice.cpp' 'monitoring_subservice.cpp' 
# Define as regras que não são nomes de arquivos
.PHONY: all run clean tar
