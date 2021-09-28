/*
Author :
Farhan Gunadi (140810190009)
Bagas Adi Firdaus (140810190037)
M. Alwan Fauzi (140810190077)
Padjadjaran University
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int modInverse(int a, int m) {
  a = a % m;
  for (int x = -m; x < m; x++)
    if ((a * x) % m == 1)
      return x;
  return 0;
}

void getCofactor(vector<vector<int>> &a, vector<vector<int>> &temp, int p,
                 int q, int n) {
  int i = 0, j = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (row != p && col != q) {
        temp[i][j++] = a[row][col];
        if (j == n - 1) {
          j = 0;
          i++;
        }
      }
    }
  }
}

int determinant(vector<vector<int>> &a, int n, int N) {
  int D = 0;
  if (n == 1)
    return a[0][0];
  vector<vector<int>> temp(N, vector<int>(N));
  int sign = 1;
  for (int f = 0; f < n; f++) {
    getCofactor(a, temp, 0, f, n);
    D += sign * a[0][f] * determinant(temp, n - 1, N);
    sign = -sign;
  }
  return D;
}

void adjoint(vector<vector<int>> &a, vector<vector<int>> &adj, int N) {
  if (N == 1) {
    adj[0][0] = 1;
    return;
  }
  int sign = 1;
  vector<vector<int>> temp(N, vector<int>(N));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      getCofactor(a, temp, i, j, N);
      sign = ((i + j) % 2 == 0) ? 1 : -1;
      adj[j][i] = (sign) * (determinant(temp, N - 1, N));
    }
  }
}

bool inverse(vector<vector<int>> &a, vector<vector<int>> &inv, int N) {
  int det = determinant(a, N, N);
  int cek_det;
  if (det > 0) {
    cek_det = det % 26;
  } else {
    cek_det = 26 - (abs(det) % 26);
  }
  if (__gcd(cek_det, 26) != 1) {
    return false;
  }
  int invDet = modInverse(det, 26);
  vector<vector<int>> adj(N, vector<int>(N));
  adjoint(a, adj, N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      inv[i][j] = (adj[i][j] * invDet) % 26;
  return true;
}

void findKey(string plaintext, string ciphertext, vector<vector<int>> &K,
             int N) {

  int i, j;

  if (plaintext.size() != ciphertext.size()) {
    cout << "Matriks key tidak dapat ditemukan, jumlah huruf plaintext dan "
            "ciphertext tidak sama\n";
  }

  if (plaintext.size() % N != 0 || N < 2) {
    cout << "Matriks key tidak dapat ditemukan, ukuran matriks kunci tidak "
            "sesuai\n";
  }

  vector<vector<int>> P(N, vector<int>(N));
  vector<vector<int>> C(N, vector<int>(N));
  vector<vector<int>> inversP(N, vector<int>(N));

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      P[i][j] = plaintext[i * N + j] - 97;
      C[i][j] = ciphertext[i * N + j] - 97;
    }
  }

  if (!inverse(P, inversP, N)) {
    cout << "Matriks key tidak dapat ditemukan\n";
  }

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      K[i][j] = 0;

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      for (int k = 0; k < N; j++)
        K[i][j] += C[i][k] * inversP[k][j];

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      cout << K[i][j] << " ";
    }
    cout << "\n";
  }
}

int main() {
mulai:
  string s, d;
  char loop = 'n';
  string ans = "";
  int x, y, i, j, k, n, opsi, temp, f, sum;
  vector<vector<int>> a(10, vector<int>(10));
  vector<vector<int>> adj(10, vector<int>(10));
  vector<vector<int>> inv(10, vector<int>(10));
  vector<vector<int>> key(10, vector<int>(10));

  cout << "\n==== Program Hill Cipher ====\n";

  cout << "1. Enkripsi\n";
  cout << "2. Dekripsi\n";
  cout << "3. Cari matriks kunci\n";
  cout << "Pilih opsi : ";
  cin >> opsi;
  switch (opsi) {
  case 1:
    cout << "Input : ";
    cin >> s;

    cout << "Ukuran matriks kunci : ";
    cin >> n;
    cout << "Matriks kunci\n";

    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        cin >> a[i][j];
      }
    }

    temp = (n - s.size() % n) % n;
    for (i = 0; i < temp; i++) {
      s += 'x';
    }
    k = 0;
    while (k < s.size()) {
      for (i = 0; i < n; i++) {
        sum = 0;
        temp = k;
        for (j = 0; j < n; j++) {
          sum += (a[i][j] % 26 * (s[temp++] - 'a') % 26) % 26;
          sum = sum % 26;
        }
        ans += (sum + 'a');
      }
      k += n;
    }
    cout << "Hasil enkripsi : " << ans << endl;
    break;
  case 2:
    cout << "Input : ";
    cin >> s;

    cout << "Ukuran matriks kunci : ";
    cin >> n;
    cout << "Matriks kunci\n";

    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        cin >> a[i][j];
      }
    }
    if (!inverse(a, inv, n)) {
      cout << "Matriks tidak memiliki invers modulo 26 sehingga cipher teks "
              "tidak dapat didekripsi\n";
      break;
    };
    k = 0;
    while (k < s.size()) {
      for (i = 0; i < n; i++) {
        sum = 0;
        temp = k;
        for (j = 0; j < n; j++) {
          sum += ((inv[i][j] + 26) % 26 * (s[temp++] - 'a') % 26) % 26;
          sum = sum % 26;
        }
        ans += (sum + 'a');
      }
      k += n;
    }
    // deskripsi+='\0';
    f = ans.size() - 1;
    while (ans[f] == 'x') {
      f--;
    }
    cout << "Hasil dekripsi : " << ans << endl;
    break;

  case 3:
    cout << "Plainteks : ";
    cin >> s;

    cout << "Cipherteks : ";
    cin >> d;

    cout << "Ukuran matriks kunci : ";
    cin >> n;

    cout << "Matriks kuncinya : \n";
    findKey(s, d, key, n);

    break;

  default:
    break;
  }
  string pilih;
  cout << "Apakah ingin diulang ? (Y/N) ";
  cin >> pilih;
  if (pilih == "Y" || pilih == "y") {
    goto mulai;
  } else {
    return 0;
  }
}