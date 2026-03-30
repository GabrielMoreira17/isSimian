#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <vector>

using json = nlohmann::json;
using namespace std;

bool isSimian(const vector<string> &dna)
{
    int n = dna.size();
    int count = 0;

    // Horizontal
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= n - 4; j++)
        {
            if (dna[i][j] == dna[i][j + 1] &&
                dna[i][j] == dna[i][j + 2] &&
                dna[i][j] == dna[i][j + 3])
            {
                count++;
            }
        }
    }

    // Vertical
    for (int i = 0; i <= n - 4; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (dna[i][j] == dna[i + 1][j] &&
                dna[i][j] == dna[i + 2][j] &&
                dna[i][j] == dna[i + 3][j])
            {
                count++;
            }
        }
    }

    // Diagonal principal
    for (int i = 0; i <= n - 4; i++)
    {
        for (int j = 0; j <= n - 4; j++)
        {
            if (dna[i][j] == dna[i + 1][j + 1] &&
                dna[i][j] == dna[i + 2][j + 2] &&
                dna[i][j] == dna[i + 3][j + 3])
            {
                count++;
            }
        }
    }

    return count >= 2;
}

bool validaMatriz(const vector<string> &dna)
{
    int n = dna.size();

    for (const string &linha : dna)
    {
        if (linha.size() != n)
            return false;

        for (char c : linha)
        {
            if (c != 'A' && c != 'T' && c != 'C' && c != 'G')
                return false;
        }
    }
    return true;
}

int main()
{
    httplib::Server svr;

    svr.Post("/simian", [](const httplib::Request &req, httplib::Response &res)
    {
        try {
            auto body = json::parse(req.body);

            vector<string> dna = body["dna"].get<vector<string>>();

            if (!validaMatriz(dna)) {
                res.status = 400;
                res.set_content("DNA inválido", "text/plain");
                return;
            }

            if (isSimian(dna)) {
                res.status = 200;
                res.set_content("Simian", "text/plain");
            } else {
                res.status = 403;
                res.set_content("Human", "text/plain");
            }

        } catch (...) {
            res.status = 400;
            res.set_content("Erro no JSON", "text/plain");
        } });

    int port = 8080;
    if (getenv("PORT")) {
        port = stoi(getenv("PORT"));
    }
    svr.listen("0.0.0.0", port);

    cout << "API rodando em http://localhost:$`port`\n";
}
