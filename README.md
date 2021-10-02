# Sobre o Trabalho
...

# Equipe

**Líder**: DANIEL NOGUES DE OLIVEIRA
**Vice-Líder**: KEVIN SOARES PEREIRA

### Time A
Na primeira parte do trabalho, o **Time A** foi responsável pelo **Analyzer** e a **Semantica** do código Assembly.
- DANIEL NOGUES DE OLIVEIRA

### Time B
Responsável pelo **Assembler** e **Maquina Virtual** 
- KEVIN SOARES PEREIRA
- JULIANO STRELOW BUSS
- EMERSON DE VASCONCELOS VIEIRA
- MATHEUS AUGUSTO CEZAR FUKUDA

### Time C
Responsável pelo Front-end
- FERNANDO RIBEIRO OLLE
- LUCAS ALVIENE PEREIRA

Essa é a definição dos times, mas no meio do trabalho também houve revesamento de membros em outros times para ajudar na implementação.

# Como rodar o Electron em desenvolvimento

Para iniciar o aplicativo em modo de desenvolvimento, utilize o comando _npm run dev_.
Para complicar o aplicativo, utilize o comando _npm run electron:build_

### Organização na pasta

- **Bin**: Referente ao código C++.
- **Theme**: Referente ao tema do programa.
- **Utils**: Para generalização do código.
- **View**: Para a separação das abas. _Console_, _Editor_ e _Register_.

### Como usar a conexão Electron <-> React

```js
const { ipcRenderer } = window.electron;
ipcRenderer.once(event, callback); // Para receber o evento Electron -> React
ipcRenderer.send(event, data?); // Para enviar o evento React -> Electron
```

### Libs

- Electron
- ReactJS
- Material-UI
- Material-UI/Icons
- Material-UI/Labs
- React ACE

===========================================

# Bibliotecas dinâmicas do C++

Ficarão organizadas na pasta _/src/bin_

Biblioteca dinâmica compilada pra intergrar com o JS: _/src/bin/release_ - Para gerar use o comando _make_

Arquivos de desenvolvimento do C++: _/src/bin/sources_

Arquivos de testes locais dos algoritmos [não vai para o github]: _/src/bin/test_ - Para compilar para testes use o comando _make debug_ - O executável ficara na pasta de testes também

NOTA: - O Makefile ainda precisa ser feito, então nem tenta rodar
