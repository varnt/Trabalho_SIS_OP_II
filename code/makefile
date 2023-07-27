# Makefile
# PROJETO DE: Felipe Bertoglio 00262669 - Matheus Costa 00297121 - Iuri Tinti 00278043 - Victor Arnt 00291097
# Nome do executável
TARGET = GreenUP_SIS_OP_2
GCC_OBJECTS = main.cpp global.cpp ./subservices/discovery_subservice.cpp ./subservices/interface_subservice.cpp ./subservices/monitoring_subservice.cpp   ./common/packet.cpp ./common/socketAPI.cpp



# Comandos do terminal
GCC = g++ -Wall -o
RM = rm -f
THREAD = -lpthread

# Regra padrão
all: $(TARGET)
$(TARGET): $(GCC_OBJECTS)
	$(GCC) $(TARGET) $(GCC_OBJECTS) $(THREAD) 


# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

run_manager: $(TARGET)
	./$(TARGET) manager
	

#Regra para Compilar e executar CLIENTE:
present:
	$(RM)  $(TARGET).tgz $(TARGET)
	clear
	$(GCC) $(TARGET) $(GCC_OBJECTS) $(THREAD) 
	./$(TARGET)
#Regra para Compilar e executar MAGANER:
presentm:
	$(RM)  $(TARGET).tgz $(TARGET)
	clear
	$(GCC) $(TARGET) $(GCC_OBJECTS) $(THREAD) 
	./$(TARGET) manager
# Regra para limpar os arquivos gerados
clean:
	$(RM)  $(TARGET).tgz $(TARGET)

tar:
	tar -cvzf $(TARGET).tgz  'main.cpp' 'makefile' 'interface_subservice.cpp' 'global.cpp' 'management_subservice.cpp' 'monitoring_subservice.cpp' 
# Define as regras que não são nomes de arquivos
.PHONY: all run clean tar
