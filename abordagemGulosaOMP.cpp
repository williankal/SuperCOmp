#include <iostream>
#include <fstream>
#include <omp.h>
#include <vector>
using namespace std;

// Function to find the maximum clique in a graph
std::vector<int> EncontrarCliqueMaxima(std::vector<std::vector<int>>& grafo, int numVertices) {
    // Initialize vectors to store the maximum clique and candidates
    std::vector<int> cliqueMaxima;
    std::vector<int> candidatos;

    // Initialize the candidates vector with all vertices
    for (int i = 0; i < numVertices; ++i) {
        candidatos.push_back(i);
    }

    // Iterate until there are no more candidates
    while (!candidatos.empty()) {
        // Get the last candidate
        int v = candidatos.back();
        candidatos.pop_back();

        // Flag to check if the current vertex can be added to the clique
        bool podeAdicionar = true;

        // Check adjacency with vertices in the current clique in parallel
        #pragma omp parallel for
        for (int u : cliqueMaxima) {
            if (grafo[u][v] == 0) {
                #pragma omp critical
                {
                    podeAdicionar = false;
                }
            }
        }

        // If the current vertex can be added to the clique
        if (podeAdicionar) {
            // Add the vertex to the clique in a critical section
            #pragma omp critical
            {
                cliqueMaxima.push_back(v);
            }

            // Initialize a vector for new candidates
            std::vector<int> novosCandidatos;

            // Check adjacency with the remaining candidates in parallel
            #pragma omp parallel for
            for (int u : candidatos) {
                bool adjacenteATodos = true;

                // Check adjacency with vertices in the current clique
                for (int c : cliqueMaxima) {
                    if (grafo[u][c] == 0) {
                        adjacenteATodos = false;
                        break;
                    }
                }

                // If adjacent to all vertices in the current clique, add to new candidates
                if (adjacenteATodos) {
                    #pragma omp critical
                    {
                        novosCandidatos.push_back(u);
                    }
                }
            }

            // Update the candidates vector in a critical section
            #pragma omp critical
            {
                candidatos = novosCandidatos;
            }
        }
    }

    return cliqueMaxima;
}

// Function to read a graph from a file
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    // Read edges from the file and update the adjacency matrix
    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // The graph is undirected
    }

    arquivo.close();

    return grafo;
}

int main(int numVertices, char* argv[]) {
    // Read the graph from a file
    vector<vector<int>> grafo;
    grafo = LerGrafo("grafo.txt", numVertices);

    // Measure the execution time of finding the maximum clique
    double tdata = omp_get_wtime();
    std::vector<int> cliqueMax = EncontrarCliqueMaxima(grafo, numVertices);
    tdata = omp_get_wtime() - tdata;

    // Print the maximum clique
    std::cout << "Clique Máxima: ";
    for (auto it = cliqueMax.rbegin(); it != cliqueMax.rend(); ++it) {
        std::cout << *it + 1 << " ";
    }

    // Print the size of the maximum clique and the duration of the computation
    std::cout << std::endl;
    cout << "Clique máxima encontrada de tamanho:" << cliqueMax.size() << endl;
    cout << "Duração: " << tdata << endl;
    cout << endl;

    return 0;
}
