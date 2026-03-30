#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <vector>
#include <sqlite3.h>

using json = nlohmann::json;
using namespace std;

sqlite3 *db;

bool validaMatriz(const vector<string> &dna)
{
    int n = dna.size();

    if (n == 0)
        return false;

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

bool isSimian(const vector<string> &dna)
{
    int n = dna.size();
    int count = 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j <= n - 4; j++)
            if (dna[i][j] == dna[i][j + 1] &&
                dna[i][j] == dna[i][j + 2] &&
                dna[i][j] == dna[i][j + 3])
                count++;

    for (int i = 0; i <= n - 4; i++)
        for (int j = 0; j < n; j++)
            if (dna[i][j] == dna[i + 1][j] &&
                dna[i][j] == dna[i + 2][j] &&
                dna[i][j] == dna[i + 3][j])
                count++;

    for (int i = 0; i <= n - 4; i++)
        for (int j = 0; j <= n - 4; j++)
            if (dna[i][j] == dna[i + 1][j + 1] &&
                dna[i][j] == dna[i + 2][j + 2] &&
                dna[i][j] == dna[i + 3][j + 3])
                count++;

    for (int i = 0; i <= n - 4; i++)
        for (int j = 3; j < n; j++)
            if (dna[i][j] == dna[i + 1][j - 1] &&
                dna[i][j] == dna[i + 2][j - 2] &&
                dna[i][j] == dna[i + 3][j - 3])
                count++;

    return count >= 2;
}

void initDB()
{
    sqlite3_open("dna.db", &db);

    const char *sql =
        "CREATE TABLE IF NOT EXISTS dna ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "sequence TEXT UNIQUE,"
        "is_simian INTEGER);";

    char *err;
    sqlite3_exec(db, sql, 0, 0, &err);
}

string joinDNA(const vector<string> &dna)
{
    string result;
    for (auto &s : dna)
        result += s;
    return result;
}

void saveDNA(const string &dna, bool is_simian)
{
    string sql = "INSERT OR IGNORE INTO dna (sequence, is_simian) VALUES (?, ?);";

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, dna.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, is_simian ? 1 : 0);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

int main()
{
    initDB();

    httplib::Server svr;

    // SIMIAN
    svr.Post("/simian", [](const httplib::Request &req, httplib::Response &res)
             {
        try {
            auto body = json::parse(req.body);

            if (!body.contains("dna")) {
                res.status = 400;
                res.set_content("Campo 'dna' ausente", "text/plain");
                return;
            }

            vector<string> dna = body["dna"].get<vector<string>>();

            if (!validaMatriz(dna)) {
                res.status = 400;
                res.set_content("DNA inválido", "text/plain");
                return;
            }

            bool simian = isSimian(dna);

            string dnaStr = joinDNA(dna);
            saveDNA(dnaStr, simian);

            if (simian) {
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

    // STATS
    svr.Get("/stats", [](const httplib::Request &, httplib::Response &res)
            {
        int simian = 0, human = 0;

        sqlite3_stmt *stmt;

        sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM dna WHERE is_simian = 1;", -1, &stmt, 0);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            simian = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);

        sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM dna WHERE is_simian = 0;", -1, &stmt, 0);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            human = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);

        double ratio = human == 0 ? 0 : (double)simian / human;

        json response = {
            {"count_mutant_dna", simian},
            {"count_human_dna", human},
            {"ratio", ratio}
        };

        res.set_content(response.dump(), "application/json"); });

    int port = 5555;

    cout << "API rodando em http://localhost:" << port << endl;

    svr.listen("0.0.0.0", port);

    sqlite3_close(db);

    return 0;
}