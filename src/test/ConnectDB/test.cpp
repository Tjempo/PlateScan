/*
#include <iostream>
#include <libpq-fe.h>

int main() {
    // Connection parameters
    const char* conninfo = "dbname=rdw user=sa password=abc123!@# hostaddr=192.168.2.24 port=5432";

    // Connect to database
    PGconn* conn = PQconnectdb(conninfo);

    // Check connection status
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    std::cout << "Connected to database successfully!" << std::endl;

    // Your database operations here

    // Close connection
    PQfinish(conn);

    return 0;
}
*/

#include <iostream>
#include <libpq-fe.h>

int main() {
    // Connection parameters
    const char* conninfo = "XXX";

    // Connect to the database
    PGconn* conn = PQconnectdb(conninfo);

    // Check connection status
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return 1;
    }

    std::cout << "Connected to database successfully!" << std::endl;

    // Execute a SQL query
    // const char* query = "SELECT \"Merk\",\"Handelsbenaming\" FROM kentekens LIMIT 100";
    const char* query = "SELECT COUNT(*) from kentekens where \"Merk\" = 'TOYOTA' ";

    PGresult* res = PQexec(conn, query);

    // Check for successful execution
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query execution failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
        return 1;
    }

    // Process the result
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    std::cout << "Query returned " << rows << " rows and " << cols << " columns." << std::endl;

    // Print column names
    for (int i = 0; i < cols; i++) {
        std::cout << PQfname(res, i) << "\t";
    }
    std::cout << std::endl;

    // Print rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << PQgetvalue(res, i, j) << "\t";
        }
        std::cout << std::endl;
    }

    // Clear result
    PQclear(res);

    // Close the connection
    PQfinish(conn);

    return 0;
}

