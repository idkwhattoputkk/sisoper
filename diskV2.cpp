#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Request {
    int track;
};

void executeFCFS(const vector<Request>& requests, int headPosition) {
    int totalSeekTime = 0;
    int currentTrack = headPosition;
    cout << "Secuencia de acceso al disco (FCFS): " << endl;

    for (const Request& request : requests) {
        cout << currentTrack << " -> " << request.track << endl;
        totalSeekTime += abs(currentTrack - request.track);
        currentTrack = request.track;
    }

    cout << "Tiempo total de búsqueda: " << totalSeekTime << endl;
}

void executeSSTF(vector<Request> requests, int headPosition) {
    int totalSeekTime = 0;
    int currentTrack = headPosition;
    cout << "Secuencia de acceso al disco (SSTF): " << endl;

    while (!requests.empty()) {
        auto closest = min_element(requests.begin(), requests.end(), 
        [currentTrack](const Request& a, const Request& b) {
            return abs(a.track - currentTrack) < abs(b.track - currentTrack);
        });

        Request nextRequest = *closest;
        requests.erase(closest);

        cout << currentTrack << " -> " << nextRequest.track << endl;
        totalSeekTime += abs(currentTrack - nextRequest.track);
        currentTrack = nextRequest.track;
    }

    cout << "Tiempo total de búsqueda: " << totalSeekTime << endl;
}

void executeSCAN(vector<Request> requests, int headPosition, int diskSize) {
    sort(requests.begin(), requests.end(), [](const Request& a, const Request& b) {
        return a.track < b.track;
    });

    int totalSeekTime = 0;
    int currentTrack = headPosition;
    cout << "Secuencia de acceso al disco (SCAN): " << endl;

    // Mover a la derecha primero
    for (const auto& request : requests) {
        if (request.track >= currentTrack) {
            cout << currentTrack << " -> " << request.track << endl;
            totalSeekTime += abs(currentTrack - request.track);
            currentTrack = request.track;
        }
    }

    // Luego a la izquierda
    for (auto it = requests.rbegin(); it != requests.rend(); ++it) {
        if (it->track < headPosition) {
            cout << currentTrack << " -> " << it->track << endl;
            totalSeekTime += abs(currentTrack - it->track);
            currentTrack = it->track;
        }
    }

    cout << "Tiempo total de búsqueda: " << totalSeekTime << endl;
}

void executeCSCAN(vector<Request> requests, int headPosition, int diskSize) {
    sort(requests.begin(), requests.end(), [](const Request& a, const Request& b) {
        return a.track < b.track;
    });

    int totalSeekTime = 0;
    int currentTrack = headPosition;
    cout << "Secuencia de acceso al disco (C-SCAN): " << endl;

    // Mover a la derecha y luego al principio
    for (const auto& request : requests) {
        if (request.track >= currentTrack) {
            cout << currentTrack << " -> " << request.track << endl;
            totalSeekTime += abs(currentTrack - request.track);
            currentTrack = request.track;
        }
    }

    // Ir al inicio y continuar
    totalSeekTime += abs(diskSize - 1 - currentTrack) + diskSize - 1;
    currentTrack = 0;

    for (const auto& request : requests) {
        if (request.track < headPosition) {
            cout << currentTrack << " -> " << request.track << endl;
            totalSeekTime += abs(currentTrack - request.track);
            currentTrack = request.track;
        }
    }

    cout << "Tiempo total de búsqueda: " << totalSeekTime << endl;
}

void executeLOOK(vector<Request> requests, int headPosition) {
    sort(requests.begin(), requests.end(), [](const Request& a, const Request& b) {
        return a.track < b.track;
    });

    int totalSeekTime = 0;
    int currentTrack = headPosition;
    cout << "Secuencia de acceso al disco (LOOK): " << endl;

    // Mover a la derecha primero
    for (const auto& request : requests) {
        if (request.track >= currentTrack) {
            cout << currentTrack << " -> " << request.track << endl;
            totalSeekTime += abs(currentTrack - request.track);
            currentTrack = request.track;
        }
    }

    // Luego a la izquierda
    for (auto it = requests.rbegin(); it != requests.rend(); ++it) {
        if (it->track < headPosition) {
            cout << currentTrack << " -> " << it->track << endl;
            totalSeekTime += abs(currentTrack - it->track);
            currentTrack = it->track;
        }
    }

    cout << "Tiempo total de búsqueda: " << totalSeekTime << endl;
}

void executeCLOOK(vector<Request> requests, int headPosition) {
    sort(requests.begin(), requests.end(), [](const Request& a, const Request& b) {
        return a.track < b.track;
    });

    int totalSeekTime = 0;
    int currentTrack = headPosition;
    cout << "Secuencia de acceso al disco (C-LOOK): " << endl;

    // Mover a la derecha y luego al menor track
    for (const auto& request : requests) {
        if (request.track >= currentTrack) {
            cout << currentTrack << " -> " << request.track << endl;
            totalSeekTime += abs(currentTrack - request.track);
            currentTrack = request.track;
        }
    }

    if (!requests.empty()) {
        auto it = requests.begin();
        totalSeekTime += abs(it->track - currentTrack);
        currentTrack = it->track;

        for (; it != requests.end(); ++it) {
            if (it->track < headPosition) {
                cout << currentTrack << " -> " << it->track << endl;
                totalSeekTime += abs(currentTrack - it->track);
                currentTrack = it->track;
            }
        }
    }

    cout << "Tiempo total de búsqueda: " << totalSeekTime << endl;
}

int main() {
    // Lectura del archivo y obtención de las solicitudes
    string line;
    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cerr << "No se pudo abrir el archivo input.txt" << endl;
        return 1;
    }
    string algorithm;
    int headPosition;
    vector<Request> requests;
    int diskSize = 200; // se modifica el valor del disco segun lo necesitado.
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

    if (algorithm == "FCFS") {
        executeFCFS(requests, headPosition);
    } else if (algorithm == "SSTF") {
        executeSSTF(requests, headPosition);
    } else if (algorithm == "SCAN") {
        executeSCAN(requests, headPosition, diskSize);
    } else if (algorithm == "C-SCAN") {
        executeCSCAN(requests, headPosition, diskSize);
    } else if (algorithm == "LOOK") {
        executeLOOK(requests, headPosition);
    } else if (algorithm == "C-LOOK") {
        executeCLOOK(requests, headPosition);
    } else {
        cerr << "Algoritmo no reconocido." << endl;
    }

    return 0;
}
