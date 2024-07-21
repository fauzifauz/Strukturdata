#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "cpp_crud";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

// Fungsi untuk terhubung ke database
MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Berhasil terhubung ke database." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

// Fungsi untuk menambah buku
void create_book(const string& title, const string& author, const string& publisher, const string& year, const string& category) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO buku (title, author, publisher, year, category) VALUES ('" + title + "', '" + author + "', '" + publisher + "', '" + year + "', '" + category + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

// Fungsi untuk menampilkan semua buku
void get_books() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM buku")) {
            cerr << "SELECT gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result gagal: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Judul: " << row[1] << ", Penulis: " << row[2] << ", Penerbit: " << row[3] << ", Tahun: " << row[4] << ", Kategori: " << row[5] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

// Fungsi untuk memperbarui buku
void update_book(int book_id, const string& title, const string& author, const string& publisher, const string& year, const string& category) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE buku SET title = '" << title << "', author = '" << author << "', publisher = '" << publisher << "', year = '" << year << "', category = '" << category << "' WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil diperbarui." << endl;
        }
        mysql_close(conn);
    }
}

// Fungsi untuk menghapus buku
void delete_book(int book_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM buku WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

// Fungsi menampilkan header perpustakaan
void display_header() {
    cout << "**********************************************\n";
    cout << "*                                            *\n";
    cout << "*          Perpustakaan Esa Unggul           *\n";
    cout << "*                                            *\n";
    cout << "**********************************************\n" << endl;
}

// Fungsi menu admin
void admin_menu() {
    int choice;
    while (true) {
        cout << "\nMenu Admin:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Semua Buku\n";
        cout << "3. Perbarui Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Logout\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        if (choice == 1) {
            string title, author, publisher, year, category;
            cout << "Masukkan judul: ";
            cin.ignore();
            getline(cin, title);
            cout << "Masukkan penulis: ";
            getline(cin, author);
            cout << "Masukkan penerbit: ";
            getline(cin, publisher);
            cout << "Masukkan tahun: ";
            getline(cin, year);
            cout << "Masukkan kategori: ";
            getline(cin, category);
            create_book(title, author, publisher, year, category);
        } else if (choice == 2) {
            get_books();
        } else if (choice == 3) {
            int book_id;
            string title, author, publisher, year, category;
            cout << "Masukkan ID buku untuk diperbarui: ";
            cin >> book_id;
            cin.ignore();
            cout << "Masukkan judul baru: ";
            getline(cin, title);
            cout << "Masukkan penulis baru: ";
            getline(cin, author);
            cout << "Masukkan penerbit baru: ";
            getline(cin, publisher);
            cout << "Masukkan tahun baru: ";
            getline(cin, year);
            cout << "Masukkan kategori baru: ";
            getline(cin, category);
            update_book(book_id, title, author, publisher, year, category);
        } else if (choice == 4) {
            int book_id;
            cout << "Masukkan ID buku untuk dihapus: ";
            cin >> book_id;
            delete_book(book_id);
        } else if (choice == 5) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

// Fungsi menu user
void user_menu() {
    int choice;
    while (true) {
        cout << "\nMenu User:\n";
        cout << "1. Tampilkan Semua Buku\n";
        cout << "2. Logout\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        if (choice == 1) {
            get_books();
        } else if (choice == 2) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

// Fungsi login
bool login() {
    string username, password;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;

    // Ganti dengan kredensial yang valid
    if (username == "admin" && password == "adminpass") {
        return true;
    } else if (username == "user" && password == "userpass") {
        return true;
    } else {
        cout << "Username atau password salah." << endl;
        return false;
    }
}

// Fungsi utama
int main() {
    display_header();

    cout << "Informasi Login:\n";
    cout << "Username: admin, Password: adminpass" << endl;
    cout << "Username: user, Password: userpass  " << endl;

    if (!login()) {
        cout << "Gagal login. Program keluar." << endl;
        return 1;
    }

    while (true) {
        int role;
        cout << "Pilih peran:\n";
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "3. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> role;

        if (role == 1) {
            admin_menu();
        } else if (role == 2) {
            user_menu();
        } else if (role == 3) {
            cout << endl;
            cout << "**********************************************" << endl;
            cout << "*                                            *" << endl;
            cout << "*               Terima Kasih                 *" << endl;
            cout << "*                                            *" << endl;
            cout << "**********************************************" << endl;
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }

    return 0;
}
