#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

using namespace std;


void BuscaExaustiva(std::vector<std::vector<int>>& grafo, int numVertices,
                    std::vector<int>& cliqueAtual, std::vector<int>& melhorClique,
                    int verticeAtual, int startVertex, int endVertex); 

std::vector<int> EncontrarCliqueMaxima(std::vector<std::vector<int>>& grafo, int numVertices, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int verticesPerProcess = numVertices / size;
    int remainder = numVertices % size;

    int startVertex = rank * verticesPerProcess;
    int endVertex = (rank == size - 1) ? (startVertex + verticesPerProcess + remainder) : (startVertex + verticesPerProcess);

    std::vector<int> cliqueMaxima;
    std::vector<int> melhorClique;

    BuscaExaustiva(grafo, numVertices, cliqueMaxima, melhorClique, 0, startVertex, endVertex);

    if (rank == 0) {
        std::vector<int> tempClique(melhorClique.size());
        MPI_Gather(melhorClique.data(), melhorClique.size(), MPI_INT, tempClique.data(), melhorClique.size(), MPI_INT, 0, comm);

        for (int i = 0; i < size; ++i) {
            if (tempClique[i * melhorClique.size()] > melhorClique.size()) {
                melhorClique.assign(tempClique.begin() + i * melhorClique.size(),
                                    tempClique.begin() + (i + 1) * melhorClique.size());
            }
        }
    } else {
        MPI_Gather(melhorClique.data(), melhorClique.size(), MPI_INT, nullptr, 0, MPI_INT, 0, comm);
    }

    return melhorClique;
}

void BuscaExaustiva(std::vector<std::vector<int>>& grafo, int numVertices,
                    std::vector<int>& cliqueAtual, std::vector<int>& melhorClique,
                    int verticeAtual, int startVertex, int endVertex) {
    if (verticeAtual == numVertices) {
        if (cliqueAtual.size() > melhorClique.size()) {
            melhorClique = cliqueAtual;
        }
        return;
    }

    bool podeAdicionar = true;
    for (int u : cliqueAtual) {
        if (grafo[u][verticeAtual] == 0) {
            podeAdicionar = false;
            break;
        }
    }

    if (podeAdicionar) {
        cliqueAtual.push_back(verticeAtual);
        BuscaExaustiva(grafo, numVertices, cliqueAtual, melhorClique, verticeAtual + 1, startVertex, endVertex);
        cliqueAtual.pop_back(); 
    }

    BuscaExaustiva(grafo, numVertices, cliqueAtual, melhorClique, verticeAtual + 1, startVertex, endVertex);
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
        grafo[v - 1][u - 1] = 1;  
    }

    arquivo.close();

    return grafo;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int numVertices;
    vector<vector<int>> grafo;

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <graph_file>" << endl;
        MPI_Finalize();
        return 1;
    }

    grafo = LerGrafo(argv[1], numVertices);
 
    MPI_Comm comm = MPI_COMM_WORLD;

    double tdata = MPI_Wtime();

    std::vector<int> cliqueMax = EncontrarCliqueMaxima(grafo, numVertices, comm);

    tdata = MPI_Wtime() - tdata;

    int rank;
    MPI_Comm_rank(comm, &rank);

    if (rank == 0) {
        std::cout << "Clique Máxima: ";
        for (auto it = cliqueMax.begin(); it != cliqueMax.end(); ++it) {
            std::cout << *it + 1 << " ";
        }
        std::cout << std::endl;
        cout << "Clique máxima encontrada de tamanho " << cliqueMax.size() << endl;
        cout << "Duração: " << tdata << endl;
        cout << endl;
    }

    MPI_Finalize();

    return 0;
}
