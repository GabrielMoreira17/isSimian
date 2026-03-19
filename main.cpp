#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Função que vai montar e validar a matriz para garantir que será NxN
bool validaMatriz(const vector<string> &dna)
{

    int n = dna.size();

    for (const string &linha : dna)
    {
        if (linha.size() != n)
        {
            return false;
        }

        // Valida que as caracteres sejam somente as que estão no DNA
        for (char c : linha)
        {
            if (c != 'A' && c != 'T' && c != 'C' && c != 'G')
            {
                return false;
            }
        }
    }
    return true;
}

// Função que vai verificar a sequência do DNA para validar se existe a sequência de 4 letras iguais
bool isSimian(const vector<string> &dna)
{
    int n = dna.size();
    int cout = 0;

    // realiza a validação na horizontal
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (dna[i][j] == dna[i][j + 1] &&
                dna[i][j] == dna[i][j + 2] &&
                dna[i][j] == dna[i][j + 3])
            {
                cout++;
            }
        }
    }

    // Realiza a validação na vertical
    for (int i = 0; i <= n - 4; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (dna[i][j] == dna[i + 1][j] &&
                dna[i][j] == dna[i + 2][j] &&
                dna[i][j] == dna[i + 3][j])
            {
                cout++;
            }
        }
    }

    // Realiza a validação na diagonal
    for (int i = 0; i <= n - 4; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (dna[i][j] == dna[i + 1][j - 1] &&
                dna[i][j] == dna[i + 2][j - 2] &&
                dna[i][j] == dna[i + 3][j - 3])
            {
                cout++;
            }
        }
    }
    // Retorna Simian caso tenha 1 ou mais sequências
    return cout;
}

int main()
{
    int n;

    cout << "Digite o tamanho da matriz (N): ";
    cin >> n;

    vector<string> dna;

    cout << "Digite as sequencias de DNA: \n";

    for (int i = 0; i < n; i++)
    {
        string linha;
        cin >> linha;

        if (linha.size() != n)
        {
            cout << "Erro: matriz inválida!" << endl;
            return 1;
        }

        dna.push_back(linha);
    }

    if (!validaMatriz(dna))
    {
        cout << "DNA invalido" << endl;
    }

    if (isSimian(dna))
    {
        cout << "Resultado: Simian" << endl;
    }
    else
    {
        cout << "Resultado: Humano" << endl;
    }

    return 0;
}