

# Como rodar o Electron em desenvolvimento

Use o comando *npm run electron:serve*
Isso executa o npm start e o electron juntos.
NÃO EXECUTE *NPM START*, esse comando *JÁ* faz isso

===========================================

# Bibliotecas dinâmicas do C++

Ficarão organizadas na pasta */src/bin*

Biblioteca dinâmica compilada pra intergrar com o JS: */src/bin/release*
    -   Para gerar use o comando *make*

Arquivos de desenvolvimento do C++: */src/bin/sources*

Arquivos de testes locais dos algoritmos [não vai para o github]: */src/bin/test*
    -   Para compilar para testes use o comando *make debug*
    -   O executável ficara na pasta de testes também

NOTA:
    -   O Makefile ainda precisa ser feito, então nem tenta rodar