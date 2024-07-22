#include <iostream>
#include <mysql/mysql.h>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

map<int, vector<int>> user_borrowed_books;

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
            cout << "\033[1;32mBerhasil terhubung ke database.\033[0m" << endl;
        } else {
            cerr << "\033[1;31mKoneksi gagal: " << mysql_error(conn) << "\033[0m" << endl;
        }
    } else {
        cerr << "\033[1;31mmysql_init gagal\033[0m" << endl;
    }
    return conn;
}

// Fungsi untuk menambah buku
void create_book(const string& title, const string& author, const string& publisher, const string& year, const string& category) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO buku (title, author, publisher, year, category) VALUES ('" + title + "', '" + author + "', '" + publisher + "', '" + year + "', '" + category + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "\033[1;31mINSERT gagal: " << mysql_error(conn) << "\033[0m" << endl;
        } else {
            cout << "\033[1;32mBuku berhasil ditambahkan.\033[0m" << endl;
        }
        mysql_close(conn);
    }
}

// Fungsi untuk meminjam buku
void borrow_book(int user_id, int book_id) {
    user_borrowed_books[user_id].push_back(book_id);
    cout << "\033[1;32mBuku berhasil dipinjam.\033[0m" << endl;
}

// Fungsi untuk mendapatkan judul buku berdasarkan ID
string get_book_title(int book_id) {
    MYSQL* conn = connect_db();
    string title;
    if (conn) {
        stringstream query;
        query << "SELECT title FROM buku WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "\033[1;31mSELECT gagal: " << mysql_error(conn) << "\033[0m" << endl;
            mysql_close(conn);
            return "";
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "\033[1;31mmysql_store_result gagal: " << mysql_error(conn) << "\033[0m" << endl;
            mysql_close(conn);
            return "";
        }

        MYSQL_ROW row;
        if ((row = mysql_fetch_row(res))) {
            title = row[0];
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
    return title;
}

// Fungsi untuk menampilkan buku yang dipinjam oleh user
void get_borrowed_books(int user_id) {
    auto it = user_borrowed_books.find(user_id);
    if (it != user_borrowed_books.end()) {
        vector<int> borrowed_books = it->second;

        cout << "\n\033[1;34mBuku yang Dipinjam:\033[0m" << endl;
        cout << "---------------------------------------------" << endl;

        for (int book_id : borrowed_books) {
            string title = get_book_title(book_id);
            cout << "ID Buku: " << book_id << " | Judul: " << title << endl;
        }

        cout << "---------------------------------------------" << endl;
    } else {
        cout << "\033[1;33mAnda belum meminjam buku apapun.\033[0m" << endl;
    }
}

// Fungsi untuk menampilkan semua buku
void get_books() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM buku")) {
            cerr << "\033[1;31mSELECT gagal: " << mysql_error(conn) << "\033[0m" << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "\033[1;31mmysql_store_result gagal: " << mysql_error(conn) << "\033[0m" << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        cout << "\n\033[1;34mDaftar Buku:\033[0m" << endl;
        cout << "---------------------------------------------" << endl;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << " | Judul: " << row[1] << " | Penulis: " << row[2] << " | Penerbit: " << row[3] << " | Tahun: " << row[4] << " | Kategori: " << row[5] << endl;
        }
        cout << "---------------------------------------------" << endl;

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
            cerr << "\033[1;31mUPDATE gagal: " << mysql_error(conn) << "\033[0m" << endl;
        } else {
            cout << "\033[1;32mBuku berhasil diperbarui.\033[0m" << endl;
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
            cerr << "\033[1;31mDELETE gagal: " << mysql_error(conn) << "\033[0m" << endl;
        } else {
            cout << "\033[1;32mBuku berhasil dihapus.\033[0m" << endl;
        }
        mysql_close(conn);
    }
}

// Fungsi menampilkan header perpustakaan
void display_header() {
    cout << "\033[1;36m**********************************************\n";
    cout << "*                                            *\n";
    cout << "*          Perpustakaan Esa Unggul           *\n";
    cout << "*                                            *\n";
    cout << "**********************************************\033[0m\n" << endl;
}

// Fungsi menu admin
void admin_menu() {
    int choice;
    while (true) {
        cout << "\n\033[1;33mMenu Admin:\033[0m\n";
        cout << "---------------------------------------------\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Semua Buku\n";
        cout << "3. Perbarui Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Logout\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        if (choice == 1) {
            string title, author, publisher, year, category;
            cout << "\033[1;36mMasukkan judul: \033[0m";
            cin.ignore();
            getline(cin, title);
            cout << "\033[1;36mMasukkan penulis: \033[0m";
            getline(cin, author);
            cout << "\033[1;36mMasukkan penerbit: \033[0m";
            getline(cin, publisher);
            cout << "\033[1;36mMasukkan tahun: \033[0m";
            getline(cin, year);
            cout << "\033[1;36mMasukkan kategori: \033[0m";
            getline(cin, category);
            create_book(title, author, publisher, year, category);
        } else if (choice == 2) {
            get_books();
        } else if (choice == 3) {
            int book_id;
            string title, author, publisher, year, category;
            cout << "\033[1;36mMasukkan ID buku untuk diperbarui: \033[0m";
            cin >> book_id;
            cin.ignore();
            cout << "\033[1;36mMasukkan judul baru: \033[0m";
            getline(cin, title);
            cout << "\033[1;36mMasukkan penulis baru: \033[0m";
            getline(cin, author);
            cout << "\033[1;36mMasukkan penerbit baru: \033[0m";
            getline(cin, publisher);
            cout << "\033[1;36mMasukkan tahun baru: \033[0m";
            getline(cin, year);
            cout << "\033[1;36mMasukkan kategori baru: \033[0m";
            getline(cin, category);
            update_book(book_id, title, author, publisher, year, category);
        } else if (choice == 4) {
            int book_id;
            cout << "\033[1;36mMasukkan ID buku untuk dihapus: \033[0m";
            cin >> book_id;
            delete_book(book_id);
        } else if (choice == 5) {
            break;
        } else {
            cout << "\033[1;31mPilihan tidak valid. Silakan coba lagi.\033[0m" << endl;
        }
    }
}

// Fungsi menu user
void user_menu(int user_id) {
    int choice;
    while (true) {
        cout << "\n\033[1;33mMenu User:\033[0m\n";
        cout << "---------------------------------------------\n";
        cout << "1. Tampilkan Semua Buku\n";
        cout << "2. Pinjam Buku\n";
        cout << "3. Tampilkan Buku yang Dipinjam\n";
        cout << "4. Logout\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        if (choice == 1) {
            get_books();
        } else if (choice == 2) {
            int book_id;
            cout << "\033[1;36mMasukkan ID buku yang ingin dipinjam: \033[0m";
            cin >> book_id;
            borrow_book(user_id, book_id);
        } else if (choice == 3) {
            get_borrowed_books(user_id);
        } else if (choice == 4) {
            break;
        } else {
            cout << "\033[1;31mPilihan tidak valid. Silakan coba lagi.\033[0m" << endl;
        }
    }
}

// Fungsi login
int login() {
    string username, password;
    cout << "\033[1;35mMasukkan username: \033[0m";
    cin >> username;
    cout << "\033[1;35mMasukkan password: \033[0m";
    cin >> password;

    if (username == "admin" && password == "adminpass") {
        return 0; // Admin ID atau indikasi admin
    } else if (username == "user" && password == "userpass") {
        return 1; // User ID atau indikasi user
    } else {
        cout << "\033[1;31mUsername atau password salah.\033[0m" << endl;
        return -1; // Menunjukkan login gagal
    }
}

// Fungsi utama
int main() {
    display_header();

    cout << "\033[1;33mInformasi Login:\033[0m\n";
    cout << "Username: admin, Password: adminpass\n";
    cout << "Username: user, Password: userpass\n";

    int user_id = login();
    if (user_id == -1) {
        cout << "\033[1;31mGagal login. Program keluar.\033[0m" << endl;
        return 1;
    }

    while (true) {
        int role;
        cout << "\n\033[1;33mPilih peran:\033[0m\n";
        cout << "---------------------------------------------\n";
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "3. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> role;

        if (role == 1) {
            admin_menu();
        } else if (role == 2) {
            user_menu(user_id);
        } else if (role == 3) {
            cout << "\n\033[1;36m**********************************************\n";
            cout << "*                                            *\n";
            cout << "*               Terima Kasih                 *\n";
            cout << "*                                            *\n";
            cout << "**********************************************\033[0m" << endl;
            break;
        } else {
            cout << "\033[1;31mPilihan tidak valid. Silakan coba lagi.\033[0m" << endl;
        }
    }

    return 0;
}
