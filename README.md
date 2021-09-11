

# Como rodar o Electron em desenvolvimento

Para iniciar o aplicativo em modo de desenvolvimento, utilize o comando *npm run dev*.
Para complicar o aplicativo, utilize o comando *npm run electron:build*

### Organização na pasta
* **Bin**: Referente ao código C++.
* **Theme**: Referente ao header.
* **Utils**: Para generalização do código.
* **View**: Para a separação das abas. *Console*, *Editor* e *Register*.


### Como usar a conexão Electron <-> React
```js
const {ipcRenderer} = window.electron;
ipcRenderer.once(event, callback); // Para receber o evento Electron -> React
ipcRenderer.send(event, data?); // Para enviar o evento React -> Electron
```

### Libs
* Electron
* ReactJS
* Material-UI
* Material-UI/Icons
* Material-UI/Labs
* React ACE

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