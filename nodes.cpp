#include <iostream>
#include <vector>
#include <ctime>
#define defaultMutationRate 0.8
#define defaultMutationAmplitude 0.1

using namespace std;

double randomDouble()
{
    return ((double)rand() / RAND_MAX);
}

class node
{
public:
    bool currentStateActive;
    bool previousStateActive;
    double defaultMemory;
    double futureMemory;
    double memory;
    double mutationRate;
    double mutationAmplitude;
    double bias;
    int numberOfConnections;
    vector<int> connections;
    vector<double> weights;

    void info()
    {
        cout << "currentStateActive: " << currentStateActive << endl;
        cout << "previousStateActive: " << previousStateActive << endl;
        cout << "defaultMemory: " << defaultMemory << endl;
        cout << "futureMemory: " << futureMemory << endl;
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

    void initialize(vector<int> givenConnections, double givenmutationRate, double givenMutationAmplitude)
    {
        currentStateActive = true;
        defaultMemory = 0;
        bias = 0;
        mutationRate = givenmutationRate;
        mutationAmplitude = givenMutationAmplitude;
        numberOfConnections = givenConnections.size();
        for (int i = 0; i < numberOfConnections; i++)
        {
            connections.push_back(givenConnections[i]);
            weights.push_back(0);
        }
    }

    node copy()
    {
        node newNode;
        newNode.currentStateActive = currentStateActive;
        newNode.previousStateActive = previousStateActive;
        newNode.defaultMemory = defaultMemory;
        newNode.futureMemory = futureMemory;
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

    void mutate()
    {
        if (randomDouble() < mutationRate)
        {
            defaultMemory += (randomDouble() * 2 - 1) * mutationAmplitude;
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
        if (randomDouble() < mutationRate)
        {
            mutationRate += (randomDouble() * 2 - 1) * mutationAmplitude;
        }
        if (randomDouble() < mutationRate)
        {
            mutationAmplitude += (randomDouble() * 2 - 1) * mutationAmplitude;
        }
    }
};

class agent
{
public:
    int numberOfInputNodes;
    int numberOfOutputNodes;
    int numberOfNodes;
    vector<node> nodes;

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

    void initialize(int numberOfInputs, int numberOfOutputs)
    {
        numberOfInputNodes = numberOfInputs;
        numberOfOutputNodes = numberOfOutputs;
        numberOfNodes = numberOfInputs + numberOfOutputs;
        for (int i = 0; i < numberOfNodes; i++)
        {
            node newNode;
            newNode.initialize({(i + 1) % numberOfNodes}, defaultMutationRate, defaultMutationAmplitude);
            newNode.mutate();
            nodes.push_back(newNode);
        }
    }

    void factoryReset()
    {
        for (int i = 0; i < numberOfNodes; i++)
        {
            nodes[i].currentStateActive = true;
            nodes[i].memory = nodes[i].defaultMemory;
        }
    }

    vector<double> evaluate(vector<int> givenInput)
    {
        for (int i = 0; i < numberOfInputNodes; i++)
        {
            nodes[i].memory += givenInput[i];
        }
        for (int i = 0; i < numberOfNodes; i++)
        {
            nodes[i].previousStateActive = nodes[i].currentStateActive;
            nodes[i].currentStateActive = false;
            nodes[i].futureMemory = 0;
        }
        for (int i = 0; i < numberOfNodes; i++)
        {
            if (nodes[i].previousStateActive)
            {
                for (int j = 0; j < nodes[i].numberOfConnections; j++)
                {
                    double sum = nodes[i].memory * nodes[i].weights[j];
                    if (sum >= 0)
                    {
                        int connectionNumber = nodes[i].connections[j];
                        nodes[connectionNumber].futureMemory += sum;
                        nodes[connectionNumber].currentStateActive = true;
                    }
                }
            }
        }
        for (int i = 0; i < numberOfNodes; i++)
        {
            if (nodes[i].currentStateActive)
            {
                nodes[i].memory = nodes[i].futureMemory + nodes[i].bias;
            }
        }
        vector<double> output;
        for (int i = numberOfInputNodes; i < numberOfInputNodes + numberOfOutputNodes; i++)
        {
            if (nodes[i].currentStateActive)
            {
                output.push_back(nodes[i].memory);
            }
            else
            {
                output.push_back(0);
            }
        }
        return output;
    }
};

// remember to factory reset the agents before use

// void timeFunction()
// {
//     clock_t start = clock();
//     int a = 2;
//     for (int i = 0; i < 10000000; i++)
//     {
//         int b = 3;
//         a = b;
//     }
//     cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
// }

int main()
{
    srand(time(NULL));
    randomDouble();

    agent newAgent;
    newAgent.initialize(3, 2);
    newAgent.factoryReset();
    vector<double> output = newAgent.evaluate({0, 0, 0});
    for (int i = 0; i < output.size(); i++)
    {
        cout << output[i] << " ";
    }
    cout << endl
         << endl;
    newAgent.info();

    int done;
    cout << "Enter Any Key To Exit: " << endl;
    cin >> done;
}
