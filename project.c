#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<dirent.h>

#include <sodium.h>
#define LINE_LEN 300
int logged_in = 0;

// Constants
#define MAX_ROWS 100
#define MAX_COLS 10
#define MAX_STR_LEN 50
#define MAX_DB_NAME 50
#define MAX_LOGIN_ID 50
#define MAX_PASSWORD 5
#define HASH_SIZE 64

// Structure declarations
struct Cell {
    char str_value[MAX_STR_LEN];
    int int_value;
    float float_value;
    char data_type;
};

struct Table {
    char name[MAX_DB_NAME];
    int rows;
    int columns;
    char row_names[MAX_ROWS][MAX_STR_LEN];
    char column_names[MAX_COLS][MAX_STR_LEN];
    struct Cell data[MAX_ROWS][MAX_COLS];
    char column_types[MAX_COLS];
};

struct Book {
    char title[50];
    char author[50];
    int available;
    char due_date[20];
};

struct Train {
    char name[50];
    int number;
    char time[20];
    char date[20];
    char location[50];
};

struct Ticket {
    char personal_id[20];
    char train_name[50];
    float ticket_price;
};

struct Contact {
    char name[50];
    char phone[20];
    char email[50];
    char address[100];
};

struct Office {
    char department[50];
    char employee_name[50];
    char position[50];
    float salary;
    char join_date[20];
};

struct Student {
    char name[50];
    int roll_number;
    char course[50];
    char department[50];
    float cgpa;
    char admission_date[20];
};

typedef struct {
    int roll;
    char name[50];
    char course[50];
    float marks;
} Student;

// Function Prototypes
int user_check(const char *username);
void register_user();
int login_user();
void logut_user(int *logged_in);
void menu();
int is_valid_database_name(const char *name);
void show_existing_databases(void);
int create_database(void);
int access_database(void);
void Delete_Database(void);
void Table_sorting(void);
void Student_Management(void);
void Library_Management(void);
void Ticket_Management(void);
void Employee_Management(void);
void Book_Management(void);
void Movie_Management(void);
void Music_Management(void);
void Restaurant_Management(void);
void Hotel_Management(void);
void Car_Management(void);
void Contact_Management(void);
int pre_made_libraries(void);
void sort_table(struct Table *table, int column_index, int ascending);
void display_sorted_table(struct Table *table);
void test_table_sorting(void);
void Modify_Table(void);
void create_table_in_database(char *database_name);
int show_tables_in_database(char *database_name);
void modify_existing_table(char *database_name);
void Office_Management(void);
void display_table_data(char *database_name);
void sort_table_in_database(char *database_name);
void display_zron_logo();
void input_table_data(char *database_name);

// Function implementations



int user_check(const char *username) {
    char line[LINE_LEN];

    FILE *fp = fopen("users.txt", "r");
    if(!fp) {
        printf("Error while opening the file.\n");
        printf("***********************************\n\n");
        return 0;
    }

    while(fgets(line, LINE_LEN, fp)) {  // This code block will check if the username is already registered or not.Th is will read the file line by line.
        char fuser[50];
        sscanf(line, "%[^:]", fuser);
        if(strcmp(username, fuser) == 0) {
            fclose(fp);
            return 1;
        } 
    }

    fclose(fp);
    return 0;
}


void register_user() {
    char username[50], password[13];
    char hash[crypto_pwhash_STRBYTES];
    char line[LINE_LEN];
    FILE *fp = fopen("users.txt", "a+");
    if(!fp) {
        printf("Error while opening the file.\n");
        printf("\n***********************************\n");
        return;
    }

    printf("Enter username: ");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    fgets(username, sizeof(username), stdin); // We use fgets istead of gets to keep the user in check acording to the buffer size we've designated and not cause overflow.
    username[strcspn(username, "\n")] = '\0'; // Remove newline from the string because sometimes fgets might get the newline character.
    
    if(user_check(username) == 1) {
        printf("\nUsername already exists.");
        printf("\n***********************************\n\n");
        fclose(fp);
        return;
    }

    printf("Enter password: ");

    fgets(password, sizeof(password), stdin);
    password[strcspn(password,"\n")] = '\0'; 
   
    if(crypto_pwhash_str(hash, password, sizeof(password), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        printf("\nError due to out of memory.");
        printf("***********************************\n\n");
        fclose(fp);
        return;
    }

    fprintf(fp, "%s:%s\n", username, hash); // Stores the credentials to the file.
    fclose(fp);

    printf("\nUser registered successfully.");
    printf("\n***********************************\n\n");

    return;
}

int login_user() {
    char username[50], password[13];
    char line[LINE_LEN];
    FILE *fp = fopen("users.txt", "r");

    if(!fp) {
        printf("Error while opening the file.\n");
        printf("***********************************\n\n");
        return 0;
    }

    printf("Enter username: ");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");

    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    while(fgets(line, LINE_LEN, fp)) {

        line[strcspn(line, "\n")] = '\0';

        char fuser[50], fpass[crypto_pwhash_STRBYTES];
        sscanf(line, "%[^:]:%s", fuser, fpass);

        if(strcmp(username, fuser) == 0) {
            if(crypto_pwhash_str_verify(fpass, password, sizeof(password)) == 0) {
                printf("Login successful.\n");
                printf("***********************************\n\n");
                fclose(fp);
                return 1;
            } else {
                printf("Invalid password.\n");
                printf("***********************************\n\n");
                fclose(fp);
                return 0;
            }
        }
    }

    fclose(fp);
    printf("Invalid User. \n");
    printf("***********************************\n\n");

    return 0;
}

void logut_user(int *logged_in) {
    *logged_in = 0;
    printf("User logged out\n");
    printf("***********************************\n\n");
}

void menu() {
    int choice = 0;
    while(1) {
        printf("***********************************\n"  // We are using " " in every line becuase in C string literals should be in a single line or we have to use " " every time so I decided to put " " every time for better formatting
            " 1) Register new user:\n"
            " 2) Login:\n"
            " 3) Logout:\n"
            " 4) Exit Menu:\n"
            "***********************************\n"
            "(Choose 1,2,3,4 accordingly.)\n"
            "***********************************\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("***********************************\n");

        switch(choice) {
            case 1:
                register_user();
                break;
            case 2:
                if(logged_in == 1) {
                    printf("User is already logged in.");
                    printf("\n***********************************\n\n");
                }
                else {
                    logged_in = login_user(); // We do so because if the login_user() return 0 means something went worng and the user wont be able to login since logged_in value is still 0 and vice versa.
                }
                break;
            case 3:
                if(logged_in ==  1) {
                    logut_user(&logged_in);
                } 
                else {
                    printf("User not logged in.\n");
                    printf("\n***********************************\n\n");
                }
                break;
            default:
                printf("Exited.\n");
                printf("\n***********************************\n\n");
                break;
        }
        if((choice == 4) || (choice == 2 && logged_in == 1)) {
            break; // If the user chooses to exit then we break out of the loop and exit the program.
        }
    }

    
    return;
}




int is_valid_database_name(const char *name) {
    if (name == NULL || strlen(name) == 0 || strlen(name) >= MAX_DB_NAME) {
        return 0;
    }
    
    // Check for invalid characters
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalnum(name[i]) && name[i] != '_' && name[i] != '-') {
            return 0;
        }
    }
    return 1;
}



// Function to sort table based on a column
void sort_table(struct Table* table, int column_index, int ascending) {
    if (column_index < 0 || column_index >= table->columns) {
        printf("Invalid column index for sorting!\n");
        return;
    }

    struct Cell* temp_column = malloc(table->rows * sizeof(struct Cell));
    int* indices = malloc(table->rows * sizeof(int));
    
    if (temp_column == NULL || indices == NULL) {
        printf("Memory allocation failed!\n");
        free(temp_column);
        free(indices);
        return;
    }
    
    for (int i = 0; i < table->rows; i++) {
        indices[i] = i;
        temp_column[i] = table->data[i][column_index];
    }

    char data_type = table->column_types[column_index];
    
    for (int i = 0; i < table->rows - 1; i++) {
        for (int j = 0; j < table->rows - i - 1; j++) {
            int swap = 0;
            
            switch(data_type) {
                case 'i':
                    swap = ascending ? 
                        (temp_column[j].int_value > temp_column[j+1].int_value) :
                        (temp_column[j].int_value < temp_column[j+1].int_value);
                    break;
                case 'f':
                    swap = ascending ? 
                        (temp_column[j].float_value > temp_column[j+1].float_value) :
                        (temp_column[j].float_value < temp_column[j+1].float_value);
                    break;
                case 's':
                    swap = ascending ? 
                        (strcmp(temp_column[j].str_value, temp_column[j+1].str_value) > 0) :
                        (strcmp(temp_column[j].str_value, temp_column[j+1].str_value) < 0);
                    break;
            }
            
            if (swap) {
                struct Cell temp_cell = temp_column[j];
                temp_column[j] = temp_column[j+1];
                temp_column[j+1] = temp_cell;
                
                int temp_index = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp_index;
            }
        }
    }

    struct Table temp_table = *table;
    
    for (int i = 0; i < table->rows; i++) {
        strcpy(temp_table.row_names[i], table->row_names[indices[i]]);
        for (int j = 0; j < table->columns; j++) {
            temp_table.data[i][j] = table->data[indices[i]][j];
        }
    }
    
    *table = temp_table;
    
    free(temp_column);
    free(indices);
    
    printf("Table sorted successfully based on column %s (%s)\n", 
           table->column_names[column_index], 
           ascending ? "ascending" : "descending");
}

// Function to display sorted table
void display_sorted_table(struct Table* table) {
    // Display column headers
    printf("\nTable: %s\n", table->name);
    for (int i = 0; i < table->columns; i++) {
        printf("%-15s", table->column_names[i]);
    }
    printf("\n");
    
    // Display separator
    for (int i = 0; i < table->columns * 15; i++) {
        printf("-");
    }
    printf("\n");
    
    // Display data
    for (int i = 0; i < table->rows; i++) {
        for (int j = 0; j < table->columns; j++) {
            switch(table->column_types[j]) {
                case 'i':
                    printf("%-15d", table->data[i][j].int_value);
                    break;
                case 'f':
                    printf("%-15.2f", table->data[i][j].float_value);
                    break;
                case 's':
                    printf("%-15s", table->data[i][j].str_value);
                    break;
            }
        }
        printf("\n");
    }
}

// Function to test the sorting functionality
void test_table_sorting() {
    struct Table test_table;
    strcpy(test_table.name, "Test Table");
    test_table.rows = 5;
    test_table.columns = 3;
    
    // Set column names and types
    strcpy(test_table.column_names[0], "ID");
    strcpy(test_table.column_names[1], "Name");
    strcpy(test_table.column_names[2], "Score");
    
    test_table.column_types[0] = 'i';  // Integer
    test_table.column_types[1] = 's';  // String
    test_table.column_types[2] = 'f';  // Float
    
    // Add sample data
    for (int i = 0; i < test_table.rows; i++) {
        // ID column
        test_table.data[i][0].int_value = 5 - i;
        
        // Name column
        sprintf(test_table.data[i][1].str_value, "Person%d", i+1);
        
        // Score column
        test_table.data[i][2].float_value = (float)(rand() % 100);
    }
    
    printf("Original Table:\n");
    display_sorted_table(&test_table);
    
    printf("\nSorting by ID (ascending):\n");
    sort_table(&test_table, 0, 1);
    display_sorted_table(&test_table);
    
    printf("\nSorting by Name (ascending):\n");
    sort_table(&test_table, 1, 1);
    display_sorted_table(&test_table);
    
    printf("\nSorting by Score (descending):\n");
    sort_table(&test_table, 2, 0);
    display_sorted_table(&test_table);
}

void Modify_Table(void) {
    char database_name[MAX_DB_NAME];
    printf("\nEnter database name: ");
    scanf("%49s", database_name);

    char table_name[MAX_STR_LEN];
    printf("Enter table name: ");
    scanf("%49s", table_name);

    char table_filename[MAX_STR_LEN];
    snprintf(table_filename, sizeof(table_filename), "%s_%s.txt", database_name, table_name);

    FILE *fptr = fopen(table_filename, "r");
    if (fptr == NULL) {
        printf("Table does not exist\n");
        return;
    }

    struct Table table;
    fscanf(fptr, "Table: %s\nRows: %d\nColumns: %d\n\n", table.name, &table.rows, &table.columns);
    
    // Read column names and types
    fscanf(fptr, "Column Names: ");
    for (int i = 0; i < table.columns; i++) {
        fscanf(fptr, "%s", table.column_names[i]);
    }
    fscanf(fptr, "\nColumn Types: ");
    for (int i = 0; i < table.columns; i++) {
        fscanf(fptr, " %c", &table.column_types[i]);
    }

    // Read data
    fscanf(fptr, "\n\nData:\n");
    for (int i = 0; i < table.rows; i++) {
        for (int j = 0; j < table.columns; j++) {
            switch(table.column_types[j]) {
                case 'i':
                    fscanf(fptr, "%d", &table.data[i][j].int_value);
                    break;
                case 'f':
                    fscanf(fptr, "%f", &table.data[i][j].float_value);
                    break;
                case 's':
                    fscanf(fptr, "%s", table.data[i][j].str_value);
                    break;
            }
        }
    }
    fclose(fptr);

    // Display current table data
    printf("\nCurrent table data:\n");
    display_sorted_table(&table);

    // Modification options
    int choice;
    do {
        printf("\nModification options:\n");
        printf("1. Modify specific cell\n");
        printf("2. Sort table by column\n");
        printf("3. Save and exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                int row, col;
                printf("Enter row index (0-%d): ", table.rows - 1);
                scanf("%d", &row);
                printf("Enter column index (0-%d): ", table.columns - 1);
                scanf("%d", &col);

                if (row >= 0 && row < table.rows && col >= 0 && col < table.columns) {
                    printf("Enter new value for %s (%c): ", table.column_names[col], table.column_types[col]);
                    switch(table.column_types[col]) {
                        case 'i':
                            scanf("%d", &table.data[row][col].int_value);
                            break;
                        case 'f':
                            scanf("%f", &table.data[row][col].float_value);
                            break;
                        case 's':
                            scanf("%49s", table.data[row][col].str_value);
                            break;
                    }
                    printf("Cell modified successfully!\n");
                } else {
                    printf("Invalid indices!\n");
                }
                break;
            }
            case 2: {
                int col, ascending;
                printf("Enter column index to sort (0-%d): ", table.columns - 1);
                scanf("%d", &col);
                printf("Sort order (1-ascending, 0-descending): ");
                scanf("%d", &ascending);
                if (col >= 0 && col < table.columns) {
                    sort_table(&table, col, ascending);
                    printf("\nSorted table:\n");
                    display_sorted_table(&table);
                } else {
                    printf("Invalid column index!\n");
                }
                break;
            }
            case 3: {
                // Save modified table
                fptr = fopen(table_filename, "w");
                if (fptr == NULL) {
                    printf("Error saving table\n");
                    return;
                }
                fprintf(fptr, "Table: %s\nRows: %d\nColumns: %d\n\n", table.name, table.rows, table.columns);
                fprintf(fptr, "Column Names: ");
                for (int i = 0; i < table.columns; i++) {
                    fprintf(fptr, "%s ", table.column_names[i]);
                }
                fprintf(fptr, "\nColumn Types: ");
                for (int i = 0; i < table.columns; i++) {
                    fprintf(fptr, "%c ", table.column_types[i]);
                }
                fprintf(fptr, "\n\nData:\n");

                for (int i = 0; i < table.rows; i++) {
                    for (int j = 0; j < table.columns; j++) {
                        switch(table.column_types[j]) {
                            case 'i':
                                fprintf(fptr, "%d ", table.data[i][j].int_value);
                                break;
                            case 'f':
                                fprintf(fptr, "%.2f ", table.data[i][j].float_value);
                                break;
                            case 's':
                                fprintf(fptr, "%s ", table.data[i][j].str_value);
                                break;
                        }
                    }
                    fprintf(fptr, "\n");
                }
                fclose(fptr);
                printf("Table saved successfully!\n");
                break;
            }
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);
}

void show_existing_databases(void) {
    DIR *d;
    struct dirent *dir;
    int count = 0;
    
    d = opendir(".");
    if (d == NULL) {
        printf("Error accessing directory\n");
        return;
    }

    printf("\nExisting Databases:\n");
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".txt") != NULL) {
            char *name = strdup(dir->d_name);
            if (name) {
                name[strlen(name) - 4] = '\0'; // Remove .txt extension
                printf("%d. %s\n", ++count, name);
                free(name);
            }
        }
    }
    closedir(d);

    if (count == 0) {
        printf("No databases found.\n");
    }
}

void create_table_in_database(char *database_name) {
    if (!is_valid_database_name(database_name)) {
        printf("Invalid database name\n");
        return;
    }

    char filename[MAX_DB_NAME + 4];
    snprintf(filename, sizeof(filename), "%s.txt", database_name);

    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Database does not exist\n");
        return;
    }
    fclose(fptr);

    char table_name[MAX_STR_LEN];
    int rows, cols;

    printf("Enter table name: ");
    if (scanf("%49s", table_name) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter number of rows (max %d): ", MAX_ROWS);
    if (scanf("%d", &rows) != 1 || rows <= 0 || rows > MAX_ROWS) {
        printf("Invalid number of rows\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter number of columns (max %d): ", MAX_COLS);
    if (scanf("%d", &cols) != 1 || cols <= 0 || cols > MAX_COLS) {
        printf("Invalid number of columns\n");
        while (getchar() != '\n');
        return;
    }

    // Create table file
    char table_filename[MAX_STR_LEN];
    snprintf(table_filename, sizeof(table_filename), "%s_%s.txt", database_name, table_name);
    
    fptr = fopen(table_filename, "w");
    if (fptr == NULL) {
        printf("Error creating table\n");
        return;
    }

    fprintf(fptr, "Table: %s\nRows: %d\nColumns: %d\n\n", table_name, rows, cols);
    fclose(fptr);

    // Update database file to include the new table
    fptr = fopen(filename, "a");
    if (fptr == NULL) {
        printf("Error updating database\n");
        return;
    }

    fprintf(fptr, "%s\n", table_name);
    fclose(fptr);
    printf("Table created successfully!\n");
}

int show_tables_in_database(char *database_name) {
    if (!is_valid_database_name(database_name)) {
        printf("Invalid database name\n");
        return 0;
    }

    char filename[MAX_DB_NAME + 4];
    snprintf(filename, sizeof(filename), "%s.txt", database_name);

    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Database does not exist\n");
        return 0;
    }

    char line[MAX_STR_LEN];
    int found_tables = 0;
    int count = 0;

    // Skip database name line
    if (fgets(line, sizeof(line), fptr) == NULL) {
        printf("Error reading database file\n");
        fclose(fptr);
        return 0;
    }

    // Skip "Tables:" line
    if (fgets(line, sizeof(line), fptr) == NULL) {
        printf("Error reading database file\n");
        fclose(fptr);
        return 0;
    }

    printf("\nTables in database '%s':\n", database_name);
    while (fgets(line, sizeof(line), fptr) != NULL) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        if (strlen(line) > 0) {
            printf("%d. %s\n", ++count, line);
            found_tables = 1;
        }
    }

    fclose(fptr);
    if (!found_tables) {
        printf("No tables found in the database.\n");
    }
    return found_tables;
}

void modify_existing_table(char *database_name) {
    if (!show_tables_in_database(database_name)) {
        return;
    }

    char table_name[MAX_STR_LEN];
    printf("\nEnter table name to modify: ");
    if (scanf("%49s", table_name) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }

    char table_filename[MAX_STR_LEN];
    snprintf(table_filename, sizeof(table_filename), "%s_%s.txt", database_name, table_name);

    FILE *fptr = fopen(table_filename, "r");
    if (fptr == NULL) {
        printf("Table does not exist\n");
        return;
    }
    fclose(fptr);

    printf("Table modification will be implemented in a future update.\n");
}

int access_database(void) {
    char database_name[MAX_DB_NAME];
    
    show_existing_databases();
    
    printf("\nEnter database name to access: ");
    if (scanf("%49s", database_name) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return -1;
    }

    if (!is_valid_database_name(database_name)) {
        printf("Invalid database name\n");
        return -1;
    }

    char filename[MAX_DB_NAME + 4];
    snprintf(filename, sizeof(filename), "%s.txt", database_name);

    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Database does not exist\n");
        return -1;
    }

    // Check if database is empty
    fseek(fptr, 0, SEEK_END);
    if (ftell(fptr) == 0) {
        printf("Database is empty\n");
        fclose(fptr);
        return -1;
    }
    fseek(fptr, 0, SEEK_SET);

    int choice;
    do {
        printf("\n========== Database: %s ==========\n", database_name);
        printf("1. Show tables\n");
        printf("2. Create table\n");
        printf("3. Input table data\n");
        printf("4. Modify table\n");
        printf("5. Display table data\n");
        printf("6. Sort table\n");
        printf("7. Back to main menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                if (!show_tables_in_database(database_name)) {
                    printf("No tables found in the database.\n");
                }
                break;
            case 2:
                create_table_in_database(database_name);
                break;
            case 3:
                input_table_data(database_name);
                break;
            case 4:
                Modify_Table();
                break;
            case 5:
                display_table_data(database_name);
                break;
            case 6:
                sort_table_in_database(database_name);
                break;
            case 7:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 7);
    
    fclose(fptr);
    return 0;
}

void Delete_Database() {
    char database_name[50];
    FILE *list_fptr, *temp_fptr;
    char temp_name[50];
    int found = 0;
    
    printf("Enter the name of the database to delete: ");
    scanf("%s", database_name);
    
    // Remove the database file
    if (remove(database_name) == 0) {
        // Remove from databases list
        list_fptr = fopen("databases.txt", "r");
        temp_fptr = fopen("temp.txt", "w");
        
        if (list_fptr != NULL && temp_fptr != NULL) {
            while (fscanf(list_fptr, "%s", temp_name) != EOF) {
                if (strcmp(temp_name, database_name) != 0) {
                    fprintf(temp_fptr, "%s\n", temp_name);
                } else {
                    found = 1;
                }
            }
            
            fclose(list_fptr);
            fclose(temp_fptr);
            
            remove("databases.txt");
            rename("temp.txt", "databases.txt");
        }
        
        if (found) {
            printf("Database deleted successfully\n");
        }
    } else {
        printf("Error deleting database\n");
    }
}

void Table_sorting() {
    struct Table table;
    char database_name[50];
    int column_index, ascending;
    
    printf("Enter database name: ");
    scanf("%s", database_name);
    
    FILE *fptr = fopen(database_name, "r");
    if (fptr == NULL) {
        printf("Error opening database\n");
        return;
    }
    
    // Read table details
    fscanf(fptr, "Table Name: %s\n", table.name);
    fscanf(fptr, "Rows: %d, Columns: %d\n", &table.rows, &table.columns);
    
    printf("Enter column index to sort (0-%d): ", table.columns - 1);
    scanf("%d", &column_index);
    
    printf("Sort order (1-ascending, 0-descending): ");
    scanf("%d", &ascending);
    
    sort_table(&table, column_index, ascending);
    display_sorted_table(&table);
    
    fclose(fptr);
}

int pre_made_libraries(void) {
    int choice;
    do {
        printf("\n========== Pre-defined Libraries ==========\n");
        printf("1. Library Management System\n");
        printf("2. Ticket Management System\n");
        printf("3. Contact Management System\n");
        printf("4. Office Management System\n");
        printf("5. Student Management System\n");
        printf("6. Back to main menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                Library_Management();
                break;
            case 2:
                Ticket_Management();
                break;
            case 3:
                Contact_Management();
                break;
            case 4:
                Office_Management();
                break;
            case 5:
                Student_Management();
                break;
            case 6:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 6);
    return 0;
}

// Contact Management System
void Contact_Management(void) {
    int choice;
    do {
        printf("\n========== Contact Management System ==========\n");
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Back to Libraries Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                struct Contact contact;
                printf("Enter name: ");
                scanf("%49s", contact.name);
                printf("Enter phone: ");
                scanf("%19s", contact.phone);
                printf("Enter email: ");
                scanf("%49s", contact.email);
                printf("Enter address: ");
                scanf("%99s", contact.address);

                FILE *fptr = fopen("contacts.txt", "a");
                if (fptr == NULL) {
                    printf("Error opening file\n");
                    return;
                }
                fprintf(fptr, "%s,%s,%s,%s\n", contact.name, contact.phone, contact.email, contact.address);
                fclose(fptr);
                printf("Contact added successfully!\n");
                break;
            }
            case 2: {
                FILE *fptr = fopen("contacts.txt", "r");
                if (fptr == NULL) {
                    printf("No contacts found\n");
                    return;
                }
                printf("\nContacts:\n");
                printf("Name\tPhone\tEmail\tAddress\n");
                printf("----------------------------------------\n");
                struct Contact contact;
                while (fscanf(fptr, "%49[^,],%19[^,],%49[^,],%99[^\n]\n", 
                    contact.name, contact.phone, contact.email, contact.address) != EOF) {
                    printf("%s\t%s\t%s\t%s\n", contact.name, contact.phone, contact.email, contact.address);
                }
                fclose(fptr);
                break;
            }
            case 3: {
                char search_name[50];
                printf("Enter name to search: ");
                scanf("%49s", search_name);
                
                FILE *fptr = fopen("contacts.txt", "r");
                if (fptr == NULL) {
                    printf("No contacts found\n");
                    return;
                }
                
                struct Contact contact;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%19[^,],%49[^,],%99[^\n]\n", 
                    contact.name, contact.phone, contact.email, contact.address) != EOF) {
                    if (strcmp(contact.name, search_name) == 0) {
                        printf("\nContact found:\n");
                        printf("Name: %s\nPhone: %s\nEmail: %s\nAddress: %s\n", 
                            contact.name, contact.phone, contact.email, contact.address);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Contact not found\n");
                }
                fclose(fptr);
                break;
            }
            case 4: {
                char delete_name[50];
                printf("Enter name to delete: ");
                scanf("%49s", delete_name);
                
                FILE *fptr = fopen("contacts.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                if (fptr == NULL || temp == NULL) {
                    printf("Error opening files\n");
                    return;
                }
                
                struct Contact contact;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%19[^,],%49[^,],%99[^\n]\n", 
                    contact.name, contact.phone, contact.email, contact.address) != EOF) {
                    if (strcmp(contact.name, delete_name) != 0) {
                        fprintf(temp, "%s,%s,%s,%s\n", contact.name, contact.phone, contact.email, contact.address);
                    } else {
                        found = 1;
                    }
                }
                
                fclose(fptr);
                fclose(temp);
                
                remove("contacts.txt");
                rename("temp.txt", "contacts.txt");
                
                if (found) {
                    printf("Contact deleted successfully\n");
                } else {
                    printf("Contact not found\n");
                }
                break;
            }
            case 5:
                printf("Returning to libraries menu...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 5);
}

// Office Management System
void Office_Management(void) {
    int choice;
    do {
        printf("\n========== Office Management System ==========\n");
        printf("1. Add Employee\n");
        printf("2. View Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee Details\n");
        printf("5. Back to Libraries Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                struct Office employee;
                printf("Enter department: ");
                scanf("%49s", employee.department);
                printf("Enter employee name: ");
                scanf("%49s", employee.employee_name);
                printf("Enter position: ");
                scanf("%49s", employee.position);
                printf("Enter salary: ");
                scanf("%f", &employee.salary);
                printf("Enter join date (DD/MM/YYYY): ");
                scanf("%19s", employee.join_date);

                FILE *fptr = fopen("office.txt", "a");
                if (fptr == NULL) {
                    printf("Error opening file\n");
                    return;
                }
                fprintf(fptr, "%s,%s,%s,%.2f,%s\n", 
                    employee.department, employee.employee_name, employee.position, 
                    employee.salary, employee.join_date);
                fclose(fptr);
                printf("Employee added successfully!\n");
                break;
            }
            case 2: {
                FILE *fptr = fopen("office.txt", "r");
                if (fptr == NULL) {
                    printf("No employees found\n");
                    return;
                }
                printf("\nEmployees:\n");
                printf("Department\tName\tPosition\tSalary\tJoin Date\n");
                printf("--------------------------------------------------------\n");
                struct Office employee;
                while (fscanf(fptr, "%49[^,],%49[^,],%49[^,],%f,%19[^\n]\n", 
                    employee.department, employee.employee_name, employee.position, 
                    &employee.salary, employee.join_date) != EOF) {
                    printf("%s\t%s\t%s\t%.2f\t%s\n", 
                        employee.department, employee.employee_name, employee.position, 
                        employee.salary, employee.join_date);
                }
                fclose(fptr);
                break;
            }
            case 3: {
                char search_name[50];
                printf("Enter employee name to search: ");
                scanf("%49s", search_name);
                
                FILE *fptr = fopen("office.txt", "r");
                if (fptr == NULL) {
                    printf("No employees found\n");
                    return;
                }
                
                struct Office employee;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%49[^,],%49[^,],%f,%19[^\n]\n", 
                    employee.department, employee.employee_name, employee.position, 
                    &employee.salary, employee.join_date) != EOF) {
                    if (strcmp(employee.employee_name, search_name) == 0) {
                        printf("\nEmployee found:\n");
                        printf("Department: %s\nName: %s\nPosition: %s\nSalary: %.2f\nJoin Date: %s\n", 
                            employee.department, employee.employee_name, employee.position, 
                            employee.salary, employee.join_date);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Employee not found\n");
                }
                fclose(fptr);
                break;
            }
            case 4: {
                char update_name[50];
                printf("Enter employee name to update: ");
                scanf("%49s", update_name);
                
                FILE *fptr = fopen("office.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                if (fptr == NULL || temp == NULL) {
                    printf("Error opening files\n");
                    return;
                }
                
                struct Office employee;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%49[^,],%49[^,],%f,%19[^\n]\n", 
                    employee.department, employee.employee_name, employee.position, 
                    &employee.salary, employee.join_date) != EOF) {
                    if (strcmp(employee.employee_name, update_name) == 0) {
                        found = 1;
                        printf("Enter new department: ");
                        scanf("%49s", employee.department);
                        printf("Enter new position: ");
                        scanf("%49s", employee.position);
                        printf("Enter new salary: ");
                        scanf("%f", &employee.salary);
                        printf("Enter new join date (DD/MM/YYYY): ");
                        scanf("%19s", employee.join_date);
                    }
                    fprintf(temp, "%s,%s,%s,%.2f,%s\n", 
                        employee.department, employee.employee_name, employee.position, 
                        employee.salary, employee.join_date);
                }
                
                fclose(fptr);
                fclose(temp);
                
                remove("office.txt");
                rename("temp.txt", "office.txt");
                
                if (found) {
                    printf("Employee details updated successfully\n");
                } else {
                    printf("Employee not found\n");
                }
                break;
            }
            case 5:
                printf("Returning to libraries menu...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 5);
}

// Student Management System
void Student_Management(void) {
    int choice;
    do {
        printf("\n========== Student Management System ==========\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student Details\n");
        printf("5. Back to Libraries Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                struct Student student;
                printf("Enter student name: ");
                scanf("%49s", student.name);
                printf("Enter roll number: ");
                scanf("%d", &student.roll_number);
                printf("Enter course: ");
                scanf("%49s", student.course);
                printf("Enter department: ");
                scanf("%49s", student.department);
                printf("Enter CGPA: ");
                scanf("%f", &student.cgpa);
                printf("Enter admission date (DD/MM/YYYY): ");
                scanf("%19s", student.admission_date);

                FILE *fptr = fopen("students.txt", "a");
                if (fptr == NULL) {
                    printf("Error opening file\n");
                    return;
                }
                fprintf(fptr, "%s,%d,%s,%s,%.2f,%s\n", 
                    student.name, student.roll_number, student.course, 
                    student.department, student.cgpa, student.admission_date);
                fclose(fptr);
                printf("Student added successfully!\n");
                break;
            }
            case 2: {
                FILE *fptr = fopen("students.txt", "r");
                if (fptr == NULL) {
                    printf("No students found\n");
                    return;
                }
                printf("\nStudents:\n");
                printf("Name\tRoll No\tCourse\tDepartment\tCGPA\tAdmission Date\n");
                printf("----------------------------------------------------------------\n");
                struct Student student;
                while (fscanf(fptr, "%49[^,],%d,%49[^,],%49[^,],%f,%19[^\n]\n", 
                    student.name, &student.roll_number, student.course, 
                    student.department, &student.cgpa, student.admission_date) != EOF) {
                    printf("%s\t%d\t%s\t%s\t%.2f\t%s\n", 
                        student.name, student.roll_number, student.course, 
                        student.department, student.cgpa, student.admission_date);
                }
                fclose(fptr);
                break;
            }
            case 3: {
                int search_roll;
                printf("Enter roll number to search: ");
                scanf("%d", &search_roll);
                
                FILE *fptr = fopen("students.txt", "r");
                if (fptr == NULL) {
                    printf("No students found\n");
                    return;
                }
                
                struct Student student;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%d,%49[^,],%49[^,],%f,%19[^\n]\n", 
                    student.name, &student.roll_number, student.course, 
                    student.department, &student.cgpa, student.admission_date) != EOF) {
                    if (student.roll_number == search_roll) {
                        printf("\nStudent found:\n");
                        printf("Name: %s\nRoll Number: %d\nCourse: %s\nDepartment: %s\nCGPA: %.2f\nAdmission Date: %s\n", 
                            student.name, student.roll_number, student.course, 
                            student.department, student.cgpa, student.admission_date);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Student not found\n");
                }
                fclose(fptr);
                break;
            }
            case 4: {
                int update_roll;
                printf("Enter roll number to update: ");
                scanf("%d", &update_roll);
                
                FILE *fptr = fopen("students.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                if (fptr == NULL || temp == NULL) {
                    printf("Error opening files\n");
                    return;
                }
                
                struct Student student;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%d,%49[^,],%49[^,],%f,%19[^\n]\n", 
                    student.name, &student.roll_number, student.course, 
                    student.department, &student.cgpa, student.admission_date) != EOF) {
                    if (student.roll_number == update_roll) {
                        found = 1;
                        printf("Enter new course: ");
                        scanf("%49s", student.course);
                        printf("Enter new department: ");
                        scanf("%49s", student.department);
                        printf("Enter new CGPA: ");
                        scanf("%f", &student.cgpa);
                    }
                    fprintf(temp, "%s,%d,%s,%s,%.2f,%s\n", 
                        student.name, student.roll_number, student.course, 
                        student.department, student.cgpa, student.admission_date);
                }
                
                fclose(fptr);
                fclose(temp);
                
                remove("students.txt");
                rename("temp.txt", "students.txt");
                
                if (found) {
                    printf("Student details updated successfully\n");
                } else {
                    printf("Student not found\n");
                }
                break;
            }
            case 5:
                printf("Returning to libraries menu...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 5);
}

// Library Management System
void Library_Management(void) {
    int choice;
    do {
        printf("\n========== Library Management System ==========\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Back to Libraries Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                struct Book book;
                printf("Enter book title: ");
                scanf("%49s", book.title);
                printf("Enter author name: ");
                scanf("%49s", book.author);
                book.available = 1;
                strcpy(book.due_date, "N/A");

                FILE *fptr = fopen("library.txt", "a");
                if (fptr == NULL) {
                    printf("Error opening file\n");
                    return;
                }
                fprintf(fptr, "%s,%s,%d,%s\n", book.title, book.author, book.available, book.due_date);
                fclose(fptr);
                printf("Book added successfully!\n");
                break;
            }
            case 2: {
                FILE *fptr = fopen("library.txt", "r");
                if (fptr == NULL) {
                    printf("No books found\n");
                    return;
                }
                printf("\nBooks:\n");
                printf("Title\tAuthor\tAvailable\tDue Date\n");
                printf("----------------------------------------\n");
                struct Book book;
                while (fscanf(fptr, "%49[^,],%49[^,],%d,%19[^\n]\n", 
                    book.title, book.author, &book.available, book.due_date) != EOF) {
                    printf("%s\t%s\t%s\t%s\n", 
                        book.title, book.author, 
                        book.available ? "Yes" : "No", 
                        book.due_date);
                }
                fclose(fptr);
                break;
            }
            case 3: {
                char search_title[50];
                printf("Enter book title to search: ");
                scanf("%49s", search_title);
                
                FILE *fptr = fopen("library.txt", "r");
                if (fptr == NULL) {
                    printf("No books found\n");
                    return;
                }
                
                struct Book book;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%49[^,],%d,%19[^\n]\n", 
                    book.title, book.author, &book.available, book.due_date) != EOF) {
                    if (strcmp(book.title, search_title) == 0) {
                        printf("\nBook found:\n");
                        printf("Title: %s\nAuthor: %s\nAvailable: %s\nDue Date: %s\n", 
                            book.title, book.author, 
                            book.available ? "Yes" : "No", 
                            book.due_date);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Book not found\n");
                }
                fclose(fptr);
                break;
            }
            case 4: {
                char issue_title[50];
                printf("Enter book title to issue: ");
                scanf("%49s", issue_title);
                
                FILE *fptr = fopen("library.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                if (fptr == NULL || temp == NULL) {
                    printf("Error opening files\n");
                    return;
                }
                
                struct Book book;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%49[^,],%d,%19[^\n]\n", 
                    book.title, book.author, &book.available, book.due_date) != EOF) {
                    if (strcmp(book.title, issue_title) == 0 && book.available) {
                        found = 1;
                        book.available = 0;
                        time_t now;
                        time(&now);
                        struct tm *local = localtime(&now);
                        local->tm_mday += 14; // Due date is 14 days from now
                        mktime(local);
                        strftime(book.due_date, sizeof(book.due_date), "%d/%m/%Y", local);
                        printf("Book issued successfully. Due date: %s\n", book.due_date);
                    }
                    fprintf(temp, "%s,%s,%d,%s\n", book.title, book.author, book.available, book.due_date);
                }
                
                fclose(fptr);
                fclose(temp);
                
                if (!found) {
                    printf("Book not found or not available\n");
                    remove("temp.txt");
                } else {
                    remove("library.txt");
                    rename("temp.txt", "library.txt");
                }
                break;
            }
            case 5: {
                char return_title[50];
                printf("Enter book title to return: ");
                scanf("%49s", return_title);
                
                FILE *fptr = fopen("library.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                if (fptr == NULL || temp == NULL) {
                    printf("Error opening files\n");
                    return;
                }
                
                struct Book book;
                int found = 0;
                while (fscanf(fptr, "%49[^,],%49[^,],%d,%19[^\n]\n", 
                    book.title, book.author, &book.available, book.due_date) != EOF) {
                    if (strcmp(book.title, return_title) == 0 && !book.available) {
                        found = 1;
                        book.available = 1;
                        strcpy(book.due_date, "N/A");
                        printf("Book returned successfully\n");
                    }
                    fprintf(temp, "%s,%s,%d,%s\n", book.title, book.author, book.available, book.due_date);
                }
                
                fclose(fptr);
                fclose(temp);
                
                if (!found) {
                    printf("Book not found or already returned\n");
                    remove("temp.txt");
                } else {
                    remove("library.txt");
                    rename("temp.txt", "library.txt");
                }
                break;
            }
            case 6:
                printf("Returning to libraries menu...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 6);
}

// Ticket Management System
void Ticket_Management(void) {
    int choice;
    do {
        printf("\n========== Ticket Management System ==========\n");
        printf("1. Book Ticket\n");
        printf("2. View Tickets\n");
        printf("3. Search Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. Back to Libraries Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                struct Ticket ticket;
                printf("Enter personal ID: ");
                scanf("%19s", ticket.personal_id);
                printf("Enter train name: ");
                scanf("%49s", ticket.train_name);
                printf("Enter ticket price: ");
                scanf("%f", &ticket.ticket_price);

                FILE *fptr = fopen("tickets.txt", "a");
                if (fptr == NULL) {
                    printf("Error opening file\n");
                    return;
                }
                fprintf(fptr, "%s,%s,%.2f\n", ticket.personal_id, ticket.train_name, ticket.ticket_price);
                fclose(fptr);
                printf("Ticket booked successfully!\n");
                break;
            }
            case 2: {
                FILE *fptr = fopen("tickets.txt", "r");
                if (fptr == NULL) {
                    printf("No tickets found\n");
                    return;
                }
                printf("\nTickets:\n");
                printf("Personal ID\tTrain Name\tPrice\n");
                printf("----------------------------------------\n");
                struct Ticket ticket;
                while (fscanf(fptr, "%19[^,],%49[^,],%f\n", 
                    ticket.personal_id, ticket.train_name, &ticket.ticket_price) != EOF) {
                    printf("%s\t%s\t%.2f\n", 
                        ticket.personal_id, ticket.train_name, ticket.ticket_price);
                }
                fclose(fptr);
                break;
            }
            case 3: {
                char search_id[20];
                printf("Enter personal ID to search: ");
                scanf("%19s", search_id);
                
                FILE *fptr = fopen("tickets.txt", "r");
                if (fptr == NULL) {
                    printf("No tickets found\n");
                    return;
                }
                
                struct Ticket ticket;
                int found = 0;
                while (fscanf(fptr, "%19[^,],%49[^,],%f\n", 
                    ticket.personal_id, ticket.train_name, &ticket.ticket_price) != EOF) {
                    if (strcmp(ticket.personal_id, search_id) == 0) {
                        printf("\nTicket found:\n");
                        printf("Personal ID: %s\nTrain Name: %s\nPrice: %.2f\n", 
                            ticket.personal_id, ticket.train_name, ticket.ticket_price);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("Ticket not found\n");
                }
                fclose(fptr);
                break;
            }
            case 4: {
                char cancel_id[20];
                printf("Enter personal ID to cancel ticket: ");
                scanf("%19s", cancel_id);
                
                FILE *fptr = fopen("tickets.txt", "r");
                FILE *temp = fopen("temp.txt", "w");
                if (fptr == NULL || temp == NULL) {
                    printf("Error opening files\n");
                    return;
                }
                
                struct Ticket ticket;
                int found = 0;
                while (fscanf(fptr, "%19[^,],%49[^,],%f\n", 
                    ticket.personal_id, ticket.train_name, &ticket.ticket_price) != EOF) {
                    if (strcmp(ticket.personal_id, cancel_id) != 0) {
                        fprintf(temp, "%s,%s,%.2f\n", 
                            ticket.personal_id, ticket.train_name, ticket.ticket_price);
                    } else {
                        found = 1;
                    }
                }
                
                fclose(fptr);
                fclose(temp);
                
                if (!found) {
                    printf("Ticket not found\n");
                    remove("temp.txt");
                } else {
                    remove("tickets.txt");
                    rename("temp.txt", "tickets.txt");
                    printf("Ticket cancelled successfully\n");
                }
                break;
            }
            case 5:
                printf("Returning to libraries menu...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 5);
}

int create_database(void) {
    char database_name[MAX_DB_NAME];
    printf("\nEnter database name: ");
    if (scanf("%49s", database_name) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return -1;
    }

    if (!is_valid_database_name(database_name)) {
        printf("Invalid database name. Use only alphanumeric characters, underscore, and hyphen.\n");
        return -1;
    }

    char filename[MAX_DB_NAME + 4]; // +4 for ".txt"
    snprintf(filename, sizeof(filename), "%s.txt", database_name);

    FILE *fptr = fopen(filename, "r");
    if (fptr != NULL) {
        printf("Database already exists!\n");
        fclose(fptr);
        return -1;
    }

    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error creating database\n");
        return -1;
    }

    fprintf(fptr, "Database: %s\nTables:\n", database_name);
    fclose(fptr);
    printf("Database created successfully!\n");
    return 0;
}

void display_table_data(char *database_name) {
    char table_name[MAX_STR_LEN];
    printf("\nEnter table name to display: ");
    if (scanf("%49s", table_name) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }

    char table_filename[MAX_STR_LEN];
    snprintf(table_filename, sizeof(table_filename), "%s_%s.txt", database_name, table_name);

    FILE *fptr = fopen(table_filename, "r");
    if (fptr == NULL) {
        printf("Table does not exist\n");
        return;
    }

    struct Table table;
    char line[MAX_STR_LEN];
    
    // Read table header
    fscanf(fptr, "Table: %s\nRows: %d\nColumns: %d\n\n", 
           table.name, &table.rows, &table.columns);

    // Display column headers
    printf("\nTable: %s\n", table.name);
    printf("Columns: ");
    for (int i = 0; i < table.columns; i++) {
        printf("%s ", table.column_names[i]);
    }
    printf("\n");

    // Display data
    printf("\nData:\n");
    while (fgets(line, sizeof(line), fptr) != NULL) {
        printf("%s", line);
    }

    fclose(fptr);
}

void sort_table_in_database(char *database_name) {
    char table_name[MAX_STR_LEN];
    printf("\nEnter table name to sort: ");
    if (scanf("%49s", table_name) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }

    char table_filename[MAX_STR_LEN];
    snprintf(table_filename, sizeof(table_filename), "%s_%s.txt", database_name, table_name);

    FILE *fptr = fopen(table_filename, "r");
    if (fptr == NULL) {
        printf("Table does not exist\n");
        return;
    }

    struct Table table;
    fscanf(fptr, "Table: %s\nRows: %d\nColumns: %d\n\n", 
           table.name, &table.rows, &table.columns);

    // Display available columns for sorting
    printf("\nAvailable columns for sorting:\n");
    for (int i = 0; i < table.columns; i++) {
        printf("%d. %s\n", i + 1, table.column_names[i]);
    }

    int column_choice;
    printf("\nEnter column number to sort by: ");
    if (scanf("%d", &column_choice) != 1 || column_choice < 1 || column_choice > table.columns) {
        printf("Invalid column choice\n");
        fclose(fptr);
        return;
    }

    int sort_order;
    printf("Sort order (1 for ascending, 0 for descending): ");
    if (scanf("%d", &sort_order) != 1 || (sort_order != 0 && sort_order != 1)) {
        printf("Invalid sort order\n");
        fclose(fptr);
        return;
    }

    // Sort the table
    sort_table(&table, column_choice - 1, sort_order);
    
    // Display sorted table
    display_sorted_table(&table);

    fclose(fptr);
}

void display_zron_logo() {
    printf("\n");
    printf("+-----------------------------------------+\n");
    printf(" _______ _____ ___       _  _____ \n");
    printf("|___   / |  ___| \\    | |/ /  _ \\\n");
    printf("   /  /  | |__ | |\\   | || | | | |\n");
    printf("  /  /   |  __|| | \\  | || | | | |\n");
    printf(" /  /__  | |___| |  \\ / /| | |_| |\n");
    printf("/_____ |  \\____\\   \\_/  \\___ / \n");
    printf("+-----------------------------------------+\n");
    printf("\n");
}

// Function to input table data
void input_table_data(char *database_name) {
    char table_name[MAX_STR_LEN];
    printf("\nEnter table name: ");
    if (scanf("%49s", table_name) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }

    char table_filename[MAX_STR_LEN];
    snprintf(table_filename, sizeof(table_filename), "%s_%s.txt", database_name, table_name);

    FILE *fptr = fopen(table_filename, "w");
    if (fptr == NULL) {
        printf("Error creating table file\n");
        return;
    }

    int rows, cols;
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    printf("Enter number of columns: ");
    scanf("%d", &cols);

    if (rows <= 0 || cols <= 0 || rows > MAX_ROWS || cols > MAX_COLS) {
        printf("Invalid dimensions. Maximum allowed: %d rows, %d columns\n", MAX_ROWS, MAX_COLS);
        fclose(fptr);
        return;
    }

    struct Table table;
    strcpy(table.name, table_name);
    table.rows = rows;
    table.columns = cols;

    // Input column names and types
    printf("\nEnter column information:\n");
    for (int i = 0; i < cols; i++) {
        printf("Column %d name: ", i + 1);
        scanf("%49s", table.column_names[i]);
        do {
            printf("Column %d type (i-integer, f-float, s-string): ", i + 1);
            scanf(" %c", &table.column_types[i]);
        } while (table.column_types[i] != 'i' && table.column_types[i] != 'f' && table.column_types[i] != 's');
    }

    // Input data
    printf("\nEnter table data:\n");
    for (int i = 0; i < rows; i++) {
        printf("Row %d:\n", i + 1);
        for (int j = 0; j < cols; j++) {
            printf("Enter %s (%c): ", table.column_names[j], table.column_types[j]);
            switch(table.column_types[j]) {
                case 'i':
                    scanf("%d", &table.data[i][j].int_value);
                    break;
                case 'f':
                    scanf("%f", &table.data[i][j].float_value);
                    break;
                case 's':
                    scanf("%49s", table.data[i][j].str_value);
                    break;
            }
        }
    }

    // Save table to file
    fprintf(fptr, "Table: %s\nRows: %d\nColumns: %d\n\n", table.name, table.rows, table.columns);
    fprintf(fptr, "Column Names: ");
    for (int i = 0; i < table.columns; i++) {
        fprintf(fptr, "%s ", table.column_names[i]);
    }
    fprintf(fptr, "\nColumn Types: ");
    for (int i = 0; i < table.columns; i++) {
        fprintf(fptr, "%c ", table.column_types[i]);
    }
    fprintf(fptr, "\n\nData:\n");

    // Save data
    for (int i = 0; i < table.rows; i++) {
        for (int j = 0; j < table.columns; j++) {
            switch(table.column_types[j]) {
                case 'i':
                    fprintf(fptr, "%d ", table.data[i][j].int_value);
                    break;
                case 'f':
                    fprintf(fptr, "%.2f ", table.data[i][j].float_value);
                    break;
                case 's':
                    fprintf(fptr, "%s ", table.data[i][j].str_value);
                    break;
            }
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
    printf("\nTable created and data saved successfully!\n");
}

int main() {

    menu();
    
    if(logged_in == 1) {
    // Display logo once after successful login
    display_zron_logo();
    printf("Welcome to the Database Management System.\n\n");
    
    int choice;
    do {
        printf("Database Management System\n");
        printf("1. Show Existing Databases\n");
        printf("2. Create Database\n");
        printf("3. Access Database\n");
        printf("4. Delete Database\n");
        printf("5. Sort Table\n");
        printf("6. Pre-defined Libraries\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch(choice) {
            case 1:
                show_existing_databases();
                break;
            case 2:
                create_database();
                break;
            case 3:
                access_database();
                break;
            case 4:
                Delete_Database();
                break;
            case 5:
                Table_sorting();
                break;
            case 6:
                pre_made_libraries();
                break;
            case 7:
                printf("\nThank you for using ZRON Database Management System\n");
                display_zron_logo();
                logut_user(&logged_in);
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    } while(choice != 7);
    



    }
    return 0;
}