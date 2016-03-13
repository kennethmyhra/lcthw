#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

//#define MAX_DATA 512
//#define MAX_ROWS 100

struct Address
{
	int id;
	int set;
	char *name;
	char *email;
};

struct Database 
{
	int max_rows;
	int data_size;
	struct Address *rows;
};

struct Connection
{
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
	Database_close(conn);

	if(errno)
	{
		perror(message);
	}
	else
	{
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to load database headers", conn);

	conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address));
	if(!conn->db->rows) die("Could not allocate needed memory for rows", conn);

	int i = 0;
	for(i = 0; i < conn->db->max_rows; i++)
	{
		rc = fread(&conn->db->rows[i], sizeof(struct Address), 1, conn->file);
		if(rc != 1) die("Failed to load Address structure meta data.", conn);

		conn->db->rows[i].name = calloc(conn->db->data_size, sizeof(char));
		if(!conn->db->rows[i].name) die("Could not allocate needed memory for field 'name' of Address structure", conn);

		rc = fread(conn->db->rows[i].name, conn->db->data_size * sizeof(char), 1, conn->file);
		if(rc != 1) die("Failed to load field 'name' of Address structure", conn);

		conn->db->rows[i].email = calloc(conn->db->data_size, sizeof(char));
		if(!conn->db->rows[i].email) die("Could not allocate needed memory for field 'email' of Address structure", conn);

		rc = fread(conn->db->rows[i].email, conn->db->data_size * sizeof(char), 1, conn->file);
		if(rc != 1) die("Failed to load field 'email' of Address structure", conn);
	}
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error", conn);

	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die("Memory error", conn);

	conn->db->max_rows = 0;
	conn->db->data_size = 0;

	if(mode == 'c')
	{
		conn->file = fopen(filename, "w");
	}
	else
	{
		conn->file = fopen(filename, "r+");

		if(conn->file)
		{
			Database_load(conn);
		}
	}

	if(!conn->file) die("Failed to open the file", conn);

	return conn;
}

void Database_close(struct Connection *conn)
{
	int i = 0;
	if(conn)
	{
		if(conn->file) fclose(conn->file);
		if(conn->db->rows) 
		{
			for(i = 0; i < conn->db->max_rows; i++)
			{
				if(conn->db->rows[i].name) free(conn->db->rows[i].name);
				if(conn->db->rows[i].email) free(conn->db->rows[i].email);
			}
			free(conn->db->rows);
		}
		if(conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	rewind(conn->file);
	
	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to write database headers", conn);

	int i = 0;
	for(i = 0; i < conn->db->max_rows; i++)
	{
		rc = fwrite(&conn->db->rows[i], sizeof(struct Address), 1, conn->file);
		if(rc != 1) die("Failed to write Address structure meta data", conn);

		rc = fwrite(conn->db->rows[i].name, conn->db->data_size * sizeof(char), 1, conn->file);
		if(rc != 1) die("Failed to write 'name' field from Address structure", conn);

		rc = fwrite(conn->db->rows[i].email, conn->db->data_size * sizeof(char), 1, conn->file);
		if(rc != 1) die("Failed to write 'email' field from Address structure", conn);
	}

	rc = fflush(conn->file);
	if(rc == -1) die("Cannot flush database", conn);
}

void Database_create(struct Connection *conn, int max_rows, int data_size)
{
	int i = 0;
	
	conn->db->data_size = data_size;
	conn->db->max_rows = max_rows;
	conn->db->rows = malloc(max_rows * sizeof(struct Address));
	for(i = 0; i < max_rows; i++)
	{
		// make a prototype to initialize it
		struct Address addr = { 
			.id = i, 
			.set = 0,
			.name = calloc(data_size, sizeof(char)),
			.email = calloc(data_size, sizeof(char))
		 };
		// then just assign it
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete it first", conn);

	addr->set = 1;
	// WARNING: bug, read the "How to Break it" and fix this
	char *res = strncpy(addr->name, name, conn->db->max_rows);
	if(!res) die("Name copy failed", conn);

	if(conn->db->max_rows > 0) addr->name[conn->db->max_rows - 1] = '\0';

	res = strncpy(addr->email, email, conn->db->max_rows);
	if(!res) die("Email copy failed", conn);

	if(conn->db->max_rows > 0) addr->email[conn->db->max_rows - 1] = '\0';
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];

	if(addr->set)
	{
		Address_print(addr);
	}
	else
	{
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set)
	{
		if(addr->name) 
		{
			free(addr->name);
			addr->name = calloc(conn->db->data_size, sizeof(char));
		}
		if(addr->email) 
		{
			free(addr->email);
			addr->email = calloc(conn->db->data_size, sizeof(char));
		}
		addr->set = 0;
	}
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < db->max_rows; i++)
	{
		struct Address *cur = &db->rows[i];

		if(cur->set)
		{
			Address_print(cur);
		}
	}
}

void Database_list_all_contents(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	printf("max_rows: %d\n", db->max_rows);
	printf("data_size: %d\n", db->data_size);
	for(i = 0; i < db->max_rows; i++)
	{
		struct Address *cur = &db->rows[i];

		printf("id: %d\n", cur->id);
		printf("set: %d\n", cur->set);
		if(cur->set)
		{
			Address_print(cur);
		}
	}

}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);

	int id = 0;
	if(argc > 3 && action != 'c') 
	{
		id = atoi(argv[3]);
		if(id >= conn->db->max_rows) die("There's not that many records", conn);
	}

	switch(action)
	{
		case 'c':
			if(argc != 5) die("Need number of rows and data size", conn);
			int max_rows = atoi(argv[3]);
			int data_size = atoi(argv[4]);
			Database_create(conn, max_rows, data_size);
			Database_write(conn);
			break;

		case 'g':
			if(argc != 4) die("Need an id to get", conn);

			Database_get(conn, id);
			break;

		case 's':
			if(argc != 6) die("Need id, name email to set", conn);

			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;

		case 'd':
			if(argc != 4) die("Need id to delete", conn);

			Database_delete(conn, id);
			Database_write(conn);
			break;

		case 'l':
			Database_list(conn);
			break;

		case 'a':
			Database_list_all_contents(conn);
			break;

		default:
			die("Invalid action, only: c=create, g=get, s=set, d=delete, l=list", conn);
	}

	Database_close(conn);

	return 0;
}
