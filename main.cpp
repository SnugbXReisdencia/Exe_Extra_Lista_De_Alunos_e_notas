#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

class Aluno {
private:
    string nome;
    string email;
    float nota1;
    float nota2;

public:
    Aluno() : nota1(0.0f), nota2(0.0f) {}

    Aluno(const string& nome, const string& email, float nota1, float nota2) :
        nome(nome), email(email), nota1(nota1), nota2(nota2) {}

    string getNome() const {
        return nome;
    }

    string getEmail() const {
        return email;
    }

    float getNota1() const {
        return nota1;
    }

    float getNota2() const {
        return nota2;
    }

    void setNome(const string& novoNome) {
        nome = novoNome;
    }

    void setEmail(const string& novoEmail) {
        email = novoEmail;
    }

    void setNota1(float novaNota1) {
        nota1 = novaNota1;
    }

    void setNota2(float novaNota2) {
        nota2 = novaNota2;
    }

    friend ostream& operator<<(ostream& os, const Aluno& aluno) {
        os << "Nome: " << aluno.nome << ", Email: " << aluno.email
           << ", Nota 1: " << aluno.nota1 << ", Nota 2: " << aluno.nota2;
        return os;
    }
};

class BancoDeDados {
private:
    vector<Aluno> alunos;
    string arquivo;

public:
    BancoDeDados(const string& nomeArquivo) : arquivo(nomeArquivo) {}

    void inserirAluno(const Aluno& aluno) {
        alunos.push_back(aluno);
    }

    vector<Aluno> getAlunos() {
        return alunos;
    }

    bool alterarAluno(const string& nome, const Aluno& novoAluno) {
        for (auto& aluno : alunos) {
            if (aluno.getNome() == nome) {
                aluno = novoAluno;
                return true;
            }
        }
        return false;
    }

    bool excluirAluno(const string& nome) {
        auto it = remove_if(alunos.begin(), alunos.end(),
            [nome](const Aluno& aluno) { return aluno.getNome() == nome; });

        if (it != alunos.end()) {
            alunos.erase(it, alunos.end());
            return true;
        }

        return false;
    }

    void salvar() {
        ofstream arquivo_saida(arquivo);
        if (arquivo_saida.is_open()) {
            for (const auto& aluno : alunos) {
                arquivo_saida << aluno.getNome() << ","
                              << aluno.getEmail() << ","
                              << aluno.getNota1() << ","
                              << aluno.getNota2() << endl;
            }
            arquivo_saida.close();
        } else {
            cout << "Erro ao abrir o arquivo para salvar." << endl;
        }
    }

    void carregar() {
        ifstream arquivo_entrada(arquivo);
        if (arquivo_entrada.is_open()) {
            alunos.clear();
            string linha;
            while (getline(arquivo_entrada, linha)) {
                istringstream iss(linha);
                string nome, email;
                float nota1, nota2;
                char delim;
                if (getline(iss, nome, ',') && getline(iss, email, ',') &&
                    iss >> nota1 >> delim >> nota2) {
                    inserirAluno(Aluno(nome, email, nota1, nota2));
                }
            }
            arquivo_entrada.close();
        } else {
            cout << "Arquivo não encontrado ou erro ao abrir o arquivo." << endl;
        }
    }

    void listarAlunos() {
        for (const auto& aluno : alunos) {
            cout << aluno << endl;
        }
    }
};

int menu();

void pause();
void limpaTela();

int main() {
    BancoDeDados banco("alunos.txt");
    banco.carregar();

    int opcao;
    do {
        limpaTela();
        opcao = menu();

        switch (opcao) {
            case 1: {
                string nome, email;
                float nota1, nota2;
                cout << "********* Cadastro de Aluno *********" << endl;
                cout << "Nome: ";
                cin.ignore();
                getline(cin, nome);
                cout << "Email: ";
                getline(cin, email);
                cout << "Nota 1: ";
                cin >> nota1;
                cout << "Nota 2: ";
                cin >> nota2;

                Aluno novoAluno(nome, email, nota1, nota2);
                banco.inserirAluno(novoAluno);
                cout << "Aluno inserido com sucesso!" << endl;
                pause();
                break;
            }
            case 2: {
                string nome, email;
                float nota1, nota2;

                cout << "Digite o nome do aluno que deseja alterar: ";
                cin.ignore();
                getline(cin, nome);

                Aluno alunoExistente;
                bool encontrado = false;
                for (auto& aluno : banco.getAlunos()) {
                    if (aluno.getNome() == nome) {
                        encontrado = true;
                        alunoExistente = aluno;
                        break;
                    }
                }

                if (encontrado) {
                    cout << "Nome: ";
                    getline(cin, nome);
                    cout << "Email: ";
                    getline(cin, email);
                    cout << "Nota 1: ";
                    cin >> nota1;
                    cout << "Nota 2: ";
                    cin >> nota2;

                    Aluno novoAluno(nome, email, nota1, nota2);
                    banco.alterarAluno(alunoExistente.getNome(), novoAluno);
                    cout << "Aluno alterado com sucesso!" << endl;
                } else {
                    cout << "Aluno não encontrado." << endl;
                }
                pause();
                break;
            }
            case 3: {
                string nome;

                cout << "Digite o nome do aluno que deseja excluir: ";
                cin.ignore();
                getline(cin, nome);

                if (banco.excluirAluno(nome)) {
                    cout << "Aluno excluído com sucesso!" << endl;
                } else {
                    cout << "Aluno não encontrado." << endl;
                }
                pause();
                break;
            }
            case 4:
                banco.listarAlunos();
                pause();
                break;
            case 5:
                banco.salvar();
                cout << "Dados salvos com sucesso!" << endl;
                pause();
                break;
            case 0:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opção inválida." << endl;
                pause();
                break;
        }
    } while (opcao != 0);

    return 0;
}

int menu() {
    int opcao;
    cout << "######### Menu #########" << endl;
    cout << "1. Inserir Aluno" << endl;
    cout << "2. Alterar Aluno" << endl;
    cout << "3. Excluir Aluno" << endl;
    cout << "4. Listar Alunos" << endl;
    cout << "5. Salvar Dados" << endl;
    cout << "0. Sair" << endl;
    cout << "Escolha uma opção: ";
    cin >> opcao;
    return opcao;
}

void pause()
{
    cin.ignore();
    cout << "\nPressione a tecla Enter para continuar...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void limpaTela()
{
    cin.clear();

    // Verifica se a variável de ambiente WINDIR está definida (ambiente Windows)
    const char *windir = getenv("WINDIR");

    if (windir != nullptr && strlen(windir) > 0)
    {
        // Se estiver definida, estamos no Windows, então use "cls"
        system("cls");
    }
    else
    {
        // Caso contrário, estamos em um ambiente Unix/Linux, use "clear"
        system("clear");
    }
}

