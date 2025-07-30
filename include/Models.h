#ifndef MODELS_H
#define MODELS_H

#include <cstdint>
#include <string>
#include <vector>

namespace Models {

struct Aviso {
    std::string aviso;
};

struct Periodo {
    std::string DataRepasse;
    std::string RepasseMaximo;
    std::string ValorFinanciado;
    std::string* PercentualRepassado = nullptr;
    std::string* SaqueRestante = nullptr;
};

struct Objecao {
    std::string objecao;
    std::string resposta;
    std::string* respostaCopiavel = nullptr;
};

struct MensagemProposta {
    std::string nome;
    std::string primeiroNome;
    std::string cpf;
    std::string apelidoBancoEmprestimoPort;
    std::string valorLiberado;
    std::string taxaContrato;
    std::string reservaDeSaldo;
    std::vector<Periodo> mensagemPeriodos;
    uint32_t numeroPeriodos;
    std::string mensagemDocumentosNecessarios;
};

struct Simulacao {
    std::string parcelaNova;
    std::string valorOperacao;
    uint32_t prazo;
    std::string bancoEmprestimo;
    std::string taxa;
    std::string nomeTabela;
    bool atualizar;
    uint32_t idOrgao;
    double coeficiente;
    std::string codigoInternoTabela;
    double percentualComissao;
    double percentualProducao;
    bool ativa;
    std::string produto;
    std::string valorLiberado;
    std::string* saldoTotal = nullptr;
    std::string salarioBruto;
    std::string reservaDeSaldo;
    std::vector<Periodo> periodos;
    std::string apelidoBancoEmprestimo;
    std::string jurosTotais;
    std::string jurosPorAno;
    std::string jurosPorMes;
    std::string jurosEmprestimoPessoalPorAno;
    bool simulacaoConfiavel;
    bool simulacaoFactivel;
    std::string dataExpiracao;
    MensagemProposta mensagemProposta;
    std::vector<std::string> avisos;
    bool isInstability;
    std::vector<std::string> informacoes;
    std::vector<Objecao> objecoes;
};

struct ApiResponse {
    std::string htmlString;
    std::string appVersion;
};

struct ResponseContent {
    bool* error = nullptr;
    std::vector<Aviso>* avisos = nullptr;
    bool* isInstability = nullptr;
    std::string* nome = nullptr;
    std::string* cpf = nullptr;
    std::string* dataNascimento = nullptr;
    uint32_t* idadeAnos = nullptr;
    uint32_t* idadeMeses = nullptr;
    std::string* telefone = nullptr;
    std::string* abordarComo = nullptr;
    Simulacao* simulacoes = nullptr;
    std::string* valorLiberado = nullptr;
    double* percentualComissao = nullptr;
    std::string* displayCardCreation = nullptr;
};

struct AuthenticationResult {
    std::string AccessToken;
};

struct AccessTokenResp {
    AuthenticationResult AuthenticationResult;
};

struct Logins {
    int login1;
    int login2;
    int login3;
    int login4;
};

struct Pessoa {
    std::string nome;
    std::string numero;
};

}

#endif