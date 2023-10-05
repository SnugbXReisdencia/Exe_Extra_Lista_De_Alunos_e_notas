# include <iostream>
# include <fstream>
# include <limits>
# include <cstring>
# include <vector>
# include <sstream>

using namespace std;

class Dados{
    public:
        string _dados;
        bool empty;
        bool savo;
        Dados();
};

Dados::Dados(){
    empty = true;
    savo = false;
}


class BancoDeDados{
    public:
        static bool saveDados(Dados dados, string);
        static Dados recuperarDados(string);
};

bool BancoDeDados::saveDados(Dados dados, string arquivo_nome){
    ofstream arquivo(arquivo_nome);
    if(arquivo.is_open()){
        for(size_t i = 0; i < dados._dados.size(); i++){
            arquivo << dados._dados[i] << endl;
        }
        dados.savo = true;
    }
    arquivo.close();
    return dados.savo;
}

Dados BancoDeDados::recuperarDados(string arquivo_nome){
    Dados dados;

    ifstream arquivo(arquivo_nome);
    if(arquivo.is_open()){
        string linha;
        while(getline(arquivo, linha)){
            dados._dados.push_back(linha);
            dados.empty = false;
        }
    }
    arquivo.close();
    return dados;
}


class Avaliacao{
    private:
        string nome;
        float nota;
    public:
        Avaliacao();
        Avaliacao(string nome, float nota);

        string getNome();

        float getNota();

        void setNome(string);

        void setNota(float);
};

Avaliacao::Avaliacao(){
    nome = "";
    nota = 0;
}

Avaliacao::Avaliacao(string nome, float nota){
    this->nome = nome;
    this->nota = nota;
}

string Avaliacao::getNome(){
    return nome;
}

float Avaliacao::getNota(){
    return nota;
}

void Avaliacao::setNome(string nome){
    this->nome = nome;
}

void Avaliacao::setNota(float nota){
    this->nota = nota;
}

class Aluno{
    private:
        string nome;
        string email;
        vector<Avaliacao> avaliacoes;
    public:
        Aluno();
        Aluno(string nome, string email, vector<Avaliacao>);

        string getNome();

        string getEmail();

        void setNome(string);

        void setEmail(string);

        void addAvaliacao(Avaliacao);

        bool persisteAluno(string arquivo);

        bool recuperarAluno(string aluno, string arquivo);
};

Aluno::Aluno(){
    nome = "";
    email = "";
}

Aluno::Aluno(string nome, string email, vector<Avaliacao> avaliacoes){
    this->nome = nome;
    this->email = email;
    this->avaliacoes = avaliacoes;
}

string Aluno::getNome(){
    return nome;
}

string Aluno::getEmail(){
    return email;
}

void Aluno::setNome(string nome){
    this->nome = nome;
}

void Aluno::setEmail(string email){
    this->email = email;
}

void Aluno::addAvaliacao(Avaliacao avaliacao){
    avaliacoes.push_back(avaliacao);
}

bool Aluno::persisteAluno(string arquivo){
    Dados dados;
    string aluno;
    dados = BancoDeDados::recuperarDados(arquivo);

    aluno = this->getNome()+","+this->getEmail()+",{";
    for(size_t i = 0; i < avaliacoes.size(); i++){
        if(i == avaliacoes.size() - 1){
            aluno += avaliacoes[i].getNome()+","+to_string(avaliacoes[i].getNota())+"}";
        }else{
            aluno += avaliacoes[i].getNome()+","+to_string(avaliacoes[i].getNota())+",";
        }
    }
    cout << "Aluno Persistido: " << aluno << endl;
    dados._dados.push_back(aluno);

    if(BancoDeDados::saveDados(dados, arquivo)){
        return true;
    }else{
        return false;
    }
}

bool Aluno::recuperarAluno(string aluno, string arquivo){
    Dados dados;
    vector<string> campos;
    vector<string> vet;
    dados = BancoDeDados::recuperarDados(arquivo);
    if(dados.empty){
        return false;
    }

    istringstream ss(dados._dados);

    return true;
}

void pause();
void limpaTela();

int main(){
    limpaTela();

    Aluno aluno;
    string nome, email, nome_Avaliacao;
    float nota;
    char sn;
    Avaliacao avaliacao = Avaliacao();

    do{
        cout << " Cadastro de Alunos\n\n";
        cout << " Informe o nome do aluno: ";
        cin >> nome;
        aluno.setNome(nome);
        cout << " Informe o email do aluno: ";
        cin >> email;
        aluno.setEmail(email);

        cout << "\n";
        do{
            cout << " Cadastro de Avaliacoes do Aluno\n\n";

            cout << " Informe o nome da Avaliacao: ";
            cin >> nome_Avaliacao;
            avaliacao.setNome(nome_Avaliacao);
            cout << " Informe a nota da Avaliacao: ";
            cin >> nota;
            avaliacao.setNota(nota);

            aluno.addAvaliacao(avaliacao);

            cout << "\nDeseja cadastrar uma nova avaliacao? (S/N)";
            cin >> sn;
        }while(sn != 'N' && sn != 'n');
        
        if(aluno.persisteAluno("alunos.txt")){
            cout << "\nAluno Cadastrado com Sucesso!\n\n";
        }else{
            cout << "\nAluno Nao Cadastrado!\n\n";
        }

        cout << "\nDeseja cadastrar Outro Aluno? (S/N)";
        cin >> sn;

    }while(sn != 'N' && sn != 'n');

    return 0;
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