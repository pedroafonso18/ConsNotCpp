#include "../include/Requests.h"
#include <iostream>
#include <curl/curl.h>
#include <fmt/format.h>
#include "../lib/json.h"
#include "../include/Utils.h"

size_t Requests::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    auto* resp = static_cast<std::string*>(userp);
    resp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

Models::AccessTokenResp* Requests::GetAccessToken(std::string access, std::string password) {
    CURL* curl = curl_easy_init();
    std::string respBody;
    if (!curl) {
        std::clog << "ERROR: Failed to initialize CURL\n";
        return nullptr;
    }
    nlohmann::json req_body = {
        {"ClientId", "63ccaojkma1th1pucikhn1n19k"},
        {"AuthFlow", "USER_PASSWORD_AUTH"},
        {"AuthParameters", {{"USERNAME", access}, {"PASSWORD", password}}}
    };

    std::string str_bod = req_body.dump();

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/x-amz-json-1.1");
    headers = curl_slist_append(headers, "X-Amz-Target: AWSCognitoIdentityProviderService.InitiateAuth");

    curl_easy_setopt(curl, CURLOPT_URL, "https://cognito-idp.us-east-2.amazonaws.com");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_bod.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBody);

    Models::AccessTokenResp* result = nullptr;
    if (curl_easy_perform(curl) == CURLE_OK) {
        nlohmann::json resp_json = nlohmann::json::parse(respBody, nullptr, false);
        if (!resp_json.is_discarded() && resp_json.contains("AuthenticationResult")) {
            result = new Models::AccessTokenResp();
            result->AuthenticationResult.AccessToken = resp_json["AuthenticationResult"]["AccessToken"].get<std::string>();
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return result;
}

Models::ResponseContent *Requests::GetApiReturn(std::string token, std::string cpf, std::string apikey) {
    auto cleaned_cpf = Utils::cleanCpf(cpf);
    CURL* curl = curl_easy_init();
    std::string respBody;
    if (!curl) {
        std::clog << "ERROR: Failed to initialize CURL\n";
        return nullptr;
    }
    nlohmann::json json_bod = {
        {"numCpf", cleaned_cpf},
        {"bancoDestinoNovo", "9993-MB"},
        {"autorizacao", true},
        {"saldoTotal", ""},
        {"salarioBruto", ""},
        {"mesesTrabalhados", ""},
        {"dtNascimentoAux", "false"},
        {"numTelefone", ""},
        {"numeroDeParcelas", "10"}
    };

    std::string str_bod = json_bod.dump();

    struct curl_slist* headers = nullptr;
    auto auth = fmt::format("acesstoken : {}", token);
    headers = curl_slist_append(headers, auth.c_str());
    headers = curl_slist_append(headers, "Content-Type : application/json");
    headers = curl_slist_append(headers, ("apikey : " + apikey).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://consig.private.app.br/api/secao/simulador-safra-fgts");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str_bod.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBody);

    Models::ResponseContent* result = nullptr;
    if (curl_easy_perform(curl) == CURLE_OK) {
        nlohmann::json resp_json = nlohmann::json::parse(respBody, nullptr, false);
        if (!resp_json.is_discarded()) {
            result = new Models::ResponseContent();

            if (resp_json.contains("error")) {
                result->error = new bool(resp_json["error"].get<bool>());
            }

            if (resp_json.contains("avisos")) {
                result->avisos = new std::vector<Models::Aviso>();
                for (const auto& aviso_json : resp_json["avisos"]) {
                    Models::Aviso aviso;
                    aviso.aviso = aviso_json["aviso"].get<std::string>();
                    result->avisos->push_back(aviso);
                }
            }

            if (resp_json.contains("isInstability")) {
                result->isInstability = new bool(resp_json["isInstability"].get<bool>());
            }

            if (resp_json.contains("nome")) {
                result->nome = new std::string(resp_json["nome"].get<std::string>());
            }

            if (resp_json.contains("cpf")) {
                result->cpf = new std::string(resp_json["cpf"].get<std::string>());
            }

            if (resp_json.contains("dataNascimento")) {
                result->dataNascimento = new std::string(resp_json["dataNascimento"].get<std::string>());
            }

            if (resp_json.contains("idadeAnos")) {
                result->idadeAnos = new uint32_t(resp_json["idadeAnos"].get<uint32_t>());
            }

            if (resp_json.contains("idadeMeses")) {
                result->idadeMeses = new uint32_t(resp_json["idadeMeses"].get<uint32_t>());
            }

            if (resp_json.contains("telefone")) {
                result->telefone = new std::string(resp_json["telefone"].get<std::string>());
            }

            if (resp_json.contains("abordarComo")) {
                result->abordarComo = new std::string(resp_json["abordarComo"].get<std::string>());
            }

            if (resp_json.contains("valorLiberado")) {
                result->valorLiberado = new std::string(resp_json["valorLiberado"].get<std::string>());
            }

            if (resp_json.contains("percentualComissao")) {
                result->percentualComissao = new double(resp_json["percentualComissao"].get<double>());
            }

            if (resp_json.contains("displayCardCreation")) {
                result->displayCardCreation = new std::string(resp_json["displayCardCreation"].get<std::string>());
            }

            if (resp_json.contains("simulacoes")) {
                result->simulacoes = new Models::Simulacao();
                const auto& sim_json = resp_json["simulacoes"];

                if (sim_json.contains("parcelaNova")) {
                    result->simulacoes->parcelaNova = sim_json["parcelaNova"].get<std::string>();
                }
                if (sim_json.contains("valorOperacao")) {
                    result->simulacoes->valorOperacao = sim_json["valorOperacao"].get<std::string>();
                }
                if (sim_json.contains("prazo")) {
                    result->simulacoes->prazo = sim_json["prazo"].get<uint32_t>();
                }
                if (sim_json.contains("bancoEmprestimo")) {
                    result->simulacoes->bancoEmprestimo = sim_json["bancoEmprestimo"].get<std::string>();
                }
                if (sim_json.contains("taxa")) {
                    result->simulacoes->taxa = sim_json["taxa"].get<std::string>();
                }
                if (sim_json.contains("nomeTabela")) {
                    result->simulacoes->nomeTabela = sim_json["nomeTabela"].get<std::string>();
                }
                if (sim_json.contains("atualizar")) {
                    result->simulacoes->atualizar = sim_json["atualizar"].get<bool>();
                }
                if (sim_json.contains("idOrgao")) {
                    result->simulacoes->idOrgao = sim_json["idOrgao"].get<uint32_t>();
                }
                if (sim_json.contains("coeficiente")) {
                    result->simulacoes->coeficiente = sim_json["coeficiente"].get<double>();
                }
                if (sim_json.contains("codigoInternoTabela")) {
                    result->simulacoes->codigoInternoTabela = sim_json["codigoInternoTabela"].get<std::string>();
                }
                if (sim_json.contains("percentualComissao")) {
                    result->simulacoes->percentualComissao = sim_json["percentualComissao"].get<double>();
                }
                if (sim_json.contains("percentualProducao")) {
                    result->simulacoes->percentualProducao = sim_json["percentualProducao"].get<double>();
                }
                if (sim_json.contains("ativa")) {
                    result->simulacoes->ativa = sim_json["ativa"].get<bool>();
                }
                if (sim_json.contains("produto")) {
                    result->simulacoes->produto = sim_json["produto"].get<std::string>();
                }
                if (sim_json.contains("valorLiberado")) {
                    result->simulacoes->valorLiberado = sim_json["valorLiberado"].get<std::string>();
                }
                if (sim_json.contains("saldoTotal")) {
                    result->simulacoes->saldoTotal = new std::string(sim_json["saldoTotal"].get<std::string>());
                }
                if (sim_json.contains("salarioBruto")) {
                    result->simulacoes->salarioBruto = sim_json["salarioBruto"].get<std::string>();
                }
                if (sim_json.contains("reservaDeSaldo")) {
                    result->simulacoes->reservaDeSaldo = sim_json["reservaDeSaldo"].get<std::string>();
                }
                if (sim_json.contains("apelidoBancoEmprestimo")) {
                    result->simulacoes->apelidoBancoEmprestimo = sim_json["apelidoBancoEmprestimo"].get<std::string>();
                }
                if (sim_json.contains("jurosTotais")) {
                    result->simulacoes->jurosTotais = sim_json["jurosTotais"].get<std::string>();
                }
                if (sim_json.contains("jurosPorAno")) {
                    result->simulacoes->jurosPorAno = sim_json["jurosPorAno"].get<std::string>();
                }
                if (sim_json.contains("jurosPorMes")) {
                    result->simulacoes->jurosPorMes = sim_json["jurosPorMes"].get<std::string>();
                }
                if (sim_json.contains("jurosEmprestimoPessoalPorAno")) {
                    result->simulacoes->jurosEmprestimoPessoalPorAno = sim_json["jurosEmprestimoPessoalPorAno"].get<std::string>();
                }
                if (sim_json.contains("simulacaoConfiavel")) {
                    result->simulacoes->simulacaoConfiavel = sim_json["simulacaoConfiavel"].get<bool>();
                }
                if (sim_json.contains("simulacaoFactivel")) {
                    result->simulacoes->simulacaoFactivel = sim_json["simulacaoFactivel"].get<bool>();
                }
                if (sim_json.contains("dataExpiracao")) {
                    result->simulacoes->dataExpiracao = sim_json["dataExpiracao"].get<std::string>();
                }

                if (sim_json.contains("periodos")) {
                    const auto& periodos_json = sim_json["periodos"];
                    for (const auto& periodo_json : periodos_json) {
                        Models::Periodo periodo;
                        if (periodo_json.contains("DataRepasse")) {
                            periodo.DataRepasse = periodo_json["DataRepasse"].get<std::string>();
                        }
                        if (periodo_json.contains("RepasseMaximo")) {
                            periodo.RepasseMaximo = periodo_json["RepasseMaximo"].get<std::string>();
                        }
                        if (periodo_json.contains("ValorFinanciado")) {
                            periodo.ValorFinanciado = periodo_json["ValorFinanciado"].get<std::string>();
                        }
                        if (periodo_json.contains("PercentualRepassado")) {
                            periodo.PercentualRepassado = new std::string(periodo_json["PercentualRepassado"].get<std::string>());
                        }
                        if (periodo_json.contains("SaqueRestante")) {
                            periodo.SaqueRestante = new std::string(periodo_json["SaqueRestante"].get<std::string>());
                        }
                        result->simulacoes->periodos.push_back(periodo);
                    }
                }

                if (sim_json.contains("mensagemProposta")) {
                    const auto& msg_json = sim_json["mensagemProposta"];
                    if (msg_json.contains("nome")) {
                        result->simulacoes->mensagemProposta.nome = msg_json["nome"].get<std::string>();
                    }
                    if (msg_json.contains("primeiroNome")) {
                        result->simulacoes->mensagemProposta.primeiroNome = msg_json["primeiroNome"].get<std::string>();
                    }
                    if (msg_json.contains("cpf")) {
                        result->simulacoes->mensagemProposta.cpf = msg_json["cpf"].get<std::string>();
                    }
                    if (msg_json.contains("apelidoBancoEmprestimoPort")) {
                        result->simulacoes->mensagemProposta.apelidoBancoEmprestimoPort = msg_json["apelidoBancoEmprestimoPort"].get<std::string>();
                    }
                    if (msg_json.contains("valorLiberado")) {
                        result->simulacoes->mensagemProposta.valorLiberado = msg_json["valorLiberado"].get<std::string>();
                    }
                    if (msg_json.contains("taxaContrato")) {
                        result->simulacoes->mensagemProposta.taxaContrato = msg_json["taxaContrato"].get<std::string>();
                    }
                    if (msg_json.contains("reservaDeSaldo")) {
                        result->simulacoes->mensagemProposta.reservaDeSaldo = msg_json["reservaDeSaldo"].get<std::string>();
                    }
                    if (msg_json.contains("numeroPeriodos")) {
                        result->simulacoes->mensagemProposta.numeroPeriodos = msg_json["numeroPeriodos"].get<uint32_t>();
                    }
                    if (msg_json.contains("mensagemDocumentosNecessarios")) {
                        result->simulacoes->mensagemProposta.mensagemDocumentosNecessarios = msg_json["mensagemDocumentosNecessarios"].get<std::string>();
                    }

                    if (msg_json.contains("mensagemPeriodos")) {
                        const auto& msg_periodos_json = msg_json["mensagemPeriodos"];
                        for (const auto& msg_periodo_json : msg_periodos_json) {
                            Models::Periodo msg_periodo;
                            if (msg_periodo_json.contains("DataRepasse")) {
                                msg_periodo.DataRepasse = msg_periodo_json["DataRepasse"].get<std::string>();
                            }
                            if (msg_periodo_json.contains("RepasseMaximo")) {
                                msg_periodo.RepasseMaximo = msg_periodo_json["RepasseMaximo"].get<std::string>();
                            }
                            if (msg_periodo_json.contains("ValorFinanciado")) {
                                msg_periodo.ValorFinanciado = msg_periodo_json["ValorFinanciado"].get<std::string>();
                            }
                            if (msg_periodo_json.contains("PercentualRepassado")) {
                                msg_periodo.PercentualRepassado = new std::string(msg_periodo_json["PercentualRepassado"].get<std::string>());
                            }
                            if (msg_periodo_json.contains("SaqueRestante")) {
                                msg_periodo.SaqueRestante = new std::string(msg_periodo_json["SaqueRestante"].get<std::string>());
                            }
                            result->simulacoes->mensagemProposta.mensagemPeriodos.push_back(msg_periodo);
                        }
                    }
                }

                if (sim_json.contains("avisos")) {
                    const auto& avisos_json = sim_json["avisos"];
                    for (const auto& aviso : avisos_json) {
                        result->simulacoes->avisos.push_back(aviso.get<std::string>());
                    }
                }

                if (sim_json.contains("informacoes")) {
                    const auto& info_json = sim_json["informacoes"];
                    for (const auto& info : info_json) {
                        result->simulacoes->informacoes.push_back(info.get<std::string>());
                    }
                }

                if (sim_json.contains("objecoes")) {
                    const auto& obj_json = sim_json["objecoes"];
                    for (const auto& obj_item : obj_json) {
                        Models::Objecao obj;
                        if (obj_item.contains("objecao")) {
                            obj.objecao = obj_item["objecao"].get<std::string>();
                        }
                        if (obj_item.contains("resposta")) {
                            obj.resposta = obj_item["resposta"].get<std::string>();
                        }
                        if (obj_item.contains("respostaCopiavel")) {
                            obj.respostaCopiavel = new std::string(obj_item["respostaCopiavel"].get<std::string>());
                        }
                        result->simulacoes->objecoes.push_back(obj);
                    }
                }

                if (sim_json.contains("isInstability")) {
                    result->simulacoes->isInstability = sim_json["isInstability"].get<bool>();
                }
            }
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return result;
}
