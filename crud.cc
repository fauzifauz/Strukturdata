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

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Terhubung ke database dengan sukses." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

void create_book(const string& title, const string& author, const string& publisher, const string& year, const string& genre, int pages) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO books (title, author, publisher, year, genre, pages) VALUES ('" + title + "', '" + author + "', '" + publisher + "', '" + year + "', '" + genre + "', " + to_string(pages) + ")";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void get_books() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM books")) {
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
            cout << "ID: " << row[0] << ", Judul: " << row[1] << ", Pengarang: " << row[2] << ", Penerbit: " << row[3] << ", Tahun: " << row[4] << ", Genre: " << row[5] << ", Halaman: " << row[6] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_book(int book_id, const string& title, const string& author, const string& publisher, const string& year, const string& genre, int pages) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE books SET title = '" << title << "', author = '" << author << "', publisher = '" << publisher << "', year = '" << year << "', genre = '" << genre << "', pages = " << pages << " WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil diperbarui." << endl;
        }
        mysql_close(conn);
    }
}

void delete_book(int book_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM books WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE gagal: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

void search_book_by_genre(const string& genre) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "SELECT * FROM books WHERE genre LIKE '%" + genre + "%'";
        if (mysql_query(conn, query.c_str())) {
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
            cout << "ID: " << row[0] << ", Judul: " << row[1] << ", Pengarang: " << row[2] << ", Penerbit: " << row[3] << ", Tahun: " << row[4] << ", Genre: " << row[5] << ", Halaman: " << row[6] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void search_book_by_year(const string& year) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "SELECT * FROM books WHERE year LIKE '%" + year + "%'";
        if (mysql_query(conn, query.c_str())) {
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
            cout << "ID: " << row[0] << ", Judul: " << row[1] << ", Pengarang: " << row[2] << ", Penerbit: " << row[3] << ", Tahun: " << row[4] << ", Genre: " << row[5] << ", Halaman: " << row[6] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void admin_menu() {
    int choice;
    while (true) {
        cout << "\nMenu Admin:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Semua Buku\n";
        cout << "3. Perbarui Buku\n";
        cout << "4. Hapus Buku\n";
        cout << "5. Cari Buku Berdasarkan Genre\n";
        cout << "6. Cari Buku Berdasarkan Tahun\n";
        cout << "7. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> choice;

        if (choice == 1) {
            string title, author, publisher, year, genre;
            int pages;
            cout << "Masukkan judul: ";
            cin.ignore();
            getline(cin, title);
            cout << "Masukkan pengarang: ";
            getline(cin, author);
            cout << "Masukkan penerbit: ";
            getline(cin, publisher);
            cout << "Masukkan tahun: ";
            getline(cin, year);
            cout << "Masukkan genre: ";
            getline(cin, genre);
            cout << "Masukkan jumlah halaman: ";
            cin >> pages;
            create_book(title, author, publisher, year, genre, pages);
        } else if (choice == 2) {
            get_books();
        } else if (choice == 3) {
            int book_id;
            string title, author, publisher, year, genre;
            int pages;
            cout << "Masukkan ID buku yang akan diperbarui: ";
            cin >> book_id;
            cin.ignore();
            cout << "Masukkan judul baru: ";
            getline(cin, title);
            cout << "Masukkan pengarang baru: ";
            getline(cin, author);
            cout << "Masukkan penerbit baru: ";
            getline(cin, publisher);
            cout << "Masukkan tahun baru: ";
            getline(cin, year);
            cout << "Masukkan genre baru: ";
            getline(cin, genre);
            cout << "Masukkan jumlah halaman baru: ";
            cin >> pages;
            update_book(book_id, title, author, publisher, year, genre, pages);
        } else if (choice == 4) {
            int book_id;
            cout << "Masukkan ID buku yang akan dihapus: ";
            cin >> book_id;
            delete_book(book_id);
        } else if (choice == 5) {
            string genre;
            cout << "Masukkan genre buku yang dicari: ";
            cin.ignore();
            getline(cin, genre);
            search_book_by_genre(genre);
        } else if (choice == 6) {
            string year;
            cout << "Masukkan tahun publikasi buku yang dicari: ";
            cin.ignore();
            getline(cin, year);
            search_book_by_year(year);
        } else if (choice == 7) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

void user_menu() {
    int choice;
    while (true) {
        cout << "\nMenu User:\n";
        cout << "1. Tampilkan Semua Buku\n";
        cout << "2. Cari Buku Berdasarkan Genre\n";
        cout << "3. Cari Buku Berdasarkan Tahun\n";
        cout << "4. Keluar\n";
                cout << "Masukkan pilihan: ";
        cin >> choice;

        if (choice == 1) {
            get_books();
        } else if (choice == 2) {
            string genre;
            cout << "Masukkan genre buku yang dicari: ";
            cin.ignore();
            getline(cin, genre);
            search_book_by_genre(genre);
        } else if (choice == 3) {
            string year;
            cout << "Masukkan tahun publikasi buku yang dicari: ";
            cin.ignore();
            getline(cin, year);
            search_book_by_year(year);
        } else if (choice == 4) {
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }
}

int main() {
    int role_choice;
    while (true) {
        cout << "Selamat datang di Sistem Manajemen Buku\n";
        cout << "Pilih peran Anda:\n";
        cout << "1. Admin\n";
        cout << "2. User\n";
        cout << "3. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> role_choice;

        if (role_choice == 1) {
            admin_menu();
        } else if (role_choice == 2) {
            user_menu();
        } else if (role_choice == 3) {
            cout << "Terima kasih telah menggunakan sistem ini. Sampai jumpa!" << endl;
            break;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }

    return 0;
}