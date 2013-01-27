#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
    // max number of data in name and email fields
    int max_data;
    // max number of addresses in database
    int max_rows;
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void die(const char *message, struct Connection *conn);

void Address_print(struct Address *address)
{
    printf("%d %s %s\n",
           address->id,
           address->name,
           address->email);
}

void Database_load(struct Connection *conn)
{
    int rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to load max_data.", conn);
    }

    rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to load max_rows.", conn);
    }

    struct Database *db = conn->db;
    size_t db_size = sizeof(struct Address) * db->max_rows;
    db->rows = malloc(db_size);

    int i;
    for (i = 0; i < db->max_rows; i++) {
        struct Address *addr = malloc(sizeof(struct Address));
        addr->name = malloc(sizeof(char) * db->max_data);
        addr->email = malloc(sizeof(char) * db->max_data);
        db->rows[i] = *addr;
    }

    printf("Database rows: %d\n", db->max_rows);
    printf("Database max data: %d\n", db->max_data);
    rc = fread(&conn->db->rows, db_size, 1, conn->file);
    if (rc != 1) {
        die("Failed to load database rows.", conn);
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn) {
        die("Memory error", conn);
    }

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db) {
        die("Memory error", conn);
    }

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    }

    if (!conn->file) {
        die("Failed to open the file.", conn);
    }

    return conn;
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1) {
        die("Failed to write database.", conn);
    }

    rc = fflush(conn->file);
    if (rc == -1) {
        die("Cannot flush database.", conn);
    }
}

void Database_create(struct Connection *conn, int max_rows, int max_data)
{
    int i;

    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
    conn->db->rows = malloc(sizeof(struct Address) * max_rows);

    printf("%d\n", max_rows);
    for (i = 0; i < max_rows; i++) {
        // then just assign it
        conn->db->rows[i].id = i;
        conn->db->rows[i].set = 0;
        conn->db->rows[i].name = malloc(sizeof(char) * conn->db->max_data);
        strcpy(conn->db->rows[i].name, '\0');
        conn->db->rows[i].email = malloc(sizeof(char) * conn->db->max_data);
        strcpy(conn->db->rows[i].email, '\0');
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if (addr->set) {
        die("Already set, delete it first.", conn);
    }

    addr->set = 1;
    char *res = strncpy(addr->name, name, conn->db->max_data);
    if (strlen(name) >= conn->db->max_data) {
        addr->name[conn->db->max_data - 1] = '\0';
    }
    if (!res) {
        die("Name copy failed.", conn);
    }

    res = strncpy(addr->email, email, conn->db->max_data);
    if (strlen(email) >= conn->db->max_data) {
        addr->email[conn->db->max_data - 1] = '\0';
    }
    if (!res) {
        die("Email copy failed.", conn);
    }
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set.", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i;
    struct Database *db = conn->db;

    for (i = 0; i < db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
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

void die(const char *message, struct Connection *conn)
{
    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close(conn);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        die("Usage: ex17-dynamic <dbfile> <action> [action params]", NULL);
    }

    char *dbfile = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(dbfile, action);
    int id = 0;
    int max_rows;
    int max_data;

    if (argc > 3) {
        id = atoi(argv[3]);

        if (id >= conn->db->max_rows) {
            die("There's not that many records.", conn);
        }
    }

    switch(action) {
        case 'c':
            printf("Number of rows in database: ");
            scanf("%d", &max_rows);
            printf("Number of data in a field: ");
            scanf("%d", &max_data);
            Database_create(conn, max_rows, max_data);
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4) {
                die("Need an id to get.", conn);
            }

            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6) {
                die("Need id, name, email to set.", conn);
            }

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
            
        case 'd':
            if (argc != 4) {
                die("Need id to delete.", conn);
            }

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

        default:
            die("Invalid action, only: c=create, s=set, d=delete, l=list.", conn);
    }

    Database_close(conn);
}
