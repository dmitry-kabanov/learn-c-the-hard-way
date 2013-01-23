#include <stdio.h>
#include <string.h>

struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    struct Address rows[MAX_ROWS];
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void die(const char *message)
{
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

void Address_print(struct Address *addres)
{
    printf("%d %s %s\n",
           address->id,
           address->name,
           address->email);
}

void Database_load(struct Connection *conn)
{
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1) {
        die("Failed to load database.");
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(Connection));
    if (!conn) {
        die("Memory error");
    }

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db) {
        die("Memory error");
    }

    if (mode == 'c') {
        conn->file = fopen(filename, 'w');
    } else {
        conn->file = fopen(filename, 'r+');
    }

    if (conn->file) {
        Database_load(conn);
    }

    if (!conn->file) {
        die("Failed to open the file.");
    }

    return conn;
}

void Database_close(struct Connection *conn)
{
    if (conn) {
        if (conn->file) {
            fclose(conn->file);
        }
        if (conn->db) {
            free(conn->db);
        }
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1) {
        die("Failed to write database.");
    }

    rc = fflush(conn->file);
    if (rc == -1) {
        die("Cannot flush database.");
    }
}

void Database_create(struct Connection *conn)
{
    int i;

    for (i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}


