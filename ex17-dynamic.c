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
    char msg[100];
    size_t field_size;
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

    field_size = sizeof(char) * db->max_data;
    int i;
    for (i = 0; i < db->max_rows; i++) {
        rc = fread(&db->rows[i].id, sizeof(int), 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to load id for row %d.", i);
            die(msg, conn);
        }

        rc = fread(&db->rows[i].set, sizeof(int), 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to load set flag for row %d.", i);
            die(msg, conn);
        }

        db->rows[i].name = malloc(field_size);
        rc = fread(db->rows[i].name, sizeof(char) * db->max_data, 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to load name field for row %d.", i);
            die(msg, conn);
        }

        db->rows[i].email = malloc(field_size);
        rc = fread(db->rows[i].email, sizeof(char) * db->max_data, 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to load email field for row %d.", i);
            die(msg, conn);
        }
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
    int rc;
    int i;
    char msg[100];
    struct Database *db = conn->db;

    rewind(conn->file);

    rc = fwrite(&db->max_data, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to write max_data value.", conn);
    }

    rc = fwrite(&db->max_rows, sizeof(int), 1, conn->file);
    if (rc != 1) {
        die("Failed to write max_rows values.", conn);
    }

    for (i = 0; i < db->max_rows; i++) {
        rc = fwrite(&db->rows[i].id, sizeof(int), 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to write id field for row %d.", i);
            die(msg, conn);
        }

        rc = fwrite(&db->rows[i].set, sizeof(int), 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to write set field for row %d.", i);
            die(msg, conn);
        }

        rc = fwrite(db->rows[i].name, sizeof(char) * db->max_data, 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to write name field for row %d.", i);
            die(msg, conn);
        }

        rc = fwrite(db->rows[i].email, sizeof(char) * db->max_data, 1, conn->file);
        if (rc != 1) {
            sprintf(msg, "Failed to write email field for row %d.", i);
            die(msg, conn);
        }
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

    char *empty_field_value = malloc(sizeof(char) * max_data);
    for (i = 0; i < max_data; i++) {
        empty_field_value[i] = '\0';
    }

    for (i = 0; i < max_rows; i++) {
        conn->db->rows[i].id = i + 1;
        conn->db->rows[i].set = 0;
        conn->db->rows[i].name = malloc(sizeof(char) * conn->db->max_data);
        strcpy(conn->db->rows[i].name, empty_field_value);
        conn->db->rows[i].email = malloc(sizeof(char) * conn->db->max_data);
        strcpy(conn->db->rows[i].email, empty_field_value);
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id - 1];
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
    struct Address *addr = &conn->db->rows[id - 1];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set.", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    conn->db->rows[id - 1].id = id;
    conn->db->rows[id - 1].set = 0;
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

void Database_find(struct Connection *conn, char *needle)
{
    struct Database *db = conn->db;

    int i;
    int count = 0;
    char *found = malloc(sizeof(char) * db->max_rows);

    for (i = 0; i < db->max_rows; i++) {
        found[i] = 0;
    }

    for (i = 0; i < db->max_rows; i++) {
        if (db->rows[i].set) {
            if (strstr(db->rows[i].name, needle) ||
                strstr(db->rows[i].email, needle))
            {
                count++;
                found[i] = 1;
            }
        }
    }

    if (!count) {
        printf("No records found for search string '%s'.\n", needle);
    } else {
        printf("Found %d records:\n", count);
        for (i = 0; i < db->max_rows; i++) {
            if (found[i]) {
                Address_print(&db->rows[i]);
            }
        }
    }
    free(found);
}

void Database_close(struct Connection *conn)
{
    if (conn) {
        if (conn->file) {
            fclose(conn->file);
        }
        if (conn->db) {
            if (conn->db->rows) {
                int i;
                for (i = 0; i < conn->db->max_rows; i++) {
                    if (conn->db->rows[i].name) {
                        free(conn->db->rows[i].name);
                    }
                    if (conn->db->rows[i].email) {
                        free(conn->db->rows[i].email);
                    }
                }
                free(conn->db->rows);
            }
            free(conn->db);
        }
        free(conn);
    }
}

void checkIntegerInRange(int number, int minValue, int maxValue, struct Connection *conn)
{
    if (number < 1) {
        die("ID must be positive.", conn);
    }

    if (number > conn->db->max_rows) {
        die("There's not that many records.", conn);
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
    int id = 1;
    int max_rows;
    int max_data;

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
            id = atoi(argv[3]);
            checkIntegerInRange(id, 1, conn->db->max_rows, conn);

            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6) {
                die("Need id, name, email to set.", conn);
            }
            id = atoi(argv[3]);
            checkIntegerInRange(id, 1, conn->db->max_rows, conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
            
        case 'd':
            if (argc != 4) {
                die("Need id to delete.", conn);
            }
            id = atoi(argv[3]);
            checkIntegerInRange(id, 1, conn->db->max_rows, conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
            
        case 'f':
            if (argc != 4) {
                die("Need a search string.", conn);
            }

            Database_find(conn, argv[3]);
            break;

        default:
            die("Invalid action, only: c=create, s=set, d=delete, l=list.", conn);
    }

    Database_close(conn);
}

