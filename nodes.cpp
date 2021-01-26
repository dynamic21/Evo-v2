#include <iostream>
#include <vector>
#include <ctime>
#define defaultMutationRate 0.5
#define defaultMutationAmplitude 0.1

using namespace std;

double randomDouble()
{
    return ((double)rand() / RAND_MAX);
}

class node
{
public:
    double preMemory;
    double memory;
    double mutationRate;
    double mutationAmplitude;
    int bias;
    int numberOfConnections;
    vector<int> connections;
    vector<double> weights;

    void initialize(vector<int> givenConnections, double givenmutationRate, double givenMutationAmplitude)
    {
        mutationRate = givenmutationRate;
        mutationAmplitude = givenMutationAmplitude;
        numberOfConnections = givenConnections.size();
        for (int i = 0; i < numberOfConnections; i++)
        {
            connections.push_back(givenConnections[i]);
            weights.push_back(0);
        }
    }

    void mutate()
    {
        if (randomDouble() < mutationRate)
        {
            mutationRate += (randomDouble() * 2 - 1) * mutationAmplitude;
        }
        if (randomDouble() < mutationRate)
        {
            mutationAmplitude += (randomDouble() * 2 - 1) * mutationAmplitude;
        }
        if (randomDouble() < mutationRate)
        {
            bias += (randomDouble() * 2 - 1) * mutationAmplitude;
        }
        for (int i = 0; i < numberOfConnections; i++)
        {
            if (randomDouble() < mutationRate)
            {
                weights[i] += (randomDouble() * 2 - 1) * mutationAmplitude;
            }
        }
    }

    node copy()
    {
        node newNode;
        newNode.preMemory = preMemory;
        newNode.memory = memory;
        newNode.mutationRate = mutationRate;
        newNode.mutationAmplitude = mutationAmplitude;
        newNode.bias = bias;
        newNode.numberOfConnections = numberOfConnections;
        for (int i = 0; i < numberOfConnections; i++)
        {
            newNode.connections.push_back(connections[i]);
            newNode.weights.push_back(weights[i]);
        }
        return newNode;
    }

    void info()
    {
        cout << "preMemory: " << preMemory << endl;
        cout << "memory: " << memory << endl;
        cout << "mutationRate: " << mutationRate << endl;
        cout << "mutationAmplitude: " << mutationAmplitude << endl;
        cout << "bias: " << bias << endl;
        cout << "numberOfConnections: " << numberOfConnections << endl;
        cout << "connections: ";
        for (int i = 0; i < numberOfConnections; i++)
        {
            cout << connections[i] << " ";
        }
        cout << endl;
        cout << "weights: ";
        for (int i = 0; i < numberOfConnections; i++)
        {
            cout << weights[i] << " ";
        }
        cout << endl;
    }
};

class agent
{
public:
    int numberOfNodes;
    vector<node> nodes;

    void initialize(int numberOdInputs, int numberOfOutputs)
    {
        int i;
        numberOfNodes = numberOdInputs + numberOfOutputs;
        for (i = 0; i < numberOfNodes; i++)
        {
            node newNode;
            newNode.initialize({(i + 1) % numberOfNodes}, defaultMutationRate, defaultMutationAmplitude);
            newNode.mutate();
            nodes.push_back(newNode);
        }
    }

    void info()
    {
        cout << "numberOfNodes: " << numberOfNodes << endl;
        cout << "nodes: " << endl
             << endl;
        for (int i = 0; i < numberOfNodes; i++)
        {
            nodes[i].info();
            cout << endl;
        }
    }
};

int main()
{
    srand(time(NULL));
    randomDouble();

    agent newAgent;
    newAgent.initialize(3, 2);
    newAgent.info();
    int done;
    cout << "Enter Any Key To Exit: " << endl;
    cin >> done;
}
