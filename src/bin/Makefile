#
#   ==============================
#       Author: Daniel Olivera
#   ==============================
#

# caminho para o diretorio de teste
TEST_PATH			=	test

# caminho para o executavel de teste
ifeq ($(OS), Windows_NT)
TEST				=	$(TEST_PATH)/test.exe
else
TEST				=	$(TEST_PATH)/test
endif

# caminho para os arquivos-fonte de cada parte
ANALYZER_PATH		=	sources/analyzer
ASSEMBLER_PATH		=	sources/assembler
VM_PATH				=	sources/VM

# caminho para o diretorio de objetos
OBJ_FOLDER			=	obj

# caminho para os arquivos objetos de cada parte
ANALYZER_OBJ_PATH	=	$(OBJ_FOLDER)/analyzer
ASSEMBLER_OBJ_PATH	=	$(OBJ_FOLDER)/assembler
VM_OBJ_PATH			=	$(OBJ_FOLDER)/VM

# caminho para o arquivo-fonte principal de teste
TEST_SRC			=	$(TEST_PATH)/test.cpp

# arquivos fonte
ANALYZER_SRC		=	$(wildcard $(ANALYZER_PATH)/*.cpp)
ASSEMBLER_SRC		=	$(wildcard $(ASSEMBLER_PATH)/*.cpp)
VM_SRC				=	$(wildcard $(VM_PATH)/*.cpp)
ALL_SRCS			=	$(ANALYZER_SRC) $(ASSEMBLER_SRC) $(VM_SRC)

# arquivos objeto
ANALYZER_OBJ		=	$(ANALYZER_SRC:$(ANALYZER_PATH)/%.cpp=$(ANALYZER_OBJ_PATH)/%.o)
ASSEMBLER_OBJ		=	$(ASSEMBLER_SRC:$(ASSEMBLER_PATH)/%.cpp=$(ASSEMBLER_OBJ_PATH)/%.o)
VM_OBJ				=	$(VM_SRC:$(VM_PATH)/%.cpp=$(VM_OBJ_PATH)/%.o)
ALL_OBJS			=	$(ANALYZER_OBJ) $(ASSEMBLER_OBJ) $(VM_OBJ)

# dependencies
#ANALYZER_DEPS		=	$(ANALYZER_SRC:$(ANALYZER_PATH)/%.cpp=$(ANALYZER_OBJ_PATH)/%.o)
#ASSEMBLER_DEPS		=	$(ASSEMBLER_SRC:$(ASSEMBLER_PATH)/%.cpp=$(ASSEMBLER_OBJ_PATH)/%.o)
#VM_DEPS				=	$(VM_SRC:$(VM_PATH)/%.cpp=$(VM_OBJ_PATH)/%.o)
#ALL_DEPS			=	$(ANALYZER_OBJ) $(ASSEMBLER_OBJ) $(VM_OBJ)

# comando de compilação
CC					=	g++

# bibliotecas adicionais
LIBS				= 

DEPS 				= 	$(patsubst %.o, %.d, $(ALL_OBJS))
-include $(DEPS)
DEPFLAGS			=	-MMD -MF $(@:.o=.d)

# chama tudo
all: clear

# compila para teste
test: prepare $(TEST) clear

# compila os objetos do C com o main.c e o objeto do fortran
#$(TEST): $(ANALYZER_OBJ) $(ASSEMBLER_OBJ) $(VM_OBJ)
#	$(CC) $(TEST_SRC) $(ANALYZER_OBJ) $(ASSEMBLER_OBJ) $(VM_OBJ) -o $@ $(LIBS)

$(TEST): $(ALL_OBJS)
	$(CC) -o $@ $^



# gera os objetos
%.o: %.cpp
	$(CC) -c $< $(DEPFLAGS)

# # gera os objetos do analyzer
# $(ANALYZER_OBJ): $(ANALYZER_SRC)
# 	$(CC) -c $< -o $@

# # gera os objetos do assembler
# $(ASSEMBLER_OBJ): $(ASSEMBLER_SRC)
# 	$(CC) -c $< -o $@

# # gera os objetos da VM
# $(VM_OBJ): $(VM_SRC)
# 	$(CC) -c $< -o $@
 
prepare:
	-@mkdir $(OBJ_FOLDER)
	-@mkdir $(ANALYZER_OBJ_PATH)
	-@mkdir $(ASSEMBLER_OBJ_PATH)
	-@mkdir $(VM_OBJ_PATH)

ifeq ($(OS), Windows_NT)
clear:
	-@rm ./$(OBJ_FOLDER)
else
clear:
	-@rm -rvf $(OBJ_FOLDER)
endif