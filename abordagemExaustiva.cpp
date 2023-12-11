#include <iostream>
#include <fstream>
#include <omp.h>
#include <vector>
using namespace std;


// Function declaration
void BuscaExaustiva(std::vector<std::vector<int>>& grafo, int numVertices,
                    std::vector<int>& cliqueAtual, std::vector<int>& melhorClique,
                    int verticeAtual);


std::vector<int> EncontrarCliqueMaxima(std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> cliqueMaxima;
    std::vector<int> melhorClique;

    BuscaExaustiva(grafo, numVertices, cliqueMaxima, melhorClique, 0);

    return melhorClique;
}

void BuscaExaustiva(std::vector<std::vector<int>>& grafo, int numVertices,
                    std::vector<int>& cliqueAtual, std::vector<int>& melhorClique,
                    int verticeAtual) {
    if (verticeAtual == numVertices) {
        // Verifica se a clique atual é maior que a melhor encontrada até agora
        if (cliqueAtual.size() > melhorClique.size()) {
            melhorClique = cliqueAtual;
        }
        return;
    }

    // Tenta adicionar o vértice atual 
    bool podeAdicionar = true;
    for (int u : cliqueAtual) {
        if (grafo[u][verticeAtual] == 0) {
            podeAdicionar = false;
            break;
        }
    }

    if (podeAdicionar) {
        cliqueAtual.push_back(verticeAtual);
        BuscaExaustiva(grafo, numVertices, cliqueAtual, melhorClique, verticeAtual + 1);
        cliqueAtual.pop_back(); // Volta ao estado anterior
    }

    // Continua a busca sem adicionar o vértice atual à clique atual
    BuscaExaustiva(grafo, numVertices, cliqueAtual, melhorClique, verticeAtual + 1);
}

  std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();

    return grafo;
}


int main(int numVertices, char* argv[]){

  vector<vector<int>> grafo;

  grafo = LerGrafo("grafo.txt", numVertices);
  
  double tdata = omp_get_wtime();

  std::vector<int> cliqueMax = EncontrarCliqueMaxima(grafo, numVertices);

  tdata = omp_get_wtime() - tdata;



  std::cout << "Clique Máxima: ";
  for (auto it = cliqueMax.begin(); it != cliqueMax.end(); ++it) {
    std::cout << *it + 1 << " ";
}
  std::cout << std::endl;
  cout << "Clique máxima encontrada de tamanho: " << cliqueMax.size() <<endl;     ;
  cout << "Duração: " << tdata << endl;
  cout << endl;



  return 0;
}
