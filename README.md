# 📁 Project Tree

Um sistema de gerenciamento de diretórios em árvore, via terminal, simulando comandos básicos de um sistema de arquivos Unix.

---

## 🚀 Funcionalidades

- 📂 Criar e organizar diretórios (`mkdir`, com suporte a `-p`)
- 📁 Navegar entre diretórios (`cd`)
- 📜 Listar diretórios (`list`, com `-r` para listar recursivamente)
- 🔎 Buscar elementos por nome (`search`)
- 🗑️ Remover arquivos e pastas (`rm`, com `-r` para diretórios)
- ✏️ Renomear arquivos ou pastas (`rename`)EXTRA!!!!
- 🔀 Mover elementos entre pastas (`move`)EXTRA!!!!
- 📖 Histórico de comandos digitados (`history`)EXTRA!!!!
- 📃 Persistência de estrutura em arquivo `directory.txt`

---

## 📦 Estrutura do Projeto

project-tree/
├── Command/ # Lógica de comandos
├── Archive/ # Funções de arquivo (persistência)
├── Interface/ # Interface do terminal
├── Tree/ # Implementação da estrutura de dados em árvore
├── main.c # Função principal
├── directory.txt # Armazena estrutura de diretórios
└── README.md # Este arquivo


---

## 💻 Comandos disponíveis (OBS: Os nomes dos Diretórios não podem contar espaço, pois senão você não conseguirá
                             fazer comandos (rm, mkdir) com eles depois, exemplo correto: "Meus_Documentos")

| Comando                         | Descrição                                                                 |
|---------------------------------|--------------------------------------------------------------------------|
| `mkdir <nome>`                 | Cria um novo diretório no diretório atual                                |
| `mkdir -p caminho/completo`    | Cria diretórios aninhados se não existirem                               |
| `cd <diretório>`               | Muda para o diretório especificado                                       |
| `list`                         | Lista o conteúdo do diretório atual                                      |
| `list -r`                      | Lista recursivamente todos os diretórios a partir do atual               |
| `rm <nome>`                    | Remove um arquivo                                                        |
| `rm -r <diretório>`           | Remove um diretório e seus filhos                                        |
| `move <origem> <destino>`      | Move um diretório/arquivo para outro diretório                           |
| `rename <path> <novo_nome>`    | Renomeia um diretório/arquivo                                            |
| `search <nome>`               | Busca por nome a partir do diretório atual                               |
| `history`                      | Exibe os últimos comandos digitados                                      |
| `clear`                        | Limpa a tela                                                             |
| `exit`                         | Encerra o programa                                                       |

---

## 📂 Exemplo de uso

```bash
mkdir Meus_Documentos
mkdir Meus_Documentos/trabalhos
mkdir Games
move Meus_Documentos/trabalhos Games
list -r

## 😓 Minhas Dificuldades

### 🧑‍💻 Dificuldades do Pedro Luca de Lacerda Alves

<!--
- Consertar a função salvar diretório/arquivo que estava duplicando root, mas depois de muita dor 
de cabeça, foi resolvido.
- Fazer as funções auxiliares para a função move que estavam sempre dando conflito.
- Estruturar o terminal para deixar o main mais elegante e enxuto.
- Função history no geral.
- Me adequar a estrutura do projeto e usar terminal pra comandos( diferente do meu parceiro , não estou acostumado a 
linux e afins, mas foi um bom aprendizado)

---

### 👨‍🏫 Dificuldades do Felipe Fonseca

<!--
Espaço para seu amigo comentar as dificuldades que você teve
e como você lidou com elas (se quiser usar como feedback).
Exemplo:
- [Seu Nome] teve dificuldades com ponteiros, mas conseguiu entender após revisar o código.
- Mandou bem no comando move depois de várias tentativas.
-->

- 


---

🛠️ Tecnologias
Linguagem: C

Compilador: gcc

Build System: CMake

IDE recomendada: CLion 
