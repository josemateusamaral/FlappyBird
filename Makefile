.PHONY: clean doxygen

CXX=g++
RM=rm

Game: main.o objeto.o
	$(CXX) main.o objeto.o -o Game `sdl2-config --cflags --libs`

objeto.o: objeto.cc objeto.h
	$(CXX) -o objeto.o `sdl2-config --cflags --libs` -c objeto.cc

main.o: main.cc
	$(CXX) -o main.o `sdl2-config --cflags --libs` -c main.cc

# receita para apagar arquivos (geralmente são arquivos gerados
# no processo de desenvolvimento)
clean:
	$(RM) -f Game
	$(RM) -f main.o
	$(RM) -f objeto.o

# receita para gerar a documentação, o arquivo Doxyfile está
# configurado para gerar um diretório chamado docs
doc:
	doxygen

# receita para apagar o conteúdo do diretório de documentação
cleandoc:
	$(RM) -fR docs
