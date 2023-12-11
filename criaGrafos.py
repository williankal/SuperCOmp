import networkx as nx
import random

# Parâmetros
num_vertices = 100 # Número de vértices no grafo
probabilidade_conexao = 0.7  # Probabilidade de haver uma aresta entre dois vértices (ajuste conforme necessário)

# Crie um grafo aleatório densamente conectado
grafo = nx.fast_gnp_random_graph(num_vertices, probabilidade_conexao)

# Nome do arquivo de saída
nome_arquivo = "grafo.txt"

# Abra o arquivo para escrita
with open(nome_arquivo, 'w') as arquivo:
    # Escreva a quantidade de vértices e número de arestas na primeira linha
    arquivo.write(f"{num_vertices} {grafo.number_of_edges()}\n")

    # Escreva as arestas no formato de lista de adjacência
    for aresta in grafo.edges():
        arquivo.write(f"{aresta[0]+1} {aresta[1]+1}\n")  # +1 para ajustar os índices (começando em 1)

print(f"Grafo densamente conectado gerado e salvo em '{nome_arquivo}'.")



# Nome do arquivo de entradapara paralelizar parte do processo, especificamente as iterações sobre os vértices. A diretiva #pragma omp parallel for é usada para paralelizar o loop, e #pra
# Nome do arquivo de entrada
nome_arquivo = "grafo.txt"

# Abrir o arquivo e pular a primeira linha
with open(nome_arquivo, 'r') as arquivo:
    next(arquivo)  # Pula a primeira linha

    # Lê o grafo a partir das linhas restantes
    G = nx.parse_adjlist(arquivo)

# Encontrar todas as cliques maximais                   Q
cliques_maximais = list(nx.find_cliques(G))
clique_maxima = max(cliques_maximais, key=len)
tamamho_maxima = len(clique_maxima)


print(f"Cliques maximais encontradas de tamanho {tamamho_maxima}: ")
for  clique in cliques_maximais:
    if len(clique) == tamamho_maxima:
        clique = sorted([int(number) for number in clique])
        print(clique)
