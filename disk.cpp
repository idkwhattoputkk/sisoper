#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Request {
    int track;
};

int main() {
    string line;
    ifstream inputFile("input2.txt");

    if (!inputFile.is_open()) {
        cerr << "No se pudo abrir el archivo input.txt" << endl;
        return 1;
    }

    string algorithm;
    int headPosition;
    vector<Request> requests;

    while (getline(inputFile, line)) {
        if (line.find("Algorithm:") != string::npos) {
            // Leer el nombre del algoritmo
            inputFile >> algorithm;
        } else if (line.find("Head:") != string::npos) {
            // Leer la posición inicial de la cabeza
            inputFile >> headPosition;
        } else if (line.find("sequence:") != string::npos) {
            // Leer las solicitudes
            while (inputFile >> line) {
                if (line == "end")
                    break;
                Request request;
                request.track = stoi(line);
                requests.push_back(request);
            }
        }
    }

    inputFile.close();

    // Aplicar el algoritmo FCFS
    int totalSeekTime = 0;
    int currentTrack = headPosition;
    cout << "Secuencia de acceso al disco (FCFS): " << endl;

    for (const Request& request : requests) {
        cout << currentTrack << " -> " << request.track << endl;
        totalSeekTime += abs(currentTrack - request.track);
        currentTrack = request.track;
    }

    cout << "Tiempo total de búsqueda: " << totalSeekTime << endl;

    return 0;
}
